#include        "Params.h"

/*--- scale_ng --- �g��k���i���̂����́C�Ԉ���Ă���j----------------------
	image_in:	���͉摜�z��
	image_out:	�o�͉摜�z��
	zx:			�g�嗦�i���j
	zy:			�g�嗦�i�c�j
-----------------------------------------------------------------------------*/
void scale_ng(unsigned char image_in[Y_SIZE][X_SIZE], 
	unsigned char image_out[Y_SIZE][X_SIZE], double zx, double zy)
{
	int	i, j, m, n;

	for (i = 0; i < Y_SIZE; i++)
		for (j = 0; j < X_SIZE; j++)
			image_out[i][j] = 0;
	for (i = 0; i < Y_SIZE; i++) {
		for (j = 0; j < X_SIZE; j++) {
			m = (int)(zy * i);
			n = (int)(zx * j);
			if ( (m >= 0) && (m < Y_SIZE) && (n >= 0) && (n < X_SIZE) )
				image_out[m][n] = image_in[i][j];
		}
	}
}
