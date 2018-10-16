#include	"Params.h"

/*--- scale_near --- 拡大縮小（最近傍法）--------------------------------------
	image_in:	入力画像配列
	image_out:	出力画像配列
	zx:			拡大率（横）
	zy:			拡大率（縦）
-----------------------------------------------------------------------------*/
void scale_near(unsigned char image_in[Y_SIZE][X_SIZE], 
	unsigned char image_out[Y_SIZE][X_SIZE], double zx, double zy)
{
	int	i, j, m, n;

	for (i = 0; i < Y_SIZE; i++) {
		for (j = 0;j < X_SIZE; j++) {
			if (i > 0) m = (int)(i/zy + 0.5);
			else m = (int)(i/zy - 0.5);
			if (j > 0) n = (int)(j/zx + 0.5);
			else n = (int)(j/zx - 0.5);
			if ( (m >= 0) && (m < Y_SIZE) && (n >= 0) && (n < X_SIZE) )
				image_out[i][j] = image_in[m][n];
			else
				image_out[i][j] = 0;
		}
	}
}
