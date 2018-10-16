#include  "Params.h"

/*--- thresh_color_diffference --- �F�̍��𗘗p����臒l���� -------------------
	image_in_rgb:	���͉摜�z��iRGB�J���[�j
	image_out:		�o�͉摜�z��(�L�[)
	thresh:		臒l
	type:		臒l�����̕��@(1:�Ԓ��S,2:�Β��S,3:���S)
-----------------------------------------------------------------------------*/
void thresh_color_difference(unsigned char image_in_rgb[3][Y_SIZE][X_SIZE], 
	unsigned char image_out[Y_SIZE][X_SIZE], int thresh, int type)
{
	int   i, j, d;
	
	for (i = 0; i < Y_SIZE; i++) {
		for (j = 0; j < X_SIZE; j++) {
			switch (type) {
				case 1:
					d = ((int)image_in_rgb[1][i][j]
					   + (int)image_in_rgb[2][i][j]) / 2
					   - (int)image_in_rgb[0][i][j];
					if (d >= thresh) image_out[i][j] = 255;
					else             image_out[i][j] = 0;
					break;
				case 2:
					d = ((int)image_in_rgb[2][i][j]
					   + (int)image_in_rgb[0][i][j]) / 2
					   - (int)image_in_rgb[1][i][j];
					if (d >= thresh) image_out[i][j] = 255;
					else             image_out[i][j] = 0;
					break;
				case 3:
					d = ((int)image_in_rgb[0][i][j]
					   + (int)image_in_rgb[1][i][j]) / 2
					   - (int)image_in_rgb[2][i][j];
					if (d >= thresh) image_out[i][j] = 255;
					else             image_out[i][j] = 0;
					break;
				default:
					break;
			}
		}
	}
}
