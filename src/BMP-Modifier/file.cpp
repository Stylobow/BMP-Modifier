#include "pch.h"
#include "file.h"
#include "FileUtils.h"
#include "Binary.h"

/* Fonction Load */
bool file::Load(BYTE* buffer, int size) {
	FileUtils fu;
	// Initialisation des Header BMP.
	BITMAPFILEHEADER bfh;
	BITMAPINFOHEADER bih;

	// Vérifie si le buffer ou la size est vide.
	if (!fu.CheckFile(buffer, size)) {
		return false;
	}

	// Copie du buffer.
	BYTE* src = buffer;

	/* Bloc BITMAP FILE HEADER */
	memcpy(&bfh, src, sizeof(BITMAPFILEHEADER));
	src += sizeof(BITMAPFILEHEADER);

	// Vérifie si l'image correspond bien a une signature BM
	if (bfh.bfType != 0x4D42) {
		return false;
	}

	/* Bloc BITMAP INFO HEADER */
	memcpy(&bih, src, sizeof(BITMAPINFOHEADER));
	src = buffer + bfh.bfOffBits;

	// Vérifie si le nombre de bit est bien égal au format du BMP.
	if (bih.biBitCount != 24) {
		return false;
	}

	// Récupération des informations Hauteur et Largeur.
	int width = bih.biWidth;
	int height = abs(bih.biHeight);

	// Vérifie si le fichier n'est pas nul.
	if (!fu.CheckSize(width, height)) {
		return false;
	}

	return true;
}

/* Fonction Read */
bool file::Read(BYTE* buffer, int size, BITMAPFILEHEADER bfh, BITMAPINFOHEADER bih) {
	FileUtils fu;
	// Vérifie si le buffer ou la size est vide.
	if (!fu.CheckFile(buffer, size)) {
		return false;
	}

	BYTE* pixelData = buffer + bfh.bfOffBits;

	// Récupération des informations Hauteur et Largeur.
	int width = bih.biWidth;
	int height = abs(bih.biHeight);

	if (!fu.CheckSize(width, height)) {
		return false;
	}

	// Nombre de Bytes par ligne
	int bytesPerRow = width * 3;
	// Padding
	int padding = (4 - (bytesPerRow % 4)) % 4;
	// Taille de la ligne
	int rowSize = bytesPerRow + padding;

	// Double boucle pour parcourir la hauteur (y) et la largeur (x).
	for (int y = 0; y < height; y++) {
		BYTE* rowPixel = pixelData + y * rowSize;

		for (int x = 0; x < width; x++) {
			BYTE bluePixel = static_cast<BYTE>(rowPixel[x * 3 + 0]);
			BYTE greenPixel = static_cast<BYTE>(rowPixel[x * 3 + 1]);
			BYTE redPixel = static_cast<BYTE>(rowPixel[x * 3 + 2]);

			std::cout << "Pixel (" << x << ", " << y << ") : "
				<< "R= " << static_cast<int>(redPixel) << " "
				<< "G= " << static_cast<int>(greenPixel) << " "
				<< "B= " << static_cast<int>(bluePixel) << "\n";
		}
	}

	return true;
}

bool file::HasSwitchedColors(const char* dest) {
	FileUtils fu;
	FILE* srcFile = fu.GetFile(dest);
	if (srcFile == nullptr) {
		PRINT_ERROR("An error occurred while opening the image.")
			return false;
	}

	int size = fu.GetSize(srcFile);
	BYTE* buffer = fu.GetBuffer(size);

	if (fread(buffer, 1, size, srcFile) != size) {
		PRINT_ERROR("An error occurred while reading the image.")
		fclose(srcFile);
		delete[] buffer;
		return false;
	}
	fclose(srcFile);

	bool switchColor = this->SwitchColors(buffer, size);

	if (switchColor) {
		FILE* newFile = nullptr;
		if (fopen_s(&newFile, "switched.bmp", "wb") == 0 && newFile != nullptr) {
			fwrite(buffer, 1, size, newFile);
			fclose(newFile);
			PRINT_SUCCESS("The colors of the image have been successfully changed.");
		}
		else {
			PRINT_ERROR("An error occurred while saving the new image.")
				switchColor = false;
		}
	}
	else {
		PRINT_ERROR("An error occurred while changing the colors of the image.")
	}

	delete[] buffer;
	return switchColor;
}

/* Fonction SwitchColors */
bool file::SwitchColors(BYTE* buffer, int size) {

	if (!Load(buffer, size)) return false;

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

	for (int y = 0; y < height; y++) {
		byte* rowPixel = pixelData + y * rowSize;

		for (int x = 0; x < width; x++) {
			byte* bluePixel = &rowPixel[x * 3 + 0];
			byte* greenPixel = &rowPixel[x * 3 + 1];
			byte* redPixel = &rowPixel[x * 3 + 2];

			byte blueTemp = *bluePixel;
			byte greenTemp = *greenPixel;
			byte redTemp = *redPixel;

			*bluePixel = greenTemp;
			*greenPixel = redTemp;
			*redPixel = blueTemp;
		}
	}

	return true;
}

/* HasClonedFile */
bool file::HasClonedFile(const char* dest) {
	FileUtils fu;
	FILE* srcFile = fu.GetFile(dest);
	if (srcFile == nullptr) {
		PRINT_ERROR("Error: Source file not found or cannot be opened.")
			return false;
	}

	int size = fu.GetSize(srcFile);
	BYTE* buffer = fu.GetBuffer(size);

	bool success = this->CloneFile(srcFile, dest, buffer, size);

	fclose(srcFile);
	delete[] buffer;

	if (!success) {
		PRINT_ERROR("Error while cloning file.")
			return false;
	}

	return true;
}

/* Fonction CloneFile */
FILE* file::CloneFile(FILE* srcFile, const char* name, BYTE* buffer, int size) {
	FileUtils fu;
	if (srcFile == nullptr || !fu.CheckFile(buffer, size)) {
		return nullptr;
	}

	if (fread(buffer, 1, size, srcFile) != size) {
		PRINT_ERROR("Error reading source file.")
			return nullptr;
	}

	if (!file::Load(buffer, size)) {
		PRINT_ERROR("Error reading the image. Check its format and color depth, which must be 24.")
			return nullptr;
	}

	FILE* destFile = nullptr;
	errno_t err = fopen_s(&destFile, "clone.bmp", "wb");
	if (err != 0 || destFile == nullptr) {
		PRINT_ERROR("Error, Failed to clone file, error code : " << err << )
			return nullptr;
	}

	if (fwrite(buffer, 1, size, destFile) != size) {
		PRINT_ERROR("Error: Failed to write complete file.")
			fclose(destFile);
		return nullptr;
	}

	fclose(destFile);
	PRINT_SUCCESS("Clone success! File saved as 'clone.bmp'.")
		return destFile;
}

/* Fonction Resize */
bool file::Resize(const char* dest, int newWidth, int newHeight) {
	FileUtils fu;
	// Vérifie que la taille de l'image sois pas nul.
	if (newWidth <= 0 || newHeight <= 0) {
		PRINT_ERROR("Width and Height cannot be lower than 1")
			return false;
	}

	FILE* srcFile = fu.GetFile(dest); // Ont récupère et lit le fichier a resize.
	// Vérifie si srcFile pointe bien sur un fichier.
	if (srcFile == nullptr) {
		PRINT_ERROR("Error while opening file\n")
			return false;
	}

	int size = fu.GetSize(srcFile);
	BYTE* buffer = fu.GetBuffer(size);

	if (fread(buffer, 1, size, srcFile) != size) {
		PRINT_ERROR("An error ocurred\n")
			delete[] buffer;
		return false;
	}
	fclose(srcFile);

	if (!Load(buffer, size)) {
		PRINT_ERROR("An error ocurred\n")
			delete[] buffer;
		return false;
	}

	// Copie de buffer.
	BYTE* src = buffer;
	BITMAPFILEHEADER bfh;
	memcpy(&bfh, src, sizeof(BITMAPFILEHEADER));
	src += sizeof(BITMAPFILEHEADER);

	BITMAPINFOHEADER bih;
	memcpy(&bih, src, sizeof(BITMAPINFOHEADER));
	src += sizeof(BITMAPINFOHEADER);

	int oldWidth = bih.biWidth;
	int oldHeight = abs(bih.biHeight);

	int oldBytesPerRow = oldWidth * 3;
	int oldPadding = (4 - (oldBytesPerRow % 4)) % 4;
	int oldRowSize = oldBytesPerRow + oldPadding;

	int newBytesPerRow = newWidth * 3;
	int newPadding = (4 - (newBytesPerRow % 4)) % 4;
	int newRowSize = newBytesPerRow + newPadding;

	int newPixelDataSize = newRowSize * newHeight;
	int headerSize = sizeof(BITMAPFILEHEADER) + sizeof(BITMAPINFOHEADER);
	int newTotalSize = headerSize + newPixelDataSize;

	BYTE* newBuffer = new BYTE[newTotalSize];
	memset(newBuffer, 0, newTotalSize);

	BITMAPFILEHEADER newBfh = bfh;
	BITMAPINFOHEADER newBih = bih;

	newBih.biWidth = newWidth;
	if (bih.biHeight < 0) {
		newBih.biHeight = -newHeight;
	}
	else {
		newBih.biHeight = newHeight;
	}
	newBih.biSizeImage = newPixelDataSize;
	newBfh.bfSize = newTotalSize;
	newBfh.bfOffBits = headerSize;

	memcpy(newBuffer, &newBfh, sizeof(BITMAPFILEHEADER));
	memcpy(newBuffer + sizeof(BITMAPFILEHEADER), &newBih, sizeof(BITMAPINFOHEADER));

	BYTE* oldPixelData = buffer + bfh.bfOffBits;
	BYTE* newPixelData = newBuffer + newBfh.bfOffBits;

	for (int y = 0; y < newHeight; y++) {
		int oldY = y * oldHeight / newHeight;
		BYTE* newRow = newPixelData + y * newRowSize;
		BYTE* oldRow = oldPixelData + oldY * oldRowSize;

		for (int x = 0; x < newWidth; x++) {
			int oldX = x * oldWidth / newWidth;

			BYTE* newPixel = &newRow[x * 3];
			BYTE* oldPixel = &oldRow[oldX * 3];

			newPixel[0] = oldPixel[0]; // Blue
			newPixel[1] = oldPixel[1]; // Green
			newPixel[2] = oldPixel[2]; // Red
		}
	}

	FILE* newFile = nullptr;
	errno_t err = fopen_s(&newFile, "resized.bmp", "wb");
	if (err == 0 && newFile != nullptr) {
		fwrite(newBuffer, 1, newTotalSize, newFile);
		fclose(newFile);
		PRINT_SUCCESS("The image has been successfully resized and saved as 'resized.bmp'.")
	}
	else {
		PRINT_ERROR("An error occurred while saving the new resized image.")
		delete[] buffer;
		delete[] newBuffer;
		return false;
	}

	delete[] buffer;
	delete[] newBuffer;
	return true;
}

/* Fonction IsHiddenMessage */
bool file::IsHiddenMessage(const char* dest, const char* hiddenMessage) {
	FileUtils fu;
	Binary binary;
	FILE* srcFile = fu.GetFile(dest);
	if (srcFile == nullptr) {
		PRINT_ERROR("Error: Cannot open source image.")
			return false;
	}

	int size = fu.GetSize(srcFile);
	BYTE* buffer = fu.GetBuffer(size);

	if (fread(buffer, 1, size, srcFile) != size) {
		PRINT_ERROR("Error: Failed to read image.")
			fclose(srcFile);
		delete[] buffer;
		return false;
	}
	fclose(srcFile);

	bool success = binary.HideMessage(buffer, size, hiddenMessage);

	if (success) {
		FILE* newFile = nullptr;
		if (fopen_s(&newFile, "stego.bmp", "wb") == 0 && newFile != nullptr) {
			fwrite(buffer, 1, size, newFile);
			fclose(newFile);
			PRINT_SUCCESS("\nMessage embedded successfully: 'stego.bmp'")
		}
		else {
			PRINT_ERROR("Error: Failed to save stego image.")
				success = false;
		}
	}
	else {
		PRINT_ERROR("Error: Failed to embed message.")
	}

	delete[] buffer;
	return success;
}