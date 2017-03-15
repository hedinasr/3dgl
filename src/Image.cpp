#include <cstdlib>
#include <cassert>
#include <cstdio>
#include "Image.h"



// ----------------------------------------------------------
void imInit(Image& im, const int dx, const int dy, const int dc)
{
	im.dimx = dx;
	im.dimy = dy;
	im.dimc = dc;
	im.data = new unsigned char[im.dimx*im.dimy*im.dimc];
}


void imFree(Image& im)
{
	delete[] im.data;
	im.data = NULL;
	im.dimx = im.dimy = im.dimc = 0;
}


void imCreateTransparency(Image& im, const unsigned char R, const unsigned char G, const unsigned char B, int option)
{
	if (im.dimc!=3)
	{
		printf("imCreateTransparency: create transparency is possible only on RGB image\n");
		return;
	}
	unsigned char* tmp = im.data;
	im.dimc = 4;
	im.data = new unsigned char[im.dimx*im.dimy*im.dimc];
	int i;
	unsigned char r,g,b;
	for(i=0;i<im.dimx*im.dimy;i++)
	{
		r = tmp[i*3+0];
		g = tmp[i*3+1];
		b = tmp[i*3+2];

		im.data[i*4+0] = r;
		im.data[i*4+1] = g;
		im.data[i*4+2] = b;
		im.data[i*4+3] = 255;
		switch(option)
		{
        case 0:
       		if ( (r==R) && (g==G) && (b==B) ) im.data[i*4+3] = 0;
       		break;
        case 1:  // GREATER
       		if ( (r>R) && (g>G) && (b>B) ) im.data[i*4+3] = 0;
       		break;
        case 2:
       		if ( (r<R) && (g<G) && (b<B) ) im.data[i*4+3] = 0;
       		break;
		}
	}
	delete[] tmp;
}


int imGetDimX(const Image& im) { return im.dimx; }
int imGetDimY(const Image& im) { return im.dimy; }
int imGetDimC(const Image& im) { return im.dimc; }
const unsigned char*  imGetData(const Image& im) { return im.data; }



// ----------------------------------------------------------
unsigned char& getPix(const Image& im, const int x, const int y, const int c)
{
	assert( im.data );
	assert( x>=0 );
	assert( x<im.dimx);
	assert( y>=0 );
	assert( y<im.dimy);
	assert( c>=0 );
	assert( c<im.dimc);
	return im.data[ (y*im.dimx+x)*im.dimc+c ];
}

void setPix(const Image& im, const int x, const int y, const int c, unsigned char v)
{
    getPix(im,x,y,c) = v;
}

const unsigned char& getPixConst(const Image& im, const int x, const int y, const int c)
{
	return getPix(im,x,y,c);
}


void imInitPPM(Image& im, const char filename[])
{
	const int taille=512;
	FILE* f;
	char txt[taille];
	f = fopen(filename, "rb");
	if (f==NULL)
	{
		printf("Erreur lors de l'ouverture de %s\n", filename);
		exit(1);
	}

	int dimx, dimy, type;

	do { fgets( txt,taille,f); } while( txt[0]=='#' );
	assert( sscanf( txt, "P%d\n", &type)==1 );
	do { fgets( txt,taille,f); } while( txt[0]=='#' );
	assert( sscanf( txt, "%d %d\n", &dimx, &dimy) ==2 );
	do { fgets( txt,taille,f); } while( txt[0]=='#' );

	bool bin=false;
	switch (type)
	{
	case 6: bin=true;
	case 3 :
		imInit( im, dimx, dimy, 3);
		break;
	case 5 : bin=true;
	case 2 :
		imInit( im, dimx, dimy, 1);
		break;
	default:
		printf("Format PPM non géré\n");
		exit(1);
		break;
	}

	//~ printf("%d %d %d %d\n", im.dimx, im.dimy, im.dimc, bin);

	int x,y,c,r;
	int p=im.dimx * im.dimy * 3-1;
	for (y=0;y<im.dimy;++y)
		for (x=0;x<im.dimx;++x)
			for (c=0;c<im.dimc;++c)
			{
				if (bin)
					//fread( &(im.data[p]), 1, 1, f);
					fread( &(getPix(im,x,im.dimy-1-y,c)), 1, 1, f);
				else
        {
					 r = fscanf(f, "%c", &(getPix(im,x,im.dimy-1-y,c)));
           assert( r==1 );
        }
				--p;
			}
	fclose(f);
	printf("Lecture de l'image %s ...OK\n", filename);
}


void imPrintf(const Image& im)
{
	printf( "%dx%d %c\n", im.dimx, im.dimy, im.dimc);      // dimension de l'image
}


bool isCoordValid(const Image& im, const int x, const int y)
{
    return ((x>=0) && (x<im.dimx) && (y>=0) && (y<im.dimy) );
}




//~ void imInitBMP(Image& im, char *filename)
//~ {
	//~ FILE *file;
	//~ unsigned long size;                 // size of the image in bytes.
	//~ unsigned long i;                    // standard counter.
	//~ unsigned short int planes;          // number of planes in image (must be 1)
	//~ unsigned short int bpp;             // number of bits per pixel (must be 24)
	//~ char temp;                          // used to convert bgr to rgb color.

	//~ // make sure the file is there.
	//~ if ((file = fopen(filename, "rb"))==NULL)
	//~ {
		//~ printf("imInitBMP: File Not Found : %s\n",filename);
		//~ return 0;
	//~ }

	//~ // seek through the bmp header, up to the width/height:
	//~ fseek(file, 18, SEEK_CUR);

	//~ // No 100% errorchecking anymore!!!

	//~ // read the width
	//~ image.dimx = getint (file);

	//~ // read the height
	//~ image.dimy = getint (file);

	//~ // calculate the size (assuming 24 bits or 3 bytes per pixel).
	//~ size = image.dimx * image.dimy * 3;

	//~ // read the planes
	//~ planes = getshort(file);
	//~ if (planes != 1)
	//~ {
		//~ printf("Planes from %s is not 1: %u\n", filename, planes);
		//~ return 0;
	//~ }

	//~ // read the bpp
	//~ bpp = getshort(file);
	//~ if (bpp != 24)
	//~ {
		//~ printf("Bpp from %s is not 24: %u\n", filename, bpp);
		//~ return 0;
	//~ }

	//~ // seek past the rest of the bitmap header.
	//~ fseek(file, 24, SEEK_CUR);

	//~ // read the data.
	//~ image->data = (char *) malloc(size);
	//~ if (image->data == NULL)
	//~ {
		//~ printf("Error allocating memory for color-corrected image data");
		//~ return 0;
	//~ }

	//~ if ((i = fread(image->data, size, 1, file)) != 1)
	//~ {
		//~ printf("Error reading image data from %s.\n", filename);
		//~ return 0;
	//~ }

	//~ for (i=0;i<size;i+=3)   // reverse all of the colors. (bgr -> rgb)
	//~ {
		//~ temp = image->data[i];
		//~ image->data[i] = image->data[i+2];
		//~ image->data[i+2] = temp;
	//~ }

	//~ // we're done.
	//~ return 1;
//~ }
