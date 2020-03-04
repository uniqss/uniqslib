import ctypes
so = ctypes.cdll.LoadLibrary
lib = so("./libpycallclass.so")
print 'display()'
lib.display()
print 'display(100)'
lib.display_int(100)
print "display(100, 'helloworld')"
lib.display(100, 'helloworld')
