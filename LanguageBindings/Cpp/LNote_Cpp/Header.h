#pragma once
#include <stdio.h>
class Test1
{
public:
	static int Get()
	{
		static int v = 0;
		v++;
		return v;
	}
};
