#pragma once
class file
{
public:
	int StrLength(const char* str);
	bool CheckFile(BYTE* buffer, int size);
	bool Load(BYTE *buffer, int size);
	FILE* GetFile(const char* dest);
	const int GetSize(FILE* file);
	BYTE* GetBuffer(const int size);
	bool Read(BYTE* buffer, int size, BITMAPFILEHEADER bfh, BITMAPINFOHEADER bih);
	FILE* CloneFile(FILE* srcFile, const char* name, BYTE* buffer, int size);
	bool HasClonedFile(const char* dest);
	bool SwitchColors(BYTE* buffer, int size);
	bool HasSwitchedColors(const char* dest);
	bool Resize(const char* dest, int newWidth, int newHeight);
	bool HideMessage(BYTE* buffer, int size, const char* hiddenMessage);
	bool IsHiddenMessage(const char* dest, const char* hiddenMessage);
};

