myTable = {}

myTable[1] = "hi"
myTable[2] = 23

myTable["test"] = function() print("a test") end

myTable.test() -- a test
myTable["test"]() -- a test