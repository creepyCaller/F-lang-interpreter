#include "stdafx.h"
#include "Calculator.h"

Calculator::Calculator()
{
}


Calculator::~Calculator()
{

}

Calculator::EStack::EStack() //�۲�����ģ���Σ��
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
		return 0; //���벻������return 0����ķ���
	--S->top;
//	printf("cs pop %lf\n", *S->top);
	return *S->top;
}

void Calculator::AnalyzerAndParser::Analyzer()
{
	char *t = nullptr;

	while (*pfile == ' ' || *pfile == '\t') //�����ո�
	{
		++pfile;
	}
	
	if (strchr(",\r\0:", *pfile))
	{
		tokenBuf[0] = *pfile;
		tokenBuf[1] = 0;
		ST = _�˳�;
		return;
	}
	
	if ((*pfile >= '0' && *pfile <= '9') || *pfile == '.') //�����ֻ���һԪ���ʽ������Buf
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
			printf("ERR: '%s' �ﺬ�ж��С����!\n", tokenBuf);
			exit(2);
		}
		tokenBuf[i] = 0;
		ST = _����;
		Points = 0;
		return;
	}

	if ((*pfile >= 'a' && *pfile <= 'z') || (*pfile >= 'A' && *pfile <= 'Z') || *pfile == '_') //�������ĸ(������)������Buf
	{
		int i = 0;
		while ((*pfile >= 'a' && *pfile <= 'z') || (*pfile >= 'A' && *pfile <= 'Z') || (*pfile >= '0' && *pfile <= '9') || *pfile == '_')
		{
			tokenBuf[i++] = *pfile++;
		}
		tokenBuf[i] = 0;
		ST = _����;
		return;
	}

	if (strchr("+-*/%()&|!=><", *pfile)) //�����,����Buf
	{
		tokenBuf[0] = *pfile++;
		tokenBuf[1] = 0;
		switch (tokenBuf[0])
		{
		case '(':
			ST = _����;
			break;
		case ')':
			ST = _������;
			break;
		case '+':
			ST = _��;
			break;
		case '-':
			ST = _��;
			break;
		case '*':
			ST = _��;
			break;
		case '/':
			ST = _��;
			break;
		case '%':
			ST = _��;
			break;
		case '&':
			ST = _��;
			break;
		case '|':
			ST = _��;
			break;
		case '!':
			ST = _��;
			break;
		case '=':
			ST = _��;
			break;
		case '>':
			ST = _����;
			break;
		case '<':
			ST = _С��;
			break;
		}
		return;
	}
	if (*pfile == '#') // ��ֻ����֧���ں���������߿հ���,����ע��,��������֮��ĵط���ע�͵Ķ�������
	{
		tokenBuf[0] = 0;
		ST = _�˳�;
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
		tempE.type = __����;
		switch (ST)
		{
		case _����:
			tempE.type = __����;
			tempE.num = atof(tokenBuf);
			output.push(tempE);
			break;
		case _����:
			tempE.type = __����;
			if (!VT.Serch(tokenBuf, &tempE.num)) //ȡ����
			{
				printf("δ�������: %s\n", tokenBuf);
				ST = _�˳�;
			}
			output.push(tempE);
			break;
		case _��:
		case _��:
			while (!eStack.isStackEmpty() && (eStack.peek() != '(' && eStack.peek() != ')')) // ջ��Ϊ��,ջ��Ԫ�ز�Ϊ����
			{
				tempE.op = eStack.pop();
				output.push(tempE);
			}
			eStack.push(*tokenBuf);
			break;
		case _��:
		case _��:
		case _��:
			while (!eStack.isStackEmpty() && (eStack.peek() == '*' || eStack.peek() == '/' || eStack.peek() == '%')) // ջ��Ϊ��,ջ��Ԫ��Ϊ [* / %]
			{
				tempE.op = eStack.pop();
				output.push(tempE);
			}
			eStack.push(*tokenBuf);
			break;
		case _����:
			eStack.push(*tokenBuf);
			break;
		case _������:
			while (!eStack.isStackEmpty() && eStack.peek() != '(') //���������
			{
				tempE.op = eStack.pop();
				output.push(tempE);
			}
			if (!eStack.isStackEmpty()) // ��������
			{
				eStack.pop();
				*tokenBuf = 0;
			}
			if (*tokenBuf == ')')
			{
				--pfile;
				ST = _�˳�;
			}
			break;
		case _ע��:
			while (*pfile != '\r' && *(pfile + 2) != 0) //ֱ�����л����ļ�ĩβ
			{
				++pfile;
			}
			pfile += 2;
			ST = _�˳�;
			break;
		default:
			break;
		}
	} while (ST != _�˳�);
	while (!eStack.isStackEmpty())
	{
		tempE.op = eStack.pop();
		output.push(tempE);
	}
	itor = output.begin();
//	printf("end = %x\nbeg = %x\nitr = %x\n", output.end(), output.begin(), itor);
	while (itor != output.end())
	{
		if (itor->type == __����)
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
					printf("��������Ϊ0\n");
					return -3.0;
				}
				calcStack.push((double)((int)L % (int)R));
				break;
			case '/':
				if (R == 0)
				{
					printf("��������Ϊ0\n");
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
