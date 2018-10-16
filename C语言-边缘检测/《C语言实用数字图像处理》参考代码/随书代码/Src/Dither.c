#include "Params.h"

/*--- dither_ordered --- 組織的ディザ法 ---------------------------------------
	image_in:	入力画像配列
	image_out:	出力画像配列
-----------------------------------------------------------------------------*/
void dither_ordered(unsigned char image_in[Y_SIZE][X_SIZE], 
		unsigned char image_out[Y_SIZE][X_SIZE])
{
	static int thres[4][4] = {  0,  8,  2, 10,
							   12,  4, 14,  6,
							    3, 11,  1,  9,
							   15,  7, 13,  5,};	/* ディザマトリクス	*/
	int		i, j, m, n;

	for (i = 0 ; i<Y_SIZE/4 ; i++)
		for (j = 0 ; j<X_SIZE/4 ; j++)
			for (m = 0; m < 4; m++)
				for (n = 0; n < 4; n++)
				if ((int)image_in[i*4+m][j*4+n] > thres[m][n]*16+8) 
					image_out[i*4+m][j*4+n] = HIGH;
				else image_out[i*4+m][j*4+n] = LOW;
}

/*--- dither_minimized --- 平均誤差最小ディザ法 -------------------------------
	image_in:	入力画像配列
	image_out:	出力画像配列
-----------------------------------------------------------------------------*/
void dither_minimized(unsigned char image_in[Y_SIZE][X_SIZE], 
		unsigned char image_out[Y_SIZE][X_SIZE])
{
	int	i, j;
	int	t, d;
	int	error[X_SIZE], error1[X_SIZE], error2[X_SIZE]; 	/* 誤差配列 */

	t = (HIGH+1)/2;		/*　閾値　この例では１２８	*/
	for (i = 0; i < X_SIZE; i++) {
		if (image_in[0][i] > t) image_out[0][i] = HIGH;
		else		image_out[0][i] = LOW;
		error1[i] = image_in[0][i]-image_out[0][i] ;
	}
	for (i = 0; i < X_SIZE; i++) {
		if (image_in[1][i] > t) image_out[1][i] = HIGH;
		else		image_out[1][i] = LOW;
		error2[i] = image_in[1][i]-image_out[1][i];
	}
	for (i = 2; i < Y_SIZE; i++) {
		if (image_in[i][0] > t) image_out[i][0] = HIGH;
		else	image_out[i][0] = LOW;
		error[0] = image_in[i][0]-image_out[i][0];
		if (image_in[i][1] > t) image_out[i][1] = HIGH;
		else	image_out[i][1] = LOW;
		error[1] = image_in[i][1]-image_out[i][1];
		for (j = 2; j < X_SIZE-2; j++) {
		    d = (error1[j-2]+error1[j-1]*3+error1[j]*5
		    	+error1[j+1]*3+error1[j+2]
		    	+error2[j-2]*3+error2[j-1]*5+error2[j]*7
		    	+error2[j+1]*5+error2[j+2]*3
		    	+error[j-2]*5+error[j-1]*7)/48;
		    if ((int)image_in[i][j]+d > t) image_out[i][j] = HIGH;
		    else	image_out[i][j] = LOW;
		    error[j] = image_in[i][j]+d-image_out[i][j];
		}
		if (image_in[i][X_SIZE-2] > t) image_out[i][X_SIZE-2] = HIGH;
		else	image_out[i][X_SIZE-2] = LOW;
		error[X_SIZE-2] = image_in[i][X_SIZE-2]-image_out[i][X_SIZE-2];
		if (image_in[i][X_SIZE-1] > t) image_out[i][X_SIZE-1] = HIGH;
		else	image_out[i][X_SIZE-1] = LOW;
		error[X_SIZE-1] = image_in[i][X_SIZE-1]-image_out[i][X_SIZE-1];
		for (j = 0; j < X_SIZE; j++) {
			error1[j] = error2[j];
			error2[j] = error[j];
		}
	}
}
