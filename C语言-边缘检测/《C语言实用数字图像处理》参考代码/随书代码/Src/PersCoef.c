#include <math.h>
#include "Params.h"

void simultaneous_equation(double ab[8][9]);

/*--- perspect_coef --- 射影変換の係数を4個の対応点から求める -----------------
	x1,y1:	対応点1の変形前の(x,y)座標
	u1,v1:	対応点1の変形後の(x,y)座標
	x2,y2:	対応点2の変形前の(x,y)座標
	u2,v2:	対応点2の変形後の(x,y)座標
	x3,y3:	対応点3の変形前の(x,y)座標
	u3,v3:	対応点3の変形後の(x,y)座標
	x4,y4:	対応点4の変形前の(x,y)座標
	u4,v4:	対応点4の変形後の(x,y)座標
	a,b,c:	X = (ax + by + c) / (gx + hy + 1)
	d,e,f:	Y = (dx + ey + f) / (gx + hy + 1)
	g,h:
-----------------------------------------------------------------------------*/
void perspect_coef(int x1, int y1, int u1, int v1, 
	int x2, int y2, int u2, int v2, 
	int x3, int y3, int u3, int v3,
	int x4, int y4, int u4, int v4,
	double *a, double *b, double *c, double *d, double *e, double *f,
	double *g, double *h)
{
	double m[8][9];

	m[0][0] = x1;      m[0][1] = y1;      m[0][2] = 1;
	m[0][3] = 0;       m[0][4] = 0;       m[0][5] = 0;
	m[0][6] = -x1*u1;  m[0][7] = -y1*u1;  m[0][8] = u1;
	m[1][0] = 0;       m[1][1] = 0;       m[1][2] = 0;
	m[1][3] = x1;      m[1][4] = y1;      m[1][5] = 1;
	m[1][6] = -x1*v1;  m[1][7] = -y1*v1;  m[1][8] = v1;
	m[2][0] = x2;      m[2][1] = y2;      m[2][2] = 1;
	m[2][3] = 0;       m[2][4] = 0;       m[2][5] = 0;
	m[2][6] = -x2*u2;  m[2][7] = -y2*u2;  m[2][8] = u2;
	m[3][0] = 0;       m[3][1] = 0;       m[3][2] = 0;
	m[3][3] = x2;      m[3][4] = y2;      m[3][5] = 1;
	m[3][6] = -x2*v2;  m[3][7] = -y2*v2;  m[3][8] = v2;
	m[4][0] = x3;      m[4][1] = y3;      m[4][2] = 1;
	m[4][3] = 0;       m[4][4] = 0;       m[4][5] = 0;
	m[4][6] = -x3*u3;  m[4][7] = -y3*u3;  m[4][8] = u3;
	m[5][0] = 0;       m[5][1] = 0;       m[5][2] = 0;
	m[5][3] = x3;      m[5][4] = y3;      m[5][5] = 1;
	m[5][6] = -x3*v3;  m[5][7] = -y3*v3;  m[5][8] = v3;
	m[6][0] = x4;      m[6][1] = y4;      m[6][2] = 1;
	m[6][3] = 0;       m[6][4] = 0;       m[6][5] = 0;
	m[6][6] = -x4*u4;  m[6][7] = -y4*u4;  m[6][8] = u4;
	m[7][0] = 0;       m[7][1] = 0;       m[7][2] = 0;
	m[7][3] = x4;      m[7][4] = y4;      m[7][5] = 1;
	m[7][6] = -x4*v4;  m[7][7] = -y4*v4;  m[7][8] = v4;
	simultaneous_equation(m);
	*a = m[0][8];
	*b = m[1][8];
	*c = m[2][8];
	*d = m[3][8];
	*e = m[4][8];
	*f = m[5][8];
	*g = m[6][8];
	*h = m[7][8];
}

/*--- simultaneous_equation --- 連立1次方程式を解く（ガウス消去法） ----------
	ab[8][9]:	8元連立1次方程式AX=Bの係数行列 0～7列にAを，8列にBを代入）
----------------------------------------------------------------------------*/
void simultaneous_equation(double ab[8][9])
{
    int    i, j, ii, p;
	double max, d, pvt;

	for (i = 0; i < 8; i++) {
		/* ピボット選択と行交換 */
		max = 0;
		p = i;
		for (ii = i; ii < 8; ii++) {
			if (fabs(ab[ii][i]) > max) {
				max = fabs(ab[ii][i]);
				p = ii;
			}
		}
		if (i != p) {
			for (j = 0; j <= 8; j++){
				d = ab[i][j];
				ab[i][j] = ab[p][j];
				ab[p][j] = d;
			}
		}
		/* 前進消去 */
		pvt = ab[i][i];
		for (j = i; j <= 8; j++) ab[i][j] /= pvt;
		for (ii = i+1; ii < 8; ii++)
			for (j = i+1; j <= 8; j++) ab[ii][j] -= ab[ii][i] * ab[i][j];
	}
	/* 後退代入 */
	for (i = 6; i >= 0; i--)
		for (j = i+1; j < 8; j++) ab[i][8] -= ab[i][j] * ab[j][8];
}
