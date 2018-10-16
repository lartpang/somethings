#include <math.h>
#include "Params.h"

#define PI 3.141592

/*--- rotation --- 回転（線形補間法）------------------------------------------
	image_in:	入力画像配列
	image_out:	出力画像配列
	deg:		回転角（度）
-----------------------------------------------------------------------------*/
void rotation(unsigned char image_in[Y_SIZE][X_SIZE], 
	unsigned char image_out[Y_SIZE][X_SIZE], double deg)
{
	int	i, j, m, n;
	double x, y, p, q, r, c, s;
	int	d;

	r = deg*3.141592/180.0;
	c = cos(r);
	s = sin(r);
	for (i = 0; i < Y_SIZE; i++) {
		for (j = 0;j < X_SIZE; j++) {
			y = j*s + i*c;
			x = j*c - i*s;
			if (y > 0) m = (int)y;
			else m = (int)(y-1);
			if (x > 0) n = (int)x;
			else n = (int)(x-1);
			q = y - m;
			p = x - n;
			if ( (m >= 0) && (m < Y_SIZE) && (n >= 0) && (n < X_SIZE) )
				d = (int)((1.0-q)*((1.0-p)*image_in[m  ][n  ]
				                       + p*image_in[m  ][n+1])
				              + q*((1.0-p)*image_in[m+1][n  ]
				                       + p*image_in[m+1][n+1]));
			else
				d = 0;
			if (d <   0) d =   0;
			if (d > 255) d = 255;
			image_out[i][j] = d;
		}
	}
}
