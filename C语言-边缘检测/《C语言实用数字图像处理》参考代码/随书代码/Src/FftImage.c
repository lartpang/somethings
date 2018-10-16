#include	<stdio.h>
#include	<stdlib.h>
#include	<math.h>
#include	"Params.h"

int fft2 (double a_rl[Y_SIZE][X_SIZE], double a_im[Y_SIZE][X_SIZE], int inv);

/*--- fftimage --- �Q�����e�e�s�̌��ʂ��摜������ -----------------------------
		�iX_SIZE�CY_SIZE���Q�ׂ̂���̏ꍇ�Ɍ���j
	image_in:	���͉摜�z��
	image_out:	�o�͉摜�z��i�e�e�s�j
-----------------------------------------------------------------------------*/
int fftimage(unsigned char image_in[Y_SIZE][X_SIZE], 
	unsigned char image_out[Y_SIZE][X_SIZE])
{
	double *ar;	/* �f�[�^�������i���o�͌��p�j*/
	double *ai;	/* �f�[�^�������i���o�͌��p�j*/
	double norm, max;
	double data;
	long i, j;

	ar = (double *)malloc((size_t)Y_SIZE*X_SIZE*sizeof(double));
	ai = (double *)malloc((size_t)Y_SIZE*X_SIZE*sizeof(double));
	if ((ar == NULL) || (ai == NULL)) return -1;
	/* ���摜��ǂݍ��݁C�Q�����e�e�s�̓��̓f�[�^�ɕϊ����� */
	for (i = 0; i < Y_SIZE; i++) {
		for (j = 0; j < X_SIZE; j++) {
			ar[X_SIZE*i + j] = (double)image_in[i][j];
			ai[X_SIZE*i + j] = 0.0;
		}
	}
	/* �Q�����e�e�s�����s���� */
	if (fft2((double (*)[X_SIZE])ar, (double (*)[X_SIZE])ai, 1) == -1) return -1;
	/* �e�e�s���ʂ��摜������ */
	max = 0;
	for (i = 0; i < Y_SIZE; i++) {
		for (j = 0; j < X_SIZE; j++) {
			norm = ar[X_SIZE*i + j]*ar[X_SIZE*i + j] 
			     + ai[X_SIZE*i + j]*ai[X_SIZE*i + j];	/* �U�������v�Z */
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
	/* �e�e�s���ʂ��摜�f�[�^�ɕϊ����� */
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
