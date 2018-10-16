#include "Params.h"

/*---- pseudo_color --- �[���J���[ --------------------------------------------
	image_r:	�o�͉摜�z��q
	image_g:	�o�͉摜�z��f
	image_b:	�o�͉摜�z��a
	type:		�ϊ��^�C�v�i1,2�j
-----------------------------------------------------------------------------*/
void pseudo_color(unsigned char image_in_m[Y_SIZE][X_SIZE],
	unsigned char image_out_r[Y_SIZE][X_SIZE],
	unsigned char image_out_g[Y_SIZE][X_SIZE],
	unsigned char image_out_b[Y_SIZE][X_SIZE], int type)
{
	int i, j;
	unsigned char lutr[256], lutg[256], lutb[256];

	switch (type) {
		case 1:
			for (i =   0; i < 256; i++) lutr[i] = i;
			for (i =   0; i < 256; i++) lutg[i] = i;
			for (i =   0; i < 256; i++) lutb[i] = 255;
			break;
		case 2:
			for (i =   0; i < 128; i++) lutr[i] = 0;
			for (i = 128; i < 256; i++) lutr[i] = 2*i-255;
			for (i =   0; i < 128; i++) lutg[i] = 2*i;
			for (i = 128; i < 256; i++) lutg[i] = 510-2*i;
			for (i =   0; i < 128; i++) lutb[i] = 255-2*i;
			for (i = 128; i < 256; i++) lutb[i] = 0;
		default:
			break;
	}
	for (i = 0; i < Y_SIZE; i++) {
		for (j = 0; j < X_SIZE; j++) {
			image_out_r[i][j] = lutr[image_in_m[i][j]];
			image_out_g[i][j] = lutg[image_in_m[i][j]];
			image_out_b[i][j] = lutb[image_in_m[i][j]];
		}
	}
}
