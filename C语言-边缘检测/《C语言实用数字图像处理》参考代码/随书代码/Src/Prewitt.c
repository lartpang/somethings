#include <math.h>
#include "Params.h"

/*--- prewitt --- Prewittの方法による輪郭抽出 ---------------------------------
	image_in:	入力画像配列
	image_out:	出力画像配列
	amp:		出力画像の利得
-----------------------------------------------------------------------------*/
void prewitt(unsigned char image_in[Y_SIZE][X_SIZE], 
	unsigned char image_out[Y_SIZE][X_SIZE], double amp)
{
	int		d0,d1,d2,d3,d4,d5,d6,d7,d8;
	int		i,j,k,max,dat;
	int		m[8];
	double	zz;

	for (i = 1; i < Y_SIZE-1; i++) {
		for (j = 1; j < X_SIZE-1; j++) {
			d0 = image_in[i-1][j-1];
			d1 = image_in[i-1][j];
			d2 = image_in[i-1][j+1];
			d3 = image_in[i][j-1];
			d4 = image_in[i][j];
			d5 = image_in[i][j+1];
			d6 = image_in[i+1][j-1];
			d7 = image_in[i+1][j];
			d8 = image_in[i+1][j+1];
	    	m[0] =  d0 + d1 + d2 + d3 -2*d4 + d5 - d6 - d7 - d8;
	    	m[1] =  d0 + d1 + d2 + d3 -2*d4 - d5 + d6 - d7 - d8;
	    	m[2] =  d0 + d1 - d2 + d3 -2*d4 - d5 + d6 + d7 - d8;
	       	m[3] =  d0 - d1 - d2 + d3 -2*d4 - d5 + d6 + d7 + d8;
	    	m[4] = -d0 - d1 - d2 + d3 -2*d4 + d5 + d6 + d7 + d8;
	    	m[5] = -d0 - d1 + d2 - d3 -2*d4 + d5 + d6 + d7 + d8;
	    	m[6] = -d0 + d1 + d2 - d3 -2*d4 + d5 - d6 + d7 + d8;
	    	m[7] =  d0 + d1 + d2 - d3 -2*d4 + d5 - d6 - d7 + d8;
			max = 0;
			for (k = 0; k < 8; k++)	if (max < m[k]) max = m[k];
	    	zz = amp*(double)(max);
			dat = (int)(zz);
			if (dat > 255) dat = 255;
			image_out[i][j] = (char)dat;
		}
	}
}
