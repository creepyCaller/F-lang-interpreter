#include "stdafx.h"
#include "Calculator.h"

Calculator::Calculator()
{
}


Calculator::~Calculator()
{

}

Calculator::EStack::EStack() //论不会用模板的危害
{
	S = new EStackT();
	S->base = new EStackET[STACK_INIT_SIZE * sizeof(EStackET)];
	if (!S->base)
		exit(OVERFLOW);
	S->top = S->base;
	S->stackSize = STACK_INIT_SIZE;
}

Calculator::EStack::~EStack()
{
	delete S;
}

status Calculator::EStack::isStackEmpty()
{
	return (S->base == S->top) ? TRUE : FALSE;
}

status Calculator::EStack::push(EStackET e)
{
	if (S->top - S->base >= S->stackSize)
	{
		S->base = (EStackET *)realloc(S->base, (S->stackSize + STACK_INCREMENT) * sizeof(EStackET));
		if (!S->base)
			exit(OVERFLOW);
		S->top = S->base + S->stackSize;
		S->stackSize += STACK_INCREMENT;
	}
	*S->top++ = e;
	return OK;
}

EStackET Calculator::EStack::peek()
{
	if (this->isStackEmpty())
		exit(ERROR);
	return *(S->top - sizeof(EStackET));
}

EStackET Calculator::EStack::pop()
{
	if (this->isStackEmpty())
		exit(ERROR);
	return *(--S->top);
	
}

Calculator::CalcStack::CalcStack()
{
	S = new CalcStackT();
	S->base = new CalcStackET[STACK_INIT_SIZE * sizeof(CalcStackET)];
	if (!S->base)
		exit(OVERFLOW);
	S->top = S->base;
	S->stackSize = STACK_INIT_SIZE;
}

Calculator::CalcStack::~CalcStack()
{
	delete S;
}

status Calculator::CalcStack::isStackEmpty()
{
	return (S->base == S->top) ? TRUE : FALSE;
}

status Calculator::CalcStack::push(CalcStackET e)
{
	if (S->top - S->base >= S->stackSize)
	{
		S->base = (CalcStackET *)realloc(S->base, (S->stackSize + STACK_INCREMENT) * sizeof(CalcStackET));
		if (!S->base)
			exit(OVERFLOW);
		S->top = S->base + S->stackSize;
		S->stackSize += STACK_INCREMENT;
	}
	*S->top++ = e;
	return OK;
}

CalcStackET Calculator::CalcStack::pop()
{
	if (this->isStackEmpty())
		return 0; //我想不出除了return 0以外的方法
	--S->top;
//	printf("cs pop %lf\n", *S->top);
	return *S->top;
}

void Calculator::AnalyzerAndParser::Analyzer()
{
	char *t = nullptr;

	while (*pfile == ' ' || *pfile == '\t') //跳过空格
	{
		++pfile;
	}
	
	if (strchr(",\r\0:", *pfile))
	{
		tokenBuf[0] = *pfile;
		tokenBuf[1] = 0;
		ST = _退出;
		return;
	}
	
	if ((*pfile >= '0' && *pfile <= '9') || *pfile == '.') //是数字或者一元表达式，读入Buf
	{
		int i = 0;
		int Points = 0;
		if (*pfile == '.')
		{
			++Points;
		}
		while (*pfile >= '0' && *pfile <= '9' || *pfile == '.')
		{
			tokenBuf[i++] = *pfile++;
			if (*pfile == '.')
			{
				++Points;
			}
		}
		if (Points > 1 || Points < 0)
		{
			printf("ERR: '%s' 里含有多个小数点!\n", tokenBuf);
			exit(2);
		}
		tokenBuf[i] = 0;
		ST = _数字;
		Points = 0;
		return;
	}

	if ((*pfile >= 'a' && *pfile <= 'z') || (*pfile >= 'A' && *pfile <= 'Z') || *pfile == '_') //如果是字母(变量名)，读入Buf
	{
		int i = 0;
		while ((*pfile >= 'a' && *pfile <= 'z') || (*pfile >= 'A' && *pfile <= 'Z') || (*pfile >= '0' && *pfile <= '9') || *pfile == '_')
		{
			tokenBuf[i++] = *pfile++;
		}
		tokenBuf[i] = 0;
		ST = _变量;
		return;
	}

	if (strchr("+-*/%()&|!=><", *pfile)) //是算符,读入Buf
	{
		tokenBuf[0] = *pfile++;
		tokenBuf[1] = 0;
		switch (tokenBuf[0])
		{
		case '(':
			ST = _括号;
			break;
		case ')':
			ST = _反括号;
			break;
		case '+':
			ST = _加;
			break;
		case '-':
			ST = _减;
			break;
		case '*':
			ST = _乘;
			break;
		case '/':
			ST = _除;
			break;
		case '%':
			ST = _余;
			break;
		case '&':
			ST = _与;
			break;
		case '|':
			ST = _或;
			break;
		case '!':
			ST = _非;
			break;
		case '=':
			ST = _等;
			break;
		case '>':
			ST = _大于;
			break;
		case '<':
			ST = _小于;
			break;
		}
		return;
	}
	if (*pfile == '#') // 我只打算支持在函数后面或者空白行,行首注释,反正在这之外的地方搞注释的都是智障
	{
		tokenBuf[0] = 0;
		ST = _退出;
		return;
	}
	return;
}

double Calculator::AnalyzerAndParser::calc()
{
	EStack eStack;
	CalcStack calcStack;
	OutputStack output;
	do
	{
		Analyzer();
		tempE.num = 0;
		tempE.op = 0;
		tempE.type = __符号;
		switch (ST)
		{
		case _数字:
			tempE.type = __数字;
			tempE.num = atof(tokenBuf);
			output.push(tempE);
			break;
		case _变量:
			tempE.type = __数字;
			if (!VT.Serch(tokenBuf, &tempE.num)) //取变量
			{
				printf("未定义变量: %s\n", tokenBuf);
				ST = _退出;
			}
			output.push(tempE);
			break;
		case _加:
		case _减:
			while (!eStack.isStackEmpty() && (eStack.peek() != '(' && eStack.peek() != ')')) // 栈不为空,栈顶元素不为括号
			{
				tempE.op = eStack.pop();
				output.push(tempE);
			}
			eStack.push(*tokenBuf);
			break;
		case _乘:
		case _除:
		case _余:
			while (!eStack.isStackEmpty() && (eStack.peek() == '*' || eStack.peek() == '/' || eStack.peek() == '%')) // 栈不为空,栈顶元素为 [* / %]
			{
				tempE.op = eStack.pop();
				output.push(tempE);
			}
			eStack.push(*tokenBuf);
			break;
		case _括号:
			eStack.push(*tokenBuf);
			break;
		case _反括号:
			while (!eStack.isStackEmpty() && eStack.peek() != '(') //弹出运算符
			{
				tempE.op = eStack.pop();
				output.push(tempE);
			}
			if (!eStack.isStackEmpty()) // 弹出括号
			{
				eStack.pop();
				*tokenBuf = 0;
			}
			if (*tokenBuf == ')')
			{
				--pfile;
				ST = _退出;
			}
			break;
		case _注释:
			while (*pfile != '\r' && *(pfile + 2) != 0) //直到换行或者文件末尾
			{
				++pfile;
			}
			pfile += 2;
			ST = _退出;
			break;
		default:
			break;
		}
	} while (ST != _退出);
	while (!eStack.isStackEmpty())
	{
		tempE.op = eStack.pop();
		output.push(tempE);
	}
	itor = output.begin();
//	printf("end = %x\nbeg = %x\nitr = %x\n", output.end(), output.begin(), itor);
	while (itor != output.end())
	{
		if (itor->type == __数字)
		{
			calcStack.push(itor->num);
		}
		else
		{
			double R = calcStack.pop(), L = calcStack.pop();
			switch (itor->op)
			{
			case '+':
				calcStack.push(L + R);
				break;
			case '-':
				calcStack.push(L - R);
				break;
			case '*':
				calcStack.push(L * R);
				break;
			case '%':
				if (R == 0)
				{
					printf("余数不能为0\n");
					return -3.0;
				}
				calcStack.push((double)((int)L % (int)R));
				break;
			case '/':
				if (R == 0)
				{
					printf("除数不能为0\n");
					return -4.0;
				}
				calcStack.push(L / R);
				break;
			}
		}
		++itor;
//		printf("end = %x\nbeg = %x\nitr = %x\n", output.end(), output.begin(), itor);
//		system("pause");
	}
	output.clear();
	return calcStack.pop();
}

void Calculator::getVal(double * exp)
{
	AnalyzerAndParser AP;
	*exp = AP.calc();
	return;
}

Calculator::OutputStack::OutputStack()
{
	S = new OutputStackET();
	S->base = new OutputET[STACK_INIT_SIZE * sizeof(OutputET)];
	if (!S->base)
	{
		exit(OVERFLOW);
	}
	S->top = S->base;
	S->stackSize = STACK_INIT_SIZE;
}

Calculator::OutputStack::~OutputStack()
{
	delete S;
}

void Calculator::OutputStack::push(OutputET e)
{
	if (S->top - S->base >= S->stackSize)
	{
		S->base = (OutputET *)realloc(S->base, (S->stackSize + STACK_INCREMENT) * sizeof(OutputET));
		if (!S->base)
			exit(OVERFLOW);
		S->top = S->base + S->stackSize;
		S->stackSize += STACK_INCREMENT;
	}
	*S->top++ = e;
	return;
}

OutputET Calculator::OutputStack::pop()
{
	if (S->base == S->top)
	{
		exit(ERROR);
	}
	--S->top;
	return *(S->top);
}

OutputET * Calculator::OutputStack::begin()
{
	return S->base;
}

OutputET * Calculator::OutputStack::end()
{
	if (S->base == S->top)
	{
		exit(ERROR);
	}
	return S->top;
}

void Calculator::OutputStack::clear()
{
	S->top = S->base;
	return;
}
