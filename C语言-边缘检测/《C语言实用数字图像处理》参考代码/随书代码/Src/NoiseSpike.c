#include	<stdlib.h>
#include	"Params.h"

/*
#define RAND_MAX 32767
RAND_MAX�irand()�̍ő�l�j������`�̂b�R���p�C���ł́C���define���𐶂����D
��`�l��2147483647�̏ꍇ������D
*/

/*--- noise_spike --- �X�p�C�N�m�C�Y�t�� -------------------------------------
	image_in:	���͉摜�z��
	image_out:	�o�͉摜�z��
	number:		�X�p�C�N�m�C�Y�̐�
	level:		�m�C�Y���x���̍ő�l
----------------------------------------------------------------------------*/
void noise_spike(unsigned char image_in[Y_SIZE][X_SIZE],
	unsigned char image_out[Y_SIZE][X_SIZE], int number, int level)
{
	int i, x, y;
	int data, noise;

	for (i = 0; i < number; i++) {
		x = (int)((rand() / (double)RAND_MAX) * X_SIZE);
		y = (int)((rand() / (double)RAND_MAX) * Y_SIZE);
		noise = (int)((rand() / (double)RAND_MAX - 0.5) * level * 2.0);
		data = image_in[y][x] + noise;
		if      (data > 255) image_out[y][x] =  255;
		else if (data <   0) image_out[y][x] =    0;
		else                 image_out[y][x] = data;
	}
}
