#include "Params.h"

/*--- amplify --- �摜�̖��邳�����{���� --------------------------------------
	image_in:	���͉摜�z��
	image_out:	�o�͉摜�z��
	a:			�{��
	b:			�o�C�A�X
-----------------------------------------------------------------------------*/
void amplify(unsigned char image_in[Y_SIZE][X_SIZE], 
	unsigned char image_out[Y_SIZE][X_SIZE], double a, double b)
{
	int i, j, d;

	for (i = 0; i < Y_SIZE; i++){
    	for (j = 0; j < X_SIZE; j++){
			d = (int)(image_in[i][j] * a + b);
			if (d <   0) d =   0;
			if (d > 255) d = 255;
			image_out[i][j] = (unsigned char)d;
		}
	}
}
