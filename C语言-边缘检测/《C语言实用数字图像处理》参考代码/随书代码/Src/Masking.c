#include "Params.h"

/*---- masking --- �}�X�N�̈悾���摜�f�[�^���R�s�[���� -----------------------
	image_in:	���͉摜�z��
	image_out:	�o�͉摜�z��
	image_mask:	�}�X�N�摜�z��i�Q�l�摜�j
-----------------------------------------------------------------------------*/
void masking(unsigned char image_in[Y_SIZE][X_SIZE],
	unsigned char image_out[Y_SIZE][X_SIZE], 
	unsigned char image_mask[Y_SIZE][X_SIZE])
{
	int	i, j;

	for (i = 0; i < Y_SIZE; i++) {
		for (j = 0; j < X_SIZE; j++) {
		  	if (image_mask[i][j] == HIGH)	image_out[i][j] = image_in[i][j];
		  	else							image_out[i][j] = 0;
		}
	}
}
