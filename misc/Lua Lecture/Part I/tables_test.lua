function PrintByIndex(t)
  print("---PrintByIndex---")
  -- table.getn() gets the length of a table
	for i=1,table.getn(t) do
	  print(t[i])
	end
	print("")
end

function PrintByPair(t)
  print("---PrintByPair---")
	for k,v in pairs(t) do
	  print(k, v)
	end
	print("")
end

test = {alpha = 0.5,
        beta  = 1.3,
        gamma = 1.1,
        "string val"}

-- print everything, even non-numerical indices
PrintByPair(test)

-- will print "string val" only, it is the only numerical index in the table
PrintByIndex(test)

-- create a new member called "phi" that is a function
test.phi = function() return "phi" end
-- prints address of phi, does not call it, () operator needed
PrintByPair(test)


-- access using dot operator
print(test.phi())

-- access using array operator
print(test["phi"]())