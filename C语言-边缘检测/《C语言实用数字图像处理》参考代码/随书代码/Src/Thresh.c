#include "Params.h"

/*--- threshold --- 閾（しきい）値処理 ----------------------------------------
	image_in:	入力画像配列
	image_out:	出力画像配列
	thresh:		閾値
	type:		閾値処理の方法(1,2)
-----------------------------------------------------------------------------*/
void threshold(unsigned char image_in[Y_SIZE][X_SIZE], 
	unsigned char image_out[Y_SIZE][X_SIZE], int thresh, int type)
{
	int	i, j;

	for (i = 0; i < Y_SIZE; i++) {
		for (j = 0; j < X_SIZE; j++) {
			switch (type) {
				case 2:
					if ((int)image_in[i][j] <= thresh) image_out[i][j] = HIGH;
					else                               image_out[i][j] =  LOW;
					break;
				default:
					if ((int)image_in[i][j] >= thresh) image_out[i][j] = HIGH;
					else                               image_out[i][j] =  LOW;
					break;
			}
		}
	}
}
