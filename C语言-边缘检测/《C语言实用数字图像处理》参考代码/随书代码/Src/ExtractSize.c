#include	<stdio.h>
#include	"Params.h"

#define	L_BASE		100	/*  連結成分のラベルのベース値 		*/

/*--- extract_size --- 面積がある範囲の連結成分を抜き出す ---------------------
	image_label_in:		入力ラベル画像配列
	image_label_out:	出力ラベル画像配列
	cnt:				連結成分の個数
	size:				面積
	size_min, size_max:	最小値，最大値
-----------------------------------------------------------------------------*/
void extract_size(unsigned char image_label_in[Y_SIZE][X_SIZE], 
	unsigned char image_label_out[Y_SIZE][X_SIZE], 
	int cnt, double size[], double size_min, double size_max)
{
	int	i, j, x, y;
	int	lno[256];

	for (i = 0, j = 0; i < cnt; i++)
		if (size[i] >= size_min && size[i] <= size_max)	lno[j++] = L_BASE+i;
	for (y = 0; y < Y_SIZE; y++) {
	    for (x = 0; x < X_SIZE; x++) {
			image_label_out[y][x] = 0;
	    	for (i=0 ; i<j ; i++)
		    	if (image_label_in[y][x] == lno[i]) 
		    		image_label_out[y][x] = image_label_in[y][x];
		}
	}
}
