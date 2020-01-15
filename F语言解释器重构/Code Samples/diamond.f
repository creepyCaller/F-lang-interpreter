l = input("输入菱形最宽处长度: ", end = "")
c = input("输入打印次数: ", end = "")
for m in range(c):
	print(m+1, ": ")
	for i in range(l):
		for j in range(l - i):
			print(" ", end = "")
		for k in range(2 * i):
			print("*", end = "")
		print()
	for i in range(l):
		for j in range(i):
			print(" ", end = "")
		for k in range(2 * l - 2 * i):
			print("*", end = "")
		print()