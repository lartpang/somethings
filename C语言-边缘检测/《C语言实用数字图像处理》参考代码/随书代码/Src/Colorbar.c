#include "Params.h"

/*---- colorbar --- �J���[�o�[����� ------------------------------------------
	image_rgb:	�o�͉摜�z��iRGB�J���[�j
	level:		�Z�x�l
-----------------------------------------------------------------------------*/
void colorbar(unsigned char image_rgb[3][Y_SIZE][X_SIZE], int level)
{
	int     i, j, width;

	width = X_SIZE / 8;
	for (i = 0; i < Y_SIZE; i++){
		for (j = 0; j < X_SIZE; j++){
			if (((j >= 0) && (j < 2*width)) ||  /* �q�v���[�� */
					((j >= 4*width) && (j < 6*width)))
				image_rgb[0][i][j] = level;
			else image_rgb[0][i][j] = 0;
			if ((j >= 0) && (j < 4*width ))     /* �f�v���[�� */
				image_rgb[1][i][j] = level;
			else image_rgb[1][i][j] = 0;
			if (((j >= 0) && (j < width )) ||   /* �a�v���[�� */
					((j >= 2*width) && (j < 3*width)) ||
					((j >= 4*width) && (j < 5*width)) ||
					((j >= 6*width) && (j < 7*width)))
				image_rgb[2][i][j] = level;
			else image_rgb[2][i][j] = 0;
		}
	}
}
