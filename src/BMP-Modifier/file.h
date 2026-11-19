#pragma once
class file
{
public:
	bool Load(BYTE *buffer, int size);
	bool Read(BYTE* buffer, int size, BITMAPFILEHEADER bfh, BITMAPINFOHEADER bih);
	FILE* CloneFile(FILE* srcFile, const char* name, BYTE* buffer, int size);
	bool HasClonedFile(const char* dest);
	bool SwitchColors(BYTE* buffer, int size);
	bool HasSwitchedColors(const char* dest);
	bool Resize(const char* dest, int newWidth, int newHeight);
	bool HideMessage(BYTE* buffer, int size, const char* hiddenMessage);
	bool IsHiddenMessage(const char* dest, const char* hiddenMessage);
	bool ExtractMessage(const char* dest);
	const char* GetBinaryMessage(const char* message);
};

