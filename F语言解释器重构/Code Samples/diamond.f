l = input("���������������: ", end = "")
c = input("�����ӡ����: ", end = "")
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