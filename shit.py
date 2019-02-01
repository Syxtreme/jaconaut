from cffi import FFI

ffi = FFI()
ffi.cdef("""
    int InitAPI();
    int CloseAPI();
    int MoveHome();
""")

# Load symbols from the current process (Python).
lib = ffi.dlopen("Kinova.API.USBCommandLayerUbuntu")
print('Loaded lib {0}'.format(lib))

