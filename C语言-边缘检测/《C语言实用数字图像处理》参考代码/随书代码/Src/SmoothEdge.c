#include	"Params.h"

unsigned char average_minvar(unsigned char p[9][9]);

/*--- smooth_edge_preserve --- エッジ保存平滑化 -------------------------------
	image_in:	入力画像配列
	image_out:	出力画像配列
-----------------------------------------------------------------------------*/
void smooth_edge_preserve(unsigned char image_in[Y_SIZE][X_SIZE], 
	unsigned char image_out[Y_SIZE][X_SIZE])
{
	int	i, j, k, m;
	unsigned char p[9][9];
	int patx[9][9] = { 0, -1,  0,  1, -1,  0,  1,  0,  0,
					   0,  1,  2,  0,  1,  2,  1,  0,  0,
					   0,  1,  2,  1,  2,  1,  2,  0,  0,
					   0,  1,  0,  1,  2,  1,  2,  0,  0,
					   0, -1,  0,  1, -1,  0,  1,  0,  0,
					   0, -1, -2, -1,  0, -2, -1,  0,  0,
					   0, -2, -1, -2, -1, -2, -1,  0,  0,
					   0, -2, -1, -2, -1,  0, -1,  0,  0,
					  -1,  0,  1, -1,  0,  1, -1,  0,  1};

	int paty[9][9] = { 0, -2, -2, -2, -1, -1, -1,  0,  0,
					   0, -2, -2, -1, -1, -1,  0,  0,  0,
					   0, -1, -1,  0,  0,  1,  1,  0,  0,
					   0,  0,  1,  1,  1,  2,  2,  0,  0,
					   0,  1,  1,  1,  2,  2,  2,  0,  0,
					   0,  0,  1,  1,  1,  2,  2,  0,  0,
					   0, -1, -1,  0,  0,  1,  1,  0,  0,
					   0, -2, -2, -1, -1, -1,  0,  0,  0,
					  -1, -1, -1,  0,  0,  0,  1,  1,  1}; 

	for (i = 0; i < Y_SIZE; i++)
		for (j = 0; j < X_SIZE; j++)
			image_out[i][j] = image_in[i][j];
	for (i = 2; i < Y_SIZE-2; i++) {
		for (j = 2; j < X_SIZE-2; j++) {
			for (k = 0; k < 9; k++) {
				for (m = 0; m < 9; m++) {
					p[k][m] = image_in[i+paty[k][m]][j+patx[k][m]];
				}
			}
			image_out[i][j] = average_minvar(p);
		}
	}
}

/*--- average_minvar --- 分散が最小となる画素配列の平均値を返す ---------------
	p:		入力画素配列
-----------------------------------------------------------------------------*/
unsigned char average_minvar(unsigned char p[9][9])
{
	int i, k, n;
	double ave[9], var[9], dmin;

	for (k = 0; k < 8; k++) {
		ave[k] = 0.0;
		for (i = 0; i < 7; i++) ave[k] += (double)p[k][i];
		ave[k] = ave[k] / 7.0;
		var[k] = 0.0;
		for (i = 0; i < 7; i++)
			var[k] += ((double)p[k][i] - ave[k]) * ((double)p[k][i] - ave[k]);
		var[k] = var[k] / 7.0;
	}
	ave[8] = 0.0;
	for (i = 0; i < 9; i++) ave[k] += (double)p[k][i];
	ave[8] = ave[k] / 9.0;
	var[8] = 0.0;
	for (i = 0; i < 9; i++)
		var[k] += ((double)p[k][i] - ave[k]) * ((double)p[k][i] - ave[k]);
	var[k] = var[k] / 9.0;
	dmin = var[0];
	n = 0;
	for (k = 1; k < 9; k++) {
		if (dmin > var[k]) {
			dmin = var[k];
			n = k;
		}
	}
	return (unsigned char)ave[n];
}
