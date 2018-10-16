#include	<stdio.h>
#include	"Params.h"

#define	L_BASE	100			/* 連結成分のラベルのベース値 */
#define	PI		(double)3.14159265
#define	ROOT2	(double)1.41421356

double calc_size(unsigned char image_label[Y_SIZE][X_SIZE], 
	int label, int *cx, int *cy);
double calc_length(unsigned char image_label[Y_SIZE][X_SIZE], int label);
double trace(unsigned char image_label[Y_SIZE][X_SIZE], int xs, int ys);

/*--- features --- 特徴パラメータを調べる -------------------------------------
	image_label_in:		入力ラベル画像配列
	image_label_out:	出力ラベル画像配列
	cnt:				物体の個数
	size:				面積
	ratio:				円形度
	buf:				メッセージ用バッファ
-----------------------------------------------------------------------------*/
void features(unsigned char	image_label_in[Y_SIZE][X_SIZE], 
	unsigned char image_label_out[Y_SIZE][X_SIZE], 
	int cnt, double size[], double ratio[], char *buf)
{
	int		i, j, center_x, center_y;
	double	l;
	int		posi, m;

	posi = 0;
	for (i = 0; i < Y_SIZE; i++)
		for (j = 0; j < X_SIZE; j++)
			image_label_out[i][j] = image_label_in[i][j];
	m = sprintf(buf, " no     area      circum       round     grav(x,y)\n");
	posi += m;
	for (i = 0; i < cnt; i++) {
		size[i] = calc_size(image_label_out, i+L_BASE, 
			&center_x, &center_y);
		l = calc_length(image_label_out, i+L_BASE);
		ratio[i] = 4*PI*size[i]/(l*l);
		image_label_out[center_y][center_x] = HIGH;	/*　重心　*/
		m = sprintf(&buf[posi], "%3d   %6d   %8.2f   %8.4f     (%3d,%3d)\n",
			i, (int)size[i], l, ratio[i], center_x, center_y);
		posi += m;
	}                                                      
}

/*--- calc_size --- 面積，重心位置を求める ------------------------------------
	image_label:	ラベル画像配列
	label:			ラベル番号
	cx, cy:			重心位置
-----------------------------------------------------------------------------*/
double calc_size(unsigned char image_label[Y_SIZE][X_SIZE], 
	int label, int *cx, int *cy)
{
	int		i, j;
	double	tx, ty, total;

	tx = 0; ty = 0; total = 0;
	for (i = 0; i < Y_SIZE; i++)
		for (j = 0; j < X_SIZE; j++)
			if (image_label[i][j] == label) {
				tx += j; ty += i; total++;
			}
	if (total == 0.0) return 0.0;
	*cx = (int)(tx/total); *cy = (int)(ty/total);
	return total;
}

/*--- calc_length --- 周囲長を求める ------------------------------------------
	image_label:	ラベル画像配列
	label:			ラベル番号
-----------------------------------------------------------------------------*/
double calc_length(unsigned char image_label[Y_SIZE][X_SIZE], int label)
{
	int		i, j;
	double	trace();

	for (i = 0; i < Y_SIZE; i++)
		for (j = 0; j < X_SIZE; j++)
			if (image_label[i][j] == label)	return trace(image_label, j-1, i);
	return 0;
}

/*--- trace --- 輪郭線を追跡する ----------------------------------------------
	image_label:	ラベル画像配列
	xs, ys:			スタート位置
-----------------------------------------------------------------------------*/
double trace(unsigned char image_label[Y_SIZE][X_SIZE], int xs, int ys)
{
	int		x, y, no, vec;
	double	l;

	l = 0;	x = xs; y = ys; no = image_label[y][x+1]; vec = 5;
	for (;;) {
		if (x == xs && y == ys && l != 0) return l;
		image_label[y][x] = HIGH;
		switch (vec) {
			case 3: 
				if (image_label[y][x+1] != no && image_label[y-1][x+1] == no)
					{x = x+1; y = y  ; l++       ; vec = 0; continue;}	
			case 4: 
				if (image_label[y-1][x+1] != no && image_label[y-1][x] == no)
					{x = x+1; y = y-1; l += ROOT2; vec = 1; continue;}	
			case 5: 
				if (image_label[y-1][x] != no && image_label[y-1][x-1] == no)
					{x = x  ; y = y-1; l++       ; vec = 2; continue;}	
			case 6: 
				if (image_label[y-1][x-1] != no && image_label[y][x-1] == no)
					{x = x-1; y = y-1; l += ROOT2; vec = 3; continue;}	
			case 7: 
				if (image_label[y][x-1] != no && image_label[y+1][x-1] == no)
					{x = x-1; y = y  ; l++       ; vec = 4; continue;}	
			case 0: 
				if (image_label[y+1][x-1] != no && image_label[y+1][x] == no)
					{x = x-1; y = y+1; l += ROOT2; vec = 5; continue;}	
			case 1: 
				if (image_label[y+1][x] != no && image_label[y+1][x+1] == no)
					{x = x  ; y = y+1; l++       ; vec = 6; continue;}	
			case 2: 
				if (image_label[y+1][x+1] != no && image_label[y][x+1] == no)
					{x = x+1; y = y+1; l += ROOT2; vec = 7; continue;}	
				vec = 3;
		}
	}
}
