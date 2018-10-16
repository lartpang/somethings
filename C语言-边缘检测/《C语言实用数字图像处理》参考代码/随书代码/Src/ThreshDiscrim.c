#include "Params.h"

void histgram(unsigned char image_in[Y_SIZE][X_SIZE], long hist[256]);
int threshdiscrim(long hist[256], double disparity);

/*--- threshold_discrim --- 判別分析法による閾値処理 --------------------------
	image_in:	入力画像配列
	image_out:	出力画像配列
	type:		閾値処理の方法(1,2)
-----------------------------------------------------------------------------*/
void threshold_discrim(unsigned char image_in[Y_SIZE][X_SIZE], 
	unsigned char image_out[Y_SIZE][X_SIZE], int type)
{
	int	i, j;
	int thresh;
	long hist[256];

	histgram(image_in, hist);
	thresh = threshdiscrim(hist, 0.0);
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

/*--- threshdiscrim --- 判別分析法で閾（しきい）値を決定する ------------------
	hist:		ヒストグラム
	disparity:	分離度（この値より大の場合に閾値を，それ以外の場合は0を返す）
-----------------------------------------------------------------------------*/
int threshdiscrim(long hist[256], double disparity)
{
	int i, k;
	double n0, n1, n2, m0, m1, m2;
	double v[256], vmax, v0;

	n0 = 0.0;
	m0 = 0.0;
	for (i = 0; i < 256; i++) {
		n0 += hist[i];
		m0 += i * hist[i];
	}
	if (n0 == 0.0) m0 = 0.0;
	else m0 /= n0;
	v0 = 0.0;
	for (i = 0; i < 256; i++) v0 += hist[i] * (i - m0) * (i - m0) / n0;
	for (k = 0; k < 256; k++) {
		n1 = 0.0;
		m1 = 0.0;
		for (i = 0; i < k; i++) {
			n1 += hist[i];
			m1 += i * hist[i];
		}
		if (n1 == 0.0) m1 = 0.0;
		else m1 /= n1;
		n2 = 0.0;
		m2 = 0.0;
		for (i = k; i < 256; i++) {
			n2 += hist[i];
			m2 += i * hist[i];
		}
		if (n2 == 0.0) m2 = 0.0;
		else m2 /= n2;
		v[k] = (n1 * (m1 - m0) * (m1 - m0) + n2 * (m2 - m0) * (m2 - m0)) / n0;
	}
	vmax = 0.0;
	for (i = 0; i < 256; i++) {
		if (vmax <= v[i]) {
			vmax = v[i];
			k = i;
		}
	}
	if (v0 == 0) return 0;
	if ((vmax / v0) >= disparity) return k;
	else return 0;
}
