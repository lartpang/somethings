#include "Params.h"

/*--- zero_cross --- �[����������ɂ��֊s���o -------------------------------
	image_in:	���͉摜�z��
	image_out:	�o�͉摜�z��
-----------------------------------------------------------------------------*/
void zero_cross(unsigned char image_in[Y_SIZE][X_SIZE], 
	unsigned char image_out[Y_SIZE][X_SIZE])
{
	int	i, j;

	for (i = 0; i < Y_SIZE; i++)
		for (j = 0; j < X_SIZE; j++)
			image_out[i][j] = LOW;

	for (i = 1; i < Y_SIZE-1; i++) {
		for (j = 1; j < X_SIZE-1; j++) {
			if ((int)image_in[i][j] == OFFSET) {
				if (((int)image_in[i][j+1] - OFFSET)
					* ((int)image_in[i][j-1] - OFFSET) < 0) image_out[i][j] = HIGH;
				if (((int)image_in[i+1][j] - OFFSET)
					* ((int)image_in[i-1][j] - OFFSET) < 0) image_out[i][j] = HIGH;
				if (((int)image_in[i+1][j+1] - OFFSET)
					* ((int)image_in[i-1][j-1] - OFFSET) < 0) image_out[i][j] = HIGH;
				if (((int)image_in[i+1][j-1] - OFFSET)
					* ((int)image_in[i-1][j+1] - OFFSET) < 0) image_out[i][j] = HIGH;
			}
			else {
				if (((int)image_in[i][j] - OFFSET)
					* ((int)image_in[i-1][j-1] - OFFSET) < 0) image_out[i][j] = HIGH;
				if (((int)image_in[i][j] - OFFSET)
					* ((int)image_in[i-1][j] - OFFSET) < 0) image_out[i][j] = HIGH;
				if (((int)image_in[i][j] - OFFSET)
					* ((int)image_in[i-1][j+1] - OFFSET) < 0) image_out[i][j] = HIGH;
				if (((int)image_in[i][j] - OFFSET)
					* ((int)image_in[i][j-1] - OFFSET) < 0) image_out[i][j] = HIGH;
			}
		}
	}
}
