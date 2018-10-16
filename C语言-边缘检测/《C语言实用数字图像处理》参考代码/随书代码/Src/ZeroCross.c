#include "Params.h"

/*--- zero_cross --- ゼロ交差判定による輪郭抽出 -------------------------------
	image_in:	入力画像配列
	image_out:	出力画像配列
-----------------------------------------------------------------------------*/
void zero_cross(unsigned char image_in[Y_SIZE][X_SIZE], 
	unsigned char image_out[Y_SIZE][X_SIZE])
{
	int	i, j;

	for (i = 0; i < Y_SIZE; i++)
		for (j = 0; j < X_SIZE; j++)
			image_out[i][j] = LOW;

	for (i = 1; i < Y_SIZE-1; i++) {
		for (j = 1; j < X_SIZE-1; j++) {
			if ((int)image_in[i][j] == OFFSET) {
				if (((int)image_in[i][j+1] - OFFSET)
					* ((int)image_in[i][j-1] - OFFSET) < 0) image_out[i][j] = HIGH;
				if (((int)image_in[i+1][j] - OFFSET)
					* ((int)image_in[i-1][j] - OFFSET) < 0) image_out[i][j] = HIGH;
				if (((int)image_in[i+1][j+1] - OFFSET)
					* ((int)image_in[i-1][j-1] - OFFSET) < 0) image_out[i][j] = HIGH;
				if (((int)image_in[i+1][j-1] - OFFSET)
					* ((int)image_in[i-1][j+1] - OFFSET) < 0) image_out[i][j] = HIGH;
			}
			else {
				if (((int)image_in[i][j] - OFFSET)
					* ((int)image_in[i-1][j-1] - OFFSET) < 0) image_out[i][j] = HIGH;
				if (((int)image_in[i][j] - OFFSET)
					* ((int)image_in[i-1][j] - OFFSET) < 0) image_out[i][j] = HIGH;
				if (((int)image_in[i][j] - OFFSET)
					* ((int)image_in[i-1][j+1] - OFFSET) < 0) image_out[i][j] = HIGH;
				if (((int)image_in[i][j] - OFFSET)
					* ((int)image_in[i][j-1] - OFFSET) < 0) image_out[i][j] = HIGH;
			}
		}
	}
}
