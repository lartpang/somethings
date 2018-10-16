#include "Params.h"

/*--- range --- �摜�̖��邳�͈̔͂����߂� ------------------------------------
	image_in:	���͉摜�z��
	fmax:		���͉摜�̔Z�x�̍ő�l
	fmin:		���͉摜�̔Z�x�̍ŏ��l
-----------------------------------------------------------------------------*/
void range(unsigned char image_in[Y_SIZE][X_SIZE], int *fmax, int *fmin)
{
	int i, j, n;

	*fmax = 0;
	*fmin = 255;
	for (i = 0; i < Y_SIZE; i++){
		for (j = 0; j < X_SIZE; j++){
			n =(int)image_in[i][j];
			if (n > *fmax) *fmax = n;
			if (n < *fmin) *fmin = n;
		}
	}
}
