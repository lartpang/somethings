#include	"Params.h"

/*--- smooth_weighted --- 重み付き移動平均去 ----------------------------------
	image_in:	入力画像配列
	image_out:	出力画像配列
	type:		重み係数タイプ（1,2,3）
-----------------------------------------------------------------------------*/
void smooth_weighted(unsigned char image_in[Y_SIZE][X_SIZE], 
	unsigned char image_out[Y_SIZE][X_SIZE], int type)
{
	int	i, j, m, n;
	double f;
	double c[3][5][5] = {     0,      0,      0,      0,      0,
				              0, 1.0/10, 1.0/10, 1.0/10,      0,
						      0, 1.0/10, 2.0/10, 1.0/10,      0,
						      0, 1.0/10, 1.0/10, 1.0/10,      0,
					          0,      0,      0,      0,      0,
					          0,      0,      0,      0,      0,
				              0, 1.0/16, 2.0/16, 1.0/16,      0,
						      0, 2.0/16, 4.0/16, 2.0/16,      0,
						      0, 1.0/16, 2.0/16, 1.0/16,      0,
					          0,      0,      0,      0,      0,
					     0.0030, 0.0133, 0.0219, 0.0133, 0.0030,
					     0.0133, 0.0596, 0.0983, 0.0596, 0.0133,
					     0.0219, 0.0983, 0.1621, 0.0983, 0.0219,
					     0.0133, 0.0596, 0.0983, 0.0596, 0.0133,
					     0.0030, 0.0133, 0.0219, 0.0133, 0.0030};

	if (type < 1) type = 1;
	if (type > 3) type = 3;
	for (i = 1; i < Y_SIZE-1; i++) {
		for (j = 1; j < X_SIZE-1; j++) {
			f = 0.0;
			for (m = -2; m <= 2; m++) {
				for (n = -2; n <= 2; n++) {
					if ((i+m >= 0) && (i+m <= Y_SIZE - 1)
						&& (j+n >= 0) && (j+n <= X_SIZE - 1))
						f += image_in[i+m][j+n] * c[type-1][2+m][2+n];
				}
			}
			if (f <   0) f = 0;
			if (f > 255) f = 255;
			image_out[i][j] = (unsigned char)f;
		}
	}
}
