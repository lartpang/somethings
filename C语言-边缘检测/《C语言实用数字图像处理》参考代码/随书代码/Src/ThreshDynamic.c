#include "Params.h"

#define DIV 8
#define XS (X_SIZE / DIV)
#define YS (Y_SIZE / DIV)
#define DTH 0.7

int threshdiscrim(long hist[256], double disparity);

/*--- threshold_dynamic --- 動的閾値処理 --------------------------------------
	image_in:	入力画像配列
	image_out:	出力画像配列
	type:		閾値処理の方法(1,2)
-----------------------------------------------------------------------------*/
void threshold_dynamic(unsigned char image_in[Y_SIZE][X_SIZE], 
	unsigned char image_out[Y_SIZE][X_SIZE], int type)
{
	int	 i, j, k, m, n, m1, m2, n1, n2, s, t;
	int  thm[DIV+1][DIV+1];
	long hist[256];
	int  thresh;
	double p, q;

	for (i = 0; i <= DIV; i++) {
		for (j = 0; j <= DIV; j++) {
			thm[i][j] = 0;
		}
	}
	/* 格子点の閾値の決定 */
	for (i = 0; i <= DIV; i++) {
		for (j = 0; j <= DIV; j++) {
			for (k = 0; k < 256; k++) hist[k] = 0;
			if (i != 0) m1 = -YS;
			else m1 = 0;
			if (i != DIV) m2 = YS;
			else m2 = 0;
			if (j != 0) n1 = -XS;
			else n1 = 0;
			if (j != DIV) n2 = XS;
			else n2 = 0;
			for (m = m1; m < m2; m++) {
				for (n = n1; n < n2; n++) {
					k = image_in[i*YS+m][j*XS+n];
					hist[k]++;
				}
			}
			thm[i][j] = threshdiscrim(hist, DTH);
		}
	}
	/* 閾値が得られなかった格子点の閾値の決定 */
	for (i = 0; i <= DIV; i++) {
		for (j = 0; j <= DIV; j++) {
			if (thm[i][j] <= 0) {
				for (k = 1; k < DIV; k++) {
					s = 0;
					t = 0;
					m1 = i - k;
					m2 = i + k;
					n1 = j - k;
					n2 = j + k;
					if (m1 <   0) m1 = 0;
					if (m2 > DIV) m2 = DIV;
					if (n1 <   0) n1 = 0;
					if (n2 > DIV) n2 = DIV;
					for (m = m1; m <= m2; m++) {
						for (n = n1; n <= n2; n++) {
							if (thm[m][n] > 0) {
								s += 1 / k;
							    t += thm[m][n] / k;
							}
						}
					}
					if (s >= 4) {
						thm[i][j] = t / s;
						break;
					}
				}
			}
		}
	}
	/* 画素ごとの閾値の決定 */
	for (i = 0; i < Y_SIZE; i++) {
		for (j = 0; j < X_SIZE; j++) {
			m = i / YS;
			n = j / XS;
			q = (double)(i % YS) / YS;
			p = (double)(j % XS) / XS;
			thresh = (int)((1.0-q)*((1.0-p)*thm[m  ][n  ]
			                            + p*thm[m  ][n+1])
			                   + q*((1.0-p)*thm[m+1][n  ]
			                            + p*thm[m+1][n+1]));
			switch (type){
				case 2:
					if ((int)image_in[i][j] <= thresh)
						image_out[i][j] = HIGH;
					else
						image_out[i][j] =  LOW;
					break;
				default:
					if ((int)image_in[i][j] >= thresh)
						image_out[i][j] = HIGH;
					else
						image_out[i][j] =  LOW;
					break;
			}
		}
	}
}

