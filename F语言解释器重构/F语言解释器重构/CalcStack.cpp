#include "stdafx.h"
#include "CalcStack.h"


CalcStack::CalcStack()
{
	S = new calcStack();
	S->base = new CalcStackET[STACK_INIT_SIZE * sizeof(CalcStackET)];
	if (!S->base)
		exit(OVERFLOW);
	S->top = S->base;
	S->stackSize = STACK_INIT_SIZE;
}

CalcStack::~CalcStack()
{
	delete S->base;
	delete S;
}

status CalcStack::isStackEmpty()
{
	return (S->base == S->top) ? TRUE : FALSE;
}

status CalcStack::push(CalcStackET e)
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

CalcStackET CalcStack::pop()
{
	if (this->isStackEmpty())
		return 0;
	return *(--S->top);
}