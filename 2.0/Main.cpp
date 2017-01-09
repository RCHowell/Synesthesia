#include <iostream>
#include <stdlib.h>
#include <fstream>
#include "WaveFile.h"

using namespace std;

int main(int argc, char * argv[]) {
    // filename := argv[1];
    // open audio file and read it in binary mode
    FILE * audioFile = fopen(argv[1], "rb");
    WaveFile * wave = new WaveFile(audioFile);
    
     // simulating 3 seconds of footage at 60 FPS
    int FPS = 60;
    int FRAMES = 180;

    unsigned int sampleWidth = wave->getSampleRate() / FPS;
    printf("Sample Width: %d\n", sampleWidth);

    FILE * audioOutputFile;
    audioOutputFile = fopen("./Assets/intensity", "wb");

    unsigned long offset = 0;
    unsigned long total = 0;
    unsigned char lo;
    unsigned char hi;
    uint16_t avg;
    // Loop over every frame and average the corresponding audio data
    unsigned char * intensities;
    intensities = (unsigned char *) malloc(sizeof(unsigned char) * FRAMES * 2);
    int x = 0; // index to write bytes
    for (int i = 0; i < FRAMES; i++) {
        offset = i * sampleWidth;
        // Reset old total and take the average for the current frame
        total = 0;
        for (int j = 0; j < sampleWidth; j++) {
            total += wave->getSample(j + offset);
        }
        avg = total / sampleWidth;
        lo = avg & 0xFF;
        hi = avg >> 8;
        // Write bytes to intensities file in little endian
        intensities[x] = hi;
        intensities[x+1] = lo;
        x += 2;
    }

    fwrite(intensities, 1, FRAMES * 2, audioOutputFile);

    delete wave;
    fclose(audioOutputFile);
    return 0;
}