#include	<stdio.h>
#include	<stdlib.h>
#include	<math.h>
#include	"Params.h"

int fft2 (double a_rl[Y_SIZE][X_SIZE], double a_im[Y_SIZE][X_SIZE], int inv);

/*--- fftimage --- ２次元ＦＦＴの結果を画像化する -----------------------------
		（X_SIZE，Y_SIZEが２のべき乗の場合に限る）
	image_in:	入力画像配列
	image_out:	出力画像配列（ＦＦＴ）
-----------------------------------------------------------------------------*/
int fftimage(unsigned char image_in[Y_SIZE][X_SIZE], 
	unsigned char image_out[Y_SIZE][X_SIZE])
{
	double *ar;	/* データ実数部（入出力兼用）*/
	double *ai;	/* データ虚数部（入出力兼用）*/
	double norm, max;
	double data;
	long i, j;

	ar = (double *)malloc((size_t)Y_SIZE*X_SIZE*sizeof(double));
	ai = (double *)malloc((size_t)Y_SIZE*X_SIZE*sizeof(double));
	if ((ar == NULL) || (ai == NULL)) return -1;
	/* 原画像を読み込み，２次元ＦＦＴの入力データに変換する */
	for (i = 0; i < Y_SIZE; i++) {
		for (j = 0; j < X_SIZE; j++) {
			ar[X_SIZE*i + j] = (double)image_in[i][j];
			ai[X_SIZE*i + j] = 0.0;
		}
	}
	/* ２次元ＦＦＴを実行する */
	if (fft2((double (*)[X_SIZE])ar, (double (*)[X_SIZE])ai, 1) == -1) return -1;
	/* ＦＦＴ結果を画像化する */
	max = 0;
	for (i = 0; i < Y_SIZE; i++) {
		for (j = 0; j < X_SIZE; j++) {
			norm = ar[X_SIZE*i + j]*ar[X_SIZE*i + j] 
			     + ai[X_SIZE*i + j]*ai[X_SIZE*i + j];	/* 振幅成分計算 */
			if (norm != 0.0) norm = log(norm) / 2.0;
			else norm = 0.0;
			ar[X_SIZE*i + j] = norm;
			if (norm > max) max = norm;
		}
	}
	for (i = 0; i < Y_SIZE; i++) {
		for (j = 0; j < X_SIZE; j++) {
			ar[X_SIZE*i + j] = ar[X_SIZE*i + j]*255 / max;
		}
	}
	/* ＦＦＴ結果を画像データに変換する */
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
	return 0;
}
