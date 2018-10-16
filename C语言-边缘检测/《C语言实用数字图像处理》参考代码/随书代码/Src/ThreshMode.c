#include "Params.h"

void histgram(unsigned char image_in[Y_SIZE][X_SIZE], long hist[256]);
void histsmooth(long hist_in[256], long hist_out[256]);
int threshmode(long hist[256]);

/*--- threshold_mode --- モード法による閾値処理 -------------------------------
	image_in:	入力画像配列
	image_out:	出力画像配列
	smt:		ヒストグラム平滑化の回数
	type:		閾値処理の方法(1,2)
-----------------------------------------------------------------------------*/
void threshold_mode(unsigned char image_in[Y_SIZE][X_SIZE], 
	unsigned char image_out[Y_SIZE][X_SIZE], int smt, int type)
{
	int	i, j, m, n;
	int thresh;
	long hist1[256], hist2[256];

	histgram(image_in, hist1);
	for (m = 0; m < smt; m++) {
		for (n = 0; n < 256; n++) hist2[n] = hist1[n];
		histsmooth(hist2, hist1);
	}
	thresh = threshmode(hist1);
	for (i = 0; i < Y_SIZE; i++) {
		for (j = 0; j < X_SIZE; j++) {
			switch (type){
				case 2:
					if ((int)image_in[i][j] <= thresh) image_out[i][j] = HIGH;
					else                               image_out[i][j] =  LOW;
					break;
				default:
					if ((int)image_in[i][j] >= thresh) image_out[i][j] = HIGH;
					else                               image_out[i][j] =  LOW;
					break;
			}
		}
	}
}

/*--- threshmode --- モード法で閾（しきい）値を決定する -----------------------
	hist:	ヒストグラム
-----------------------------------------------------------------------------*/
int threshmode(long hist[256])
{
	int m, n;
	long max, min;

	max = 0;
	for (m = 0; m < 256; m++) {
		if (max <= hist[m]) max = hist[m];
		else break;
	}
	min = max;
	for (n = m; n < 256; n++) {
		if (min >= hist[n]) min = hist[n];
		else break;
	}
	return n - 1;
}
