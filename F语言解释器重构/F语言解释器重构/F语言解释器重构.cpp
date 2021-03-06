// F语言解释器重构.cpp: 定义控制台应用程序的入口点。
//重构为oop形式,换上自己的取词器,更换print和for的实现方式,取消单引号和双引号的区别( 完成,添加while, endwhile, elif
//具体类:变量树,语法分析器,文件输入流,控制台 (完成
//更改注释方式: 支持 # + 行注释 (完成
//添加子函数支持
//更换求值器 (完成
//

#include "stdafx.h"

using namespace std;

// 外部变量
char *pfile, *fileHead;
char tokenBuf[64];
TokenType TT;
bool printPrompt = false;
LexicalAnalyzer LA;
Calculator Calc;
ValTree VT;
ForStack *forStack;
int forStackSize; //为什么不用size_t?因为用size_t的话在下边跟Top做边缘判断会产生不必要的格式转换
int skipFor, forStackTop;
int _Block = 0; //代码块

int getCodeBlock()
{
	uint32_t block = 0;
	char * t = pfile;
	if (*t == 0)
	{
		return 0;
	}
	while (*t == ' ' || *t == '\t')
	{
		
		if (*t == ' ')
			block += 1;
		else if(*t == '\t')
			block += 4;
		++t;
	}
	return block;
}

void commandTable()
{
	char* cmdBuf = new char[64]{};
	errno_t err = 1;
	FileLoader FL;
	do
	{
		if (!printPrompt)
		{
			printf(">: ");
			printPrompt = true;
		}
		gets_s(cmdBuf, 64);
		if (!strcmp(cmdBuf, "help"))
		{
			cout << "命令列表:" << endl;
			cout << "quit		退出程序" << endl;
			cout << "dir		显示目录下的所有文件" << endl;
			cout << "cls		清屏" << endl;
			cout << "文件名		解释文件" << endl;

			putchar('\n');
			printPrompt = false;
		}
		else if (!strcmp(cmdBuf, "quit"))
		{
			exit(0);
		}
		else if (!strcmp(cmdBuf, "dir"))
		{
			system(cmdBuf);
			printPrompt = false;
		}
		else if (!strcmp(cmdBuf, "cls"))
		{
			system(cmdBuf);
			printPrompt = false;
		}
		else
		{
			err = FL.FileReader(cmdBuf);
			if (err != 0 && cmdBuf[0] != 0)
			{
				printf_s("'%s' 不是命令，也不是待解释的文件。\n", cmdBuf);
				printPrompt = false;
			}
		}
	cmdBuf[0] = 0;
	} while (err);
	err = 1;
	FL.LoadProgram(pfile);
	fileHead = pfile;
	_Block = getCodeBlock();
	if (_Block != 0) //首行的代码块必须要为 0
	{
		puts("首行缩进错误");
		*pfile = 0;
	}
	return;
}

void restoreAnalyze()//将pfile退回至上一次调用Analyer
{
	char * t = tokenBuf;
	while (*t != 0)
	{
		++t;
		--pfile;
	}
	return;
}

double input();

void assignment() //赋值
{
	double exp, t;
	char valname[256];
	strcpy_s(valname, tokenBuf);
	LA.Analyzer();
	if (!strchr("=+-*/%", *tokenBuf)) // A "=, +=, -=, *=, /=, %=" (表达式)
	{
		printf("运算符对变量的操作不正确, TB = %s, TT = %d\n", tokenBuf, TT);
		return;
	}
	else if(strchr("+-*/%", *tokenBuf)) //+=, -=, *=, /=, %=
	{
		switch (*tokenBuf)
		{
		case '+':
			LA.Analyzer();
			if (*tokenBuf != '=')
			{
				puts("你的意思是 '+=' 或是 '++' ? 注: 不支持后者");
				return;
			}
			LA.Analyzer();
			if (LA.getCmdType() == kw_input) //如果是a += input()
			{
				t = input();
				if (!VT.Serch(valname, &exp))
				{
					puts("+= 操作必须依赖于已经声明的左值");
				}
				exp = exp + t;
				VT.Insert(exp, valname);
				return;
			}
			else //a += (表达式)
			{
				restoreAnalyze();
				if (!VT.Serch(valname, &exp))
				{
					puts("+= 操作必须依赖于已经声明的左值");
					return;
				}
				Calc.getVal(&t);
				exp = exp + t;
				VT.Insert(exp, valname);
			}
			break;
		case '-':
			LA.Analyzer();
			if (*tokenBuf != '=')
			{
				puts("你的意思是 '-=' 或是 '-- ? 注: 不支持后者");
				return;
			}
			LA.Analyzer();
			if (LA.getCmdType() == kw_input)
			{
				t = input();
				if (!VT.Serch(valname, &exp))
				{
					puts("-= 操作必须依赖于已经声明的左值");
					return;
				}
				exp = exp - t;
				VT.Insert(exp, valname);
				return;
			}
			else
			{
				restoreAnalyze();
				if (!VT.Serch(valname, &exp))
				{
					puts("-= 操作必须依赖于已经声明的左值");
					return;
				}
				Calc.getVal(&t);
				exp = exp - t;
				VT.Insert(exp, valname);
			}
			break;
		case '*':
			LA.Analyzer();
			if (*tokenBuf != '=')
			{
				puts("你的意思是 '*=' ?");
				return;
			}
			LA.Analyzer();
			if (LA.getCmdType() == kw_input) // A *= input()
			{
				t = input();
				if (!VT.Serch(valname, &exp))
				{
					puts("*= 操作必须依赖于已经声明的左值");
					return;
				}
				exp = exp * t;
				VT.Insert(exp, valname);
				return;
			}
			else
			{
				restoreAnalyze();
				if (!VT.Serch(valname, &exp))
				{
					puts("*= 操作必须依赖于已经声明的左值");
					return;
				}
				Calc.getVal(&t);
				exp = exp * t;
				VT.Insert(exp, valname);
			}
			break;
		case '/':
			LA.Analyzer();
			if (*tokenBuf != '=')
			{
				puts("你的意思是 '/=' ?");
				return;
			}
			LA.Analyzer();
			if (LA.getCmdType() == kw_input)
			{
				t = input();
				if (!VT.Serch(valname, &exp))
				{
					puts("/= 操作必须依赖于已经声明的左值");
					return;
				}
				if (t == 0.0)
				{
					printf("%.2lf...不能与0进行 '/=' 操作\n", exp);
					return;
				}
				exp = exp / t;
				VT.Insert(exp, valname);
				return;
			}
			else
			{
				restoreAnalyze();
				if (!VT.Serch(valname, &exp))
				{
					puts("/= 操作必须依赖于已经声明的左值");
					return;
				}
				Calc.getVal(&t);
				if (t == 0.0)
				{
					printf("%.2lf...不能与0进行 '/=' 操作\n", exp);
					return;
				}
				exp = exp / t;
				VT.Insert(exp, valname);
			}
			break;
		case '%':
			LA.Analyzer();
			if (*tokenBuf != '=')
			{
				puts("你的意思是 '%=' ?");
				return;
			}
			LA.Analyzer();
			if (LA.getCmdType() == kw_input)
			{
				t = input();
				if (!VT.Serch(valname, &exp))
				{
					puts("%= 操作必须依赖于已经声明的左值");
					return;
				}
				if (t == 0.0)
				{
					printf("%.2lf...不能与0进行 '%=' 操作\n", exp);
					return;
				}
				exp = (double)(((int)exp) % ((int)t));
				VT.Insert(exp, valname);
				return;
			}
			else
			{
				restoreAnalyze();
				if (!VT.Serch(valname, &exp))
				{
					puts("%= 操作必须依赖于已经声明的左值");
					return;
				}
				Calc.getVal(&t);
				if (t == 0.0)
				{
					printf("%.2lf...不能与0进行 '%=' 操作\n", exp);
					return;
				}
				exp = (double)(((int)exp) % ((int)t));
				VT.Insert(exp, valname);
			}
			break;
		default:
			LA.Analyzer();
			if (*tokenBuf != '=')
			{
				puts("你的意思是 \"+-*/%\" + '=' ?");
			}
			break;
		}
		return;
	}
	else // =
	{
		LA.Analyzer();
		if (LA.getCmdType() == kw_input) // A = input(...)
		{
			VT.Insert(input(), valname);
		}
		else
		{
			restoreAnalyze(); //还原到上次分析前
			Calc.getVal(&exp); //计算表达式并赋值
			VT.Insert(exp, valname);
		}
	}
	return;
}

void print()
{
	double exp;
	char disp_val[64], end[64];
	bool CustomEnd = false;
	LA.Analyzer(); // '('
	if (*tokenBuf != '(')
	{
		printf("print要加'('括号的!!\n");
		return;
	}
	do
	{
		LA.Analyzer();
		if (TT == 行末 || TT == 文件末 || TT == 分隔符)
		{
			break;
		}
		else if (TT == 字符串)
		{
			printf("%s", tokenBuf);
			LA.Analyzer();
		}
		else if (TT == 数字)
		{
			printf("%s", tokenBuf);
			LA.Analyzer();
		}
		else if (TT == 函数) // 调用input(...)输入
		{
			cmdType tempCMD = LA.getCmdType();
			int i, printlen;
			switch (tempCMD)
			{
			case kw_input:
				sprintf_s(disp_val, "%lf", input());
				i = 0;
				printlen = strlen(disp_val) - 1;
				while (disp_val[printlen] == '0')
				{
					--printlen;
				}
				if (disp_val[printlen] == '.')
				{
					--printlen;
				}
				for (i = 0; i <= printlen; ++i)
				{
				putchar(disp_val[i]);
				}
				LA.Analyzer();
				break;
			default:
				i = 0;
				printlen = 0;
				puts("错误: 未指定函数??");
				break;
			}
		}
		else //是变量或表达式或关键词参数,不显示无用的0
		{
			if (!strcmp(tokenBuf, "end")) //参数为end = ''
			{
				CustomEnd = true;
				LA.Analyzer(); //取 '='
				if (strcmp(tokenBuf, "="))
				{
					puts("end = *");
					return;
				}
				LA.Analyzer();
				if (TT != 字符串)
				{
					puts("end = String");
					return;
				}
				strcpy_s(end, tokenBuf);
				LA.Analyzer();
			}
			else
			{
				restoreAnalyze(); 
				Calc.getVal(&exp);
				sprintf_s(disp_val, "%lf", exp);
				int i = 0, printlen = strlen(disp_val) - 1;
				while (disp_val[printlen] == '0')
				{
					--printlen;
				}
				if (disp_val[printlen] == '.')
				{
					--printlen;
				}
				for (i = 0; i <= printlen; ++i)
				{
					putchar(disp_val[i]);
				}
				LA.Analyzer();
			}
		}
	} while (*tokenBuf == ',');
	if (*tokenBuf != ')')
	{
		printf("print要加')'括号的!!\n");
		return;
	}
	if (CustomEnd)
	{
		printf_s("%s", end);
	}
	else
	{
		putchar('\n');
	}
	return;
}

double input() // 1.a = input(...), 2.xxx(input(...))
{
	char inputBuf[256];
	print(); // call print 打印提示内容 (...)
	gets_s(inputBuf, 256); //输入
	return atof(inputBuf);
}

void _if() // 进行逻辑运算 '&&', '||', '!', '==', >= <= !=
{
	
}

void elif()
{

}

void _else()
{

}

void endif()
{

}

void forStackPush(ForStack f)
{
	if (forStackTop > forStackSize)
	{
		forStack = (ForStack *)realloc(forStack, (forStackSize + STACK_INCREMENT) * sizeof(ForStack));
		if (!forStack)
			exit(OVERFLOW);
		forStackSize += STACK_INCREMENT;
//		printf("memory alloc, forStackSize = %d\n", forStackSize);
	}
//	printf("压入f: 变量名:%s = %.0lf, end = %.0lf, step = %.0lf\n", f.name, VT.getVal(forStack[forStackTop].name), f.end, f.step);
	forStack[forStackTop++] = f;
	return;
}

ForStack forStackPeek()
{
	return forStack[forStackTop - 1];
}

ForStack forStackPop()
{
	--forStackTop;
	if (forStackTop < 0)
	{
		printf("forStack Error! top = %d\n", forStackTop);
		return ForStack();
	}
//	printf("弹出f: 变量名:%s = %.0lf, end = %.0lf, step = %.0lf\n", forStack[forStackTop].name, VT.getVal(forStack[forStackTop].name),  forStack[forStackTop].end, forStack[forStackTop].step);
	return forStack[forStackTop];
}

void _for() // for VAL in range(start, end[, step]) ... endfor, start, end[, step] probably is expression
{
	bool onLine = false;
	double start;
	ForStack f;
	LA.Analyzer(); // VAL
	if (!(*tokenBuf >= 'a' && *tokenBuf <= 'z' || *tokenBuf >= 'A' && *tokenBuf <= 'Z' || *tokenBuf == '_'))
	{
		puts("for [VAL] in range(...)");
		return;
	}
	strcpy_s(f.name, tokenBuf);
	LA.Analyzer(); // in
	if (strcmp("in", tokenBuf))
	{
		puts("for VAL [in] range(...)");
		return;
	}
	LA.Analyzer(); // range
	if (strcmp("range", tokenBuf))
	{
		puts("for VAL in [range](...)");
		return;
	}
	LA.Analyzer(); // get '('
	if (*tokenBuf != '(')
	{
		puts("range[(]...)");
		return;
	}
	Calc.getVal(&start); // get end or start
	LA.Analyzer(); // get ',' or ')' or something else
	if (*tokenBuf == ',') // range(start, end[, step])
	{
		Calc.getVal(&f.end);
		LA.Analyzer(); // ',' or ')'
		if (*tokenBuf == ')') // end of for def, step = 1
		{
			f.step = 1.0;
			LA.Analyzer();
			if (*tokenBuf != ':') // see whether ':' in
			{
				puts("for VAl in range([start, ]end[, step])':'");
				return;
			}
		}
		else if (*tokenBuf == ',') // else, with [, step], get expression of step
		{
			Calc.getVal(&f.step);
			LA.Analyzer();
			if (*tokenBuf != ')') // see whether ':' in
			{
				puts("for VAl in range([start, ]end, step')':");
				return;
			}
			LA.Analyzer();
			if (*tokenBuf != ':') // see whether ':' in
			{
				puts("for VAl in range([start, ]end, step)':'");
				return;
			}
		}
		else // 提醒range的括号不对称
		{
			puts("range括号不对称!");
			return;
		}
	}
	else if (*tokenBuf == ')') // range(end), 0 - end
	{
		f.end = start;
		start = 0.0;
		f.step = 1;
		LA.Analyzer();
		if (*tokenBuf != ':') // see whether ':' in
		{
			puts("for VAl in range(end)':'");
			return;
		}
	}
	else
	{
		puts("range括号不对称!");
		return;
	}
	VT.Insert(start, f.name);
	if (f.step == 0)
	{
		puts("for循环增量不能为0");
		return;
	}
	//接下来是看是否满足跳出for的条件
	if (start > f.end && f.step > 0)
	{
//		printf("for循环(%s) start(%lf) > f.end(%lf) && f.step(%lf) > 0", f.name, start, f.end, f.step);
//		return;
		f.isEnd = true;
	}
	else if (start < f.end && f.step < 0)
	{
//		printf("for循环(%s) start(%lf) < f.end(%lf) && f.step(%lf) < 0", f.name, start, f.end, f.step);
//		return;
		f.isEnd = true;
	}
	else if (start == f.end)
	{
		f.isEnd = true;
	}
	
	f.retBlock = _Block; //保存当前代码块
	LA.Analyzer(); // 取':'后一个token
	if (TT == 文件末) //如果for完了文件也完了
	{
		f.isEnd = true;
	}
	else if(TT == 行末) //如果是行末
	{
		f.block = _Block; // 取for的代码块
//		printf("for(%s) retBlock = %d, for on block = %d\n", f.name, f.retBlock, f.block);
		if (f.block == f.retBlock) // 判断for内代码块是否与当前代码块相同,如果相同就报错
		{
			puts("代码块分割错误");
			return;
		}
	}
	f.ret = pfile;
	if (f.isEnd) //如果已经确认该跳出for循环
	{
//		printf("for循环(使用%s为循环变量)结束,end = %lf, step = %lf\n", f.name, f.end, f.step);
		while (_Block != f.retBlock) // 当当前代码块不等于for外的代码块,就跳到行末或者文件末
		{
			LA.Analyzer();
			if (TT == 文件末)
			{
				break;
			}
			else if (forStackTop != 0 && TT == 行末)
			{
				return;
			}
		}
	}
	else
	{
		forStackPush(f); //如果for循环的start, end, step符合就让这个for循环入栈
	}
	return;
}

void forLoop()
{
	ForStack f;
	f = forStackPop();
	double forVAL = VT.ChangeSerch(f.name, f.step);
	if (f.step > 0) // 增量大于0, 即forVAL + step 向 end 逼近, 可能是 range(start < end, step > 0)
	{
		if (forVAL >= f.end)
		{
			f.isEnd = true;	
			return;
		}
		
	}
	else //可能是 range(start > end, step < 0)
	{
		if (forVAL <= f.end)
		{
			f.isEnd = true;
			return;
		}
	}
	forStackPush(f); //如果不满足for循环跳出条件就入栈
	pfile = f.ret; //使文件指针还原至for循环伊始
	return;
}

void systemCmd()
{
	LA.Analyzer();
	if (*tokenBuf != '(')
	{
		puts("system(const char *)");
		return;
	}
	LA.Analyzer();
	system(tokenBuf);
	LA.Analyzer();
	if (*tokenBuf != ')')
	{
		puts("system(const char *)");
		return;
	}
	return;
}

void run()
{
	do
	{
		LA.Analyzer();
		if (TT == 变量)
		{
			assignment();
		}
		else if (TT == 行末)
		{
			if (forStackTop != 0)
			{
				ForStack f = forStackPeek();
//				printf("预览f: 变量名:%s , end = %.0lf, step = %.0lf\n", f.name, f.end, f.step);
//				printf("for block = %d, now Block = %d\n", f.block, _Block);
				if (f.block != _Block) //退出条件: 当前代码块不等于for定义所在代码块
				{
					forLoop();
				}
			}

		}
		else
		{
			switch (LA.getCmdType())
			{
			case kw_print:
				print();
				break;
			case kw_input:
				input();
				break;
			case kw_if: //if
				_if();
				break;
			case kw_elif: //else
				elif();
				break;
			case kw_else:
				_else();
				break;
			case kw_endif: //endif
				endif();
				break;
			case kw_for:
				_for();
				break;
			case kw_systemCmd:
				systemCmd();
				break;
			default:
				break;
			}
		}
	} while (TT != 文件末);
//	VT.Order(VT.getRoot());
	VT.DeleteTree();
	forStackTop = 0;
	return;
}

int main(int argc, char **argv)
{
	system("title 简单Python语言解释器");
	char title[128] = { 't', 'i', 't', 'l', 'e', ' ', 0 };
	strcat_s(title, *argv);
	system(title);
	printf("F语言解释器\n输入 \"help\" 以取得更多信息.\n");
	pfile = new char[FILEBUFSIZE];
	forStack = (ForStack *)malloc(FORSTACKSIZE * sizeof(ForStack));
	forStackSize = FORSTACKSIZE;
	if (!pfile)
	{
		cout << "申请内存失败!" << endl;
		exit(1);
	}
	while (true)
	{
		commandTable();
		run();
		putchar('\n');
		printPrompt = false;
	}
	delete []pfile;
	free(forStack);
    return 0;
}