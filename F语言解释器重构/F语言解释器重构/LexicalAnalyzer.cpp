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
	TT = 初始;

	if ( pfile == nullptr || *pfile == 0) //判断是否是文件末
	{
		tokenBuf[0] = 0;
		if (forStackTop != 0)
		{
			_Block = 0;
			TT = 行末;
		}
		else
		{
			TT = 文件末;
		}
		PTB
		return;
	}

	if (*pfile == '\r') //行末
	{
		pfile += 2;
		tokenBuf[0] = 0;
		TT = 行末;
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

	while (*pfile == ' ' || *pfile == '\t') //跳过空格
	{
		++pfile;
	}

	if (strchr("+-*/%!:=,()<>", *pfile)) //是分隔符,读入Buf
	{
		tokenBuf[0] = *pfile;
		++pfile;
		tokenBuf[1] = 0;
		TT = 分隔符;
		PTB
		return;
	}

	if (*pfile == '\'') //字符
	{
		int i = 0;
		++pfile;
		while (*pfile != '\'')
		{
			tokenBuf[i++] = *pfile++;
		}
		++pfile;
		tokenBuf[i] = 0;
		TT = 字符串;
		PTB
		return;
	}

	if (*pfile == '"') //字符串
	{
		int i = 0;
		++pfile;
		while (*pfile != '"')
		{
			tokenBuf[i++] = *pfile++;
		}
		++pfile;
		tokenBuf[i] = 0;
		TT = 字符串;
		PTB
		return;
	}

	if (*pfile >= 'a' && *pfile <= 'z' || *pfile >= 'A' && *pfile <= 'Z' || *pfile == '_') //如果是字母，就先读入Buf
	{
		int i = 0;
		while ((*pfile >= 'a' && *pfile <= 'z' || *pfile >= 'A' && *pfile <= 'Z') || (*pfile >= '0' && *pfile <= '9') || *pfile == '_')
		{
			tokenBuf[i++] = *pfile++;
		}
		tokenBuf[i] = 0;
		CMD = retrieveCmd(tokenBuf); //在函数/关键字表中找不到tokenBuf,就是说可能是变量或者是函数
		if (CMD == init)
		{
			TT = 变量;
		}
		else
		{
			TT = 函数;
		}
		PTB
		return;
	}

	if ((*pfile >= '0' && *pfile <= '9') || strchr("+-.", *pfile)) //是数字或者一元表达式，读入Buf
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
			return;
		}
		tokenBuf[i] = 0;
		TT = 数字;
		Points = 0;
		PTB
		return;
	}

	if (*pfile == '#') // 我只打算支持在函数后面注释或者空白行,行首,在这之外的地方搞注释的都是智障
	{
		tokenBuf[0] = 0;
		skipToEOLorEOF();
		TT = 行末;
		PTB
		return;
	}

	TT = 不明;
	return;
}