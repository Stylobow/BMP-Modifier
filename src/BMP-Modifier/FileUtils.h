#pragma once

class FileUtils
{
public:
	static int StrLength(const char* str);
	static bool CheckFile(BYTE* buffer, int size);
	static bool CheckSize(int width, int height);
	static FILE* GetFile(const char* dest);
	static const int GetSize(FILE* file);
	static BYTE* GetBuffer(const int size);
};
