#include "Params.h"

void amplify(unsigned char image_in[Y_SIZE][X_SIZE], 
	unsigned char image_out[Y_SIZE][X_SIZE], double a, double b);

/*--- expand --- 濃度を0から255の範囲に拡大する -------------------------------
	image_in:	入力画像配列
	image_out:	出力画像配列
	fmax:		入力画像の濃度の最大値
	fmin:		入力画像の濃度の最小値
-----------------------------------------------------------------------------*/
void expand(unsigned char image_in[Y_SIZE][X_SIZE], 
	unsigned char image_out[Y_SIZE][X_SIZE], int fmax, int fmin)
{
	double a, b;

	a = 255.0 / (double)(fmax - fmin);
	b = -255.0 * fmin / (double)(fmax - fmin);
	amplify(image_in, image_out, a, b);
}
