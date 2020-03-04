import ctypes

def ERROR_MSG(str):
    print str

class ByteArray_20(ctypes.Structure):
    _fields_ = [\
("e1", ctypes.c_ubyte), \
("e2", ctypes.c_ubyte), \
("e3", ctypes.c_ubyte), \
("e4", ctypes.c_ubyte), \
("e5", ctypes.c_ubyte), \
("e6", ctypes.c_ubyte), \
("e7", ctypes.c_ubyte), \
("e8", ctypes.c_ubyte), \
("e9", ctypes.c_ubyte), \
("e10", ctypes.c_ubyte), \
("e11", ctypes.c_ubyte), \
("e12", ctypes.c_ubyte), \
("e13", ctypes.c_ubyte), \
("e14", ctypes.c_ubyte), \
("e15", ctypes.c_ubyte), \
("e16", ctypes.c_ubyte), \
("e17", ctypes.c_ubyte), \
("e18", ctypes.c_ubyte), \
("e19", ctypes.c_ubyte), \
("e20", ctypes.c_ubyte)]


class ByteArray_20_3(ctypes.Structure):
    _fields_ = [\
("e1", ByteArray_20), \
("e2", ByteArray_20), \
("e3", ByteArray_20)]
    def __init__(self):
        self.aaa = 123
        self.bbb = [1, 2, 3, 4, 5]
        self.ccc = "alksdfjlasdfjk"
    def test(self):
        self.aaa = 123
        self.bbb = [1, 2, 3, 4, 5]
        self.ccc = "alksdfjlasdfjk"
        self.e1.e1 = 5
        self.e1.e2 = 20


so = ctypes.cdll.LoadLibrary
lib = so("./libpycallclass.so")
print('display()')
ret = ByteArray_20_3()
ret.test()
ERROR_MSG(ret.e1.e1)
ERROR_MSG(ret.e1.e2)
print("before lib.display(ctypes.byref(ret))")
lib.display(ctypes.byref(ret))
print("after lib.display(ctypes.byref(ret))")
print('#######################################################################################')
print(ret)
ERROR_MSG(ret.e1)
ERROR_MSG(ret.e2)
ERROR_MSG(ret.e3)
ERROR_MSG(ret.e1.e1)
ERROR_MSG(ret.e1.e2)
ERROR_MSG(ret.e1.e3)
ERROR_MSG(ret.e2.e1)
ERROR_MSG(ret.e2.e2)
ERROR_MSG(ret.e2.e3)
ERROR_MSG(type(ret))

ret.test()
ERROR_MSG(ret.e1.e1)
ERROR_MSG(ret.e1.e2)
