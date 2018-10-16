#include	<stdio.h>
#include	<stdlib.h>
#include	<windows.h>
#include	"WinParams.h"

PRINTDLG	pd;
DOCINFO		di = {sizeof(DOCINFO), "Image", NULL};

/*--- ImgPrint --- 画像を印刷する ---------------------------------------------
	pr:			入力Ｒ画像
	pg:			入力Ｇ画像
	pb:			入力Ｂ画像
-----------------------------------------------------------------------------*/
int ImgPrint(unsigned char pr[P_Y_SIZE][P_X_SIZE], 
	unsigned char pg[P_Y_SIZE][P_X_SIZE], 
	unsigned char pb[P_Y_SIZE][P_X_SIZE])
{
	int	i, j;

	if (pd.hDevNames == NULL && pd.hDevMode == NULL) {
		/*	PRINTDLG構造体の初期化	*/
		memset(&pd, 0, sizeof(PRINTDLG));
		pd.lStructSize = sizeof(PRINTDLG);
		pd.Flags = PD_RETURNDEFAULT;
		if(!PrintDlg(&pd) || !(pd.hDevMode)) return -1;
		pd.Flags &= ~PD_RETURNDEFAULT;
	}
	pd.Flags |= PD_RETURNDC;
	if (!PrintDlg(&pd)) return -1;
	if (StartDoc(pd.hDC, &di) > 0)
	if (StartPage(pd.hDC) > 0){
		for (i = 0; i < P_Y_SIZE; i++)
			for (j = 0; j < P_X_SIZE; j++)
				SetPixel(pd.hDC, j, i, RGB(pr[i][j], 
						pg[i][j], pb[i][j]));
		EndPage(pd.hDC);
		EndDoc(pd.hDC);
	}
	return 0;
}

/*--- o_dither --- 組織的ディザ法 ---------------------------------------------
	image_in:	入力画像配列
	image_out:	出力画像配列
-----------------------------------------------------------------------------*/
void o_dither(unsigned char image_in[P_Y_SIZE][P_X_SIZE], 
		unsigned char image_out[P_Y_SIZE][P_X_SIZE])
{
	static int thres[4][4] = {  0,  8,  2, 10,
							   12,  4, 14,  6,
							    3, 11,  1,  9,
							   15,  7, 13,  5,};	/* ディザマトリクス	*/
	int		i, j, m, n;

	for (i = 0 ; i<P_Y_SIZE/4 ; i++)
		for (j = 0 ; j<P_X_SIZE/4 ; j++)
			for (m = 0; m < 4; m++)
				for (n = 0; n < 4; n++)
					if ((int)image_in[i*4+m][j*4+n] > thres[m][n]*16+8) 
			    		image_out[i*4+m][j*4+n] = HIGH;
					else image_out[i*4+m][j*4+n] = LOW;
}

/*--- m_dither --- 平均誤差最小ディザ法 ---------------------------------------
	image_in:	入力画像配列
	image_out:	出力画像配列
-----------------------------------------------------------------------------*/
void m_dither(unsigned char image_in[P_Y_SIZE][P_X_SIZE], 
		unsigned char image_out[P_Y_SIZE][P_X_SIZE])
{
	int	i, j;
	int	t, d;
	int	error[P_X_SIZE], error1[P_X_SIZE], error2[P_X_SIZE]; 	/* 誤差配列 */

	t = (HIGH+1)/2;		/*　閾値　この例では１２８	*/
	for (i = 0; i < P_X_SIZE; i++) {
		if (image_in[0][i] > t) image_out[0][i] = HIGH;
		else		image_out[0][i] = LOW;
		error1[i] = image_in[0][i]-image_out[0][i] ;
	}
	for (i = 0; i < P_X_SIZE; i++) {
		if (image_in[1][i] > t) image_out[1][i] = HIGH;
		else		image_out[1][i] = LOW;
		error2[i] = image_in[1][i]-image_out[1][i];
	}
	for (i = 2; i < P_Y_SIZE; i++) {
		if (image_in[i][0] > t) image_out[i][0] = HIGH;
		else	image_out[i][0] = LOW;
		error[0] = image_in[i][0]-image_out[i][0];
		if (image_in[i][1] > t) image_out[i][1] = HIGH;
		else	image_out[i][1] = LOW;
		error[1] = image_in[i][1]-image_out[i][1];
		for (j = 2; j < P_X_SIZE-2; j++) {
		    d = (error1[j-2]+error1[j-1]*3+error1[j]*5
		    	+error1[j+1]*3+error1[j+2]
		    	+error2[j-2]*3+error2[j-1]*5+error2[j]*7
		    	+error2[j+1]*5+error2[j+2]*3
		    	+error[j-2]*5+error[j-1]*7)/48;
		    if ((int)image_in[i][j]+d > t) image_out[i][j] = HIGH;
		    else	image_out[i][j] = LOW;
		    error[j] = image_in[i][j]+d-image_out[i][j];
		}
		if (image_in[i][P_X_SIZE-2] > t) image_out[i][P_X_SIZE-2] = HIGH;
		else	image_out[i][P_X_SIZE-2] = LOW;
		error[P_X_SIZE-2] = image_in[i][P_X_SIZE-2]-image_out[i][P_X_SIZE-2];
		if (image_in[i][P_X_SIZE-1] > t) image_out[i][P_X_SIZE-1] = HIGH;
		else	image_out[i][P_X_SIZE-1] = LOW;
		error[P_X_SIZE-1] = image_in[i][P_X_SIZE-1]-image_out[i][P_X_SIZE-1];
		for (j = 0; j < P_X_SIZE; j++) {
			error1[j] = error2[j];
			error2[j] = error[j];
		}
	}
}
