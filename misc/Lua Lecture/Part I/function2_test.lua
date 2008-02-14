function a()
  function b()
    return "function b"
  end

  print(b())
  return "function a"
end

print(a())
--print(b()) -- not available in this scope