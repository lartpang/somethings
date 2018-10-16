#include "Params.h"

void tran_ysh(int image_in_ysh[3][Y_SIZE][X_SIZE],
	int image_out_ysh[3][Y_SIZE][X_SIZE], 
	double ya, double yb, double sa, double sb, double hb);
void range_ys(int image_in_ysh[3][Y_SIZE][X_SIZE],
	int *ymax, int *ymin, int *smax, int *smin);

/*--- expand_ysh --- �P�x�C�ʓx��0����255�͈̔͂Ɋg�傷�� ----------------------
	image_in_ysh:	���͉摜�z��iYSH�J���[�j
	image_out_ysh:	�o�͉摜�z��iYSH�J���[�j
	type:			�g��̕��@(0:Y�̂�, 1:YS����)
-----------------------------------------------------------------------------*/
void expand_ysh(int image_in_ysh[3][Y_SIZE][X_SIZE],
	int image_out_ysh[3][Y_SIZE][X_SIZE], int type)
{
	int ymax, ymin, smax, smin;
	double ya, yb, sa;

	switch (type) {
		case 1:
			range_ys(image_in_ysh, &ymax, &ymin, &smax, &smin);
			ya = 255.0 / (double)(ymax - ymin);
			yb = -255.0 * ymin / (double)(ymax - ymin);
			sa = 255.0 / (double)smax;
			tran_ysh(image_in_ysh, image_in_ysh, ya, yb, sa, 0.0, 0.0);
			break;
		default:
			range_ys(image_in_ysh, &ymax, &ymin, &smax, &smin);
			ya = 255.0 / (double)(ymax - ymin);
			yb = -255.0 * ymin / (double)(ymax - ymin);
			tran_ysh(image_in_ysh, image_in_ysh, ya, yb, 1.0, 0.0, 0.0);
			break;
	}
}

/*--- range_ys --- �P�x�C�ʓx�͈̔͂����߂� ------------------------------------
	image_in_ysh:	���͉摜�z��iYSH�J���[�j
	ymax:		�P�x�̍ő�l
	ymin:		�P�x�̍ŏ��l
	smax:		�ʓx�̍ő�l
	smin:		�ʓx�̍ŏ��l
-----------------------------------------------------------------------------*/
void range_ys(int image_in_ysh[3][Y_SIZE][X_SIZE],
	int *ymax, int *ymin, int *smax, int *smin)
{
	int i, j, n;

	*ymax = 0;
	*ymin = 255;
	for (i = 0; i < Y_SIZE; i++){
		for (j = 0; j < X_SIZE; j++){
			n = image_in_ysh[0][i][j];
			if (n > *ymax) *ymax = n;
			if (n < *ymin) *ymin = n;
		}
	}
	*smax = 0;
	*smin = 255;
	for (i = 0; i < Y_SIZE; i++){
		for (j = 0; j < X_SIZE; j++){
			n = image_in_ysh[1][i][j];
			if (n > *smax) *smax = n;
			if (n < *smin) *smin = n;
		}
	}
}
