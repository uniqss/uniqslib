import time
from ctypes import *
import ctypes
import platform


def ERROR_MSG(str):
	print(str)

class tagOutCardResult_py(ctypes.Structure):
	_fields_ = [("cbCardCount", ctypes.c_ubyte), \
("cbResultCard1", ctypes.c_ubyte), \
("cbResultCard2", ctypes.c_ubyte), \
("cbResultCard3", ctypes.c_ubyte)]

class game:
	def __init__(self):
		self.loop_count = 0
		so = ctypes.cdll.LoadLibrary
		if platform.system() == "Windows":
			self.lib = so("./pycallcpp.dll")
		else:
			self.lib = so("./libpycallclass.so")
		self.lib.init(1)
		self.lib.init(2)

	def test(self):
		ret1 = tagOutCardResult_py(1, 2, 3)
		ERROR_MSG("before lib.display(ctypes.byref(ret))")
		t1 = ctypes.c_longlong(0)
		t2 = ctypes.c_longlong(0)
		t3 = ctypes.c_longlong(0)
		t4 = ctypes.c_longlong(0)
		t1.value = 4294967396
		t2.value = 4294967496
		self.lib.test_cpp(1, ctypes.byref(ret1), t1, t2, ctypes.byref(t3), ctypes.byref(t4))
		print(t3.value)
		print(t4.value)
		self.lib.p(1)
		ret2 = tagOutCardResult_py(4, 5, 6)
		ERROR_MSG("before lib.display(ctypes.byref(ret))")
		t1.value = 5294967296
		t2.value = 6294967296
		self.lib.test_cpp(2, ctypes.byref(ret2), t1, t2, ctypes.byref(t3), ctypes.byref(t4))
		print(t3.value)
		print(t4.value)
		self.lib.p(2)

g = game()
g.test()

