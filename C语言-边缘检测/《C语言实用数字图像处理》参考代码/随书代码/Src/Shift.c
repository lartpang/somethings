#include	"Params.h"

/*--- shift --- 位置の移動（線形補間法）---------------------------------------
	image_in:	入力画像配列
	image_out:	出力画像配列
	px:			移動量（横）
	py:			移動量（縦）
-----------------------------------------------------------------------------*/
void shift(unsigned char image_in[Y_SIZE][X_SIZE], 
	unsigned char image_out[Y_SIZE][X_SIZE], double px, double py)
{
	int	i, j, m, n;
	double x, y, p, q;
	int	d;

	for (i = 0; i < Y_SIZE; i++) {
		for (j = 0; j < X_SIZE; j++) {
			y = i - py;
			x = j - px;
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
