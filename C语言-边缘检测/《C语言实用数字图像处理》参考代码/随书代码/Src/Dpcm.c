#include "Params.h"

#define  B_VAL  128     /* 画面の枠外のレベル */

/*--- dpcm1 --- 予測符号化　ＤＰＣＭ（(1)の予測法：１ラインずつの処理）--------
	image_in:	画像データ
	line:		ライン番号
	data_out:	１ライン分のＤＰＣＭデータ
-----------------------------------------------------------------------------*/
void dpcm1(unsigned char image_in[Y_SIZE][X_SIZE], 
	int line, short data_out[X_SIZE])
{
	int pred ,j;

	for (j = 0; j < X_SIZE; j++) {
		if (j == 0) pred = B_VAL;
		else        pred = (int)image_in[line][j-1];
		data_out[j] = (int)image_in[line][j] - pred;
	}
}

/*--- dpcm2 --- 予測符号化　ＤＰＣＭ（(2)の予測法：１ラインずつの処理）--------
	image_in:	画像データ
	line:		ライン番号
	data_out:	１ライン分のＤＰＣＭデータ
-----------------------------------------------------------------------------*/
void dpcm2(unsigned char image_in[Y_SIZE][X_SIZE], 
	int line, short data_out[X_SIZE])
{
	int pred, j;

	if (line == 0) {         /* 第１ライン目 */
		for (j = 0; j < X_SIZE; j++) {
			if (j == 0) pred = B_VAL;
			else        pred = (image_in[line][j-1] + B_VAL) / 2;
			data_out[j] = image_in[line][j] - pred;
		}
	}
	else {                  /* その他のライン */
		for (j = 0; j < X_SIZE; j++) {
			if (j == 0) pred = (B_VAL + image_in[line-1][j]) / 2;
			else pred = (image_in[line][j-1] + image_in[line-1][j]) / 2;
			data_out[j] = image_in[line][j] - pred;
		}
	}
}
