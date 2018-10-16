#include <math.h>
#include "Params.h"
#define PI 3.14
#define SQRT2 1.414
#define TAP 25

void laplacian_of_gaussian(unsigned char image_in[Y_SIZE][X_SIZE], 
		unsigned char image_out[Y_SIZE][X_SIZE], double var, double amp);
void zero_cross(unsigned char image_in[Y_SIZE][X_SIZE], 
	unsigned char image_out[Y_SIZE][X_SIZE]);

/*--- zero_cross_log --- LoG�̃[����������ɂ��֊s���o ----------------------
	image_in:	���͉摜�z��
	image_out:	�o�͉摜�z��
	var:		���U
-----------------------------------------------------------------------------*/
void log_zero_cross(unsigned char image_in[Y_SIZE][X_SIZE], 
	unsigned char image_out[Y_SIZE][X_SIZE], double var)
{
	unsigned char image_buf[Y_SIZE][X_SIZE];

	laplacian_of_gaussian(image_in, image_buf, var, 1.0);
	zero_cross(image_buf, image_out);
}

/*--- laplacian_of_gaussian --- Log�t�B���^ -----------------------------------
	image_in:	���͉摜�z��
	image_out:	�o�͉摜�z��
	var:		���U
	amp:		�o�͉摜�̗���
-----------------------------------------------------------------------------*/
void laplacian_of_gaussian(unsigned char image_in[Y_SIZE][X_SIZE], 
		unsigned char image_out[Y_SIZE][X_SIZE], double var, double amp)
{
	double c[TAP*2+1][TAP*2+1];
	int	i, j, k, m, n, x, y;
	double r2, v2, v4, d;

	k = (int)(3 * SQRT2 * var);
	if (k > TAP) k = TAP;
	v2 = var * var;
	v4 = 1 / (v2 * v2 * PI);
	for (m = -k; m <= k; m++) {
		for (n = -k; n <= k; n++) {
			r2 = (m*m + n*n) / v2 / 2;
			c[TAP+m][TAP+n] = v4 * (r2 - 1) * exp(-r2);
		}
	}

	for (i = 0; i < Y_SIZE; i++) {
		for (j = 0; j < X_SIZE; j++) {
			d = OFFSET;
			for (m = -k; m <= k; m++) {
				for (n = -k; n <= k; n++) {
					y = i + m;
					x = j + n;
					if (y < 0) y = 0;
					if (x < 0) y = 0;
					if (y > Y_SIZE-1) y = Y_SIZE-1;
					if (x > X_SIZE-1) x = X_SIZE-1;
					d += c[TAP+m][TAP+n] * image_in[y][x];
				}
			}
			if (d <   0) d =   0;
			if (d > 255) d = 255;
			image_out[i][j] = (unsigned char)d;
		}
	}
}
