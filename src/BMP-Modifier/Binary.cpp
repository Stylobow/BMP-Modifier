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

	// Convertit le message en binaire
	const char* binaryMessage = this->GetBinaryMessage(hiddenMessage);
	int msgBinaryLength = fu.StrLength(binaryMessage);

	int bitIndex = 0;

	// On parcout l'image sur toute la hauteur tant que il y a encore le message a cacher
	for (int y = 0; y < height && bitIndex < msgBinaryLength; y++) {
		byte* rowPixel = pixelData + y * rowSize;
		
		//Même chose pour l'axe x, la la longueur
		for (int x = 0; x < width && bitIndex < msgBinaryLength; x++) {
			byte* redPixel = &rowPixel[x * 3 + 2];

			if (binaryMessage[bitIndex] == '1') {
				// 0x01 = 00000001 en binaire -> avec "|" ça force le dernier bit à 1
				*redPixel = (*redPixel | 0x01);
			}
			else {
				// ~0x01 = 11111110 en binaire -> avec "&" ça force le dernier bit à 0
				*redPixel = (*redPixel & ~0x01);
			}

			bitIndex++;
		}
	}

	delete[] binaryMessage;

	if (bitIndex < msgBinaryLength) { // Sécurité si l'image est trop petite pour le message en entier
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
			int lsb = redPixel & 0x01; // On récupère le LSB du pixel rouge

			// Décale les bits déjà là vers la gauche pour faire de la place, puis ajoute le nouveau bit à droite
			currentChar = (currentChar << 1) | lsb;
			bitCount++;

			if (bitCount == 8) { // Quand on a un caractère complet
				if (currentChar == '\0') { // La fin du message
					messageFinished = true;
				}
				else {
					std::cout << currentChar;
				}
				// Remet à zéro pour lire le prochain caractère.
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

	// + 1 pour inclure le '\0' à la toute fin du message et * 8 car 1 caractère = 8 bits
	int totalBits = (messageLength + 1) * 8;

	char* result = new char[totalBits + 1]; // + 1 pour le '\0'
	int index = 0;

	for (int i = 0; i <= messageLength; i++) {
		char character = message[i];

		// On part du bit 7, le plus à gauche vers le bit 0, le plus à droite
		for (int bit = 7; bit >= 0; bit--) {
			// Décale le bit voulu tout à droite, garde juste ce bit (& 1), et ajoute 0 pour avoir 0 ou 1
			result[index++] = ((character >> bit) & 1) + '0';
		}
	}

	result[index] = '\0'; // Fin du message
	PRINT_SUCCESS("The message was put into binary.")
	PRINT_SUCCESS("Binary message: [" << result << "]")

	return result;
}