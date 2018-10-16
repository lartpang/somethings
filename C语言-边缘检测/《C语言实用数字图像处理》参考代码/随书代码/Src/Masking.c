#include "Params.h"

/*---- masking --- マスク領域だけ画像データをコピーする -----------------------
	image_in:	入力画像配列
	image_out:	出力画像配列
	image_mask:	マスク画像配列（２値画像）
-----------------------------------------------------------------------------*/
void masking(unsigned char image_in[Y_SIZE][X_SIZE],
	unsigned char image_out[Y_SIZE][X_SIZE], 
	unsigned char image_mask[Y_SIZE][X_SIZE])
{
	int	i, j;

	for (i = 0; i < Y_SIZE; i++) {
		for (j = 0; j < X_SIZE; j++) {
		  	if (image_mask[i][j] == HIGH)	image_out[i][j] = image_in[i][j];
		  	else							image_out[i][j] = 0;
		}
	}
}
