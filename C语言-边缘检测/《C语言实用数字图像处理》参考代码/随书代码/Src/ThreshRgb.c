#include  "Params.h"

/*--- thresh_rgb --- R,G,BlΙζιθl ----------------------------------
	image_in_rgb:	όΝζzρiRGBJ[j
	image_out:		oΝζzρiL[j
	rmin, rmax:		qΜθl (min,max)
	gmin, gmax:		fΜθl (min,max)
	bmin, bmax:		aΜθl (min,max)
-----------------------------------------------------------------------------*/
void thresh_rgb(unsigned char image_in_rgb[3][Y_SIZE][X_SIZE], 
	unsigned char image_out[Y_SIZE][X_SIZE], 
	int rmin, int rmax, int gmin, int gmax, int bmin, int bmax)
{
	int   i, j;

	for (i = 0; i < Y_SIZE; i++) {
		for (j = 0; j < X_SIZE; j++) {
			if      (image_in_rgb[0][i][j] < rmin) image_out[i][j] = LOW;
			else if (image_in_rgb[0][i][j] > rmax) image_out[i][j] = LOW;
			else if (image_in_rgb[1][i][j] < gmin) image_out[i][j] = LOW;
			else if (image_in_rgb[1][i][j] > gmax) image_out[i][j] = LOW;
			else if (image_in_rgb[2][i][j] < bmin) image_out[i][j] = LOW;
			else if (image_in_rgb[2][i][j] > bmax) image_out[i][j] = LOW;
			else                                   image_out[i][j] = HIGH;
		}
	}
}
