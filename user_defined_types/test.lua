array = require "array"
a = array.new(10)
print(a)

-- flip the first bit. lua is 1-indexed
a[1] = 1

print(a[1])
print(a)
