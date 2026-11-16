#pragma once
class file
{
public:
	bool CheckFile(BYTE* buffer, int size);
	bool Load(BYTE *buffer, int size);
	void Save();
	void Write();
	bool Read(BYTE* buffer, int size, BITMAPFILEHEADER bfh, BITMAPINFOHEADER bih);
};

