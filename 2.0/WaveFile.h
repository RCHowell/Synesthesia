/**

Wave File Object and Header Struct
More info here: http://soundfile.sapp.org/doc/WaveFormat/

@Author R. Conner Howell

**/

#ifndef WAVEFILE_H_
#define WAVEFILE_H_

typedef struct WaveHeader {
    unsigned char chunkID[4];      // big endian 
    unsigned char chunkSize[4];    // little endian
    unsigned char format[4];       // big endian
    unsigned char subchunk1ID[4];  // big endian
    unsigned char subchunk1Size[4];// little endian
    unsigned char audioFormat[2]; // little endian
    unsigned char numChannels[2];  // little endian
    unsigned char sampleRate[4];   // little endian 
    unsigned char byteRate[4];     // little endian
    unsigned char blockAlign[2];   // little endian
    unsigned char bitsPerSample[2];// little endian
    unsigned char subchunk2ID[4];  // big endian
    unsigned char subchunk2Size[4];// little endian
    unsigned char data[1];         // little endian
} WaveHeader;

class WaveFile {
    // unsigned long numChannels; // Number of Channels
    // This project only uses single channel wave files
    const short headerSize; // Wave files all have a header of 44 bytes
    unsigned long sampleRate;
    unsigned long byteRate; // SampleRate * NumChannels * BitsPerSample/8
    unsigned int bitsPerSample; // 8 bits = 8, 16 bits = 16, etc.
    unsigned int bytesPerSample; // bitsPerSample / 8
    unsigned long fileSize; //File Size
    unsigned char * data; // Data is a buffer containing raw byte data
    WaveHeader * hdr; // Points to the wave file in memory

    public:
        WaveFile();// Default constructor
        WaveFile(FILE *); // Create wave file from file pointer
        ~WaveFile(); // Destructor
        int getSample(unsigned long); // Returns the ith sample
        unsigned long getSampleRate();

    private:
        unsigned long getFileSize(FILE *); // Returns file size
        unsigned int getLittleEndian4(unsigned char *); // Evaluates 4 bytes in little endian
};

#endif