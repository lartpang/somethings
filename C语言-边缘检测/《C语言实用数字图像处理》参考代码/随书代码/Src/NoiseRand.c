#include	<stdlib.h>
#include	"Params.h"

/*
#define RAND_MAX 32767
RAND_MAX�irand()�̍ő�l�j������`�̂b�R���p�C���ł́C���define���𐶂����D
��`�l��2147483647�̏ꍇ������D
*/

/*--- noise_rand --- �����_���m�C�Y�t�� --------------------------------------
	image_in:	���͉摜�z��
	image_out:	�o�͉摜�z��
	level:		�m�C�Y���x���̍ő�l
----------------------------------------------------------------------------*/
void noise_rand(unsigned char image_in[Y_SIZE][X_SIZE],
	unsigned char image_out[Y_SIZE][X_SIZE], int level)
{
	int i, j;
	int data, noise;

	for (i = 0; i < Y_SIZE; i++) {
		for (j = 0; j < X_SIZE; j++) {
			noise = (int)((rand() / (double)RAND_MAX - 0.5) * level * 2.0);
			data = image_in[i][j] + noise;
			if      (data > 255) image_out[i][j] =  255;
			else if (data <   0) image_out[i][j] =    0;
			else                 image_out[i][j] = data;
		}
	}
}
