function test()
  --code goes here
end

function test2(a, b)
  return a
end

function test3(a)
  return a, 2, "test"
end

test3("testing")
test3()
a, b = test3()
print(a, b) -- nil 2
a, b, c = test3("testing")
print(a, b, c) -- testing 2 test