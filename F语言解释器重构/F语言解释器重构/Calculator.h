#pragma once
#define TRUE 1
#define FALSE 0
#define OK 1
#define ERROR 0
#define INFEASIBLE - 1
#define STACK_INIT_SIZE 50
#define STACK_INCREMENT 5

using namespace std;

typedef enum _SymbolType
{
	_��ʼ,
	_����,
	_��,
	_��,
	_��,
	_��,
	_��,
	_��,
	_��,
	_��,
	_��,
	_����,
	_С��,
	_����,
	_������,
	_����,
	_��ĩ,
	_ע��,
	_�˳�
} SymbolType;

typedef enum _ListEType
{
	__����,
	__����
} OutputStackEType;

typedef struct {
	double num;
	char op;
	OutputStackEType type;
} OutputET;

typedef int status;
typedef char EStackET;
typedef double CalcStackET;

//ȫ�ֱ���
extern class LexicalAnalyzer LA;
extern class ValTree VT;
extern char *pfile;
extern char tokenBuf[64];
extern TokenType TT;

typedef struct
{
	OutputET *base;
	OutputET *top;
	int stackSize;
} OutputStackET;

typedef struct {
	EStackET *base;
	EStackET *top;
	int stackSize;
} EStackT;

typedef struct {
	CalcStackET *base;
	CalcStackET *top;
	int stackSize;
} CalcStackT;

class Calculator
{
public:
	Calculator();
	~Calculator();
	class OutputStack
	{
	public:
		OutputStack();
		~OutputStack();
		void push(OutputET e);
		OutputET pop();
		OutputET * begin();
		OutputET * end();
		void clear();
	private:
		OutputStackET * S;
	};
	class EStack
	{
	public:
		EStack();
		~EStack();
		status isStackEmpty();
		status push(EStackET e);
		EStackET peek();
		EStackET pop();
	private:
		EStackT * S;
	};
	class CalcStack
	{
	public:
		CalcStack();
		~CalcStack();
		status isStackEmpty();
		status push(CalcStackET e);
		CalcStackET pop();
	private:
		CalcStackT * S;
	};
	class AnalyzerAndParser
	{
	private:
		SymbolType ST = _��ʼ;
		OutputET *itor;
		OutputET tempE;
		char tokenBuf[64];
	public:
		void Analyzer();
		double calc();
	};
	void getVal(double * exp);
};