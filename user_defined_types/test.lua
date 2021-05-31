array = require "bitarray"
a = array.new(64)
a[1] = true
a[64] = true
b = array.new(100)
b[1] = true
b[64] = false
b[100] = true

print(a)
print(b)

print(a + b)
print(a * b)


-- flip the first bit. lua is 1-indexed
--a[1] = true
--print(a[1])
--print(a)

--a[1] = false
--print(a[1])
--print(a)
