from distutils.core import setup, Extension

ext_g1 = Extension('g1', ['g1.cc', 'compress.cc'], \
      extra_compile_args=["/MD"],
      #extra_objects=["libeay32MT.lib"],
      include_dirs=["d:/Python27/include",
                    "e:/projects/ClionProjects/python-extension/include",
                    "d:/Program Files (x86)/Microsoft Visual Studio 9.0/VC/include",
                    "c:/Program Files/Microsoft SDKs/Windows/v6.0A/Include",
                    ],
      library_dirs=['c:/Program Files/Microsoft SDKs/Windows/v6.0A/Lib', 'e:/projects/ClionProjects/python-extension/lib'], \
      libraries=['libeay32', 'zdll'])

setup(name='g1', version='1.0', ext_modules=[ext_g1])
