print("输出i - j (i <= j) 的乘法口诀表")
i = input("输入i: ", end = "")
j = input("输入j: ", end = "")
for _i in range(i, j + 1):
	for _j in range(i, _i + 1):
		print(_j, " * ", _i, " = ", _i * _j, "	", end = "")
	print()
