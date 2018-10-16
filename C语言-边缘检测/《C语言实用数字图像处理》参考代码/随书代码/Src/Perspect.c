#include	"Params.h"

/*--- perspect --- 射影変換（線形補間法）--------------------------------------
	image_in:	入力画像配列
	image_out:	出力画像配列
	a,b,c:		X = (ax + by + c) / (gx + hy + 1)
	d,e,f:		Y = (dx + ey + f) / (gx + hy + 1)
	g,h:
-----------------------------------------------------------------------------*/
void perspect(unsigned char image_in[Y_SIZE][X_SIZE], 
	unsigned char image_out[Y_SIZE][X_SIZE],
	double a, double b, double c, double d, double e, double f,
	double g, double h)
{
	int	i, j, m, n;
	double x, y, w, p, q;
	int dat;

	for (i = 0; i < Y_SIZE; i++) {
		for (j = 0; j < X_SIZE; j++) {
			w = (d*h - e*g)*j + (b*g - a*h)*i + (a*e - b*d);
			x = (e - f*h)*j + (c*h - b)*i + (b*f - c*e);
			y = (f*g - d)*j + (a - c*g)*i + (c*d - a*f);
			x = x / w;
			y = y / w;
			if (y > 0) m = (int)y;
			else m = (int)(y-1);
			if (x > 0) n = (int)x;
			else n = (int)(x-1);
			q = y - m;
			p = x - n;
			if ( (m >= 0) && (m < Y_SIZE) && (n >= 0) && (n < X_SIZE) )
				dat = (int)((1.0-q)*((1.0-p)*image_in[m  ][n  ]
				                         + p*image_in[m  ][n+1])
				                + q*((1.0-p)*image_in[m+1][n  ]
				                         + p*image_in[m+1][n+1]));
			else
				dat = 0;
				if (dat <   0) dat =   0;
				if (dat > 255) dat = 255;
				image_out[i][j] = (unsigned char)dat;
		}
	}
}
