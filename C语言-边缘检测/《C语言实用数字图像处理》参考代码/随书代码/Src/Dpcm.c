#include "Params.h"

#define  B_VAL  128     /* ��ʂ̘g�O�̃��x�� */

/*--- dpcm1 --- �\���������@�c�o�b�l�i(1)�̗\���@�F�P���C�����̏����j--------
	image_in:	�摜�f�[�^
	line:		���C���ԍ�
	data_out:	�P���C�����̂c�o�b�l�f�[�^
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

/*--- dpcm2 --- �\���������@�c�o�b�l�i(2)�̗\���@�F�P���C�����̏����j--------
	image_in:	�摜�f�[�^
	line:		���C���ԍ�
	data_out:	�P���C�����̂c�o�b�l�f�[�^
-----------------------------------------------------------------------------*/
void dpcm2(unsigned char image_in[Y_SIZE][X_SIZE], 
	int line, short data_out[X_SIZE])
{
	int pred, j;

	if (line == 0) {         /* ��P���C���� */
		for (j = 0; j < X_SIZE; j++) {
			if (j == 0) pred = B_VAL;
			else        pred = (image_in[line][j-1] + B_VAL) / 2;
			data_out[j] = image_in[line][j] - pred;
		}
	}
	else {                  /* ���̑��̃��C�� */
		for (j = 0; j < X_SIZE; j++) {
			if (j == 0) pred = (B_VAL + image_in[line-1][j]) / 2;
			else pred = (image_in[line][j-1] + image_in[line-1][j]) / 2;
			data_out[j] = image_in[line][j] - pred;
		}
	}
}
