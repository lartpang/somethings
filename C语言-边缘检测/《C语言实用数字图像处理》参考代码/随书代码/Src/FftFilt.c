#include <stdio.h>
#include <stdlib.h>
#include <math.h>
#include "Params.h"

int fft2 (double a_rl[Y_SIZE][X_SIZE], double a_im[Y_SIZE][X_SIZE], int inv);

/*--- fftfilter --- �e�e�s�ɂ��摜�̃t�B���^���� ----------------------------
		�iX_SIZE�CY_SIZE���Q�ׂ̂���̏ꍇ�Ɍ���j
	image_in:	���͉摜�z��
	image_out:	�o�͉摜�z��
	deg:		��]�p�i�x�j
	a, b:		�ʉߑш�ia�ȏ�Cb�ȉ��̎��g��������ʉ߂���j
				a=0�Cb=X_SIZE=Y_SIZE�̂Ƃ��C�S�ш��ʉ�
-----------------------------------------------------------------------------*/
int fftfilter(unsigned char image_in[Y_SIZE][X_SIZE], 
	unsigned char image_out[Y_SIZE][X_SIZE], int a, int b)
{
	double *ar;   /*�@�f�[�^�������i���o�͌��p�j�@*/
	double *ai;   /*�@�f�[�^�������i���o�͌��p�j�@*/
	double *ff;   /*�@�t�B���^�̋�Ԏ��g�������@�@*/
	double data;
	long i, j, circ;

	ar = (double *)malloc((size_t)Y_SIZE*X_SIZE*sizeof(double));
	ai = (double *)malloc((size_t)Y_SIZE*X_SIZE*sizeof(double));
	ff = (double *)malloc((size_t)Y_SIZE*X_SIZE*sizeof(double));
	if ((ar == NULL) || (ai == NULL) || (ff == NULL)) return -1;
	/* ���摜��ǂݍ��݁C�Q�����e�e�s�̓��̓f�[�^�ɕϊ����� */
	for (i = 0; i < Y_SIZE; i++) {
		for (j = 0; j < X_SIZE; j++) {
			ar[X_SIZE*i + j] = (double)image_in[i][j];
			ai[X_SIZE*i + j] = 0.0;
		}
	}
	/* �e�e�s�����s���C���摜�����g�������ɕϊ����� */
	if (fft2((double (*)[X_SIZE])ar, (double (*)[X_SIZE])ai, 1) == -1)
		return -1;
	/* ���g��a�ȏ�b�ȉ��̐���������ʉ߂���t�B���^����� */
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
	/* ���摜�̎��g�������ɑ΂��ăt�B���^�������s�Ȃ� */
	for (i = 0; i < Y_SIZE; i++) {
		for (j = 0; j < X_SIZE; j++) {
			ar[X_SIZE*i + j] *= ff[X_SIZE*i + j];
			ai[X_SIZE*i + j] *= ff[X_SIZE*i + j];
		}
	}
	/* �t�e�e�s�����s���C�t�B���^�������ꂽ���g���������摜�ɖ߂� */
	if (fft2((double (*)[X_SIZE])ar, (double (*)[X_SIZE])ai, -1) == -1)
		return -1;
	/* ���ʂ��摜�f�[�^�ɕϊ����� */
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
