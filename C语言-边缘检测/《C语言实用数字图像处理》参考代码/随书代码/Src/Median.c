#include	"Params.h"

int median_value(unsigned char c[9]);

/*--- median --- メディアンフィルタによるノイズ除去 ---------------------------
	image_in:	入力画像配列
	image_out:	出力画像配列
-----------------------------------------------------------------------------*/
void median(unsigned char image_in[Y_SIZE][X_SIZE], 
	unsigned char image_out[Y_SIZE][X_SIZE])
{
	int i, j;
  	unsigned char c[9];

	for (i = 1; i < Y_SIZE-1; i++) {
		for (j = 1; j < X_SIZE-1; j++) {
			c[0] = image_in[i-1][j-1];
			c[1] = image_in[i-1][j];
			c[2] = image_in[i-1][j+1];
			c[3] = image_in[i][j-1];
			c[4] = image_in[i][j];
			c[5] = image_in[i][j+1];
			c[6] = image_in[i+1][j-1];
			c[7] = image_in[i+1][j];
			c[8] = image_in[i+1][j+1];
			image_out[i][j] = median_value(c);
		}
	}
}

/*--- median_value --- ９つの画素の中央値（メディアン）を求める ---------------
	c:	画素
-----------------------------------------------------------------------------*/
int median_value(unsigned char c[9])
{
	int i, j, buf;
    
	for (j = 0; j < 8; j++) {
		for (i = 0; i < 8; i++) {
			if (c[i+1] < c[i]) {
				buf = c[i+1];
				c[i+1] = c[i];
				c[i] = buf;
			}
		}
	}
	return c[4];
}
