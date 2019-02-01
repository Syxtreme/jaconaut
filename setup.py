# from distutils.core import setup
# from Cython.Build import cythonize

# setup(
#     ext_modules = cythonize("hello_shit.pyx")
# )
# setup.py file
import sys
import os
import shutil

from distutils.core import setup
from distutils.extension import Extension
from Cython.Distutils import build_ext
from Cython.Build import cythonize

# clean previous build
for root, dirs, files in os.walk(".", topdown=False):
    for name in files:
        if (name.startswith("simple_comm") and not(name.endswith(".pyx") or name.endswith(".pxd"))):
            os.remove(os.path.join(root, name))
    for name in dirs:
        if (name == "build"):
            shutil.rmtree(name)

# build "myext.so" python extension to be added to "PYTHONPATH" afterwards...
setup(
    cmdclass = {'build_ext': build_ext},
    ext_modules = 
        cythonize(Extension("simple_comm", 
                  sources=["simple_comm.pyx", "kinova_api.cpp"],
                #   libraries=["Kinova.API.CommLayerUbuntu", "Kinova.API.UsbCommandLayerUbuntu"],  # refers to "libexternlib.so"
                  language="c++",                   # remove this if C and not C++
                  extra_compile_args=["-g", "-rdynamic", "-ldl"],  # CFLAGS
                #   extra_link_args=[os.path.abspath("./lib/Kinova.API.CommLayerUbuntu.so"), os.path.abspath("./lib/Kinova.API.USBCommandLayerUbuntu.so")],  # LDFLAGS
                  include_dirs=["./include"]
                #   library_dirs=["./lib"]
             ))
        
)           