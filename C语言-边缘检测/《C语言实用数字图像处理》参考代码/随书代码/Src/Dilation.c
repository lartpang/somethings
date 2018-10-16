#include	"Params.h"

/*--- dilation --- �c������ ---------------------------------------------------
	image_in:	���͉摜�z��
	image_out:	�o�͉摜�z��
-----------------------------------------------------------------------------*/
void dilation(unsigned char image_in[Y_SIZE][X_SIZE], 
	unsigned char image_out[Y_SIZE][X_SIZE])
{
	int	i, j;

	for (i = 1; i < Y_SIZE-1; i++) {
		for (j = 1; j < X_SIZE-1; j++) {
			image_out[i][j] = image_in[i][j];
			if (image_in[i-1][j-1] == HIGH) image_out[i][j] = HIGH;
			if (image_in[i-1][j  ] == HIGH) image_out[i][j] = HIGH;
			if (image_in[i-1][j+1] == HIGH) image_out[i][j] = HIGH;
			if (image_in[i  ][j-1] == HIGH) image_out[i][j] = HIGH;
			if (image_in[i  ][j+1] == HIGH) image_out[i][j] = HIGH;
			if (image_in[i+1][j-1] == HIGH) image_out[i][j] = HIGH;
			if (image_in[i+1][j  ] == HIGH) image_out[i][j] = HIGH;
			if (image_in[i+1][j+1] == HIGH) image_out[i][j] = HIGH;
	  }
	}
}
