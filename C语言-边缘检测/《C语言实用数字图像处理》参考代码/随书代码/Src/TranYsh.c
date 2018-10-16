#include "Params.h"

/*--- tran_ysh --- 輝度,彩度,色相を変える -------------------------------------
	image_in_ysh:	入力画像配列（YSHカラー）
	image_out_ysh:	出力画像配列（YSHカラー）
	ya:			輝度倍率
	yb:			輝度バイアス
	sa:			彩度倍率
	sb:			彩度バイアス
	hb:			色相バイアス(度)
-----------------------------------------------------------------------------*/
void tran_ysh(int image_in_ysh[3][Y_SIZE][X_SIZE],
	int image_out_ysh[3][Y_SIZE][X_SIZE], 
	double ya, double yb, double sa, double sb, double hb)
{
	int i, j;

	for (i = 0; i < Y_SIZE; i++){
		for (j = 0; j < X_SIZE; j++){
			image_out_ysh[0][i][j] = (int)(image_in_ysh[0][i][j] * ya + yb);
			image_out_ysh[1][i][j] = (int)(image_in_ysh[1][i][j] * sa + sb);
			image_out_ysh[2][i][j] = (int)(image_in_ysh[2][i][j] + hb);
			if(image_out_ysh[2][i][j] > 180) image_out_ysh[2][i][j] -= 360;
			if(image_out_ysh[2][i][j] < -180) image_out_ysh[2][i][j] += 360;
		}
	}
}
