#include <string.h>
#include <assert.h>
#include <windows.h>
#include <Python.h>

#define ZLIB_CONST
#include <zlib.h>

#include <openssl/conf.h>
#include <openssl/evp.h>
#include <openssl/err.h>
//#include <openssl/applink.c>

#include "typedefs.h"


namespace cipher {

    static void handleErrors(void)
    {
        ERR_print_errors_fp(stderr);
        abort();
        // char err_str[256] = {0};
        // ERR_error_string_n(ERR_get_error(), err_str, 256);
        // PyObject_Print(Py_BuildValue("s", err_str), stdout, 0);
    }

    static int encrypt(byte *plaintext, int plaintext_len, byte *key, byte *iv, byte *ciphertext)
    {
        EVP_CIPHER_CTX *ctx;

        int len;

        int ciphertext_len;

        /* Create and initialise the context */
        if(!(ctx = EVP_CIPHER_CTX_new())) handleErrors();

        /* Initialise the encryption operation. IMPORTANT - ensure you use a key
        * and IV size appropriate for your cipher
        * In this example we are using 256 bit AES (i.e. a 256 bit key). The
        * IV size for *most* modes is the same as the block size. For AES this
        * is 128 bits */
        if(1 != EVP_EncryptInit_ex(ctx, EVP_aes_256_ecb(), NULL, key, NULL))
            handleErrors();

        /* Provide the message to be encrypted, and obtain the encrypted output.
        * EVP_EncryptUpdate can be called multiple times if necessary
        */
        if(1 != EVP_EncryptUpdate(ctx, ciphertext, &len, plaintext, plaintext_len))
            handleErrors();
        ciphertext_len = len;

        /* Finalise the encryption. Further ciphertext bytes may be written at
        * this stage.
        */
        if(1 != EVP_EncryptFinal_ex(ctx, ciphertext + len, &len)) handleErrors();
        ciphertext_len += len;

        /* Clean up */
        EVP_CIPHER_CTX_free(ctx);

        return ciphertext_len;
    }

    static int decrypt(byte *ciphertext, int ciphertext_len, byte *key, byte *iv, byte *plaintext)
    {
        EVP_CIPHER_CTX *ctx;

        int len;

        int plaintext_len;

        /* Create and initialise the context */
        if(!(ctx = EVP_CIPHER_CTX_new())) handleErrors();

        /* Initialise the decryption operation. IMPORTANT - ensure you use a key
        * and IV size appropriate for your cipher
        * In this example we are using 256 bit AES (i.e. a 256 bit key). The
        * IV size for *most* modes is the same as the block size. For AES this
        * is 128 bits */
        if(1 != EVP_DecryptInit_ex(ctx, EVP_aes_256_ecb(), NULL, key, NULL))
            handleErrors();

        /* Provide the message to be decrypted, and obtain the plaintext output.
        * EVP_DecryptUpdate can be called multiple times if necessary
        */
        if(1 != EVP_DecryptUpdate(ctx, plaintext, &len, ciphertext, ciphertext_len))
            handleErrors();
        plaintext_len = len;

        /* Finalise the decryption. Further plaintext bytes may be written at
        * this stage.
        */
        if(1 != EVP_DecryptFinal_ex(ctx, plaintext + len, &len)) handleErrors();
        plaintext_len += len;

        /* Clean up */
        EVP_CIPHER_CTX_free(ctx);

        return plaintext_len;
    }

    PyObject* aes_test(void)
    {
        //CRYPTO_malloc_init();
        /* Set up the key and iv. Do I need to say to not hard code these in a
        * real application? :-)
        */

        /* A 256 bit key */
        byte *key = (byte *) "01234567890123456789012345678901";

        /* A 128 bit IV */
        byte *iv = (byte *) "01234567890123456";

        /* Message to be encrypted */
        byte *plaintext = (byte *) "The quick brown fox jumps over the lazy dogs";

        /* Buffer for ciphertext. Ensure the buffer is long enough for the
        * ciphertext which may be longer than the plaintext, dependant on the
        * algorithm and mode
        */
        byte ciphertext[128];

        /* Buffer for the decrypted text */
        byte decryptedtext[128];

        int decryptedtext_len, ciphertext_len;

        /* Initialise the library */
        ERR_load_crypto_strings();
        OpenSSL_add_all_algorithms();
        OPENSSL_config(NULL);

        /* Encrypt the plaintext */
        ciphertext_len = encrypt(plaintext, strlen ((char *)plaintext), key, iv,
                                    ciphertext);

        /* Do something useful with the ciphertext here */
        //printf("Ciphertext is:\n");
        //BIO_dump_fp (stdout, (const char *)ciphertext, ciphertext_len);

        /* Decrypt the ciphertext */
        decryptedtext_len = decrypt(ciphertext, ciphertext_len, key, iv,
            decryptedtext);

        /* Add a NULL terminator. We are expecting printable text */
        decryptedtext[decryptedtext_len] = '\0';

        /* Show the decrypted text */
        PyObject* s = Py_BuildValue("s", decryptedtext);
        PyObject_Print(s, stdout, 0);
        Py_DECREF(s);

        /* Clean up */
        EVP_cleanup();
        ERR_free_strings();

        Py_RETURN_NONE;
    }
}

namespace zlib {
    int compress(const byte* src, int* out_buf_size, byte* dest, int in_buf_size) {
        /* allocate deflate state */
        z_stream strm;
        strm.next_in = Z_NULL;
        strm.zalloc = Z_NULL;
        strm.zfree = Z_NULL;
        strm.opaque = Z_NULL;

        // 负的windowBits表示生成raw deflate
        int ret = deflateInit2(&strm, Z_BEST_COMPRESSION, Z_DEFLATED, -15, 8, Z_DEFAULT_STRATEGY);
        if (ret != Z_OK)
            return ret;

        // 设置输入参数
        strm.avail_in = in_buf_size;
        strm.next_in = src;

        // 设置输出参数
        strm.avail_out = in_buf_size;
        strm.next_out = dest;

        // 开始压缩
        ret = deflate(&strm, Z_FINISH);
        // 没有错误发生
        assert(ret != Z_STREAM_ERROR);
       // 所有数据处理完毕
        assert(strm.avail_out > 0);
        // 计算压缩后数据的大小
        *out_buf_size = in_buf_size - strm.avail_out;

        // 释放zlib资源
        (void)deflateEnd(&strm);
        return Z_OK;
    }

    int decompress(const byte* src, int* out_buf_size, byte* dest, int in_buf_size) {
        /* allocate deflate state */
        z_stream strm;
        strm.next_in = Z_NULL;
        strm.zalloc = Z_NULL;
        strm.zfree = Z_NULL;
        strm.opaque = Z_NULL;

        // 负的windowBits表示raw deflate
        int ret = inflateInit2(&strm, -15);
        if (ret != Z_OK)
            return ret;

        // 设置输入参数
        strm.avail_in = in_buf_size;
        strm.next_in = src;

        // 设置输出参数
        strm.avail_out = in_buf_size;
        strm.next_out = dest;

        // 开始压缩
        ret = deflate(&strm, Z_FINISH);
        // 没有错误发生
        assert(ret != Z_STREAM_ERROR);
        // 所有数据处理完毕
        assert(strm.avail_out > 0);
        // 计算解压后数据的大小
        *out_buf_size = in_buf_size - strm.avail_out;

        // 释放zlib资源
        (void)inflateEnd(&strm);
        return Z_OK;
    }

    PyObject* test_zlib()
    {
        const byte input[] = "abcdefg";
        byte output[1024];
        int out_buf_size = 0;
        compress(input, &out_buf_size, output, sizeof input);
        output[out_buf_size] = '\0';
        return Py_BuildValue("s", output);
    }
}

namespace g1 {
    typedef struct {
        PyObject_HEAD
        /* Type-specific fields go here. */
    } MXCipher;

    static PyTypeObject MXCipherType = {
            PyObject_HEAD_INIT(NULL)
            0,                         /*ob_size*/
            "g1.MXCipher",             /*tp_name*/
            sizeof(MXCipher),          /*tp_basicsize*/
            0,                         /*tp_itemsize*/
            0,                         /*tp_dealloc*/
            0,                         /*tp_print*/
            0,                         /*tp_getattr*/
            0,                         /*tp_setattr*/
            0,                         /*tp_compare*/
            0,                         /*tp_repr*/
            0,                         /*tp_as_number*/
            0,                         /*tp_as_sequence*/
            0,                         /*tp_as_mapping*/
            0,                         /*tp_hash */
            0,                         /*tp_call*/
            0,                         /*tp_str*/
            0,                         /*tp_getattro*/
            0,                         /*tp_setattro*/
            0,                         /*tp_as_buffer*/
            Py_TPFLAGS_DEFAULT,        /*tp_flags*/
            "MXCipher objects",        /* tp_doc */
    };

    static void MXCipher_dealloc(MXCipher* self)
    {
        //Py_XDECREF(self->first);
        //Py_XDECREF(self->last);
        self->ob_type->tp_free((PyObject*)self);
    }

    // static PyObject* MXCipher_new(PyTypeObject *type, PyObject *args, PyObject *kwds)
    // {
    //     MXCipher *self;

    //     self = (MXCipher *)type->tp_alloc(type, 0);
    //     if (self != NULL) {
    //         self->first = PyString_FromString("");
    //         if (self->first == NULL)
    //         {
    //             Py_DECREF(self);
    //             return NULL;
    //         }

    //         self->last = PyString_FromString("");
    //         if (self->last == NULL)
    //         {
    //             Py_DECREF(self);
    //             return NULL;
    //         }

    //         self->number = 0;
    //     }

    //     return (PyObject *)self;
    // }

    static PyObject* win32_version(PyObject *self)
    {
        OSVERSIONINFOEX version = {0};
        char szOS[1024];
        version.dwOSVersionInfoSize = sizeof(version);
        GetVersionEx((OSVERSIONINFO*) &version);
        sprintf(szOS, "Microsoft Windows %d.%d", version.dwMajorVersion, version.dwMinorVersion);
        return Py_BuildValue("s", szOS);
    }

    static PyMethodDef g1_methods[] =
    {
        {
            "win32_version",
            (PyCFunction) win32_version,
            METH_NOARGS,
            "win32_version(): return Windows version information",
        },
        {
            "aes_test",
            (PyCFunction) cipher::aes_test,
            METH_NOARGS,
            "aes_test(): aes test",
        },
        {
            "test_zlib",
            (PyCFunction) zlib::test_zlib,
            METH_NOARGS,
            "test_zlib(): test zlib",
        },
        {NULL}
    };

    void initg1(void)
    {
        MXCipherType.tp_new = PyType_GenericNew;
        if (PyType_Ready(&MXCipherType) < 0)
            return;

        PyObject* m = Py_InitModule3("g1", g1_methods, "G1 Extension module");

        Py_INCREF(&MXCipherType);
        PyModule_AddObject(m, "MXCipher", (PyObject *)&MXCipherType);
    }
}
