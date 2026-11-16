#pragma once
class file
{
public:
	bool CheckFile(BYTE* buffer, int size);
	bool Load(BYTE *buffer, int size);
	void Write();
	bool Read(BYTE* buffer, int size, BITMAPFILEHEADER bfh, BITMAPINFOHEADER bih);
	FILE* CloneFile(FILE* srcFile, const char* name, BYTE* buffer, int size);
};

