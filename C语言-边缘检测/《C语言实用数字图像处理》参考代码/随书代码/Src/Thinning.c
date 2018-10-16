#include "Params.h"

#define TMP 128	/* 背景候補の濃度暫定値 */

int ncon(int p[9]);

/*--- thinning --- ２値画像を細線化する ---------------------------------------
	image_in:	入力画像配列
	image_out:	出力画像配列
-----------------------------------------------------------------------------*/
void thinning(unsigned char image_in[Y_SIZE][X_SIZE], 
	unsigned char image_out[Y_SIZE][X_SIZE])
{
 	int flg = 1;
	int i, j, k, n;
	int p[9];	/* 図形:1，背景：0，背景候補：-1 */

	for (i = 0; i < Y_SIZE; i++)
		for (j = 0; j < X_SIZE; j++)
			image_out[i][j] = image_in[i][j];
	while (flg != 0) {
		flg = 0;
		for (i = 1; i < Y_SIZE-1; i++) {
			for (j = 1; j < X_SIZE-1; j++) {
                p[0] = image_out[i  ][j  ];
				p[1] = image_out[i  ][j+1];
	  			p[2] = image_out[i-1][j+1];
				p[3] = image_out[i-1][j  ];
			  	p[4] = image_out[i-1][j-1];
			  	p[5] = image_out[i  ][j-1];
			  	p[6] = image_out[i+1][j-1];
			  	p[7] = image_out[i+1][j  ];
			  	p[8] = image_out[i+1][j+1];
				for (k = 0; k < 9; k++) {
					if      (p[k] == HIGH) p[k] =  1;
                    else if (p[k] ==  LOW) p[k] =  0;
					else                   p[k] = -1;
				}
				/* 条件1:図形の一部である */
				if (p[0] != 1) continue;
				/* 条件2:境界画素である(4近傍のうち1個以上が背景) */
				if (p[1] * p[3] * p[5] * p[7] != 0) continue;
				/* 条件3:端点を保存する(8近傍のうち2個以上が図形) */
				n = 0;
				for (k = 1; k < 9; k++) if (p[k] != 0) n++;
				if (n < 2) continue;
				/* 条件4:孤立点を保存する(8近傍のうち1個以上が図形) */
				n = 0;
				for (k = 1; k < 9; k++) if (p[k] == 1) n++;
				if (n < 1) continue;
				/* 条件5:連結性を保存する(8連結数が1である) */
				if (ncon(p) != 1) continue;
				/* 条件6:線幅が2のとき片方だけ除去する(8近傍全てにおいて，
				   -1でないか，-1の時この値を0とした場合の8連結数が1である) */
				n = 0;
				for (k = 1; k < 9; k++) {
					if (p[k] != -1) n++;
					else if (p[k] == -1) {
						p[k] = 0;
						if (ncon(p) == 1) n++;
						p[k] = -1;
					}
				}
				if (n < 8) continue;
				/* 条件1～6を全て満たす場合が削除対象 */
				image_out[i][j] = TMP;
				flg++;
			}
		}
		for (i = 1; i < Y_SIZE-1; i++)
			for (j = 1; j < X_SIZE-1; j++)
				if (image_out[i][j] == TMP) image_out[i][j] = LOW;
	}
}

/*--- ncon --- 連結数（8近傍連結）を調べる ------------------------------------
	p:	画素配列（3x3）
-----------------------------------------------------------------------------*/
int ncon(int p[9])
{
	int	i, i1, i2;
	int q[9];
	int n = 0;

	for (i = 0; i < 9; i++) {
		if ((p[i] == 1) || (p[i] == -1)) q[i] = 0;
		else q[i] = 1;
	}
	for (i = 1; i < 9; i+=2) {
		i1 = i + 1;
		i2 = i + 2;
		if (i2 == 9) i2 = 1;
		n = n + q[i] - q[i] * q[i1] * q[i2];
	}
	return n;
}
