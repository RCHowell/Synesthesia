/**

Bitmap File Object and Header Struct
More info here: http://www.dragonwins.com/domains/getteched/bmp/bmpfileformat.htm

@Author R. Conner Howell

**/

#include <iostream>
#include <stdlib.h>
#include <fstream>
#include "Bitmap.h"

// Default constructor
Bitmap::Bitmap() : headerSize(14), infoSize(40) {
    hdr = NULL;
    info = NULL;
    data = NULL;
}

// Destructor
Bitmap::~Bitmap() {
    free(hdr);
    free(info);
    free(data);
}

// Create bitmap object from file pointer
Bitmap::Bitmap(FILE * file) : headerSize(14), infoSize(40) {
    //printf("...Creating Bitmap...\t");
    dataSize = getFileSize(file) - headerSize - infoSize;

    // Read the file headers into the header structs
    hdr = (BitmapHeader *) malloc(headerSize);
    info = (BitmapInfo *) malloc(infoSize);
    fread(hdr, 1, headerSize, file);
    fread(info, 1, infoSize, file);

    // Check that the bitmap is valid
    if (isValid() == false) return;

    width = getLittleEndian4(info->width);
    height = getLittleEndian4(info->height);
    //printf("H: %d W: %d\t", height, width);

    // Allocate enough space for the bitmap pixel data
    data = (unsigned char *) malloc(sizeof(unsigned char) * dataSize);
    size_t bytesRead;
    // Read pixel data into the data buffer
    bytesRead = fread(data, 1, dataSize, file);

    //printf("%zu bytes\t", bytesRead + headerSize);
    fclose(file);
    //printf("Done\n");
}

// Determine the size of a file
unsigned long 
Bitmap::getFileSize(FILE * file) {
  fseek(file, 0, SEEK_END);
  unsigned long size = ftell(file);
  rewind(file);
  return size;
}

// Get integer equivalent of 4-bytes represented in little endian
unsigned int 
Bitmap::getLittleEndian4(unsigned char * src) {
    return src[0] + (src[1]<<8) + (src[2]<< 16) + (src[3] << 24);
}

// Check that the bitmap has the proper BM header and the info header is 40 bytes
bool
Bitmap::isValid() {
    if (hdr->signature[0] != 'B' || hdr->signature[1] != 'M') return false;
    if (getLittleEndian4(info->headerSize) != 40) return false;
    return true;
}

void
// Change the image to black and white
Bitmap::toBlackAndWhite() {
    unsigned char value;
    unsigned char pix[3];
    
    for (int x = 0; x < width; x++) {
        for (int y = 0; y < height; y++) {
            getPixel(pix, x, y);
            value = 0.21 * pix[0] + 0.72 * pix[1] + 0.07 * pix[2];
            setPixel(x, y, value, value, value);
        }
    }
}

void
// Save bitmap to specified filename
Bitmap::saveToFile(const char * filename) {
    FILE * file = fopen(filename, "wb"); // Create file pointer from filename
    fwrite(hdr, 1, headerSize, file); // Write file header
    fwrite(info, 1, infoSize, file); // Write info header
    fwrite(data, 1, dataSize, file); // Write pixel data
    fclose(file); // Save file after writing
}

// Get R, G, B values of the pixel at x,y
void
Bitmap::getPixel(unsigned char * RGB, unsigned int x, unsigned int y) {
    // Calculate pixel position in the data array
    unsigned long i = (x + y * width) * 3;
    RGB[2] = data[i]; // Blue
    RGB[1] = data[i+1]; // Green
    RGB[0] = data[i+2]; // Red
}

// Set R,G,B of the pixel at x,y
void
Bitmap::setPixel(unsigned int x, unsigned int y, unsigned char R, unsigned char G, unsigned char B) {
    // Calculate pixel position in the flat data array
    if (x >= width || y >= height) return;
    unsigned long i = (x + y * width) * 3;
    data[i] = B;
    data[i+1] = G;
    data[i+2] = R;
}

unsigned char
Bitmap::average(unsigned char * RGB) {
    return ((unsigned int)RGB[0] + (unsigned int)RGB[1] + (unsigned int)RGB[2]) / 3;
}

void
// Threshold filter with a foreground and background
Bitmap::threshold2(unsigned char fore, unsigned char back, unsigned char threshold) {
    unsigned char avg;
    unsigned char pix[3];
    
    for (int x = 0; x < width; x++) {
        for (int y = 0; y < height; y++) {
            getPixel(pix, x, y);
            avg = average(pix);
            if (avg < threshold) {
                setPixel(x, y, back, back, back);
            } else {
                setPixel(x, y, fore, fore, fore);
            }
        }
    }
}


void
// Photoshop dodge effect (add corresponding RGB values together)
Bitmap::dodge(unsigned char R, unsigned char G, unsigned char B) {
    unsigned char pix[3];
    for (int x = 0; x < width; x++) {
        for (int y = 0; y < height; y++) {
            getPixel(pix, x, y);
            setPixel(x, y, max(pix[0] + R), max(pix[1] + G), max(pix[2] + B));
        }
    }
}

unsigned char
Bitmap::max(unsigned int x) {
    return (x > 255) ? 255 : x;
}

unsigned int Bitmap::getWidth() { return width; };

unsigned int Bitmap::getHeight() { return height; };

void Bitmap::drawRect(int x, int y, int w, int h) {
    for (u_int i = 0; i < w; i++) {
        for (u_int j = 0; j < h; j++) {
            setPixel(x+i, y+j, 0, 0, 0);
        }
    }
}

