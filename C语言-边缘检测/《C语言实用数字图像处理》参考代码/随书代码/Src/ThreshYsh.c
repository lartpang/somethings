#include  "Params.h"

/*--- thresh_ysh --- 輝度,彩度,色相による閾値処理 -----------------------------
	image_in_ysh:	入力画像配列（YSHカラー）
	image_out:		出力画像配列（キー）
	ymin, ymax:		輝度Yの閾値の範囲指定　0～255
	smin, smax:		彩度Sの閾値の範囲指定　0～255
	hmin, hmax:		色相Hの閾値の範囲指定　-180～180
					(hmaxは，180度をまたぐ角度指定が可能)
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
