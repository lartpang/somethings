#include <stdio.h>
#include <stdlib.h>
#include <math.h>
#include "Params.h"

void fft1core(double a_rl[], double a_im[], int length, 
	int ex, double sin_tbl[], double cos_tbl[], double buf[]);
void cstb(int length, int inv, double sin_tbl[], double cos_tbl[]);
void rvmtx1(double a[Y_SIZE][X_SIZE], double b[X_SIZE][Y_SIZE], 
	int xsize, int ysize);
void rvmtx2(double a[X_SIZE][Y_SIZE], double b[Y_SIZE][X_SIZE], 
	int xsize, int ysize);

/*--- fft2 --- ２次元ＦＦＴの実行 ---------------------------------------------
		（X_SIZE，Y_SIZEが２のべき乗の場合に限る）
	a_rl:	データ実数部（入出力兼用）
	a_im:	データ虚数部（入出力兼用）
	inv:	1: ＤＦＴ，-1: 逆ＤＦＴ
-----------------------------------------------------------------------------*/
int fft2 (double a_rl[Y_SIZE][X_SIZE], double a_im[Y_SIZE][X_SIZE], int inv)
{
	double *b_rl;		/* データ転置作業用配列（実数部）*/
	double *b_im;		/* データ転置作業用配列（虚数部）*/
	double *hsin_tbl;	/* 水平用SIN計算用テーブル		*/
	double *hcos_tbl;	/* 水平用COS計算用テーブル		*/
	double *vsin_tbl;	/* 垂直用SIN計算用テーブル		*/
	double *vcos_tbl;	/* 垂直用COS計算用テーブル		*/
	double *buf_x;		/* 作業用バッファ（水平方向）	*/
	double *buf_y;		/* 作業用バッファ（垂直方向）	*/
	int i;

	b_rl = (double *)calloc((size_t)X_SIZE*Y_SIZE, sizeof(double));
	b_im = (double *)calloc((size_t)X_SIZE*Y_SIZE, sizeof(double));
	hsin_tbl = (double *)calloc((size_t)X_SIZE, sizeof(double));
	hcos_tbl = (double *)calloc((size_t)X_SIZE, sizeof(double));
	vsin_tbl = (double *)calloc((size_t)Y_SIZE, sizeof(double));
	vcos_tbl = (double *)calloc((size_t)Y_SIZE, sizeof(double));
	buf_x = (double *)malloc((size_t)X_SIZE*sizeof(double));
	buf_y = (double *)malloc((size_t)Y_SIZE*sizeof(double));
	if ((b_rl == NULL) || (b_im == NULL)
		|| (hsin_tbl == NULL) || (hcos_tbl == NULL)
		|| (vsin_tbl == NULL) || (vcos_tbl == NULL)
		|| (buf_x == NULL) || (buf_y == NULL)) {
		return -1;
	}
	cstb(X_SIZE, inv, hsin_tbl, hcos_tbl);	/* 水平用SIN,COSテーブル作成	*/
	cstb(Y_SIZE, inv, vsin_tbl, vcos_tbl);	/* 垂直用SIN,COSテーブル作成	*/
	/* 水平方向のＦＦＴ */
	for (i = 0; i < Y_SIZE; i++) {
		fft1core(&a_rl[(long)i][0], &a_im[(long)i][0],
					X_SIZE, X_EXP, hsin_tbl, hcos_tbl, buf_x);
	}
	/* ２次元データの転置 */
	rvmtx1((double (*)[X_SIZE])a_rl, (double (*)[X_SIZE])b_rl, X_SIZE, Y_SIZE);
	rvmtx1((double (*)[X_SIZE])a_im, (double (*)[X_SIZE])b_im, X_SIZE, Y_SIZE);
	/* 垂直方向のＦＦＴ */
	for (i = 0; i < X_SIZE; i++) {
		fft1core(&b_rl[(long)Y_SIZE*i], &b_im[(long)Y_SIZE*i], 
					Y_SIZE, Y_EXP, vsin_tbl, vcos_tbl, buf_y);
	}
	/* ２次元データの転置 */
	rvmtx2((double (*)[Y_SIZE])b_rl, (double (*)[Y_SIZE])a_rl, X_SIZE, Y_SIZE);
	rvmtx2((double (*)[Y_SIZE])b_im, (double (*)[Y_SIZE])a_im, X_SIZE, Y_SIZE);
	free(b_rl);
	free(b_im);
	free(hsin_tbl);
	free(hcos_tbl);
	free(vsin_tbl);
	free(vcos_tbl);
	return 0;
}

/*--- rvmtx1 --- ２次元データの転置 -------------------------------------------
	a:		２次元入力データ
	b:		２次元出力データ
	xsize:	水平データ個数
	ysize:	垂直データ個数
-----------------------------------------------------------------------------*/
void rvmtx1(double a[Y_SIZE][X_SIZE], double b[X_SIZE][Y_SIZE], 
	int xsize, int ysize)
{
	int i, j;

	for (i = 0; i < ysize; i++)
		for (j = 0; j < xsize; j++)
			b[j][i] = a[i][j];
}

/*--- rvmtx2 --- ２次元データの転置 -------------------------------------------
	a:		２次元入力データ
	b:		２次元出力データ
	xsize:	水平データ個数
	ysize:	垂直データ個数
-----------------------------------------------------------------------------*/
void rvmtx2(double a[X_SIZE][Y_SIZE], double b[Y_SIZE][X_SIZE], 
	int xsize, int ysize)
{
	int i, j;

	for (i = 0; i < ysize; i++)
		for (j = 0; j < xsize; j++)
			b[i][j] = a[j][i];
}
