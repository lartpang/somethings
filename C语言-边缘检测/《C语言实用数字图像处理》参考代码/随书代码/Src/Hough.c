#include <stdio.h>
#include <math.h>
#include "Params.h"

#define	PI	3.141592
#define DMAX 1000


void hough_line(unsigned char image_in[Y_SIZE][X_SIZE],
	unsigned char image_out[Y_SIZE][X_SIZE], double rho, double theta);

/*--- hough --- Hough変換による直線の抽出 ------------------------------------
	image_in:		入力画像配列
	image_out:		出力画像配列（直線抽出）
	image_hough:	出力画像配列（Hough変換）
	thres:			閾値
	buf:			メッセージ用バッファ
-----------------------------------------------------------------------------*/
void hough(unsigned char image_in[Y_SIZE][X_SIZE], 
	unsigned char image_out[Y_SIZE][X_SIZE],
	unsigned char image_hough[Y_SIZE][X_SIZE], int  thresh, char *buf)
{
	int i, j, u, v, n;
	double rho, theta, d, a;
	double p[DMAX][2];
	int posi, m;
	
	d = PI / X_SIZE;
	a = Y_SIZE/2/sqrt(X_SIZE/2 * X_SIZE/2 + Y_SIZE/2 * Y_SIZE/2);
	for (i = 0; i < Y_SIZE; i++)
		for (j = 0; j < X_SIZE; j++)
			image_hough[i][j] = 0;
	for (i = 0; i < Y_SIZE; i++) {
		for (j = 0; j < X_SIZE; j++) {
			if (image_in[i][j] != HIGH) continue;
			for (u = 0; u < X_SIZE; u++) {
				theta = u * d;
				rho = (j - X_SIZE/2) * cos(theta) + (Y_SIZE/2 - i) 
					* sin(theta);
				v = (int)(rho * a + Y_SIZE/2 + 0.5);
				if (v >= 0 && v  < Y_SIZE)
					if (image_hough[v][u] < 255) image_hough[v][u] += 1;
			}
		}
	}
	n = 0; posi = 0;
	for (u = 0; u < X_SIZE; u++)
		for (v = 0; v < Y_SIZE; v++) {
			if (image_hough[v][u] < thresh) continue;
			if (u != 0 && v != 0 &&
				image_hough[v][u] < image_hough[v-1][u-1]) continue;
			if (v != 0 &&
				image_hough[v][u] < image_hough[v-1][u  ]) continue;
			if (u != X_SIZE-1 && v != 0 &&
				image_hough[v][u] < image_hough[v-1][u+1]) continue;
			if (u != 0 &&
				image_hough[v][u] < image_hough[v  ][u-1]) continue;
			if (u != X_SIZE-1 &&
				image_hough[v][u] < image_hough[v  ][u+1]) continue;
			if (u != 0 && v != Y_SIZE-1 &&
				image_hough[v][u] < image_hough[v+1][u-1]) continue;
			if (v != Y_SIZE-1 &&
				image_hough[v][u] < image_hough[v+1][u  ]) continue;
			if (u != X_SIZE-1 && v != Y_SIZE-1 &&
				image_hough[v][u] < image_hough[v+1][u+1]) continue;
			theta = u * d;
			rho = (v - Y_SIZE/2)/a;
			p[n][0] = rho; p[n][1] = theta; n++;
			m = sprintf(&buf[posi], "theta = %10.3lf, rho = %10.3lf, value = %5d\n",
				theta * 180 / PI, rho, image_hough[v][u]);
			posi += m;
			if (n == DMAX) return;
			};
	for (i = 0; i < Y_SIZE; i++)
		for (j = 0; j < X_SIZE; j++)
		image_out[i][j] = image_in[i][j];
	for (i = 0; i < n; i++)
		hough_line(image_in, image_out, p[i][0], p[i][1]);
}

/*--- hough_line --- 直線を描画する -------------------------------------------
	image_in:	入力画像配列
	image_out:	出力画像配列
	u, v:		直線のパラメータ
-----------------------------------------------------------------------------*/
void hough_line(unsigned char image_in[Y_SIZE][X_SIZE],
	unsigned char image_out[Y_SIZE][X_SIZE], double rho, double theta)
{
	int i, j;
	
	if ((theta >=0 && theta < PI/4) || theta >= 3*PI/4) {
		for (i = 0; i < Y_SIZE; i++) {
			j = (int)((rho - (Y_SIZE/2 - i) * sin(theta)) 
				/ cos(theta) + X_SIZE/2 + 0.5);
			if (j >= 0 && j < X_SIZE) image_out[i][j] = 255;
		}
	} else {
		for (j = 0; j < X_SIZE; j++) {
			i = (int)((-rho + (j - X_SIZE/2) * cos(theta)) 
				/ sin(theta) + Y_SIZE/2 + 0.5);
			if (i >= 0 && i < Y_SIZE) image_out[i][j] = 255;
		}
	}
}
