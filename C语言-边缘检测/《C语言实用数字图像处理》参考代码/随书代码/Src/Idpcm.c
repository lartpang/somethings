#include "Params.h"

#define  B_VAL  128     /* ��ʂ̘g�O�̃��x�� */

/*--- idpcm1 --- �c�o�b�l�̕����i(1)�̗\���@�F�P���C�����̏����j-------------
	data_in:	�P���C�����̂c�o�b�l�f�[�^
	line:		���C���ԍ�
	image_out:	�摜�f�[�^
-----------------------------------------------------------------------------*/
void idpcm1(short data_in[X_SIZE], int line, 
	unsigned char image_out[Y_SIZE][X_SIZE])
{
	int pred;               /* �\���l */
	int j;

	for (j = 0; j < X_SIZE; j++) {
		if (j == 0) pred = B_VAL;
		else        pred = (int)image_out[line][j-1];
		image_out[line][j] = (unsigned char)(pred + (int)data_in[j]);
	}
}

/*--- idpcm2 --- �c�o�b�l�̕����i(2)�̗\���@�F�P���C�����̏����j-------------
	data_in:	�P���C�����̂c�o�b�l�f�[�^
	line:		���C���ԍ�
	image_out:	�摜�f�[�^
-----------------------------------------------------------------------------*/
void idpcm2(short data_in[X_SIZE], int line, 
	unsigned char image_out[Y_SIZE][X_SIZE])
{
	int pred, j;

	if (line == 0) {                /* ��P���C���� */
		for (j = 0; j < X_SIZE; j++) {
			if (j == 0) pred = B_VAL;
			else        pred = (image_out[line][j-1] + B_VAL) / 2;
			image_out[line][j] = pred + data_in[j];
		}
	}
	else {                  /* ���̑��̃��C�� */
		for (j = 0; j < X_SIZE; j++) {
			if (j == 0) pred = (B_VAL + image_out[line-1][j]) / 2;
			else pred = (image_out[line][j-1] + image_out[line-1][j]) / 2;
			image_out[line][j] = pred + data_in[j];
		}
	}
}
