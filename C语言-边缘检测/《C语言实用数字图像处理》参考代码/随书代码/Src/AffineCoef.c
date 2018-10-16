#include "Params.h"

/*--- affine_coef --- アフィン変換の係数を3個の対応点から求める ---------------
	x1,y1:	対応点1の変形前の(x,y)座標
	u1,v1:	対応点1の変形後の(x,y)座標
	x2,y2:	対応点2の変形前の(x,y)座標
	u2,v2:	対応点2の変形後の(x,y)座標
	x3,y3:	対応点3の変形前の(x,y)座標
	u3,v3:	対応点3の変形後の(x,y)座標
	a,b,c:	変換係数 X = ax + by + c
	d,e,f:	変換係数 Y = dx + ey + f
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
