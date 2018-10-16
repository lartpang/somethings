#include <math.h>
#include "Params.h"

/*--- mozaic_coef_blockmatch_rgb --- �u���b�N�}�b�`���O�Ń��U�C�N�ʒu�����߂� --
	image_in1:	���͉摜�z��1(�J���[)
	image_in2:	���͉摜�z��2(�J���[)
	x1, y1:		�摜1,2�𒣂荇�킹��ۂ̑Ή��ʒu�T���̉摜1�ɂ������_
	x2, y2:		�摜1,2�𒣂荇�킹��ۂ̑Ή��ʒu�T���̉摜2�ɂ������_
	xd, yd:		�摜1,2�𒣂荇�킹��ۂ̑Ή��ʒu�T���͈̔́i�}xd,�}yd�j
	bx, by:		�}�b�`���O�̈�
	mx, my:		�d�ˍ��킹�ʒu
-----------------------------------------------------------------------------*/
void mosaic_coef_blockmatch_rgb(unsigned char image_in1[3][Y_SIZE][X_SIZE], 
	unsigned char image_in2[3][Y_SIZE][X_SIZE],
	int x1, int y1, int x2, int y2, int xd, int yd, int bx, int by,
	int *mx, int *my)
{
	int i, j, m, n;
	int xx1, yy1, xx2, yy2;
	double d, min;

	for (i = -yd; i <= yd; i++) {
		for (j = -xd; j <= xd; j++) {
 			d = 0;
			for (m = -by/2; m < by/2; m++) {
				yy1 = y1 + m;
				yy2 = y2 + i + m;
				for (n = -bx/2; n < bx/2; n++) {
					xx1 = x1 + n;
					xx2 = x2 + j + n;
					d += abs(image_in1[0][yy1][xx1] - image_in2[0][yy2][xx2])
					   + abs(image_in1[1][yy1][xx1] - image_in2[1][yy2][xx2])
					   + abs(image_in1[2][yy1][xx1] - image_in2[2][yy2][xx2]);
				}
			}
			if (j == -xd && i == -yd) {
				min = d;
				*mx = j + x1 - x2;
				*my = i + y1 - y2;
			}
			if (d < min) {
				min = d;
				*mx = j + x1 - x2;
				*my = i + y1 - y2;
			}
 		}
	}
}
