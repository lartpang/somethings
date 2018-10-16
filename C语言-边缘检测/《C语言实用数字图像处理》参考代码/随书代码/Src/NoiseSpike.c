#include	<stdlib.h>
#include	"Params.h"

/*
#define RAND_MAX 32767
RAND_MAX（rand()の最大値）が未定義のＣコンパイラでは，上のdefine文を生かす．
定義値が2147483647の場合がある．
*/

/*--- noise_spike --- スパイクノイズ付加 -------------------------------------
	image_in:	入力画像配列
	image_out:	出力画像配列
	number:		スパイクノイズの数
	level:		ノイズレベルの最大値
----------------------------------------------------------------------------*/
void noise_spike(unsigned char image_in[Y_SIZE][X_SIZE],
	unsigned char image_out[Y_SIZE][X_SIZE], int number, int level)
{
	int i, x, y;
	int data, noise;

	for (i = 0; i < number; i++) {
		x = (int)((rand() / (double)RAND_MAX) * X_SIZE);
		y = (int)((rand() / (double)RAND_MAX) * Y_SIZE);
		noise = (int)((rand() / (double)RAND_MAX - 0.5) * level * 2.0);
		data = image_in[y][x] + noise;
		if      (data > 255) image_out[y][x] =  255;
		else if (data <   0) image_out[y][x] =    0;
		else                 image_out[y][x] = data;
	}
}
