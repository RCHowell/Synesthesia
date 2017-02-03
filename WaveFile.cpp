/**

Wave File Object
More info here: http://soundfile.sapp.org/doc/WaveFormat/

@Author R. Conner Howell

**/
#include <iostream>
#include <stdlib.h>
#include <fstream>
#include "WaveFile.h"

// default constructor
WaveFile::WaveFile() : headerSize(44) {
    hdr = NULL;
    data = NULL;
}

// Create wave file from pointer
WaveFile::WaveFile(FILE * file) : headerSize(44){
    //printf("...Creating WaveFile...\n");
    
    // Read the header into the WaveHeader struct
    hdr = (WaveHeader *) malloc(headerSize);
    fread(hdr, 1, headerSize, file);
    //printf("%c%c%c%c\n", hdr->chunkID[0], hdr->chunkID[1], hdr->chunkID[2], hdr->chunkID[3]);

    // Assign header data to member variables
    sampleRate = getLittleEndian4(hdr->sampleRate);
    //printf("Sample Rate: %luhz\n", sampleRate);
    byteRate = getLittleEndian4(hdr->byteRate);
    //printf("Byte Rate: %lu\n", byteRate);
    bitsPerSample = hdr->bitsPerSample[0] + (hdr->bitsPerSample[1]<<8);
    bytesPerSample = bitsPerSample / 8;
    //printf("Bits Per Sample: %u\n", bitsPerSample);

    fileSize = getFileSize(file); // call helper to find the size of file

    // allocate data to the size of the data (total-header) in bytes
    data = (unsigned char *) malloc(sizeof(unsigned char) * fileSize - headerSize);

    // fseek required because fileSize() rewinds the file pointer
    fseek(file, headerSize, SEEK_SET);
    size_t bytesRead; 
    bytesRead = fread(data, 1, fileSize, file); // Read sound data and store the number of bytes read

    //printf("Read %zu bytes\t", bytesRead + headerSize);
    fclose(file);
    //printf("Done\n");
}

// Destructor
WaveFile::~WaveFile() {
    free(hdr);
    free(data);
}

// Determine the size of a file
unsigned long 
WaveFile::getFileSize(FILE * file) {
  fseek(file, 0, SEEK_END);
  unsigned long size = ftell(file);
  rewind(file);
  return size;
}

// Get integer equivalent of 4-bytes represented in little endian
unsigned int 
WaveFile::getLittleEndian4(unsigned char * src) {
    return src[0] + (src[1]<<8) + (src[2]<< 16) + (src[3] << 24);
}

int
WaveFile::getSample(unsigned long i) {
    unsigned long offset = bytesPerSample * i; // Offset to bytes location
    int value;
    // If 2 bytes per sample, evaluate little endian
    value = (bytesPerSample == 2)  ? (int16_t)(data[offset] + (data[offset+1]<<8)) : data[offset];
    return value;
}

unsigned long
WaveFile::getSampleRate() {
    return sampleRate;
}