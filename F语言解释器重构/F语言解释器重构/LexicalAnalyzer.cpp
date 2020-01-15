#include "stdafx.h"
#include "LexicalAnalyzer.h"

cmds cmd[14] =
{
	"", init,
	"if", kw_if,
	"elif", kw_elif,
	"else", kw_else,
	"endif", kw_endif,
	"for", kw_for,
	"do", kw_do,
	"while", kw_while,
	"endwhile", kw_endwhile,
	"print", kw_print,
	"input", kw_input,
	"system", kw_systemCmd
};

LexicalAnalyzer::LexicalAnalyzer()
{
	pfile = nullptr;
}

LexicalAnalyzer::~LexicalAnalyzer(){}

void LexicalAnalyzer::skipToEOLorEOF()
{
	while (*pfile != '\r' && *pfile != 0)
	{
		++pfile;
	}
	return;
}

TokenType LexicalAnalyzer::getTokenType()
{
	return TT;
}

cmdType LexicalAnalyzer::getCmdType()
{
	return CMD;
}

cmdType LexicalAnalyzer::retrieveCmd(char * tokenBuf)
{
	for (int i = 0; i < 14; ++i)
	{
		if (!strcmp(cmd[i].cmdstr, tokenBuf))
		{
			return cmd[i].cmdenum;
		}
	}
	return init;
}

void LexicalAnalyzer::Analyzer()
{
	char *t = nullptr;
	CMD = init;
	TT = ��ʼ;

	if ( pfile == nullptr || *pfile == 0) //�ж��Ƿ����ļ�ĩ
	{
		tokenBuf[0] = 0;
		if (forStackTop != 0)
		{
			_Block = 0;
			TT = ��ĩ;
		}
		else
		{
			TT = �ļ�ĩ;
		}
		PTB
		return;
	}

	if (*pfile == '\r') //��ĩ
	{
		pfile += 2;
		tokenBuf[0] = 0;
		TT = ��ĩ;
		if (*pfile == 0) // \r\n\0
		{
			_Block = 0;
			return;
		}
		else // \r\n...
		{
			_Block = 0;
			while (*pfile == ' ' || *pfile == '\t')
			{
				if (*pfile == ' ')
				{
					_Block += 1;
				}
				else if (*pfile == '\t')
				{
					_Block += 4;
				}
				++pfile;
			}
			return;
		}
		return;
	}

	while (*pfile == ' ' || *pfile == '\t') //�����ո�
	{
		++pfile;
	}

	if (strchr("+-*/%!:=,()<>", *pfile)) //�Ƿָ���,����Buf
	{
		tokenBuf[0] = *pfile;
		++pfile;
		tokenBuf[1] = 0;
		TT = �ָ���;
		PTB
		return;
	}

	if (*pfile == '\'') //�ַ�
	{
		int i = 0;
		++pfile;
		while (*pfile != '\'')
		{
			tokenBuf[i++] = *pfile++;
		}
		++pfile;
		tokenBuf[i] = 0;
		TT = �ַ���;
		PTB
		return;
	}

	if (*pfile == '"') //�ַ���
	{
		int i = 0;
		++pfile;
		while (*pfile != '"')
		{
			tokenBuf[i++] = *pfile++;
		}
		++pfile;
		tokenBuf[i] = 0;
		TT = �ַ���;
		PTB
		return;
	}

	if (*pfile >= 'a' && *pfile <= 'z' || *pfile >= 'A' && *pfile <= 'Z' || *pfile == '_') //�������ĸ�����ȶ���Buf
	{
		int i = 0;
		while ((*pfile >= 'a' && *pfile <= 'z' || *pfile >= 'A' && *pfile <= 'Z') || (*pfile >= '0' && *pfile <= '9') || *pfile == '_')
		{
			tokenBuf[i++] = *pfile++;
		}
		tokenBuf[i] = 0;
		CMD = retrieveCmd(tokenBuf); //�ں���/�ؼ��ֱ����Ҳ���tokenBuf,����˵�����Ǳ��������Ǻ���
		if (CMD == init)
		{
			TT = ����;
		}
		else
		{
			TT = ����;
		}
		PTB
		return;
	}

	if ((*pfile >= '0' && *pfile <= '9') || strchr("+-.", *pfile)) //�����ֻ���һԪ���ʽ������Buf
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
			return;
		}
		tokenBuf[i] = 0;
		TT = ����;
		Points = 0;
		PTB
		return;
	}

	if (*pfile == '#') // ��ֻ����֧���ں�������ע�ͻ��߿հ���,����,����֮��ĵط���ע�͵Ķ�������
	{
		tokenBuf[0] = 0;
		skipToEOLorEOF();
		TT = ��ĩ;
		PTB
		return;
	}

	TT = ����;
	return;
}