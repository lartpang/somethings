#include "Params.h"

/*--- radial_distortion --- レンズの放射方向歪曲収差補正（線形補間法）---------
	image_in:	入力画像配列（補正前）
	image_out:	出力画像配列（補正後）
	a,b:		歪係数
-----------------------------------------------------------------------------*/
void radial_distortion(unsigned char image_in[Y_SIZE][X_SIZE], 
	unsigned char image_out[Y_SIZE][X_SIZE], double a, double b)
{
	int   i, j, m, n, d;
	int   xs = X_SIZE/2;
	int   ys = Y_SIZE/2;
	double x, y, p, q, r2;

	for (i = -ys; i < ys; i++) {
		for (j = -xs; j < xs; j++) {
			r2 = i*i + j*j;
			x = (1 + a + b*r2)*j;
			y = (1 + a + b*r2)*i;
			if (y > 0) m = (int)y;
			else m = (int)(y-1);
			if (x > 0) n = (int)x;
			else n = (int)(x-1);
			q = y - m;
			p = x - n;
			if ((m >= -ys) && (m < ys) && (n >= -xs) && (n < xs))
				d = (int)((1.0-q)*((1.0-p)*image_in[m  +ys][n  +xs]
				                       + p*image_in[m  +ys][n+1+xs])
				              + q*((1.0-p)*image_in[m+1+ys][n  +xs]
				                       + p*image_in[m+1+ys][n+1+xs]));
			else
				d = 0;
			if (d <   0) d =   0;
			if (d > 255) d = 255;
			image_out[i+ys][j+xs] = d;
		}
	}
}
