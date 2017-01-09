/**

Bitmap File Object and Header Struct
More info here: http://www.dragonwins.com/domains/getteched/bmp/bmpfileformat.htm

@Author R. Conner Howell

x=0,y=0 corresponds to the bottom left corner

**/

typedef struct BitmapHeader {
    unsigned char signature[2]; // Characters BM
    unsigned char fileSize[4]; // Size of file
    unsigned char unused[4]; // 4 unused bytes
    unsigned char offset[4]; // Offset to pixel data
} BitmapHeader;

typedef struct BitmapInfo {
    unsigned char headerSize[4]; // Must be at least 40
    unsigned char width[4]; // Image width in pixels
    unsigned char height[4]; // Image height in pixels
    unsigned char planes[2]; // Always 1
    unsigned char bitsPerPixel[2]; // 1, 4, 8, 16, 24, or 32
    unsigned char compression[0]; // Compression type (0 = uncompressed)
    unsigned char imageSize[4]; // Image Size - may be zero for uncompressed images
    unsigned char xpelsPerMeter[4]; // Preferred resolution in pixels per meter
    unsigned char ypelsPerMeter[4]; // Preferred resolution in pixels per meter
    unsigned char colorUsed[4]; // Number Color Map entries that are actually used
    unsigned char colorImportant[4]; // Number of significant colors
} BitmapInfo;

class Bitmap {
    const short headerSize; // Header size in bytes
    const short infoSize; // Info header in bytes
    unsigned int width; // Image width in pixels
    unsigned int height; // Image height in pixels
    unsigned char * data; // Pixel data
    unsigned long dataSize; // size not including the headers
    BitmapHeader * hdr; // Pointer to header
    BitmapInfo * info; // Pointer to info header

    public:
        Bitmap(); // Default Contstructor
        Bitmap(FILE *); // Create Bitmap object from fileSize
        ~Bitmap();
        void toBlackAndWhite(); // Convert image to black and white using lumosity greyscale
        void saveToFile(const char *); // Save bitmap object to the specified filename
        void threshold2(unsigned char, unsigned char, unsigned char); // Threshold filter with two colors
        void dodge(unsigned char, unsigned char, unsigned char); // Photoshop dodge filter
        void setPixel(unsigned int, unsigned int, unsigned char, unsigned char, unsigned char);
        void getPixel(unsigned char *, unsigned int, unsigned int); // Get the pixel at x,y
        unsigned char average(unsigned char * RGB); // Return average color intensity
        unsigned int getWidth();
        unsigned int getHeight();

    private:
        unsigned long   getFileSize(FILE *); // Returns file size
        unsigned int    getLittleEndian4(unsigned char *); // Evaluates 4 bytes in little endian
        bool            isValid(); // Checks validity of the bitmap file
        unsigned char   max(unsigned int); // Returns max value of unsigned char is int > 255 else returns int
};