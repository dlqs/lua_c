array = require "bitarray"
a = array.new(10)
print(a)

-- flip the first bit. lua is 1-indexed
a[1] = true
print(a[1])
print(a)

a[1] = false
print(a[1])
print(a)
