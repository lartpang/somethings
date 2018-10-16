#include <stdio.h>
#include <stdlib.h>
#include <math.h>
#include "Params.h"

int fft2 (double a_rl[Y_SIZE][X_SIZE], double a_im[Y_SIZE][X_SIZE], int inv);

/*--- fftfilter --- ＦＦＴによる画像のフィルタ処理 ----------------------------
		（X_SIZE，Y_SIZEが２のべき乗の場合に限る）
	image_in:	入力画像配列
	image_out:	出力画像配列
	deg:		回転角（度）
	a, b:		通過帯域（a以上，b以下の周波数成分を通過する）
				a=0，b=X_SIZE=Y_SIZEのとき，全帯域を通過
-----------------------------------------------------------------------------*/
int fftfilter(unsigned char image_in[Y_SIZE][X_SIZE], 
	unsigned char image_out[Y_SIZE][X_SIZE], int a, int b)
{
	double *ar;   /*　データ実数部（入出力兼用）　*/
	double *ai;   /*　データ虚数部（入出力兼用）　*/
	double *ff;   /*　フィルタの空間周波数特性　　*/
	double data;
	long i, j, circ;

	ar = (double *)malloc((size_t)Y_SIZE*X_SIZE*sizeof(double));
	ai = (double *)malloc((size_t)Y_SIZE*X_SIZE*sizeof(double));
	ff = (double *)malloc((size_t)Y_SIZE*X_SIZE*sizeof(double));
	if ((ar == NULL) || (ai == NULL) || (ff == NULL)) return -1;
	/* 原画像を読み込み，２次元ＦＦＴの入力データに変換する */
	for (i = 0; i < Y_SIZE; i++) {
		for (j = 0; j < X_SIZE; j++) {
			ar[X_SIZE*i + j] = (double)image_in[i][j];
			ai[X_SIZE*i + j] = 0.0;
		}
	}
	/* ＦＦＴを実行し，原画像を周波数成分に変換する */
	if (fft2((double (*)[X_SIZE])ar, (double (*)[X_SIZE])ai, 1) == -1)
		return -1;
	/* 周波数a以上b以下の成分だけを通過するフィルタを作る */
	for (i = 0; i < Y_SIZE; i++) {
		for(j = 0; j < X_SIZE; j++) {
			data = (double)((j-X_SIZE/2)*(j-X_SIZE/2)
				+ (i-Y_SIZE/2)*(i-Y_SIZE/2));
			circ = (long)sqrt(data);
			if ((circ >= a) && (circ <= b))
				ff[X_SIZE*i + j] = 1.0;
			else
				ff[X_SIZE*i + j] = 0.0;
		}
	}
	/* 原画像の周波数成分に対してフィルタ処理を行なう */
	for (i = 0; i < Y_SIZE; i++) {
		for (j = 0; j < X_SIZE; j++) {
			ar[X_SIZE*i + j] *= ff[X_SIZE*i + j];
			ai[X_SIZE*i + j] *= ff[X_SIZE*i + j];
		}
	}
	/* 逆ＦＦＴを実行し，フィルタ処理された周波数成分を画像に戻す */
	if (fft2((double (*)[X_SIZE])ar, (double (*)[X_SIZE])ai, -1) == -1)
		return -1;
	/* 結果を画像データに変換する */
	for (i = 0; i < Y_SIZE; i++) {
		for (j = 0; j < X_SIZE; j++) {
			data = ar[X_SIZE*i + j];
			if (data > 255) data = 255;
			if (data <   0) data = 0;
			image_out[i][j] = (unsigned char)data;
		}
	}
	free(ar);
	free(ai);
	free(ff);
	return 0;
}
