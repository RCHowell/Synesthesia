
#include <stdio.h>
#include <stdlib.h>
#include <math.h>

typedef struct {
  int offset;
  int width;
  int height;
} Image;

long int littleEnd(char *, int, int);
void squareFilter(char *, int, long, int, int, int, int);
void thresholdFilter(char *, int, long, int);
int averageColor(char *, long, int, int, int, int);
long fileSize(FILE *);
Image createImage(char *);
void makeSquare(char *, Image, int, int, int, int);
void checkerFilter(char *, Image, long, int, int);

const int w1[3] = {182, 244, 243}; // lightest color
const int w2[3] = {130, 211, 237};
const int w3[3] = {79, 158,  252};
const int w4[3] = {27, 82,  255};
const int w5[3]  = {34,   1,  2}; // darkest color

// cool colors
const int c1[3] = {255, 134, 58}; // lightest color
const int c2[3] = {236, 56, 131};
const int c3[3] = {11, 190,  255};
const int c4[3] = {7, 86,  251};
const int c5[3]  = {110,   0,  255}; // darkest color

const int pltt[10][3] = {
  {182, 244, 243}, 
  {130, 211, 237}, 
  {79, 158,  252}, 
  {27, 82,  255}, 
  {34,   1,  2}, 
  {255, 134, 58}, 
  {236, 56, 131}, 
  {11, 190,  255}, 
  {7, 86,  251}, 
  {110,   0,  255} };
int plttOffset = 5;

// argv[1] = input filename
// argv[2] = output filename
// argv[3] = filter level
int main (int argc, char * argv[]) {

    if (argc != 5 && argc != 4) {
      printf("Usage:\n");
      printf("\tThree Parameters:\n");
      printf("\t... input.bmp output.bmp filterLevel\n");
      return 1;
    }

    FILE * pFile;
    FILE * wFile;
    long lSize;
    char * buffer;
    int filterLevel = atol(argv[3]);
    
    // open file using argv[1] as filename
    pFile = fopen ( argv[1] , "rb" );
    if (pFile==NULL) {fputs ("File error",stderr); exit (1);}
    // open file to be written to argv[2]
    wFile = fopen(argv[2], "wb");
    // obtain file size:
    lSize = fileSize(pFile);
    // allocate memory to contain the whole file:
    buffer = (char*) malloc (sizeof(char)*lSize);
    if (buffer == NULL) {fputs ("Memory error",stderr); exit (2);}
    // populate buffer with file bytes
    //printf("Writing to buffer...\t");
    fread (buffer,1,lSize,pFile);
    //printf("DONE!\n");
    // create image struct to later be edited
    Image image = createImage(buffer);
    //printf("Created Image\n");
    // print image metadata to ensure the image loaded properly
    //printf("Offset: %d W: %d H: %d\n", image.offset, image.width, image.height);
    //checkerFilter(buffer, image, lSize, filterLevel, atol(argv[4]));
    // apply threshold filter
    thresholdFilter(buffer, image.offset, lSize, filterLevel);
    //printf("Applying Filter..\t");
    //squareFilter(buffer, image.offset, lSize, atol(argv[4]), image.width, image.height, filterLevel);
    //printf("DONE!\n");
    // write buffer to out file
    //printf("Writing buffer..\t");
    fwrite(buffer, 1, lSize, wFile);
    //printf("DONE!\n");
    // close files and exit
    fclose (pFile);
    fclose(wFile);
    free (buffer);
    return 0;
}


// createImage struct based upon bmp filetype
Image createImage(char * buffer) {
  //printf("Creating Image..\n");
  // evaluate 4 bytes (little endian) to determine pixle offset, width and height
  Image newImage;
  //printf("Determining Offset..\t");
  newImage.offset = littleEnd(buffer, 10, 4);
  //printf("DONE!\n");
  //printf("Determining Width..\t");
  newImage.width = littleEnd(buffer, 18, 4);
  // printf("DONE!\n");
  // printf("Determining Height..\t");
  newImage.height = littleEnd(buffer, 22, 4);
  // printf("DONE!\n");
  return newImage;
}

void thresholdFilter(char * buffer, int offset, long l, int threshold) {
  int i;
  // b, g, r values 0-255
  // black and white 
    // newVal = 0.21 × R + 0.72 × G + 0.07 × B
  // color palette {b, g, r}
  // warm colors
  // const int w1[] = {182, 244, 243}; // lightest color
  // const int w2[] = {130, 211, 237};
  // const int w3[] = {79, 158,  252};
  // const int w4[] = {27, 82,  255};
  // const int w5[]  = {34,   1,  2}; // darkest color

  // cool colors
  const int c1[] = {247, 255, 221}; // lightest color
  const int c2[] = {216, 225, 147};
  const int c3[] = {158, 166,  255};
  const int c4[] = {101, 68,  170};
  const int c5[]  = {87,   22,  134}; // darkest color

  for (i = offset; i < l - offset; i += 3) {
     int b = (unsigned char)buffer[i];
     int g = (unsigned char)buffer[i+1];
     int r = (unsigned char)buffer[i+2];
     int total = 0.21*r+0.72*g+0.07*b;
     buffer[i] = total;
     buffer[i+1] = total;
     buffer[i+2] = total;
    //  if      (total < threshold * 1 / 5){ buffer[i] = c5[0]; buffer[i+1] = c5[1]; buffer[i+2] = c5[2];}
    //  else if (total < threshold * 2 / 5){ buffer[i] = c4[0]; buffer[i+1] = c4[1]; buffer[i+2] = c4[2];}
    //  else if (total < threshold * 3 / 5){ buffer[i] = c3[0]; buffer[i+1] = c3[1]; buffer[i+2] = c3[2];}
    //  else if (total < threshold * 4 / 5){ buffer[i] = c2[0]; buffer[i+1] = c2[1]; buffer[i+2] = c2[2];}
    //  else                               { buffer[i] = c1[0]; buffer[i+1] = c1[1]; buffer[i+2] = c1[2];}
  }
}

void squareFilter(char * buffer, int offset, long l, int bound, int w, int h, int threshold) {
  int i;
  int center = (l/3) / 2;
  // b, g, r values 0-255
  // color palette {b, g, r}
  // warm colors
  

  for (i = offset; i < l - offset; i += 3) {
     int x = (i/3) % w;
     int y = (i/3) / w;
     //if (i < l * ((double)bound/100.0)) plttOffset = 5;
     //else plttOffset = 0;
     
     int b = (unsigned char)buffer[i];
     int g = (unsigned char)buffer[i+1];
     int r = (unsigned char)buffer[i+2];
     int total = r+g+b;
     if      (total < threshold * 1 / 5){ buffer[i] = pltt[plttOffset+0][0]; buffer[i+1] = pltt[plttOffset+0][1]; buffer[i+2] = pltt[plttOffset+0][2];}
     else if (total < threshold * 2 / 5){ buffer[i] = pltt[plttOffset+1][0]; buffer[i+1] = pltt[plttOffset+1][1]; buffer[i+2] = pltt[plttOffset+1][2];}
     else if (total < threshold * 3 / 5){ buffer[i] = pltt[plttOffset+2][0]; buffer[i+1] = pltt[plttOffset+2][1]; buffer[i+2] = pltt[plttOffset+2][2];}
     else if (total < threshold * 4 / 5){ buffer[i] = pltt[plttOffset+3][0]; buffer[i+1] = pltt[plttOffset+3][1]; buffer[i+2] = pltt[plttOffset+3][2];}
     else                               { buffer[i] = pltt[plttOffset+4][0]; buffer[i+1] = pltt[plttOffset+4][1]; buffer[i+2] = pltt[plttOffset+4][2];}
  }
}


// n is the number of squares (fit top to bottom)
void checkerFilter(char * buffer, Image img, long l, int n, int threshold) {
  // each pixel is 3 entries in the buffer
  // pixel data starts at offset (bottom-left to top-right)
  // pixels per square
  int size = (img.height / n) * 3;
  // squares per row
  int sperr = (img.width / size) * 3;
  int row = 1;
  //printf("Squares per row: %d\n", sperr);
  //printf("Size: %d\n", size);
  int x = 0; // number of squares placed
  // size * n is the max number of squares
  int i = img.offset;
  // makeSquare(buffer, img, i, size, 1);
  // i += size;
  // makeSquare(buffer, img, i, size, 0);
  // i += size;
  // makeSquare(buffer, img, i, size, 1);
  int c = 0;
  while (c < sperr * n) {
    makeSquare(buffer, img, i, size, x%2, threshold);
    // increment i to where the next square needs to be started
    
    if (x >= sperr * row) {
      row++;
      i += img.width * size;
      //printf("S: %d\tRow: %d\n", x, row);
    }

    x++;
    c++;
    i += size;
  }

}

void makeSquare(char * buffer, Image img, int offset, int size, int isEven, int threshold) {
  int coffset = 0;
  // z is the buffer index for getting the pixels
  int i, j, z;
  for (i = 0; i < size; i += 3) {
    for (j = 0; j < size; j += 3) {
      z = (i + j * img.width) + offset;
      if (z < img.width * img.height * 3) {
        if   (isEven) coffset = 5;
        else coffset = 0;
        int b = (unsigned char)buffer[z];
        int g = (unsigned char)buffer[z+1];
        int r = (unsigned char)buffer[z+2];
        int total = r+g+b;
        if      (total < threshold * 1 / 5){ buffer[z] = pltt[coffset+0][0]; buffer[z+1] = pltt[coffset+0][1]; buffer[z+2] = pltt[coffset+0][2];}
        else if (total < threshold * 2 / 5){ buffer[z] = pltt[coffset+1][0]; buffer[z+1] = pltt[coffset+1][1]; buffer[z+2] = pltt[coffset+1][2];}
        else if (total < threshold * 3 / 5){ buffer[z] = pltt[coffset+2][0]; buffer[z+1] = pltt[coffset+2][1]; buffer[z+2] = pltt[coffset+2][2];}
        else if (total < threshold * 4 / 5){ buffer[z] = pltt[coffset+3][0]; buffer[z+1] = pltt[coffset+3][1]; buffer[z+2] = pltt[coffset+3][2];}
        else                               { buffer[z] = pltt[coffset+4][0]; buffer[z+1] = pltt[coffset+4][1]; buffer[z+2] = pltt[coffset+4][2];}
        
      }
    }
  }
}

long int littleEnd(char * buffer, int start, int bytes) {
   long int sum = 0;
   int power = 0;
   int i = 0;
   while (i < bytes) {
     sum += (unsigned char)buffer[start + i] * (int)pow(16, power);
     power += 2;
     i++;
   } 
   return sum;
}

long fileSize(FILE * pFile) {
  fseek (pFile , 0 , SEEK_END);
  long size = ftell (pFile);
  rewind (pFile);
  return size;
}