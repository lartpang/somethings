#include	<stdlib.h>
#include	"Params.h"

/*
#define RAND_MAX 32767
RAND_MAX（rand()の最大値）が未定義のＣコンパイラでは，上のdefine文を生かす．
定義値が2147483647の場合がある．
*/

/*--- noise_rand --- ランダムノイズ付加 --------------------------------------
	image_in:	入力画像配列
	image_out:	出力画像配列
	level:		ノイズレベルの最大値
----------------------------------------------------------------------------*/
void noise_rand(unsigned char image_in[Y_SIZE][X_SIZE],
	unsigned char image_out[Y_SIZE][X_SIZE], int level)
{
	int i, j;
	int data, noise;

	for (i = 0; i < Y_SIZE; i++) {
		for (j = 0; j < X_SIZE; j++) {
			noise = (int)((rand() / (double)RAND_MAX - 0.5) * level * 2.0);
			data = image_in[i][j] + noise;
			if      (data > 255) image_out[i][j] =  255;
			else if (data <   0) image_out[i][j] =    0;
			else                 image_out[i][j] = data;
		}
	}
}
