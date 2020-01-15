#pragma once
typedef enum _tokenType
{
	初始,
	待确认,
	分隔符,
	注释符,
	变量,
	算式,
	数字,
	函数,
	字符串,
	行末,
	文件末,
	不明
} TokenType;

/*
	下面是我要为我的解释器实现的关键字
	kw_if,
	kw_elif,
	kw_for,
	kw_do,
	kw_while,
	kw_break,
	kw_continue,
	kw_def,
	kw_ret,
	kw_call,
	kw_defclass
*/

typedef enum _cmdType
{
	init,
	kw_if,
	kw_elif,
	kw_else,
	kw_endif,
	kw_for,
	kw_do,
	kw_while,
	kw_endwhile,
	kw_print,
	kw_input,
	kw_systemCmd
} cmdType;

typedef struct
{
	char cmdstr[10];
	cmdType cmdenum;
} cmds;

extern char tokenBuf[64];
extern char *pfile;
extern TokenType TT;
extern int _Block;
extern int forStackTop;

class LexicalAnalyzer
{
private:
	cmdType CMD;
public:
	LexicalAnalyzer();
	~LexicalAnalyzer();
	void skipToEOLorEOF();
	TokenType getTokenType();
	cmdType getCmdType();
	cmdType retrieveCmd(char *tokenBuf);
	void Analyzer();
};