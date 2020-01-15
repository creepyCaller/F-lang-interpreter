#pragma once
typedef struct _ForStack
{
	double end, step;
	int block, retBlock; //for循环所在的代码块
	bool isEnd = false;
	char name[256];
	char * ret;
} ForStack;