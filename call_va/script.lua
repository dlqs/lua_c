function add(a, b)
  return a + b
end

function div(a, b)
  return a/b
end

function say_hello(a)
  return "Hello " .. a
end

function xor(a, b)
  return not (a and b)
end

function swap(a, b)
  return b, a
end

function sleep(n)
  os.execute("sleep " .. tonumber(n))
end

