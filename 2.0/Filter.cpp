#include <iostream>
#include <stdlib.h>
#include <fstream>
#include <Math.h>
#include "Bitmap.h"

using namespace std;

void sobel(Bitmap *, int); // Apply sobel operator for edege detection

int main(int argc, char * argv[]) {
    // filename := argv[1];
    // open audio file and read it in binary mode
    // if (argc != 5) {
    //     cout << "Usage: ./filter input.bmp output.bmp filterType intensity" << endl;
    //     return 1;
    // }

    FILE * bmFile = fopen(argv[1], "rb");
    Bitmap * image = new Bitmap(bmFile);
    //image->toBlackAndWhite();
    sobel(image, atol(argv[3]));
    image->saveToFile(argv[2]);
    fclose(bmFile);

    // char str[20];
    // for (int i = 0; i < 180; i++) {
    //     FILE * bmFile = fopen(argv[1], "rb");
    //     Bitmap * image = new Bitmap(bmFile);
    //     //image->toBlackAndWhite();
    //     sprintf(str, "./Output/out-%03d.bmp", i);
    //     cout << str << endl;
    //     sobel(image, i);
    //     image->saveToFile(str);
    //     fclose(bmFile);
    // }
    
    
    return 0;
}

void sobel(Bitmap * img, int threshold) {


    const char x_op[3][3] = {
        {-1, 0, 1},
        {-2, 0, 2},
        {-1, 0, 1}
    };

    const char y_op[3][3] = {
        {1, 2, 1},
        {0, 0, 0},
        {-1, -2, -1}
    };

    unsigned int width = img->getWidth();
    unsigned int height = img->getHeight();
    unsigned char pix[3];

    int val = 0;
    int xVal;
    int yVal;

    //cout << "Beginning Filter\n";

    for (int x = 0; x < width; x++) {
        for (int y = 0; y < height; y++) {
            // Set border pixels to white
            if (x == 0 || x == width - 1 || y == 0 || y == height - 1) {
                //cout << "Set border\n";
                img->setPixel(x, y, 0xFF, 0xFF, 0xFF);
            } else {
                // Apply the sobel operators
                xVal = 0;
                yVal = 0;
                for (int i = -1; i <= 1; i++) {
                    for (int j = -1; j <= 1; j++) {
                        img->getPixel(pix, i+x, j+y);
                        val = img->average(pix);
                        xVal += val * x_op[1+i][1+j];
                        yVal += val * y_op[1+i][1+j];
                    }
                }

                //val = abs(xVal) + abs(yVal);
                val = abs(xVal) * 2;
                unsigned char char_val=(255-(unsigned char)(val));
                //make sure the pixel value is between 0 and 255 and add thresholds
                //char_val = (char_val > threshold) ? 255 : 0;
                if (char_val > threshold) {
                    img->setPixel(x, y, 255, 255, 255);
                }
            }
        }
    }
}