#include <stdio.h>
#include <stdlib.h>
#include <math.h>

#define	PI	3.141592
#define	OPT	1	/* OPT = 1�@���w�I�c�e�s�i�������������j */
				/* OPT = 0�@�ʏ�̂c�e�s�i�����������[�j */

void fft1core(double a_rl[], double a_im[], int length, 
	int ex, double sin_tbl[], double cos_tbl[], double buf[]);
void cstb(int length, int inv, double sin_tbl[], double cos_tbl[]);
void birv(double a[], int length, int ex, double b[]);

/*--- fft1 --- �P�����e�e�s�̎��s ---------------------------------------------
	a_rl:	�f�[�^�������i���o�͌��p�j
	a_im:	�f�[�^�������i���o�͌��p�j
	ex:		�f�[�^�����Q�ׂ̂���ŗ^����(�f�[�^�� = 2��ex��j
	inv:	1: �c�e�s�C-1: �t�c�e�s
-----------------------------------------------------------------------------*/
int fft1(double a_rl[], double a_im[], int ex, int inv)
{
	int i, length = 1;
	double *sin_tbl;	/* SIN�v�Z�p�e�[�u���@*/
	double *cos_tbl;	/* COS�v�Z�p�e�[�u���@*/
	double *buf;		/* ��Ɨp�o�b�t�@�@*/

	for (i = 0; i < ex; i++) length *= 2;		/* �f�[�^���̌v�Z */
	sin_tbl = (double *)malloc((size_t)length*sizeof(double));
	cos_tbl = (double *)malloc((size_t)length*sizeof(double));
	buf = (double *)malloc((size_t)length*sizeof(double));
	if ((sin_tbl == NULL) || (cos_tbl == NULL) || (buf == NULL)) return -1;
	cstb(length, inv, sin_tbl, cos_tbl);	/* SIN,COS�e�[�u���쐬 */
	fft1core(a_rl, a_im, length, ex, sin_tbl, cos_tbl, buf);
	free(sin_tbl);
	free(cos_tbl);
	return 0;
}

/*--- fft1core --- �P�����e�e�s�̌v�Z�̊j�ɂȂ镔�� ---------------------------
	a_rl:	�f�[�^�������i���o�͌��p�j
	a_im:	�f�[�^�������i���o�͌��p�j
	ex:		�f�[�^�����Q�ׂ̂���ŗ^����(�f�[�^�� = 2��ex��j
	sin_tbl:	SIN�v�Z�p�e�[�u��
	cos_tbl:	COS�v�Z�p�e�[�u��
-----------------------------------------------------------------------------*/
void fft1core(double a_rl[], double a_im[], int length, 
	int ex, double sin_tbl[], double cos_tbl[], double buf[])
{
	int i, j, k, w, j1, j2;
	int numb, lenb, timb;
	double xr, xi, yr, yi, nrml;

	if (OPT == 1) {
		for (i = 1; i < length; i+=2) {
			a_rl[i] = -a_rl[i];
			a_im[i] = -a_im[i];
		}
	}
	numb = 1;
	lenb = length;
	for (i = 0; i < ex; i++) {
		lenb /= 2;
		timb = 0;
		for (j = 0; j < numb; j++) {
			w = 0;
			for (k = 0; k < lenb; k++) {
				j1 = timb + k;
				j2 = j1 + lenb;
				xr = a_rl[j1];
				xi = a_im[j1];
				yr = a_rl[j2];
				yi = a_im[j2];
				a_rl[j1] = xr + yr;
				a_im[j1] = xi + yi;
				xr = xr - yr;
				xi = xi - yi;
				a_rl[j2] = xr*cos_tbl[w] - xi*sin_tbl[w];
				a_im[j2] = xr*sin_tbl[w] + xi*cos_tbl[w];
				w += numb;
			}
			timb += (2*lenb);
		}
		numb *= 2;
	}
	birv(a_rl, length, ex, buf);		/*�@�����f�[�^�̕��׊����@*/
	birv(a_im, length, ex, buf);		/*�@�����f�[�^�̕��׊����@*/
	if (OPT == 1) {
		for (i = 1; i < length; i+=2) {
			a_rl[i] = -a_rl[i];
			a_im[i] = -a_im[i];
		}
	}
	nrml = 1.0 / sqrt((double)length);
	for (i = 0; i < length; i++) {
		a_rl[i] *= nrml;
		a_im[i] *= nrml;
	}
}

/*--- cstb --- SIN,COS�e�[�u���쐬 --------------------------------------------
	length:		�f�[�^��
	inv:		1: �c�e�s, -1: �t�c�e�s
	sin_tbl:	SIN�v�Z�p�e�[�u��
	cos_tbl:	COS�v�Z�p�e�[�u��
-----------------------------------------------------------------------------*/
void cstb(int length, int inv, double sin_tbl[], double cos_tbl[])
{
	int i;
	double xx, arg;

	xx = -PI* 2.0 / (double)length;
	if (inv < 0) xx = -xx;
	for (i = 0; i < length; i++) {
		arg = i * xx;
		sin_tbl[i] = sin(arg);
		cos_tbl[i] = cos(arg);
	}
}

/*--- birv --- �f�[�^�̕��׊��� -----------------------------------------------
	a:		�f�[�^�̔z��
	length:	�f�[�^��
	ex:		�f�[�^�����Q�ׂ̂���ŗ^����(length = 2��ex��j
	b:		��Ɨp�o�b�t�@
-----------------------------------------------------------------------------*/
void birv(double a[], int length, int ex, double b[])
{
	int	i, ii, k, bit;

	for (i = 0; i < length; i++) {
		for (k = 0, ii=i, bit=0; ; bit<<=1, ii>>=1) {
			bit = (ii & 1) | bit;
			if (++k == ex) break;
		}
		b[i] = a[bit];
	}
	for (i = 0; i < length; i++) a[i] = b[i];
}
