#include "Params.h"

/*--- histgram --- �q�X�g�O���������߂鏈�� -----------------------------------
	image_in:	���͉摜�z��
	hist:		�q�X�g�O����
-----------------------------------------------------------------------------*/
void histgram(unsigned char image_in[Y_SIZE][X_SIZE], long hist[256])
{
	int	i, j, n;

	for (n = 0; n < 256; n++) hist[n] = 0;
	for (i = 0; i < Y_SIZE; i++) {
		for ( j = 0; j < X_SIZE; j++) {
			n = image_in[i][j];
			hist[n]++;
		}
	}
}
