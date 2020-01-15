#pragma once
typedef enum _tokenType
{
	��ʼ,
	��ȷ��,
	�ָ���,
	ע�ͷ�,
	����,
	��ʽ,
	����,
	����,
	�ַ���,
	��ĩ,
	�ļ�ĩ,
	����
} TokenType;

/*
	��������ҪΪ�ҵĽ�����ʵ�ֵĹؼ���
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