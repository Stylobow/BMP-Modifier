#include "pch.h"
#include "file.h"
#include <iostream>

/* Fonction CheckFile */
bool file::CheckFile(BYTE* buffer, int size) {
    if (buffer == nullptr || size == 0) {
        return false;
    }
    return true;
}

/* Fonction GetFile */
FILE* file::GetFile(const char* dest) {
    FILE* file = nullptr;

    errno_t err = fopen_s(&file, dest, "rb");

    if (err != 0) {
        std::cout << "Error code : " << err << std::endl;
        return 0;
    }

    return file;
}

/* Fonction GetSize */
const int file::GetSize(FILE* file) {
    fseek(file, 0, SEEK_END);
    const int size = ftell(file);
    fseek(file, 0, SEEK_SET);

    return size;
}

/* Fonction GetBuffer */
BYTE* file::GetBuffer(const int size) {
    BYTE* buffer = new BYTE[size];

    return buffer;
}

/* Fonction Load */
bool file::Load(BYTE* buffer, int size) {
    // Initialisation des Header BMP.
    BITMAPFILEHEADER bfh;
    BITMAPINFOHEADER bih;

    // Vérifie si le buffer ou la size est vide.
    if (!CheckFile(buffer, size)) {
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
    if (width == 0 || height == 0) {
        return false;
    }

    return true;
}

/* Fonction Read */
bool file::Read(BYTE* buffer, int size, BITMAPFILEHEADER bfh, BITMAPINFOHEADER bih) {
    // Vérifie si le buffer ou la size est vide.
    if (!CheckFile(buffer, size)) {
        return false;
    }

    BYTE* pixelData = buffer + bfh.bfOffBits;

    // Récupération des informations Hauteur et Largeur.
    int width = bih.biWidth;
    int height = abs(bih.biHeight);

    if (width == 0 || height == 0) {
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

/* Fonction CloneFile */
FILE* file::CloneFile(const char* dest) {
    FILE* srcFile = GetFile(dest);
    if (!srcFile) {
        std::cout << "Error: Source file not found or cannot be opened." << std::endl;
        return nullptr;
    }

    int size = GetSize(srcFile);
    BYTE* buffer = GetBuffer(size);

    if (!CheckFile(buffer, size)) {
        fclose(srcFile);
        delete[] buffer;
        return nullptr;
    }

    FILE* destFile = nullptr;
    errno_t err = fopen_s(&destFile, "clone.bmp", "wb");

    if (err != 0 || destFile == nullptr) {
        std::cout << "Error, Failed to clone file, error code : " << err << std::endl;
        fclose(srcFile);
        delete[] buffer;
        return nullptr;
    }

    fread(buffer, 1, size, srcFile);

    bool imageLoaded = file::Load(buffer, size);
    if (!imageLoaded) {
        std::cout << "Error reading the image. Check its format and color depth, which must be 24." << std::endl;
        fclose(srcFile);
        fclose(destFile);
        delete[] buffer;
        return nullptr;
    }

    fseek(srcFile, 0, SEEK_SET);

    if (fwrite(buffer, 1, size, destFile) != size) {
        std::cout << "Error: Failed to write complete file." << std::endl;
        fclose(srcFile);
        fclose(destFile);
        delete[] buffer;
        return nullptr;
    }

    fclose(srcFile);
    fclose(destFile);
    delete[] buffer;

    std::cout << "Clone success! File saved as 'clone.bmp'." << std::endl;

    return destFile;
}