/*********************************************************************/
/*    Image.c: Assignment 7 for CENG231, Spring 2023                 */
/*********************************************************************/
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>
#include <assert.h>
#ifndef IMAGE_H
#define IMAGE_H

typedef struct 
{
	unsigned int W;	   /* image width */
	unsigned int H;    /* image height */
	unsigned char *R;  /* pointer to the memory storing all the R intensity values */
	unsigned char *G;  /* pointer to the memory storing all the G intensity values */
	unsigned char *B;  /* pointer to the memory storing all the B intensity values */
} IMAGE;

/* Get the R intensity of pixel (x, y) in image */
unsigned char GetPixelR(IMAGE *image, unsigned int x,  unsigned int y)
{
	unsigned char r;
	r = *((image->R)+(x+(y* (image->W))));
	return(r);
}

/* Get the G intensity of pixel (x, y) in image */
unsigned char GetPixelG(IMAGE *image, unsigned int x,  unsigned int y)
{
	unsigned char g;
	g = *((image->G)+(x+(y* (image->W))));
	return(g);
}

/* Get the B intensity of pixel (x, y) in image */
unsigned char GetPixelB(IMAGE *image, unsigned int x,  unsigned int y)
{
	unsigned char b;
	b = *((image->B)+(x+(y* (image->W))));
	return(b);
}

/* Set the R intensity of pixel (x, y) in image to r */
void SetPixelR(IMAGE *image, unsigned int x,  unsigned int y, unsigned char r)
{	
	*((image->R)+(x+(y* (image->W)))) = r ;
}

void SetPixelG(IMAGE *image, unsigned int x,  unsigned int y, unsigned char g)
{
	*(image->G+(x+(y* (image->W)))) = g ;
}

/* Set the B intensity of pixel (x, y) in image to b */
void SetPixelB(IMAGE *image, unsigned int x,  unsigned int y, unsigned char b)
{
	*(image->B+(x+(y* (image->W)))) = b ;
}

/* The new DeleteImage function */
void DeleteImage(IMAGE *image)
{
	free(image->R);
	free(image->G);
	free(image->B);
	free(image);
}


/* Allocate dynamic memory for the image structure and its R/G/B values */
/* Return the pointer to the image, or NULL in case of error */
IMAGE *CreateImage(unsigned int Width, unsigned int Height)
 {
        IMAGE *image;
        image= (IMAGE*)malloc(sizeof(IMAGE));
        image->R = (unsigned char *)malloc(Width *Height);
        image->G = (unsigned char *)malloc(Width *Height);
        image->B = (unsigned char *)malloc(Width *Height);
	image-> W = Width;
        image-> H = Height;
        return(image);

}


/* Return the image's width in pixels */
unsigned int ImageWidth(const IMAGE *image)
{
    assert(image != NULL);
    return image->W;
}

/* Return the image's height in pixels */
unsigned int ImageHeight(const IMAGE *image)
{
    assert(image != NULL);
    return image->H;
}

#endif
