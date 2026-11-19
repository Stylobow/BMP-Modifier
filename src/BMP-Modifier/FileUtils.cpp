#include "pch.h"
#include "FileUtils.h"

/* Fonction StrLength */
int FileUtils::StrLength(const char* str) {
	int lenght = 0;
	while (str[lenght] != '\0') {
		lenght++;
	}
	return lenght;
}

/* Fonction CheckFile */
bool FileUtils::CheckFile(BYTE* buffer, int size) {
	if (buffer == nullptr || size == 0) {
		return false;
	}
	return true;
}

/* Fonction CheckSize */
bool FileUtils::CheckSize(int width, int height) {
	if (width <= 0 || height <= 0) {
		return false;
	}
	return true;
}

/* Fonction GetFile */
FILE* FileUtils::GetFile(const char* dest) {
	FILE* file = nullptr;

	errno_t err = fopen_s(&file, dest, "rb");

	if (err != 0) {
		std::cout << "Error code : " << err << std::endl;
		return 0;
	}

	return file;
}

/* Fonction GetSize */
const int FileUtils::GetSize(FILE* file) {
	fseek(file, 0, SEEK_END);
	const int size = ftell(file);
	fseek(file, 0, SEEK_SET);

	return size;
}

/* Fonction GetBuffer */
BYTE* FileUtils::GetBuffer(const int size) {
	BYTE* buffer = new BYTE[size];

	return buffer;
}
