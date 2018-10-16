#include <math.h>
#include "Params.h"

#define	PI			3.141592

/*--- rgb_to_ysh --- R,G,B����P�xY,�ʓxS,�F��H�ɕϊ����� ---------------------
	image_in_rgb:	���͉摜�z��iRGB�J���[�j
	image_out_ysh:	�o�͉摜�z��iYSH�J���[�j
-----------------------------------------------------------------------------*/
void rgb_to_ysh(unsigned char image_in_rgb[3][Y_SIZE][X_SIZE], 
	int image_out_ysh[3][Y_SIZE][X_SIZE])
{
	int    i, j;
	double r, g, b, y, cb, cr, s, h;

	for (i = 0; i < Y_SIZE; i++) {
		for (j = 0; j < X_SIZE; j++) {
			r = (double)image_in_rgb[0][i][j];
			g = (double)image_in_rgb[1][i][j];
			b = (double)image_in_rgb[2][i][j];
			y  = 0.2126 * r + 0.7152 * g + 0.0722 * b;
			cb = (b - y) / 1.8556;
			cr = (r - y) / 1.5748;
			s  = sqrt(cb * cb + cr * cr);
			if (s != 0) h  = atan2(cr, cb) * 180.0 / PI;
			else        h = 0;
			image_out_ysh[0][i][j] = (int)y;
			image_out_ysh[1][i][j] = (int)s;
			image_out_ysh[2][i][j] = (int)h;
		}  
	}
}

/*--- ysh_to_rgb --- �P�xY,�ʓxS,�F��H����R,G,B�ɕϊ����� ---------------------
	image_in_ysh:	���͉摜�z��iYSH�J���[�j
	image_out_rgb:	�o�͉摜�z��iRGB�J���[�j
-----------------------------------------------------------------------------*/
void ysh_to_rgb(int image_in_ysh[3][Y_SIZE][X_SIZE],
	unsigned char image_out_rgb[3][Y_SIZE][X_SIZE])
{
	int    i, j;
	double r, g, b, y, cb, cr, rad;

	for (i = 0; i < Y_SIZE; i++) {
		for (j = 0; j < X_SIZE; j++) {
			y  = image_in_ysh[0][i][j];
			rad = (double)(PI * image_in_ysh[2][i][j] / 180.0);
			cb = image_in_ysh[1][i][j] * cos(rad);
			cr = image_in_ysh[1][i][j] * sin(rad);
			r = y + 1.5748 * cr;
			b = y + 1.8556 * cb;
			g = (y - 0.2126 * r - 0.0722 * b) / 0.7152;
			if (r <   0) r =   0;
			if (r > 255) r = 255;
			if (g <   0) g =   0;
			if (g > 255) g = 255;
			if (b <   0) b =   0;
			if (b > 255) b = 255;
			image_out_rgb[0][i][j] = (unsigned char)r;
			image_out_rgb[1][i][j] = (unsigned char)g;
			image_out_rgb[2][i][j] = (unsigned char)b;
		}
	}
}
