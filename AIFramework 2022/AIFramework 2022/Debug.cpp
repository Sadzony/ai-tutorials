#include "Debug.h"

void Debug::LogVal(int value)
{
	char size[256] = { 0 };
	sprintf_s(size, "%d", value);
	OutputDebugStringA(size);
}

void Debug::LogVal(float value)
{
	char size[256] = { 0 };
	sprintf_s(size, "%f", value);
	OutputDebugStringA(size);
}

void Debug::LogString(string str)
{
	OutputDebugStringA(str.c_str());
}
void Debug::LogString(char* str)
{
	OutputDebugStringA(str);
}
