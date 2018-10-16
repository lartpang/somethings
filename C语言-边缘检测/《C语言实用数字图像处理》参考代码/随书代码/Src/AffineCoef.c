#include "Params.h"

/*--- affine_coef --- �A�t�B���ϊ��̌W����3�̑Ή��_���狁�߂� ---------------
	x1,y1:	�Ή��_1�̕ό`�O��(x,y)���W
	u1,v1:	�Ή��_1�̕ό`���(x,y)���W
	x2,y2:	�Ή��_2�̕ό`�O��(x,y)���W
	u2,v2:	�Ή��_2�̕ό`���(x,y)���W
	x3,y3:	�Ή��_3�̕ό`�O��(x,y)���W
	u3,v3:	�Ή��_3�̕ό`���(x,y)���W
	a,b,c:	�ϊ��W�� X = ax + by + c
	d,e,f:	�ϊ��W�� Y = dx + ey + f
-----------------------------------------------------------------------------*/
void affine_coef(int x1, int y1, int u1, int v1, 
	int x2, int y2, int u2, int v2, 
	int x3, int y3, int u3, int v3,
	double *a, double *b, double *c, double *d, double *e, double *f)
{
	double g;

	g  = (x1-x2)*(y2-y3) -(x2-x3)*(y1-y2);
	*a = ((u1-u2)*(y2-y3) - (u2-u3)*(y1-y2)) / g;
	*b = ((u2-u3)*(x1-x2) - (u1-u2)*(x2-x3)) / g;
	*c = u1 - *a*x1 - *b*y1;
	*d = ((v1-v2)*(y2-y3) - (v2-v3)*(y1-y2)) / g;
	*e = ((v2-v3)*(x1-x2) - (v1-v3)*(x2-x3)) / g;
	*f = v1 - *d*x1 - *e*y1;
}
