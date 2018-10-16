#include "Params.h"

/*--- tran_ysh --- �P�x,�ʓx,�F����ς��� -------------------------------------
	image_in_ysh:	���͉摜�z��iYSH�J���[�j
	image_out_ysh:	�o�͉摜�z��iYSH�J���[�j
	ya:			�P�x�{��
	yb:			�P�x�o�C�A�X
	sa:			�ʓx�{��
	sb:			�ʓx�o�C�A�X
	hb:			�F���o�C�A�X(�x)
-----------------------------------------------------------------------------*/
void tran_ysh(int image_in_ysh[3][Y_SIZE][X_SIZE],
	int image_out_ysh[3][Y_SIZE][X_SIZE], 
	double ya, double yb, double sa, double sb, double hb)
{
	int i, j;

	for (i = 0; i < Y_SIZE; i++){
		for (j = 0; j < X_SIZE; j++){
			image_out_ysh[0][i][j] = (int)(image_in_ysh[0][i][j] * ya + yb);
			image_out_ysh[1][i][j] = (int)(image_in_ysh[1][i][j] * sa + sb);
			image_out_ysh[2][i][j] = (int)(image_in_ysh[2][i][j] + hb);
			if(image_out_ysh[2][i][j] > 180) image_out_ysh[2][i][j] -= 360;
			if(image_out_ysh[2][i][j] < -180) image_out_ysh[2][i][j] += 360;
		}
	}
}
