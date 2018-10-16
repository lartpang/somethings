#include	<stdio.h>
#include	"Params.h"

#define	L_BASE	100		/* 連結成分のラベルのベース値 */

void labelset(unsigned char image[Y_SIZE][X_SIZE], int xs, int ys, int label);

/*--- labeling --- 画像のラベリングを行う -------------------------------------
	image_in:		入力画像配列（２値画像）
	image_label:	出力画像配列（ラベル画像）
-----------------------------------------------------------------------------*/
int labeling(unsigned char image_in[Y_SIZE][X_SIZE], 
	unsigned char image_label[Y_SIZE][X_SIZE])
{
	int	i, j, label;

	for (i = 0; i < Y_SIZE; i++)
		for (j = 0; j < X_SIZE; j++)
			image_label[i][j] = image_in[i][j];
	label = L_BASE;
	for (i = 0; i < Y_SIZE; i++)
		for (j = 0; j < X_SIZE; j++) {
			if (image_label[i][j] == HIGH) {
				if (label >= HIGH) return -1;
				labelset(image_label, j, i, label); label++;
			}
	    }
	return label - L_BASE;
}

/*--- labelset --- 連結している画素すべてにラベル付けする ---------------------
	image:	画像配列
	xs, ys:	スタート位置
	label:	ラベル番号
-----------------------------------------------------------------------------*/
void labelset(unsigned char	image[Y_SIZE][X_SIZE], int xs, int ys, int label)
{
	int	i, j, cnt, im, ip, jm, jp;

	image[ys][xs] = label;
	for (;;) {
		cnt = 0;
		for (i = 0; i < Y_SIZE; i++)
			for (j = 0; j < X_SIZE; j++)
				if (image[i][j] == label) {
					im = i-1; ip = i+1; jm = j-1; jp = j+1;
					if (im < 0) im = 0; if (ip >= Y_SIZE) ip = Y_SIZE-1;
					if (jm < 0) jm = 0; if (jp >= X_SIZE) jp = X_SIZE-1;
					if (image[i ][jp] == HIGH) {
						image[i ][jp] = label; cnt++;
					}
					if (image[im][jp] == HIGH) {
						image[im][jp] = label; cnt++;
					}
					if (image[im][j ] == HIGH) {
						image[im][j ] = label; cnt++;
					}
					if (image[im][jm] == HIGH) {
						image[im][jm] = label; cnt++;
					}
					if (image[i ][jm] == HIGH) {
						image[i ][jm] = label; cnt++;
					}
					if (image[ip][jm] == HIGH) {
						image[ip][jm] = label; cnt++;
					}
					if (image[ip][j ] == HIGH) {
						image[ip][j ] = label; cnt++;
					}
					if (image[ip][jp] == HIGH) {
						image[ip][jp] = label; cnt++;
					}
				}
		if (cnt == 0) break;
	}
}
