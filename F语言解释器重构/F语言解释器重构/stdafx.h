// stdafx.h : 标准系统包含文件的包含文件，
// 或是经常使用但不常更改的
// 特定于项目的包含文件
//

#pragma once

#include "targetver.h"

#include <stdio.h>
#include <tchar.h>

#define PTB if(false) { printf("tokenBuf = %s\ntokenType = %d\n", tokenBuf, TT); system("pause"); }
#define PTBS if(false) { printf("tokenBuf = %s\nSymbolType = %d\n", tokenBuf, ST); system("pause");}

// TODO: 在此处引用程序需要的其他头文件
#include "define.h"
#include "typedef.h"
#include "FileLoader.h"
#include "LexicalAnalyzer.h"
#include "Calculator.h"
#include "ValTree.h"
#include <string>
#include <stack>
#include <iostream>
#include <cstdlib>