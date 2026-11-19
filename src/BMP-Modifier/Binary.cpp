#include "pch.h"
#include "Binary.h"
#include "FileUtils.h"
#include "file.h"

bool Binary::HideMessage(BYTE* buffer, int size, const char* hiddenMessage) {
	FileUtils fu;
	file fm;
	if (!fm.Load(buffer, size)) return false;

	byte* src = buffer;

	BITMAPFILEHEADER bfh;
	memcpy(&bfh, src, sizeof(BITMAPFILEHEADER));
	src += sizeof(BITMAPFILEHEADER);
	if (bfh.bfType != 0x4D42)
		return false;

	BITMAPINFOHEADER bih;
	memcpy(&bih, src, sizeof(BITMAPINFOHEADER));
	src += sizeof(BITMAPINFOHEADER);
	if (bih.biBitCount != 24)
		return false;

	byte* pixelData = buffer + bfh.bfOffBits;
	int width = bih.biWidth;
	int height = abs(bih.biHeight);

	int bytesPerRow = width * 3;
	int padding = (4 - (bytesPerRow % 4)) % 4;
	int rowSize = bytesPerRow + padding;

	const char* binaryMessage = this->GetBinaryMessage(hiddenMessage);
	int msgBinaryLength = fu.StrLength(binaryMessage);

	int bitIndex = 0;

	for (int y = 0; y < height && bitIndex < msgBinaryLength; y++) {
		byte* rowPixel = pixelData + y * rowSize;

		for (int x = 0; x < width && bitIndex < msgBinaryLength; x++) {
			byte* redPixel = &rowPixel[x * 3 + 2];

			if (binaryMessage[bitIndex] == '1') {
				*redPixel = (*redPixel | 0x01);
			}
			else {
				*redPixel = (*redPixel & ~0x01);
			}

			bitIndex++;
		}
	}

	delete[] binaryMessage;

	if (bitIndex < msgBinaryLength) {
		return false;
	}

	return true;
}

bool Binary::ExtractMessage(const char* dest) {
	FileUtils fu;
	FILE* srcFile = fu.GetFile(dest);
	if (srcFile == nullptr) {
		PRINT_ERROR("Error : Cannot open image (ExtractMessage func)")
		return false;
	}

	int size = fu.GetSize(srcFile);
	BYTE* buffer = fu.GetBuffer(size);

	if (fread(buffer, 1, size, srcFile) != size) {
		PRINT_ERROR("Error: failed to read image.")
		fclose(srcFile);
		delete[] buffer;
		return false;
	}
	fclose(srcFile);

	BITMAPFILEHEADER bfh;
	memcpy(&bfh, buffer, sizeof(BITMAPFILEHEADER));

	BITMAPINFOHEADER bih;
	memcpy(&bih, buffer + sizeof(BITMAPFILEHEADER), sizeof(BITMAPINFOHEADER));

	BYTE* pixelData = buffer + bfh.bfOffBits;
	int width = bih.biWidth;
	int height = abs(bih.biHeight);

	int bytesPerRow = width * 3;
	int padding = (4 - (bytesPerRow % 4)) % 4;
	int rowSize = bytesPerRow + padding;

	PRINT_SUCCESS("Message found: ")

	char currentChar = 0;
	int bitCount = 0;
	bool messageFinished = false;

	for (int y = 0; y < height && !messageFinished; y++) {
		BYTE* rowPixel = pixelData + y * rowSize;

		for (int x = 0; x < width && !messageFinished; x++) {
			BYTE redPixel = rowPixel[x * 3 + 2];
			int lsb = redPixel & 0x01;

			currentChar = (currentChar << 1) | lsb;
			bitCount++;

			if (bitCount == 8) {
				if (currentChar == '\0') {
					messageFinished = true;
				}
				else {
					std::cout << currentChar;
				}
				currentChar = 0;
				bitCount = 0;
			}
		}
	}
	std::cout << std::endl;

	delete[] buffer;
	return true;
}

const char* Binary::GetBinaryMessage(const char* message) {
	FileUtils fu;
	int messageLength = fu.StrLength(message);

	int totalBits = (messageLength + 1) * 8;

	char* result = new char[totalBits + 1];
	int index = 0;

	for (int i = 0; i <= messageLength; i++) {
		char character = message[i];

		for (int bit = 7; bit >= 0; bit--) {
			result[index++] = ((character >> bit) & 1) + '0';
		}
	}

	result[index] = '\0';
	PRINT_SUCCESS("The message was put into binary.")
	PRINT_SUCCESS("Binary message: [" << result << "]")

	return result;
}