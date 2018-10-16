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

/*--- fft2 --- �Q�����e�e�s�̎��s ---------------------------------------------
		�iX_SIZE�CY_SIZE���Q�ׂ̂���̏ꍇ�Ɍ���j
	a_rl:	�f�[�^�������i���o�͌��p�j
	a_im:	�f�[�^�������i���o�͌��p�j
	inv:	1: �c�e�s�C-1: �t�c�e�s
-----------------------------------------------------------------------------*/
int fft2 (double a_rl[Y_SIZE][X_SIZE], double a_im[Y_SIZE][X_SIZE], int inv)
{
	double *b_rl;		/* �f�[�^�]�u��Ɨp�z��i�������j*/
	double *b_im;		/* �f�[�^�]�u��Ɨp�z��i�������j*/
	double *hsin_tbl;	/* �����pSIN�v�Z�p�e�[�u��		*/
	double *hcos_tbl;	/* �����pCOS�v�Z�p�e�[�u��		*/
	double *vsin_tbl;	/* �����pSIN�v�Z�p�e�[�u��		*/
	double *vcos_tbl;	/* �����pCOS�v�Z�p�e�[�u��		*/
	double *buf_x;		/* ��Ɨp�o�b�t�@�i���������j	*/
	double *buf_y;		/* ��Ɨp�o�b�t�@�i���������j	*/
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
	cstb(X_SIZE, inv, hsin_tbl, hcos_tbl);	/* �����pSIN,COS�e�[�u���쐬	*/
	cstb(Y_SIZE, inv, vsin_tbl, vcos_tbl);	/* �����pSIN,COS�e�[�u���쐬	*/
	/* ���������̂e�e�s */
	for (i = 0; i < Y_SIZE; i++) {
		fft1core(&a_rl[(long)i][0], &a_im[(long)i][0],
					X_SIZE, X_EXP, hsin_tbl, hcos_tbl, buf_x);
	}
	/* �Q�����f�[�^�̓]�u */
	rvmtx1((double (*)[X_SIZE])a_rl, (double (*)[X_SIZE])b_rl, X_SIZE, Y_SIZE);
	rvmtx1((double (*)[X_SIZE])a_im, (double (*)[X_SIZE])b_im, X_SIZE, Y_SIZE);
	/* ���������̂e�e�s */
	for (i = 0; i < X_SIZE; i++) {
		fft1core(&b_rl[(long)Y_SIZE*i], &b_im[(long)Y_SIZE*i], 
					Y_SIZE, Y_EXP, vsin_tbl, vcos_tbl, buf_y);
	}
	/* �Q�����f�[�^�̓]�u */
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

/*--- rvmtx1 --- �Q�����f�[�^�̓]�u -------------------------------------------
	a:		�Q�������̓f�[�^
	b:		�Q�����o�̓f�[�^
	xsize:	�����f�[�^��
	ysize:	�����f�[�^��
-----------------------------------------------------------------------------*/
void rvmtx1(double a[Y_SIZE][X_SIZE], double b[X_SIZE][Y_SIZE], 
	int xsize, int ysize)
{
	int i, j;

	for (i = 0; i < ysize; i++)
		for (j = 0; j < xsize; j++)
			b[j][i] = a[i][j];
}

/*--- rvmtx2 --- �Q�����f�[�^�̓]�u -------------------------------------------
	a:		�Q�������̓f�[�^
	b:		�Q�����o�̓f�[�^
	xsize:	�����f�[�^��
	ysize:	�����f�[�^��
-----------------------------------------------------------------------------*/
void rvmtx2(double a[X_SIZE][Y_SIZE], double b[Y_SIZE][X_SIZE], 
	int xsize, int ysize)
{
	int i, j;

	for (i = 0; i < ysize; i++)
		for (j = 0; j < xsize; j++)
			b[i][j] = a[j][i];
}
