#ifndef IMAGE_H_INCLUDED
#define IMAGE_H_INCLUDED



//! An image is defined by Width, Height, a number of Color and the data
struct Image
{
    unsigned char* data;
    int dimx;
    int dimy;
    int dimc;
};


//! Init an black image
void imInit(Image& im, const int dx, const int dy, const int dc);

//! Init an image from a PPM file
void imInitPPM(Image& im, const char filename[]);

//! Add transparency to an image: all white pixels (R=255,G=255,B=255) get transparent
void imCreateTransparency(Image& im, const unsigned char R=255, const unsigned char G=255, const unsigned char B=255, int option=0);

//! Free memory
void imFree(Image& im);

//! return Weight of the image
int imGetDimX(const Image& im);

//! return Height of the image
int imGetDimY(const Image& im);

//! return number of colors of the image: 1=greyscale image; 3=RGB; 4=RGBA
int imGetDimC(const Image& im);

//! return pointer on data
const unsigned char*  imGetData(const Image& im);

//! return the color value of a pixel (x,y); c=0 return R; c=1 return G; c=2 return B; c=4 return Alpha
unsigned char& getPix(const Image& im, const int x, const int y, const int c=0);

//! return the color value of a pixel (x,y); c=0 return R; c=1 return G; c=2 return B; c=4 return Alpha
const unsigned char& getPixConst(const Image& im, const int x, const int y, const int c=0);

//! return the color value of a pixel (x,y); c=0 return R; c=1 return G; c=2 return B; c=4 return Alpha
void setPix(const Image& im, const int x, const int y, const int c, unsigned char v);


//! print size of the image
void imPrintf(const Image& im);

//! true if (x,y) is into the image otherwise false
bool isCoordValid(const Image& im, const int x, const int y);

#endif // IMAGE_H_INCLUDED
