from distutils.core import setup, Extension
setup(name='g1', version='1.0',  \
      ext_modules=[Extension('g1', ['g1.c'])])
