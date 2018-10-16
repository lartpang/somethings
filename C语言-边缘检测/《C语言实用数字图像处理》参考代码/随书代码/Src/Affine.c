#include "Params.h"

#define	PI	3.141592

/*--- affine --- アフィン変換（線形補間法）------------------------------------
	image_in:	入力画像配列
	image_out:	出力画像配列
	a,b,c:		変換係数 X = ax + by + c
	d,e,f:		変換係数 Y = dx + ey + f
-----------------------------------------------------------------------------*/
void affine(unsigned char image_in[Y_SIZE][X_SIZE], 
	unsigned char image_out[Y_SIZE][X_SIZE], 
	double a, double b, double c, double d, double e, double f)
{
	int i, j, m, n;
	double x, y, p, q;
	double aa, bb, cc, dd, ee, ff, gg;
	int dat;

	gg = 1 / (a*e - b*d);
	aa = e * gg;
	bb = -b * gg;
	cc = (b*f - c*e) * gg;
	dd = -d * gg;
	ee = a * gg;
	ff = (c*d - a*f) * gg;
	for (i = 0; i < Y_SIZE; i++) {
		for (j = 0; j < X_SIZE; j++) {
			x =  aa * j + bb * i + cc;
			y =  dd * j + ee * i + ff;
			if (y > 0) m = (int)y;
			else m = (int)(y-1);
			if (x > 0) n = (int)x;
			else n = (int)(x-1);
			q = y - m;
			p = x - n;
			if ((m >= 0) && (m < Y_SIZE) && (n >= 0) && (n < X_SIZE))
				dat = (int)((1.0-q)*((1.0-p)*image_in[m  ][n  ]
				                         + p*image_in[m  ][n+1])
				                + q*((1.0-p)*image_in[m+1][n  ]
				                         + p*image_in[m+1][n+1]));
			else
				dat = 0;
			if (dat <   0) dat =   0;
			if (dat > 255) dat = 255;
			image_out[i][j] = dat;
		}
	}
}

