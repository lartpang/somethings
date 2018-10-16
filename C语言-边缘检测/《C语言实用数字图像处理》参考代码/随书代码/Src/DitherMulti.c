#include "Params.h"

void d_quantize(int in, unsigned char *pout, int nq);

/*--- dither_minimized_multi --- 平均誤差最小ディザ法（多階調） ---------------
	image_in:	入力画像配列
	image_out:	出力画像配列
	nq:			階調数
-----------------------------------------------------------------------------*/
void dither_minimized_multi(unsigned char image_in[Y_SIZE][X_SIZE],
	unsigned char image_out[Y_SIZE][X_SIZE], int nq)
{
	int     i, j;
	int     d;
	int	error[X_SIZE], error1[X_SIZE], error2[X_SIZE]; 	/* 誤差配列 */

	for (i = 0; i < X_SIZE; i++) {
		d_quantize((int)image_in[0][i], &image_out[0][i], nq);
     		error1[i] = (int)image_in[0][i]-(int)image_out[0][i];
     	}
	for (i = 0; i < X_SIZE; i++) {
		d_quantize((int)image_in[1][i], &image_out[1][i], nq);
     		error2[i] = (int)image_in[1][i]-image_out[1][i];
	}
	for (i = 2; i < Y_SIZE; i++) {
		d_quantize((int)image_in[i][0], &image_out[i][0], nq);
		error[0] = (int)image_in[i][0]-(int)image_out[i][0];
		d_quantize((int)image_in[i][1], &image_out[i][1], nq);
		error[1] = (int)image_in[i][1]-(int)image_out[i][1];
		for (j = 2; j < X_SIZE-2; j++) {
		    d = (error1[j-2]+error1[j-1]*3+error1[j]*5
			+error1[j+1]*3+error1[j+2]
			+error2[j-2]*3+error2[j-1]*5+error2[j]*7
			+error2[j+1]*5+error2[j+2]*3
			+error[j-2]*5+error[j-1]*7)/48;
			d_quantize((int)image_in[i][j]+d, &image_out[i][j], nq);
			error[j] = (int)image_in[i][j]+d-(int)image_out[i][j];
		}
		d_quantize((int)image_in[i][X_SIZE-2], &image_out[i][X_SIZE-2], nq);
		error[X_SIZE-2] = (int)image_in[i][X_SIZE-2]
			- (int)image_out[i][X_SIZE-2];
		d_quantize((int)image_in[i][X_SIZE-1], &image_out[i][X_SIZE-1], nq);
		error[X_SIZE-1] = (int)image_in[i][X_SIZE-1]
			- (int)image_out[i][X_SIZE-1];
		for (j = 0; j < X_SIZE; j++) {
			error1[j] = error2[j];
			error2[j] = error[j];
		}
	}
}

/*--- quantize --- 量子化する ------------------------------------------------
	image_in:	入力画像配列
	image_out:	出力画像配列
	nq:			階調数
-----------------------------------------------------------------------------*/
void quantize(unsigned char image_in[Y_SIZE][X_SIZE],
	unsigned char image_out[Y_SIZE][X_SIZE], int nq)
{
	int i, j;
	
	for (i = 0; i < Y_SIZE; i++)
		for (j = 0; j < X_SIZE; j++)
			d_quantize(image_in[i][j], &image_out[i][j], nq);
}

/*--- d_quantize --- 量子化（データ） -----------------------------------------
	in:		入力
	pout:	出力
	nq:		階調数
-----------------------------------------------------------------------------*/
void d_quantize(int in, unsigned char *pout, int nq)
{
	int t, i;

	t = (HIGH + 1)/(nq - 1);
	i = (int)((double)in / t + 0.5) * t;
	if (i > HIGH)	*pout = HIGH;
	else *pout = i;
}
