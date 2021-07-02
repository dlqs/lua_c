function foo(a, b) 
  return {
      a = a,
      b = b
  }
end

function factorial(n)
  function helper(m)
    if m == 0 then
      return 1
    else
      return m * helper(m - 1)
    end
  end
  return {
    n = helper(n)
  }
end

