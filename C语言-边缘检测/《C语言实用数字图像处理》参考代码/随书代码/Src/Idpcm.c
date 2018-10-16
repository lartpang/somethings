#include "Params.h"

#define  B_VAL  128     /* 画面の枠外のレベル */

/*--- idpcm1 --- ＤＰＣＭの復号（(1)の予測法：１ラインずつの処理）-------------
	data_in:	１ライン分のＤＰＣＭデータ
	line:		ライン番号
	image_out:	画像データ
-----------------------------------------------------------------------------*/
void idpcm1(short data_in[X_SIZE], int line, 
	unsigned char image_out[Y_SIZE][X_SIZE])
{
	int pred;               /* 予測値 */
	int j;

	for (j = 0; j < X_SIZE; j++) {
		if (j == 0) pred = B_VAL;
		else        pred = (int)image_out[line][j-1];
		image_out[line][j] = (unsigned char)(pred + (int)data_in[j]);
	}
}

/*--- idpcm2 --- ＤＰＣＭの復号（(2)の予測法：１ラインずつの処理）-------------
	data_in:	１ライン分のＤＰＣＭデータ
	line:		ライン番号
	image_out:	画像データ
-----------------------------------------------------------------------------*/
void idpcm2(short data_in[X_SIZE], int line, 
	unsigned char image_out[Y_SIZE][X_SIZE])
{
	int pred, j;

	if (line == 0) {                /* 第１ライン目 */
		for (j = 0; j < X_SIZE; j++) {
			if (j == 0) pred = B_VAL;
			else        pred = (image_out[line][j-1] + B_VAL) / 2;
			image_out[line][j] = pred + data_in[j];
		}
	}
	else {                  /* その他のライン */
		for (j = 0; j < X_SIZE; j++) {
			if (j == 0) pred = (B_VAL + image_out[line-1][j]) / 2;
			else pred = (image_out[line][j-1] + image_out[line-1][j]) / 2;
			image_out[line][j] = pred + data_in[j];
		}
	}
}
