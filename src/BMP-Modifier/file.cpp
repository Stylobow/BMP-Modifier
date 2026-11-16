#include "pch.h"
#include "file.h"

bool file::Load(BYTE *buffer, int size) {
	// Initialisation des Header BMP.
	BITMAPFILEHEADER bfh;
	BITMAPINFOHEADER bih;

	// Vérifie si le buffer ou la size est vide.
	if (buffer == nullptr || size == 0) {
		return false;
	}

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

	if (width == 0 || height == 0) {
		return 0;
	}
}