#include	"Params.h"

/*--- erosion --- ���k���� ----------------------------------------------------
	image_in:	���͉摜�z��
	image_out:	�o�͉摜�z��
-----------------------------------------------------------------------------*/
void erosion(unsigned char image_in[Y_SIZE][X_SIZE], 
	unsigned char image_out[Y_SIZE][X_SIZE])
{
	int	i, j;

	for (i = 1; i < Y_SIZE-1; i++) {
		for (j = 1; j < X_SIZE-1; j++) {
			image_out[i][j] = image_in[i][j];
			if (image_in[i-1][j-1] == LOW) image_out[i][j] = LOW;
			if (image_in[i-1][j  ] == LOW) image_out[i][j] = LOW;
			if (image_in[i-1][j+1] == LOW) image_out[i][j] = LOW;
			if (image_in[i  ][j-1] == LOW) image_out[i][j] = LOW;
			if (image_in[i  ][j+1] == LOW) image_out[i][j] = LOW;
			if (image_in[i+1][j-1] == LOW) image_out[i][j] = LOW;
			if (image_in[i+1][j  ] == LOW) image_out[i][j] = LOW;
			if (image_in[i+1][j+1] == LOW) image_out[i][j] = LOW;
		}
	}
}
