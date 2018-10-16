#include "Params.h"

/*---- lattice --- �i�q����� ------------------------------------------
	image:	�o�͉摜�z��iRGB�J���[�j
-----------------------------------------------------------------------------*/
void lattice(unsigned char image[Y_SIZE][X_SIZE])
{
	int i, j, width;

	width = X_SIZE / 8;
	for (i = 0; i < Y_SIZE; i++) {
		for (j = 0; j < X_SIZE; j++) {
		if (((i - Y_SIZE/2) % 32 == 0) || (i == Y_SIZE-1) ||
			((j - X_SIZE/2) % 32 == 0) || (j == X_SIZE-1))
			image[i][j] = 255;
		else
			image[i][j] = 0;
		}
	}
}
