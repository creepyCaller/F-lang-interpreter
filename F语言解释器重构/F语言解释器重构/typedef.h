#pragma once
typedef struct _ForStack
{
	double end, step;
	int block, retBlock; //forѭ�����ڵĴ����
	bool isEnd = false;
	char name[256];
	char * ret;
} ForStack;