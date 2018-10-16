#include  "Params.h"

/*--- thresh_ysh --- �P�x,�ʓx,�F���ɂ��臒l���� -----------------------------
	image_in_ysh:	���͉摜�z��iYSH�J���[�j
	image_out:		�o�͉摜�z��i�L�[�j
	ymin, ymax:		�P�xY��臒l�͈͎̔w��@0�`255
	smin, smax:		�ʓxS��臒l�͈͎̔w��@0�`255
	hmin, hmax:		�F��H��臒l�͈͎̔w��@-180�`180
					(hmax�́C180�x���܂����p�x�w�肪�\)
-----------------------------------------------------------------------------*/
void thresh_ysh(int image_in_ysh[3][Y_SIZE][X_SIZE], 
	unsigned char image_out[Y_SIZE][X_SIZE], 
	int ymin, int ymax, int smin, int smax, int hmin, int hmax)
{
	int   i, j;

	if (hmax > 180) {
		hmax -= 360;
		for (i = 0; i < Y_SIZE; i++) {
			for (j = 0; j < X_SIZE; j++) {
				if      (image_in_ysh[0][i][j] < ymin) image_out[i][j] = LOW;
				else if (image_in_ysh[0][i][j] > ymax) image_out[i][j] = LOW;
				else if (image_in_ysh[1][i][j] < smin) image_out[i][j] = LOW;
				else if	(image_in_ysh[1][i][j] > smax) image_out[i][j] = LOW;
				else if ((image_in_ysh[2][i][j] < hmin) &&
					   (image_in_ysh[2][i][j] > hmax)) image_out[i][j] = LOW; 
				else                                   image_out[i][j] = HIGH;
			}
		}
	} else {
		for (i = 0; i < Y_SIZE; i++) {
			for (j = 0; j < X_SIZE; j++) {
				if      (image_in_ysh[0][i][j] < ymin) image_out[i][j] = LOW;
				else if (image_in_ysh[0][i][j] > ymax) image_out[i][j] = LOW;
				else if (image_in_ysh[1][i][j] < smin) image_out[i][j] = LOW;
				else if	(image_in_ysh[1][i][j] > smax) image_out[i][j] = LOW;
				else if (image_in_ysh[2][i][j] < hmin) image_out[i][j] = LOW;
				else if (image_in_ysh[2][i][j] > hmax) image_out[i][j] = LOW;
				else                                   image_out[i][j] = HIGH;
			}
		}
	}

}
