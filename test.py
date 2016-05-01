# -*- coding: cp936-*-
import sys
import g1


def main():
    print g1.win32_version()
    p = g1.MXCipher()
    print g1.MXCipher.__name__
    g1.aes_test()

if __name__ == '__main__':
    main()
