#include	"Params.h"

/*--- smooth --- �ړ����ϖ@�ɂ��m�C�Y���� -----------------------------------
	image_in:	���͉摜�z��
	image_out:	�o�͉摜�z��
	type:		�������͈́i3:3x3��f�C5:5x5��f�C7:7x7��f�C�E�E�E�j
-----------------------------------------------------------------------------*/
void smooth(unsigned char image_in[Y_SIZE][X_SIZE], 
	unsigned char image_out[Y_SIZE][X_SIZE], int type)
{
	int i, j, m, n, k, x, y;
	double sum, num;

	k = type / 2;
	num = (double)type * type;
	for (i = 0; i < Y_SIZE; i++) {
		for (j = 0; j < X_SIZE; j++) {
			sum = 0.0;
			for (m = -k; m <= k; m++) {
				for (n = -k; n <= k; n++) {
					y = i + m;
					x = j + n;
					if ( y < 0) y = 0;
					if ( x < 0) x = 0;
					if ( y > Y_SIZE) y = Y_SIZE;
					if ( x > X_SIZE) x = X_SIZE;
					sum += image_in[y][x];
				}
			}
			sum = sum / num;
			if (sum <   0) sum =   0;
			if (sum > 255) sum = 255;
			image_out[i][j] = (unsigned char)sum;
		}
	}
}
