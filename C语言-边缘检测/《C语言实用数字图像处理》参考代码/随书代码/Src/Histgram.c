#include "Params.h"

/*--- histgram --- ヒストグラムを求める処理 -----------------------------------
	image_in:	入力画像配列
	hist:		ヒストグラム
-----------------------------------------------------------------------------*/
void histgram(unsigned char image_in[Y_SIZE][X_SIZE], long hist[256])
{
	int	i, j, n;

	for (n = 0; n < 256; n++) hist[n] = 0;
	for (i = 0; i < Y_SIZE; i++) {
		for ( j = 0; j < X_SIZE; j++) {
			n = image_in[i][j];
			hist[n]++;
		}
	}
}
