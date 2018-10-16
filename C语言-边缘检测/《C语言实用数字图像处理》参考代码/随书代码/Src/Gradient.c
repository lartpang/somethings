#include <math.h>
#include "Params.h"

int cx_difference[9] = 	{ 0, 0, 0,	/* オペレータの係数ｘ(通常の差分) */
						  0, 1,-1,
						  0, 0, 0};
int cy_difference[9] = 	{ 0, 0, 0,	/* オペレータの係数ｙ(通常の差分) */
						  0, 1, 0,	
						  0,-1, 0 };	

int cx_roberts[9] = 	{ 0, 0, 0,	/* オペレータの係数ｘ(Roberts) */
						  0, 1, 0,
						  0, 0,-1};
int cy_roberts[9] = 	{ 0, 0, 0,	/* オペレータの係数ｙ(Roberts) */
						  0, 0, 1,	
						  0,-1, 0 };	

int cx_sobel[9] = 		{-1, 0, 1,	/* オペレータの係数ｘ(Sobel) */
						 -2, 0, 2,
						 -1, 0, 1 };
int cy_sobel[9] = 		{-1,-2,-1,	/* オペレータの係数ｙ(Sobel) */
						  0, 0, 0,	
						  1, 2, 1 };	

void gradient(unsigned char image_in[Y_SIZE][X_SIZE], 
	unsigned char image_out[Y_SIZE][X_SIZE], double amp, int cx[9], int cy[9]);

/*--- gradient_difference --- 画像の１次微分（通常の差分） --------------------
	image_in:	入力画像配列
	image_out:	出力画像配列
	amp:		出力画像の利得
-----------------------------------------------------------------------------*/
void gradient_difference(unsigned char image_in[Y_SIZE][X_SIZE], 
	unsigned char image_out[Y_SIZE][X_SIZE], double amp)
{
	gradient(image_in, image_out, amp, cx_difference, cy_difference);
}

/*--- gradient_roberts --- 画像の１次微分（Robertsオペレータ） ----------------
	image_in:	入力画像配列
	image_out:	出力画像配列
	amp:		出力画像の利得
-----------------------------------------------------------------------------*/
void gradient_roberts(unsigned char image_in[Y_SIZE][X_SIZE], 
	unsigned char image_out[Y_SIZE][X_SIZE], double amp)
{
	gradient(image_in, image_out, amp, cx_roberts, cy_roberts);
}

/*--- gradient_sobel --- 画像の１次微分（Sobelオペレータ） --------------------
	image_in:	入力画像配列
	image_out:	出力画像配列
	amp:		出力画像の利得
-----------------------------------------------------------------------------*/
void gradient_sobel(unsigned char image_in[Y_SIZE][X_SIZE], 
	unsigned char image_out[Y_SIZE][X_SIZE], double amp)
{
	gradient(image_in, image_out, amp, cx_sobel, cy_sobel);
}

/*--- gradient --- 画像の１次微分（任意オペレータ） ---------------------------
	image_in:	入力画像配列
	image_out:	出力画像配列
	amp:		出力画像の利得
	cx, cy:		微分オペレータ
-----------------------------------------------------------------------------*/
void gradient(unsigned char image_in[Y_SIZE][X_SIZE], 
	unsigned char image_out[Y_SIZE][X_SIZE], double amp, int cx[9], int cy[9])
{
	int		d[9];
	int		i, j, dat;
	double	xx, yy, zz;

	for (i = 1; i < Y_SIZE-1; i++) {
	    for (j = 1; j < X_SIZE-1; j++) {
			d[0] = image_in[i-1][j-1];
			d[1] = image_in[i-1][j];
			d[2] = image_in[i-1][j+1];
			d[3] = image_in[i][j-1];
			d[4] = image_in[i][j];
			d[5] = image_in[i][j+1];
			d[6] = image_in[i+1][j-1];
			d[7] = image_in[i+1][j];
			d[8] = image_in[i+1][j+1];
		    xx = (double)(cx[0]*d[0] + cx[1]*d[1] + cx[2]*d[2]
					   + cx[3]*d[3] + cx[4]*d[4] + cx[5]*d[5]
					   + cx[6]*d[6] + cx[7]*d[7] + cx[8]*d[8]);
			yy = (double)(cy[0]*d[0] + cy[1]*d[1] + cy[2]*d[2]
					   + cy[3]*d[3] + cy[4]*d[4] + cy[5]*d[5]
					   + cy[6]*d[6] + cy[7]*d[7] + cy[8]*d[8]);
			zz = (double)(amp*sqrt(xx*xx+yy*yy));
			dat = (int)zz;
			if(dat > 255) dat = 255;
			image_out[i][j] = (char)dat;
		}
	}
}
