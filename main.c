#include <stdlib.h>
#include <stdio.h>
#include <gd.h>

FILE *inFile;
FILE *outFile;
int blockSize = 1024;
int zoomLevel = 1;
char* title;

void processArgs(int argc, char* argv[]);
void processImage();
void cleanUp();

int main(int argc, char* argv[]) {
    processArgs(argc, argv);
    processImage();
    cleanUp();
    return 0;
}

void processArgs(int argc, char* argv[]) {
    if (argc < 3) {
        printf("Usage: disk2png infile outfile [blocksize=1024 zoom=b ...]\n");
        abort();
    }

    if ((inFile = fopen(argv[1], "rb")) == NULL) {
        printf("Error opening input file: %s\n", argv[1]);
        abort();
    }

    if ((outFile = fopen(argv[2], "wb")) == NULL) {
        printf("Error opening output file: %s\n", argv[2]);
        abort();
    }

    if (argc >= 4) blockSize = atoi(argv[3]);
    if (blockSize <= 0) {
        printf("Invalid block size: %d\n", blockSize);
        abort();
    }
    if (argc >= 5 && argv[4][0] == 'B') zoomLevel = 8;

    printf("Input file: %s\n", argv[1]);
    printf("Output file: %s\n", argv[2]);
    printf("Block size: %d\n", blockSize);
    printf("Zoom: %i\n", zoomLevel);

}

void processImage() {
    int inFileSize, height, width, currentCol, processBlockSize, colourMod = 0, colour;
    unsigned char* contents;
    size_t bytesRead;
    gdImagePtr image;
    int palette[256];

    for (int i = 0; i < 256; i++) palette[i] = 0;
    contents = malloc(blockSize * sizeof(char));

    fseek(inFile, 0L, SEEK_END);
    inFileSize = ftell(inFile);
    rewind(inFile);

    height = blockSize / zoomLevel;
    width = inFileSize / blockSize;

    image = gdImageCreate(width, height);
    colour = gdImageColorAllocate(image, 0, 0, 0);
    palette[0] = colour;
    colourMod = 255/8;

    processBlockSize = blockSize / zoomLevel;

    currentCol = 0;

    printf("inFileSize: %d\n", inFileSize);
    printf("height: %d\n", height);
    printf("width: %d\n", width);
    printf("currentCol: %d\n", currentCol);
    printf("processBlockSize: %d\n", processBlockSize);

    while ((bytesRead = fread (contents, sizeof *contents, blockSize, inFile)) == blockSize) {
        //printf("%s\n", contents);
        //$contents = fread($handle, blockSize);
        //if (feof($handle)) break;

        for (int i = 0; i < processBlockSize; i++) {
            unsigned char ch;
            if (zoomLevel != 8) {
                int r = 0, g = 0, b = 0;
                ch = contents[i];
                if (ch == 0) {
                }
                else if (ch > 0 && ch <= 31) {
                    b = ch * 6 + 64;
                }
                else if (ch > 31 && ch <= 127) {
                    g = ch * 2;
                }
                else {
                    r = (ch - 128) * 2;
                }
                //DEBUG printf("%d[%d,%d,%d] ", i, r, g, b);
                if (palette[ch] == 0) {
                    palette[ch] = gdImageColorAllocate(image, r, g, b);
                }
                colour = palette[ch];
                //echo "ch,$colour \t";
            }
            else {
                int count = 0;
                int finalColour = 0;
                for (int offset = 0; offset < 8; offset++) {
                    ch = contents[(i*8) + offset];
                    //echo "ooo: ". ($i*8)+$offset . ", " . ch . "\n";
                    if (ch > 0) count++;
                }
                finalColour = count * colourMod;
                //echo $count . ", " . finalColour . "\n";
                if (palette[count] == 0) {
                    palette[count] = gdImageColorAllocate(image, finalColour, finalColour, finalColour);
                }
                colour = palette[count];
            }
            gdImageSetPixel(image, currentCol, i, colour);
        }

        currentCol++;
        // DEBUG printf("\ncol: %d\n\n", currentCol);
    }
    gdImagePng(image, outFile);

}

void cleanUp() {
    fclose(inFile);
    fclose(outFile);
}
