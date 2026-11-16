#include "pch.h"
#include "file.h"

/*

	Fonction CheckFile

*/

bool CheckFile(BYTE* buffer, int size) {
	if (buffer == nullptr || size == 0) {
		return false;
	}

	return true;
}

/* 

	Fonction Load

*/

bool file::Load(BYTE *buffer, int size) {
	// Initialisation des Header BMP.
	BITMAPFILEHEADER bfh;
	BITMAPINFOHEADER bih;

	// Vérifie si le buffer ou la size est vide.
	CheckFile(buffer, size);

	// Copie du buffer.
	BYTE* src = buffer;

	/*
	
		Bloc BITMAP FILE HEADER

	*/

	// Copie de la mémoire avec la taille du File Header en l'intégrant dans la var src.
	memcpy(&bfh, src, sizeof(BITMAPFILEHEADER));
	src += sizeof(BITMAPFILEHEADER);

	// Vérifie si l'image correspond bien a une signature BM
	if (bfh.bfType != 0x4D42) {
		return false;
	}

	/*
	
		Bloc BITMAP INFO HEADER
	
	*/

	// Copie de la mémoire avec la taille du Info Header en l'intégrant dans la var src.
	memcpy(&bih, src, sizeof(BITMAPINFOHEADER));
	src = buffer + bfh.bfOffBits;

	// Vérfie si le nombre de bit est bien égal au format du BMP.
	if (bih.biBitCount != 24) {
		return false;
	}

	// Récupération des information Hauteur et Largeur.
	int width = bih.biWidth;
	int height = abs(bih.biHeight);

	// Vérifie si le fichier n'est pas nul.
	if (width == 0 || height == 0) {
		return 0;
	}
}

/*

Fonction Read

*/

bool file::Read(BYTE* buffer, int size, BITMAPFILEHEADER bfh, BITMAPINFOHEADER bih) {
	// Vérifie si le buffer ou la size est vide.
	CheckFile(buffer, size);

	BYTE* pixelData = buffer + bfh.bfOffBits;

	// Récupération des information Hauteur et Largeur.
	int width = bih.biWidth;
	int height = abs(bih.biHeight);

	// Vérifie si le fichier n'est pas nul.
	if (width == 0 || height == 0) {
		return false;
	}

	// Nombre de Bit par ligne
	int bytesPerRow = width * 3;
	// Padding
	int padding = (4 - (bytesPerRow % 4)) % 4;
	// Taille de la ligne
	int rowSize = bytesPerRow + padding;

	// Double boucle for imbriqué afin de parcourir la hauteur et la largeur de l'image.
	for (int x = 0; x <= width; x++) {
		BYTE* rowPixel = pixelData + x * rowSize;

		for (int y = 0; y <= height; y++) {
			BYTE bluePixel = static_cast<BYTE>(rowPixel[y * 3 + 0]);
			BYTE greenPixel = static_cast<BYTE>(rowPixel[y * 3 + 1]);
			BYTE redPixel = static_cast<BYTE>(rowPixel[y * 3 + 2]);

			std::cout << "Pixel (" << x << ", " << y << ") : "
				<< "R= " << static_cast<int>(redPixel) << " "
				<< "G= " << static_cast<int>(greenPixel) << " "
				<< "B= " << static_cast<int>(bluePixel) << "\n";
		}
	}

	return true;
}