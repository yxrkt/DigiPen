-- the original table
t = {}
-- overload the "index" [] (also dot) operator to access functions from the
-- original table (in other words, don't copy them, just make a reference to
-- the old table)
t_mt = { __index = t }

-- the "constructor"
function t:new()
  return setmetatable({value = 10}, t_mt)
end

-- some member functions
function t:IncValue()
  self.value = self.value + 1
end

function t:DecValue()
  self.value = self.value - 1
end


-- driver code
myTable = t:new()
myTable2 = t:new()

-- only prints the instance member "value"
for k,v in pairs(myTable) do print(k, v) end

print(myTable.value)

myTable:IncValue()
myTable:IncValue()

print("myTable.value:", myTable.value)   -- prints 12
print("myTable2.value:", myTable2.value) -- prints 10