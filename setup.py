from distutils.core import setup, Extension

ext_g1 = Extension('g1', ['g1.cc'], \
      extra_compile_args=["/MD"],
      extra_objects=["libeay32MT.lib"],
      include_dirs=["d:/Python27/include",
                    "d:/Program Files (x86)/Microsoft Visual Studio 9.0/VC/include",
                    "c:/Program Files/Microsoft SDKs/Windows/v6.0A/Include",
                    #"c:/Program Files (x86)/Windows Kits/10/Include/10.0.10240.0/ucrt",
                    "d:/OpenSSL-Win32/include"],
      library_dirs=['c:/Program Files/Microsoft SDKs/Windows/v6.0A/Lib', 'd:/OpenSSL-Win32/lib/VC', 'd:/OpenSSL-Win32/lib/VC/static'], \
      libraries=['libeay32MD'])

setup(name='g1', version='1.0', ext_modules=[ext_g1])
