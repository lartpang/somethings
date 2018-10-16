#include  "Params.h"

/*--- thresh_rgb --- R,G,B�l�ɂ��臒l���� ----------------------------------
	image_in_rgb:	���͉摜�z��iRGB�J���[�j
	image_out:		�o�͉摜�z��i�L�[�j
	rmin, rmax:		�q��臒l (min,max)
	gmin, gmax:		�f��臒l (min,max)
	bmin, bmax:		�a��臒l (min,max)
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
