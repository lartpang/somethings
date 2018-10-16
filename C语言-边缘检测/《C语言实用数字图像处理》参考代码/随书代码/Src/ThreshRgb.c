#include  "Params.h"

/*--- thresh_rgb --- R,G,B値による閾値処理 ----------------------------------
	image_in_rgb:	入力画像配列（RGBカラー）
	image_out:		出力画像配列（キー）
	rmin, rmax:		Ｒの閾値 (min,max)
	gmin, gmax:		Ｇの閾値 (min,max)
	bmin, bmax:		Ｂの閾値 (min,max)
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
