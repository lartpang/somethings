#include "Params.h"

/*---- colorbar --- カラーバーを作る ------------------------------------------
	image_rgb:	出力画像配列（RGBカラー）
	level:		濃度値
-----------------------------------------------------------------------------*/
void colorbar(unsigned char image_rgb[3][Y_SIZE][X_SIZE], int level)
{
	int     i, j, width;

	width = X_SIZE / 8;
	for (i = 0; i < Y_SIZE; i++){
		for (j = 0; j < X_SIZE; j++){
			if (((j >= 0) && (j < 2*width)) ||  /* Ｒプレーン */
					((j >= 4*width) && (j < 6*width)))
				image_rgb[0][i][j] = level;
			else image_rgb[0][i][j] = 0;
			if ((j >= 0) && (j < 4*width ))     /* Ｇプレーン */
				image_rgb[1][i][j] = level;
			else image_rgb[1][i][j] = 0;
			if (((j >= 0) && (j < width )) ||   /* Ｂプレーン */
					((j >= 2*width) && (j < 3*width)) ||
					((j >= 4*width) && (j < 5*width)) ||
					((j >= 6*width) && (j < 7*width)))
				image_rgb[2][i][j] = level;
			else image_rgb[2][i][j] = 0;
		}
	}
}
