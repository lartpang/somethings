#include  "Params.h"

void range(unsigned char image_in[Y_SIZE][X_SIZE], int *fmax, int *fmin);
void expand(unsigned char image_in[Y_SIZE][X_SIZE], 
	unsigned char image_out[Y_SIZE][X_SIZE], int fmax, int fmin);

/*--- expand_rgb --- R,G,Bを0から255の範囲に拡大する ---------------------------
	image_in_rgb:	入力画像配列（RGBカラー）
	image_out_r:	出力画像配列（RGBカラー）
	type:			拡大の方法(0:RGB独立, 1:RGB共通)
-----------------------------------------------------------------------------*/
void expand_rgb(unsigned char image_in_rgb[3][Y_SIZE][X_SIZE], 
	unsigned char image_out_rgb[3][Y_SIZE][X_SIZE], int type)
{
    int	fmax[3], fmin[3], gmax, gmin;

	switch (type) {
		case 1:
			range(image_in_rgb[0], &fmax[0], &fmin[0]);
			range(image_in_rgb[1], &fmax[1], &fmin[1]);
			range(image_in_rgb[2], &fmax[2], &fmin[2]);
			gmax = fmax[0];
			if (fmax[1] > gmax) gmax = fmax[1];
			if (fmax[2] > gmax) gmax = fmax[2];
			gmin = fmin[0];
			if (fmin[1] < gmin) gmin = fmin[1];
			if (fmin[2] < gmin) gmin = fmin[2];
			expand(image_in_rgb[0], image_out_rgb[0], gmax, gmin);
			expand(image_in_rgb[1], image_out_rgb[1], gmax, gmin);
			expand(image_in_rgb[2], image_out_rgb[2], gmax, gmin);
			break;
		default:
			range(image_in_rgb[0], &fmax[0], &fmin[0]);
			range(image_in_rgb[1], &fmax[1], &fmin[1]);
			range(image_in_rgb[2], &fmax[2], &fmin[2]);
			expand(image_in_rgb[0], image_out_rgb[0], fmax[0], fmin[0]);
			expand(image_in_rgb[1], image_out_rgb[1], fmax[1], fmin[1]);
			expand(image_in_rgb[2], image_out_rgb[2], fmax[2], fmin[2]);
			break;
	}
}
