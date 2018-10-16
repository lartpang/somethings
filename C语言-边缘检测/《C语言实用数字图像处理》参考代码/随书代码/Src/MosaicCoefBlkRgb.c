#include <math.h>
#include "Params.h"

/*--- mozaic_coef_blockmatch_rgb --- ブロックマッチングでモザイク位置を求める --
	image_in1:	入力画像配列1(カラー)
	image_in2:	入力画像配列2(カラー)
	x1, y1:		画像1,2を張り合わせる際の対応位置探索の画像1における基準点
	x2, y2:		画像1,2を張り合わせる際の対応位置探索の画像2における基準点
	xd, yd:		画像1,2を張り合わせる際の対応位置探索の範囲（±xd,±yd）
	bx, by:		マッチング領域
	mx, my:		重ね合わせ位置
-----------------------------------------------------------------------------*/
void mosaic_coef_blockmatch_rgb(unsigned char image_in1[3][Y_SIZE][X_SIZE], 
	unsigned char image_in2[3][Y_SIZE][X_SIZE],
	int x1, int y1, int x2, int y2, int xd, int yd, int bx, int by,
	int *mx, int *my)
{
	int i, j, m, n;
	int xx1, yy1, xx2, yy2;
	double d, min;

	for (i = -yd; i <= yd; i++) {
		for (j = -xd; j <= xd; j++) {
 			d = 0;
			for (m = -by/2; m < by/2; m++) {
				yy1 = y1 + m;
				yy2 = y2 + i + m;
				for (n = -bx/2; n < bx/2; n++) {
					xx1 = x1 + n;
					xx2 = x2 + j + n;
					d += abs(image_in1[0][yy1][xx1] - image_in2[0][yy2][xx2])
					   + abs(image_in1[1][yy1][xx1] - image_in2[1][yy2][xx2])
					   + abs(image_in1[2][yy1][xx1] - image_in2[2][yy2][xx2]);
				}
			}
			if (j == -xd && i == -yd) {
				min = d;
				*mx = j + x1 - x2;
				*my = i + y1 - y2;
			}
			if (d < min) {
				min = d;
				*mx = j + x1 - x2;
				*my = i + y1 - y2;
			}
 		}
	}
}
