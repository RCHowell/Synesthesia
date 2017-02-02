#include <iostream>
#include <stdlib.h>
#include <fstream>
#include <math.h>
#include "WaveFile.h"
#include "Bitmap.h"

#include <fftw3.h> // One-Dimensional Fast Fourier Transform Library
// NOTE: Programs using RFFTW should link with -lrfftw -lfftw -lm

using namespace std;

typedef unsigned int u_int;

int main(int argc, char * argv[]) {

    // filename := argv[1];
    // open audio file and read it in binary mode
    FILE * audioFile = fopen(argv[1], "rb");
    WaveFile * wave = new WaveFile(audioFile); 
    
    u_int FPS = 60;
    char outputFile[20]; // buffer for output filename
    u_int N = wave->getSampleRate() / FPS; // (Samples / Second) / (Frames / second) = Samples per Frame
    
    double *in;
    fftw_complex *out;
    in  = (double*) fftw_malloc(sizeof(double) * N);
    out = (fftw_complex*) fftw_malloc(sizeof(fftw_complex) * N);

    fftw_plan plan = fftw_plan_dft_r2c_1d(N, in, out, FFTW_ESTIMATE);

    u_int max[5] = {0, 0, 0, 0, 0}; // Maximum magnitudes for 
    
    u_int t0 = 20; // Initial time
    u_int tF = 24; // Final time
   
    u_int recWidth = atof(argv[3]);
    u_int frames = FPS * (tF-t0);
    
    for (u_int frame = 0; frame < frames; frame++) {
        FILE * bmFile = fopen(argv[2], "rb");
        Bitmap * image = new Bitmap(bmFile);
        u_int imgHeight = image->getHeight();
        unsigned long sampleNumber = wave->getSampleRate() * t0 + N*frame; // Samples / Sec * Sec = Sample at time
        for (u_int i = 0; i < N; i++) {
            in[i] = (double) wave->getSample(sampleNumber + i); // Fill input array with proper samples
        }

        fftw_execute(plan); // Perform a Real to Complex Discrete Fourier Transform in One Dimension

        for (u_int i = 0; i < N/2; i++) {
            // Draw bar representing the magnitude of the real values in the transform output
            u_int magnitude = (0.01 * sqrt(out[i][0] * out[i][0] + out[i][1]*out[i][1]) / 3000) * imgHeight; // Calculate complex vector magnitude
            //cout << magnitude << endl;
            u_int freq = i * wave->getSampleRate() / N;
           
            // Find max of magnitudes in three freq buckets
            if (freq <= 2600){
                image->drawRect(i*recWidth, 0, recWidth, magnitude);
                //cout << magnitude << endl;
            }
        }

        sprintf(outputFile, "./Output/out-%03d.bmp", frame);
        image->saveToFile(outputFile);
        fclose(bmFile);
        delete image;
    }
  
    fftw_destroy_plan(plan);
    fftw_free(in);
    fftw_free(out);
    delete wave;
    //delete image;
    
    return 0;
}