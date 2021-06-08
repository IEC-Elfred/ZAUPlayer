#pragma once
#include <cstdio>

class MMThread
{
public:
	virtual void run() = 0;
	int Start();
};