#include        "Params.h"

/*--- scale_ng --- 拡大縮小（このやり方は，間違っている）----------------------
	image_in:	入力画像配列
	image_out:	出力画像配列
	zx:			拡大率（横）
	zy:			拡大率（縦）
-----------------------------------------------------------------------------*/
void scale_ng(unsigned char image_in[Y_SIZE][X_SIZE], 
	unsigned char image_out[Y_SIZE][X_SIZE], double zx, double zy)
{
	int	i, j, m, n;

	for (i = 0; i < Y_SIZE; i++)
		for (j = 0; j < X_SIZE; j++)
			image_out[i][j] = 0;
	for (i = 0; i < Y_SIZE; i++) {
		for (j = 0; j < X_SIZE; j++) {
			m = (int)(zy * i);
			n = (int)(zx * j);
			if ( (m >= 0) && (m < Y_SIZE) && (n >= 0) && (n < X_SIZE) )
				image_out[m][n] = image_in[i][j];
		}
	}
}
