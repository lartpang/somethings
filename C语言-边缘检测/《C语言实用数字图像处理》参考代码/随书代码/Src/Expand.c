#include "Params.h"

void amplify(unsigned char image_in[Y_SIZE][X_SIZE], 
	unsigned char image_out[Y_SIZE][X_SIZE], double a, double b);

/*--- expand --- �Z�x��0����255�͈̔͂Ɋg�傷�� -------------------------------
	image_in:	���͉摜�z��
	image_out:	�o�͉摜�z��
	fmax:		���͉摜�̔Z�x�̍ő�l
	fmin:		���͉摜�̔Z�x�̍ŏ��l
-----------------------------------------------------------------------------*/
void expand(unsigned char image_in[Y_SIZE][X_SIZE], 
	unsigned char image_out[Y_SIZE][X_SIZE], int fmax, int fmin)
{
	double a, b;

	a = 255.0 / (double)(fmax - fmin);
	b = -255.0 * fmin / (double)(fmax - fmin);
	amplify(image_in, image_out, a, b);
}
