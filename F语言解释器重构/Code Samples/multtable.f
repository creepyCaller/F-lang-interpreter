print("���i - j (i <= j) �ĳ˷��ھ���")
i = input("����i: ", end = "")
j = input("����j: ", end = "")
for _i in range(i, j + 1):
	for _j in range(i, _i + 1):
		print(_j, " * ", _i, " = ", _i * _j, "	", end = "")
	print()
