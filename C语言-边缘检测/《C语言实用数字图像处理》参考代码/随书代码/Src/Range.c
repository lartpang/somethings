#include "Params.h"

/*--- range --- 画像の明るさの範囲を求める ------------------------------------
	image_in:	入力画像配列
	fmax:		入力画像の濃度の最大値
	fmin:		入力画像の濃度の最小値
-----------------------------------------------------------------------------*/
void range(unsigned char image_in[Y_SIZE][X_SIZE], int *fmax, int *fmin)
{
	int i, j, n;

	*fmax = 0;
	*fmin = 255;
	for (i = 0; i < Y_SIZE; i++){
		for (j = 0; j < X_SIZE; j++){
			n =(int)image_in[i][j];
			if (n > *fmax) *fmax = n;
			if (n < *fmin) *fmin = n;
		}
	}
}
