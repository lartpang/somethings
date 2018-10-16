#include "Params.h"

/*--- threshold --- 臁i�������j�l���� ----------------------------------------
	image_in:	���͉摜�z��
	image_out:	�o�͉摜�z��
	thresh:		臒l
	type:		臒l�����̕��@(1,2)
-----------------------------------------------------------------------------*/
void threshold(unsigned char image_in[Y_SIZE][X_SIZE], 
	unsigned char image_out[Y_SIZE][X_SIZE], int thresh, int type)
{
	int	i, j;

	for (i = 0; i < Y_SIZE; i++) {
		for (j = 0; j < X_SIZE; j++) {
			switch (type) {
				case 2:
					if ((int)image_in[i][j] <= thresh) image_out[i][j] = HIGH;
					else                               image_out[i][j] =  LOW;
					break;
				default:
					if ((int)image_in[i][j] >= thresh) image_out[i][j] = HIGH;
					else                               image_out[i][j] =  LOW;
					break;
			}
		}
	}
}
