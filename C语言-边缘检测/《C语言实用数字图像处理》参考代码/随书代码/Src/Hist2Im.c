#include  "Params.h"

#define BIAS	128	/*　ヒストグラム画像のバイアス値　*/

/*--- hist2_image --- ２次元ヒストグラムを求め画像化する ----------------------
	image_in1:	画像データ　Ｘ軸用
	image_in2:	画像データ　Ｙ軸用
	image_hist:	２次元ヒストグラム
-----------------------------------------------------------------------------*/
void hist2_image(unsigned char  image_in1[Y_SIZE][X_SIZE], 
	unsigned char image_in2[Y_SIZE][X_SIZE], 
	unsigned char image_hist[Y_SIZE][X_SIZE])
{
	int   i, j, kx, ky;
	int   hx, hy, max, kk;

	for (i = 0; i < Y_SIZE; i++)                      /* 初期化 */
		for (j = 0; j < X_SIZE; j++)
			image_hist[i][j] = 0;
	max = 0;
	ky = 256 / Y_SIZE;
	kx = 256 / X_SIZE; 
	for (i = 0; i < Y_SIZE; i++) {
		for (j = 0; j < X_SIZE; j++) {
			hy = (HIGH - (int)image_in2[i][j]) / ky;
			hx = ((int)image_in1[i][j]) / kx;
			if (image_hist[hy][hx] < HIGH) image_hist[hy][hx]++;
			if (max < image_hist[hy][hx]) max = image_hist[hy][hx];
		}
	}
	for (i = 0; i < Y_SIZE; i++) {
		for (j = 0; j < X_SIZE; j++) {
			if (image_hist[i][j] != 0) {
				kk = (long)image_hist[i][j] * HIGH / max + BIAS;
				if (kk > HIGH)	image_hist[i][j] = HIGH;
				else			image_hist[i][j] = kk;
			}
		}
	}
	for (i = 0; i < Y_SIZE; i++) image_hist[i][0] = HIGH;   	   	/* Ｘ軸 */
	for (j = 0; j < X_SIZE; j++) image_hist[Y_SIZE-1][j] = HIGH; 	/* Ｙ軸 */
}
