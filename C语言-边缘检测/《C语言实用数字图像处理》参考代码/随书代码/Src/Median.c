#include	"Params.h"

int median_value(unsigned char c[9]);

/*--- median --- ���f�B�A���t�B���^�ɂ��m�C�Y���� ---------------------------
	image_in:	���͉摜�z��
	image_out:	�o�͉摜�z��
-----------------------------------------------------------------------------*/
void median(unsigned char image_in[Y_SIZE][X_SIZE], 
	unsigned char image_out[Y_SIZE][X_SIZE])
{
	int i, j;
  	unsigned char c[9];

	for (i = 1; i < Y_SIZE-1; i++) {
		for (j = 1; j < X_SIZE-1; j++) {
			c[0] = image_in[i-1][j-1];
			c[1] = image_in[i-1][j];
			c[2] = image_in[i-1][j+1];
			c[3] = image_in[i][j-1];
			c[4] = image_in[i][j];
			c[5] = image_in[i][j+1];
			c[6] = image_in[i+1][j-1];
			c[7] = image_in[i+1][j];
			c[8] = image_in[i+1][j+1];
			image_out[i][j] = median_value(c);
		}
	}
}

/*--- median_value --- �X�̉�f�̒����l�i���f�B�A���j�����߂� ---------------
	c:	��f
-----------------------------------------------------------------------------*/
int median_value(unsigned char c[9])
{
	int i, j, buf;
    
	for (j = 0; j < 8; j++) {
		for (i = 0; i < 8; i++) {
			if (c[i+1] < c[i]) {
				buf = c[i+1];
				c[i+1] = c[i];
				c[i] = buf;
			}
		}
	}
	return c[4];
}
