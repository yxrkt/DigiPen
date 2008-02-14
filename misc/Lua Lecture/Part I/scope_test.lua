A = {}
B = {}

function A.NormalFunction(a, b, c)
  print("self = "..tostring(self))
  print("a    = "..tostring(a))
  print("b    = "..tostring(b))
  print("c    = "..tostring(c))
end

function B:MemberFunction(a, b, c)
  print("self = "..tostring(self))
  print("a    = "..tostring(a))
  print("b    = "..tostring(b))
  print("c    = "..tostring(c))
end

print("---"..1)
A.NormalFunction("test", 1)
print("\n---"..2)
A:NormalFunction("test", 2)
print("\n---"..3)
B.MemberFunction("test", 3)
print("\n---"..4)
B:MemberFunction("test", 4)