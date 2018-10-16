#include "Params.h"

/*--- laplacian --- 画像の２次微分 ---------------------------------------------
	image_in:	入力画像配列
	image_out:	出力画像配列
	amp:		出力画像の利得
	type:		係数タイプ(1,2,3)
-----------------------------------------------------------------------------*/
void laplacian(unsigned char image_in[Y_SIZE][X_SIZE], 
	unsigned char image_out[Y_SIZE][X_SIZE], double amp, int type)
{
	int	i, j;
	int d;
	int c[3][9] = { 0, -1,  0, -1,  4, -1,  0, -1,  0,
				   -1, -1, -1, -1,  8, -1, -1, -1, -1,
				    1, -2,  1, -2,  4, -2,  1, -2,  1};

	type = type - 1;
	if (type < 0) type = 0;
	if (type > 2) type = 2;
	for (i = 1; i < Y_SIZE-1; i++) {
		for (j = 1; j < X_SIZE-1; j++) {
			d = c[type][0] * image_in[i-1][j-1]
			  + c[type][1] * image_in[i-1][j  ]
			  + c[type][2] * image_in[i-1][j+1]
			  + c[type][3] * image_in[i  ][j-1]
			  + c[type][4] * image_in[i  ][j  ]
			  + c[type][5] * image_in[i  ][j+1]
			  + c[type][6] * image_in[i+1][j-1]
			  + c[type][7] * image_in[i+1][j  ]
			  + c[type][8] * image_in[i+1][j+1];
			d = (int)(d * amp) + OFFSET;
			if (d <   0) d = 0;
			if (d > 255) d = 255;
			image_out[i][j] = (unsigned char)d;
		}
	}
}
