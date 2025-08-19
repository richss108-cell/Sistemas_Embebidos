#include <stdio.h>
#include <stdlib.h>
#include <string.h>

// Structure for BMP file header (14 bytes)
typedef struct {
    unsigned short bfType;      // BMP signature (BM)
    unsigned int bfSize;        // File size in bytes
    unsigned short bfReserved1; // Reserved
    unsigned short bfReserved2; // Reserved
    unsigned int bfOffBits;     // Offset to pixel data
} BMPHeader;

// Structure for BMP info header (40 bytes for BITMAPINFOHEADER)
typedef struct {
    unsigned int biSize;          // Size of info header
    int biWidth;                 // Image width
    int biHeight;                // Image height
    unsigned short biPlanes;      // Number of color planes
    unsigned short biBitCount;    // Bits per pixel
    unsigned int biCompression;   // Compression type
    unsigned int biSizeImage;     // Image size
    int biXPelsPerMeter;         // X pixels per meter
    int biYPelsPerMeter;         // Y pixels per meter
    unsigned int biClrUsed;       // Number of colors used
    unsigned int biClrImportant;  // Number of important colors
} BMPInfoHeader;

// Structure to hold RGB pixel
typedef struct {
    unsigned char blue;
    unsigned char green;
    unsigned char red;
} Pixel;

// Function to read BMP file
int readBMP(const char* filename, BMPHeader* header, BMPInfoHeader* infoHeader, Pixel*** pixelMatrix) {
    FILE* file = fopen(filename, "rb");
    if (!file) {
        printf("Error: No se puede abrir el archivo %s\n", filename);
        return 0;
    }

    // Read BMP header
    fread(&header->bfType, sizeof(unsigned short), 1, file);
    fread(&header->bfSize, sizeof(unsigned int), 1, file);
    fread(&header->bfReserved1, sizeof(unsigned short), 1, file);
    fread(&header->bfReserved2, sizeof(unsigned short), 1, file);
    fread(&header->bfOffBits, sizeof(unsigned int), 1, file);

    // Read Info header
    fread(infoHeader, sizeof(BMPInfoHeader), 1, file);

    // Verify BMP format and bit depth
    if (header->bfType != 0x4D42 || infoHeader->biBitCount != 24) {
        printf("Error: Solo archivos 24-bit BMP.\n");
        fclose(file);
        return 0;
    }

    // Allocate memory for pixel matrix
    int width = infoHeader->biWidth;
    int height = abs(infoHeader->biHeight);
    *pixelMatrix = (Pixel**)malloc(height * sizeof(Pixel*));
    for (int i = 0; i < height; i++) {
        (*pixelMatrix)[i] = (Pixel*)malloc(width * sizeof(Pixel));
    }

    // Move to pixel data
    fseek(file, header->bfOffBits, SEEK_SET);

    // Calculate padding (rows are aligned to 4-byte boundaries)
    int padding = (4 - (width * 3) % 4) % 4;

    // Read pixel data (BMP stores pixels bottom-up)
    for (int i = height - 1; i >= 0; i--) {
        for (int j = 0; j < width; j++) {
            fread(&(*pixelMatrix)[i][j].blue, 1, 1, file);
            fread(&(*pixelMatrix)[i][j].green, 1, 1, file);
            fread(&(*pixelMatrix)[i][j].red, 1, 1, file);
        }
        fseek(file, padding, SEEK_CUR); // Skip padding bytes
    }

    fclose(file);
    return 1;
}

// Function to write BMP file
void writeBMP(const char* filename, BMPHeader* header, BMPInfoHeader* infoHeader, Pixel** pixelMatrix) {
    FILE* file = fopen(filename, "wb");
    if (!file) {
        printf("Error: No se puede crear el archivo %s\n", filename);
        return;
    }

    // Write headers
    fwrite(&header->bfType, sizeof(unsigned short), 1, file);
    fwrite(&header->bfSize, sizeof(unsigned int), 1, file);
    fwrite(&header->bfReserved1, sizeof(unsigned short), 1, file);
    fwrite(&header->bfReserved2, sizeof(unsigned short), 1, file);
    fwrite(&header->bfOffBits, sizeof(unsigned int), 1, file);
    fwrite(infoHeader, sizeof(BMPInfoHeader), 1, file);

    // Calculate padding
    int width = infoHeader->biWidth;
    int height = abs(infoHeader->biHeight);
    int padding = (4 - (width * 3) % 4) % 4;
    unsigned char pad[3] = {0, 0, 0};

    // Write pixel data
    for (int i = height - 1; i >= 0; i--) {
        for (int j = 0; j < width; j++) {
            fwrite(&pixelMatrix[i][j].blue, 1, 1, file);
            fwrite(&pixelMatrix[i][j].green, 1, 1, file);
            fwrite(&pixelMatrix[i][j].red, 1, 1, file);
        }
        fwrite(pad, 1, padding, file); // Write padding
    }

    fclose(file);
}

// Convert image to grayscale
void toGrayscale(Pixel** pixelMatrix, int width, int height) {
    for (int i = 0; i < height; i++) {
        for (int j = 0; j < width; j++) {
            // Weighted average for grayscale (ITU-R 601-2 luma transform)
            unsigned char gray = (unsigned char)(0.299 * pixelMatrix[i][j].red +
                                                0.587 * pixelMatrix[i][j].green +
                                                0.114 * pixelMatrix[i][j].blue);
            pixelMatrix[i][j].red = gray;
            pixelMatrix[i][j].green = gray;
            pixelMatrix[i][j].blue = gray;
        }
    }
}

// Apply convolution with a 3x3 kernel
void applyConvolution(Pixel** pixelMatrix, int width, int height, float kernel[3][3], float divisor) {
    Pixel** tempMatrix = (Pixel**)malloc(height * sizeof(Pixel*));
    for (int i = 0; i < height; i++) {
        tempMatrix[i] = (Pixel*)malloc(width * sizeof(Pixel));
        memcpy(tempMatrix[i], pixelMatrix[i], width * sizeof(Pixel));
    }

    for (int i = 1; i < height - 1; i++) {
        for (int j = 1; j < width - 1; j++) {
            float r = 0, g = 0, b = 0;
            for (int ki = -1; ki <= 1; ki++) {
                for (int kj = -1; kj <= 1; kj++) {
                    r += pixelMatrix[i + ki][j + kj].red * kernel[ki + 1][kj + 1];
                    g += pixelMatrix[i + ki][j + kj].green * kernel[ki + 1][kj + 1];
                    b += pixelMatrix[i + ki][j + kj].blue * kernel[ki + 1][kj + 1];
                }
            }
            // Divide and clamp to [0, 255]
            tempMatrix[i][j].red = (unsigned char)(fmin(fmax(r / divisor, 0), 255));
            tempMatrix[i][j].green = (unsigned char)(fmin(fmax(g / divisor, 0), 255));
            tempMatrix[i][j].blue = (unsigned char)(fmin(fmax(b / divisor, 0), 255));
        }
    }

    // Copy back to original matrix
    for (int i = 1; i < height - 1; i++) {
        memcpy(pixelMatrix[i] + 1, tempMatrix[i] + 1, (width - 2) * sizeof(Pixel));
    }

    for (int i = 0; i < height; i++) {
        free(tempMatrix[i]);
    }
    free(tempMatrix);
}

// Function to read kernel from user
void readKernel(float kernel[3][3], float* divisor) {
    printf("Ingresa 3x3 valores kernel (fila por fila):\n");
    for (int i = 0; i < 3; i++) {
        for (int j = 0; j < 3; j++) {
            printf("kernel[%d][%d]: ", i, j);
            scanf("%f", &kernel[i][j]);
        }
    }
    printf("Ingrese divisor para kernel: ");
    scanf("%f", divisor);
}

// Main menu
void showMenu(Pixel** pixelMatrix, int width, int height, BMPHeader* header, BMPInfoHeader* infoHeader) {
    int choice;
    char outputFile[100];
    float kernel[3][3];
    float divisor;

    while (1) {
        printf("\nMenu:\n");
        printf("1. Convertir a escala de grises\n");
        printf("2. Aplicar Convolucion\n");
        printf("3. Exit\n");
        printf("Escoge una opción: ");
        scanf("%d", &choice);

        if (choice == 3) break;

        switch (choice) {
            case 1:
                printf("Ponle nombre al archivo de salida(e.g., output_gray.bmp): ");
                scanf("%s", outputFile);
                toGrayscale(pixelMatrix, width, height);
                writeBMP(outputFile, header, infoHeader, pixelMatrix);
                printf("Grayscale image saved as %s\n", outputFile);
                break;
            case 2:
                printf("Ponle nombre al archivo de salida(e.g., output_conv.bmp): ");
                scanf("%s", outputFile);
                readKernel(kernel, &divisor);
                applyConvolution(pixelMatrix, width, height, kernel, divisor);
                writeBMP(outputFile, header, infoHeader, pixelMatrix);
                printf("Convoluted image saved as %s\n", outputFile);
                break;
            default:
                printf("Invalid choice. Try again.\n");
        }
    }
}

int main() {
    char filename[100];
    BMPHeader header;
    BMPInfoHeader infoHeader;
    Pixel** pixelMatrix;

    printf("Enter BMP filename to load: ");
    scanf("%s", filename);

    if (!readBMP(filename, &header, &infoHeader, &pixelMatrix)) {
        return 1;
    }

    int width = infoHeader.biWidth;
    int height = abs(infoHeader.biHeight);
    printf("Image loaded: %dx%d pixels\n", width, height);

    showMenu(pixelMatrix, width, height, &header, &infoHeader);

    // Free memory
    for (int i = 0; i < height; i++) {
        free(pixelMatrix[i]);
    }
    free(pixelMatrix);

    return 0;
}
