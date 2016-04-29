#include <Python.h>
#include <windows.h>

static PyObject* win32_version(PyObject *self)
{
    OSVERSIONINFOEX version = {0};
    char szOS[1024];
    auto a = 1;
    version.dwOSVersionInfoSize = sizeof(version);
    GetVersionEx((OSVERSIONINFO*) &version);
    sprintf(szOS, "Microsoft Windows %d.%d", version.dwMajorVersion, version.dwMinorVersion);
    return Py_BuildValue("s", szOS);
}

static PyMethodDef g1_funcs[] =
{
    {
        "win32_version",
        (PyCFunction) win32_version,
        METH_NOARGS,
        "win32_version(): return Windows version information"
    },
    {NULL}
};

void initg1(void) {
    Py_InitModule3("g1", g1_funcs, "G1 Extension module");
}
