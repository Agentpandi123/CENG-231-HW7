/*********************************************************************/
/* PhotoLab.c: Assignment 7 for CENG231, Spring 2023                 */
/*********************************************************************/

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <assert.h>
#include "Image.h"

/*** global definitions ***/
#define SLEN      80		/* maximum length of file names */
#define MAX_PIXEL 255           /* maximum pixel intensity value */

/*** function declarations ***/
/* Read image from a file 						*/
/* The size of the image needs to be pre-set				*/
/* The memory spaces of the image will be allocated in this function	*/
/* Return values:							*/
/* NULL: fail to load or create an image				*/
/* image: load or create an image successfully				*/
IMAGE *LoadImage(const char *fname)
{
	FILE         *File;
	char         Type[SLEN];
	int          W, H, MaxValue;
	unsigned int x, y;
	char         ftype[] = ".ppm";
	char         fname_tmp[SLEN];
	IMAGE        *image;
	strcpy(fname_tmp, fname);
	strcat(fname_tmp, ftype);
	File = fopen(fname_tmp, "r");
	if (!File) {
		printf("\nCan't open file \"%s\" for reading!\n", fname);
		return NULL;
	}

	fscanf(File, "%79s", Type);
	if (Type[0] != 'P' || Type[1] != '6' || Type[2] != 0) {
		printf("\nUnsupported file format!\n");
		fclose(File);
		return NULL;
	}

	fscanf(File, "%d", &W);
	if (W <= 0) {
		printf("\nUnsupported image width %d!\n", W);
		fclose(File);
		return NULL;
	}

	fscanf(File, "%d", &H);
	if (H <= 0) {
		printf("\nUnsupported image height %d!\n", H);
		fclose(File);
		return NULL;
	}

	fscanf(File, "%d", &MaxValue);
	if (MaxValue != 255) {
		printf("\nUnsupported image maximum value %d!\n", MaxValue);
		fclose(File);
		return NULL;
	}
	if ('\n' != fgetc(File)) {
		printf("\nCarriage return expected at the end of the file!\n");
		fclose(File);
		return NULL;
	}

	image = CreateImage(W, H);

	if (!image) {
		printf("\nError creating image from %s!\n", fname_tmp);
		fclose(File);
		return NULL;
	}
	else {
		for (y = 0; y < ImageHeight(image); y++)
			for (x = 0; x < ImageWidth(image); x++) {
				SetPixelR(image, x, y, fgetc(File));
				SetPixelG(image, x, y, fgetc(File));
				SetPixelB(image, x, y, fgetc(File));
			}

		if (ferror(File)) {
			printf("\nFile error while reading from file!\n");
			DeleteImage(image);
			return NULL;
		}

		printf("%s was read successfully!\n", fname_tmp);
		fclose(File);
		return image;
	}
}

/* Save a processed image 		*/
/* Return values:			*/
/* 0: successfully saved the image	*/
/* 1: Cannot open the file for writing	*/
/* 2: File error while writing to file	*/
int SaveImage(const char *fname, const IMAGE *image)
{
    assert(image != NULL && "No image to save!\n");
	FILE         *File;
	int          x, y;
	char         SysCmd[SLEN * 5];
	char         ftype[] = ".ppm";
	char         fname_tmp[SLEN];
	char         fname_tmp2[SLEN];
	unsigned int WIDTH = ImageWidth(image);
	unsigned int HEIGHT = ImageHeight(image);
	strcpy(fname_tmp, fname);
	strcpy(fname_tmp2, fname);
	strcat(fname_tmp2, ftype);

	File = fopen(fname_tmp2, "w");
	if (!File) {
		printf("\nCan't open file \"%s\" for writing!\n", fname);
		return 1;
	}
	fprintf(File, "P6\n");
	fprintf(File, "%d %d\n", WIDTH, HEIGHT);
	fprintf(File, "255\n");

	for (y = 0; y < HEIGHT; y++)
		for (x = 0; x < WIDTH; x++) {
			fputc(GetPixelR(image, x, y), File);
			fputc(GetPixelG(image, x, y), File);
			fputc(GetPixelB(image, x, y), File);
		}

	if (ferror(File)) {
		printf("\nError while writing to file!\n");
		return 2;
	}
	fclose(File);
	printf("%s was saved successfully. \n", fname_tmp2);
	/*
	 * Rename file to image.ppm, convert it to ~/public_html/<fname>.jpg
	 * and make it world readable
	 */
	sprintf(SysCmd, "~ceng231/bin/pnmtojpeg.sh %s",
			fname_tmp2);
	if (system(SysCmd) != 0) {
		printf("\nError while converting to JPG:\nCommand \"%s\" failed!\n", SysCmd);
		return 3;
	}
	printf("%s.jpg was stored for viewing. \n", fname_tmp);
	return 0;
}

/**************************************************************/
/* Please add your function definitions here...               */
/**************************************************************/

/* Age the image */
IMAGE *Aging(IMAGE *image)
{
    assert(image);
    int x, y;
    for (y = 0; y < image->H; y++) {
        for (x = 0; x < image->W; x++) {
            unsigned char tmpR, tmpG, tmpB;
            tmpR = GetPixelR(image, x, y);
            tmpG = GetPixelG(image, x, y);
            tmpB = GetPixelB(image, x, y);
            unsigned char tmp = (unsigned char) ((0.3 * tmpR) + (0.59 * tmpG) + (0.11 * tmpB));
            SetPixelR(image, x, y, tmp);
            SetPixelG(image, x, y, tmp);
            SetPixelB(image, x, y, tmp);
            tmpR = GetPixelR(image, x, y);
            tmpG = GetPixelG(image, x, y);
            tmpB = GetPixelB(image, x, y);
            SetPixelR(image, x, y, (unsigned char) ((tmpR + 25) > 255 ? 255 : (tmpR + 25)));
            SetPixelG(image, x, y, (unsigned char) ((tmpG + 10) > 255 ? 255 : (tmpG + 10)));
            SetPixelB(image, x, y, (unsigned char) ((tmpB - 10) < 0 ? 0 : (tmpB - 10)));
        }
    }
    return image;
}

/* Black and White */
IMAGE *BlackNWhite(IMAGE *image)
{
    assert(image);
    int i, j, intensity;
    for (i = 0; i < image->W; i++) {
        for (j = 0; j < image->H; j++) {
            intensity = (GetPixelR(image, i, j) + GetPixelG(image, i, j) + GetPixelB(image, i, j)) / 3;
            SetPixelR(image, i, j, intensity);
            SetPixelG(image, i, j, intensity);
            SetPixelB(image, i, j, intensity);
        }
    }
    return image;
}

/* Reverse image color */
IMAGE *Negative(IMAGE *image)
{
    assert(image);
    int i, j;
    for (i = 0; i < image->W; i++) {
        for (j = 0; j < image->H; j++) {
            SetPixelR(image, i, j, 255 - GetPixelR(image, i, j));
            SetPixelG(image, i, j, 255 - GetPixelG(image, i, j));
            SetPixelB(image, i, j, 255 - GetPixelB(image, i, j));
        }
    }
    return image;
}

/* flip image vertically */
IMAGE *VFlip(IMAGE *image)
{
assert(image);
int i, j;
IMAGE *result = CreateImage(image->W, image->H);
for (i = 0; i < image->W; i++) 
{
    for (j = 0; j < image->H; j++) 
    {
        /* swap pixel (i,j) with pixel (i,HEIGHT-j-1) */
        SetPixelR(result, i, j, GetPixelR(image, i, image->H-j-1));
        SetPixelG(result, i, j, GetPixelG(image, i, image->H-j-1));
        SetPixelB(result, i, j, GetPixelB(image, i, image->H-j-1));
    }
}

return result;
}

/* mirror image horizontally */
IMAGE *HMirror(IMAGE *image) {
    int x, y;
    int WIDTH = ImageWidth(image);
    int HEIGHT = ImageHeight(image);
    for (y = 0; y < HEIGHT / 2; y ++) {
        for (x = 0; x < WIDTH; x ++) {
            SetPixelR(image, x, HEIGHT - 1 - y, GetPixelR(image, x, y));
            SetPixelG(image, x, HEIGHT - 1 - y, GetPixelG(image, x, y));
            SetPixelB(image, x, HEIGHT - 1 - y, GetPixelB(image, x, y));
        }
    }
    return image;
}



/* sharpen the image */
IMAGE *Sharpen(IMAGE *image)
{
    IMAGE *new_image = CreateImage(image->W, image->H);
    int tmpR, tmpG, tmpB;
    int i, j;

    /* Calculate sharpened pixel values */
    for (i = 1; i < image->H - 1; i++) 
    {
        for (j = 1; j < image->W - 1; j++) 
        {
            tmpR = -GetPixelR(image, j-1, i-1) - GetPixelR(image, j, i-1) - GetPixelR(image, j+1, i-1)
                          - GetPixelR(image, j-1, i)   + 9*GetPixelR(image, j, i)   - GetPixelR(image, j+1, i)
                          - GetPixelR(image, j-1, i+1) - GetPixelR(image, j, i+1) - GetPixelR(image, j+1, i+1);
            tmpG = -GetPixelG(image, j-1, i-1) - GetPixelG(image, j, i-1) - GetPixelG(image, j+1, i-1)
                          - GetPixelG(image, j-1, i)   + 9*GetPixelG(image, j, i)   - GetPixelG(image, j+1, i)
                          - GetPixelG(image, j-1, i+1) - GetPixelG(image, j, i+1) - GetPixelG(image, j+1, i+1);
            tmpB = -GetPixelB(image, j-1, i-1) - GetPixelB(image, j, i-1) - GetPixelB(image, j+1, i-1)
                          - GetPixelB(image, j-1, i)   + 9*GetPixelB(image, j, i)   - GetPixelB(image, j+1, i)
                          - GetPixelB(image, j-1, i+1) - GetPixelB(image, j, i+1) - GetPixelB(image, j+1, i+1);

            SetPixelR(new_image, j, i, tmpR);
            SetPixelG(new_image, j, i, tmpG);
            SetPixelB(new_image, j, i, tmpB);
        }
    }

    return new_image;
}

/* exchange R and B color channels */
IMAGE *ExRB(IMAGE *image)
{
    int i, j;
    unsigned char temp;
    for (i = 0; i < image->W; i++) 
    {
        for (j = 0; j < image->H; j++) 
        {
            /* swap red and blue intensity values */
            temp = GetPixelR(image, i, j);
            SetPixelR(image, i, j, GetPixelB(image, i, j));
            SetPixelB(image, i, j, temp);
        }
    }
    
    /* return a pointer to the modified image */
    return image;
}

/* [Bonus] crop an image */
IMAGE *Crop(IMAGE *image, int x, int y, int W, int H)
{
    assert(image != NULL && "Error: Input image is NULL");

    int cropW = (W > (int)image->W - x) ? (int)image->W - x : W;
    int cropH = (H > (int)image->H - y) ? (int)image->H - y : H;

    IMAGE *croppedImage = CreateImage(cropW, cropH);

    int i, j;
    for (i = 0; i < cropH; i++) {
        for (j = 0; j < cropW; j++) {
            SetPixelR(croppedImage, j, i, GetPixelR(image, x + j, y + i));
            SetPixelG(croppedImage, j, i, GetPixelG(image, x + j, y + i));
            SetPixelB(croppedImage, j, i, GetPixelB(image, x + j, y + i));
        }
    }

    return croppedImage;
}

/* auto test */
int AutoTest(void) {
    int result;
    const char fname[SLEN] = "wilson";

	IMAGE *image = NULL;
    image = LoadImage(fname);
    if (! image) return 11;
    image = Aging(image);
    if (! image) return 12;
    result = SaveImage("aging", image);
    if (result) return result;
    printf("Aging tested!\n\n");
	DeleteImage(image);
	image = NULL;

    image = LoadImage(fname);
    if (! image) return 13;
    image = BlackNWhite(image);
    if (! image) return 14;
    result = SaveImage("bw", image);
    if (result) return result;
    printf("Black and White tested!\n\n");
	DeleteImage(image);
	image = NULL;

    image = LoadImage(fname);
    if (! image) return 15;
    image = Negative(image);
    if (! image) return 16;
    result = SaveImage("negative", image);
    if (result) return result;
    printf("Negative tested!\n\n");
    DeleteImage(image);
	image = NULL;

    image = LoadImage(fname);
    if (! image) return 17;
    image = VFlip(image);
    if (! image) return 18;
    result = SaveImage("vflip", image);
    if (result) return result;
    printf("VFlip tested!\n\n");
    DeleteImage(image);
	image = NULL;

    image = LoadImage(fname);
    if (! image) return 19;
    image = HMirror(image);
    if (! image) return 20;
    result = SaveImage("hmirror", image);
    if (result) return result;
    printf("HMirror tested!\n\n");
    DeleteImage(image);
	image = NULL;

    image = LoadImage(fname);
    if (! image) return 21;
    image = Sharpen(image);
    if (! image) return 22;
    result = SaveImage("sharpen", image);
    if (result) return result;
    printf("Sharpen tested!\n\n");
    DeleteImage(image);
	image = NULL;

    image = LoadImage(fname);
    if (! image) return 23;
    image = ExRB(image);
    if (! image) return 24;
    result = SaveImage("xRB", image);
    if (result) return result;
    printf("ExRB tested!\n\n");
    DeleteImage(image);
	image = NULL;

    image = LoadImage(fname);
    if (! image) return 25;
    image = Crop(image, 200, 150, 115, 130);
    if (! image) return 26;
    result = SaveImage("crop", image);
    if (result) return result;
    printf("Crop tested!\n\n");
    DeleteImage(image);
	image = NULL;
    return 0; /* success! */
}

/**************************************************************/
/* Main function (do not modify!)                             */
/**************************************************************/

int main(void)
{
  int result;
  result = AutoTest();
  if (result) {
	printf("AutoTest failed, error code %d!\n", result);
  }
  else{
	printf("AutoTest finished.\n");
  }
  return 0;
}
