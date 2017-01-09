#include <stdio.h>
#include <stdlib.h>
#include <math.h>

typedef struct {
    long lSize;
    int offset;
    int width;
    int height;
    char * data;
} Image;

// reads a bitmap into an Image struct and returns the struct
Image readBitmap(const char[] filename) {
    FILE * pFile;
    FILE * wFile;
    long lSize;
    char * buffer;

    pFile = fopen ( filename , "rb" );
    if (pFile==NULL) {fputs ("File error",stderr); exit (1);}
}

int writeBitmap(const char[] filename, Image img) {
    
}