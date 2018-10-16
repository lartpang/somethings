#include	"Params.h"

/*--- smooth --- 移動平均法によるノイズ除去 -----------------------------------
	image_in:	入力画像配列
	image_out:	出力画像配列
	type:		平滑化範囲（3:3x3画素，5:5x5画素，7:7x7画素，・・・）
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
