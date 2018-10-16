#include "Params.h"

/*--- y_image --- �P�x�f�[�^��Z�W�摜������ ----------------------------------
	image_in_y:	�P�x�̃f�[�^�z��
	image_out:	�o�͉摜�z��
-----------------------------------------------------------------------------*/
void y_image(int image_in_y[Y_SIZE][X_SIZE],
	unsigned char image_out[Y_SIZE][X_SIZE])
{
	int i, j, d;

	for (i = 0; i < Y_SIZE; i++){
		for (j = 0; j < X_SIZE; j++ ){
			d = image_in_y[i][j];
			if (d <   0) d =   0;
			if (d > 255) d = 255;
			image_out[i][j] = (unsigned char)d;
		}
	}
}

/*--- sat_image --- �ʓx�f�[�^��Z�W�摜������ --------------------------------
	image_in_sat:	�ʓx�̃f�[�^�z��
	image_out:		�o�͉摜�z��
-----------------------------------------------------------------------------*/
void sat_image(int image_in_sat[Y_SIZE][X_SIZE],
	unsigned char image_out[Y_SIZE][X_SIZE])
{
	int i, j, d;

	for (i = 0; i < Y_SIZE; i++){
		for (j = 0; j < X_SIZE; j++ ){
			d = image_in_sat[i][j];
			if (d <   0) d =   0;
			if (d > 255) d = 255;
			image_out[i][j] = (unsigned char)d;
		}
	}
}

/*--- hue_image --- �F���f�[�^���摜������ ------------------------------------
	image_in_sat:	�ʓx�̃f�[�^�z��
	image_in_hue:	�F���̃f�[�^�z��
	image_out:		�o�͉摜�z��
	org:			��F��(�x)
-----------------------------------------------------------------------------*/
void hue_image(int image_in_sat[Y_SIZE][X_SIZE],
	int image_in_hue[Y_SIZE][X_SIZE],
	unsigned char image_out[Y_SIZE][X_SIZE], int org)
{
	int    i, j;
	double d;

	for (i = 0; i < Y_SIZE; i++){
		for (j = 0; j < X_SIZE; j++){
			if (image_in_sat[i][j] > 0) {
				d = image_in_hue[i][j];
				d = d - org;
				if (d <    0) d = -d;
				if (d >  180) d = 360.0 - d;
				d = 255.0 - d * 255.0 / 180.0;
				if (d <   0) d =   0;
				if (d > 255) d = 255;
				image_out[i][j] = (unsigned char)d;
			}
			else image_out[i][j]=0;
		}
	}
}
