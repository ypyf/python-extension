#include <Python.h>
#include <windows.h>

static PyObject *win32_version(PyObject *self)
{
    OSVERSIONINFOEX version = {0};
    char szOS[1024];

    version.dwOSVersionInfoSize = sizeof(version);
    GetVersionEx((OSVERSIONINFO*)&version);

    sprintf(szOS, "Microsoft Windows %d.%d", version.dwMajorVersion, version.dwMinorVersion);

    return Py_BuildValue("s", szOS);
}

//static PyObject *helloworld2(PyObject *self) {
//    struct ifaddrs *ifaddr, *ifa;
//    int family, s, n;
//    char host[NI_MAXHOST];
//
//    if (getifaddrs(&ifaddr) == -1) {
//        PyErr_SetString(PyExc_RuntimeError, "getifaddrs error");
//        return NULL;
//    }
//
//    // 创建一个pylist作为返回值
//    PyObject* pylist = PyList_New(0);
//
//    /* Walk through linked list, maintaining head pointer so we
//       can free list later */
//
//    for (ifa = ifaddr, n = 0; ifa != NULL; ifa = ifa->ifa_next, n++) {
//        if (ifa->ifa_addr == NULL)
//            continue;
//
//        family = ifa->ifa_addr->sa_family;
//
//        /* Display interface name and family (including symbolic
//           form of the latter for the common families) */
//        char buff[1024] = {0};
//        sprintf(buff, "%-8s %s (%d)\n",
//               ifa->ifa_name,
//               (family == AF_PACKET) ? "AF_PACKET" :
//               (family == AF_INET) ? "AF_INET" :
//               (family == AF_INET6) ? "AF_INET6" : "???",
//               family);
//        PyList_Append(pylist, Py_BuildValue("s", buff));
//        /* For an AF_INET* interface address, display the address */
//
////        if (family == AF_INET || family == AF_INET6) {
////            s = getnameinfo(ifa->ifa_addr,
////                            (family == AF_INET) ? sizeof(struct sockaddr_in) :
////                            sizeof(struct sockaddr_in6),
////                            host, NI_MAXHOST,
////                            NULL, 0, NI_NUMERICHOST);
////            if (s != 0) {
////                printf("getnameinfo() failed: %s\n", gai_strerror(s));
////                exit(EXIT_FAILURE);
////            }
////
////            printf("\t\taddress: <%s>\n", host);
////
////        } else if (family == AF_PACKET && ifa->ifa_data != NULL) {
////            struct rtnl_link_stats *stats = ifa->ifa_data;
////
////            printf("\t\ttx_packets = %10u; rx_packets = %10u\n"
////                           "\t\ttx_bytes   = %10u; rx_bytes   = %10u\n",
////                   stats->tx_packets, stats->rx_packets,
////                   stats->tx_bytes, stats->rx_bytes);
////        }
//    }
//    freeifaddrs(ifaddr);
//    return pylist;
//}

static PyObject *helloworld(PyObject *self) {
    return Py_BuildValue("s", "Hello, Python extensions!!");
}

static char helloworld_docs[] =
        "helloworld( ): Any message you want to put here!!\n";

static PyMethodDef g1_funcs[] = {
        {"helloworld", (PyCFunction) helloworld,
                METH_NOARGS, helloworld_docs},
        {"win32_version", (PyCFunction) win32_version,
                METH_NOARGS, "win32_version( ): 返回Windows版本号\n"},
        {NULL}
};

void initg1(void) {
    Py_InitModule3("g1", g1_funcs, "G1 Extension module");
}
