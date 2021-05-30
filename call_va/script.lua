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

function map(f, xs)
  local ys = {}
  for i, x in ipairs(xs) do
    ys[i] = f(x)
  end
  return ys
end

function call(f, a)
  return f(a)
end
