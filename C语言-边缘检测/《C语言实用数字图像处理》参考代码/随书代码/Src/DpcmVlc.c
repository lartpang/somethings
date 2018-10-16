#include "Params.h"

#define DPCM		dpcm1		/* (1)�̗\���@,(2)�̏ꍇ��dpcm2�ɑウ�� */
#define MAX_LENG	X_SIZE*4	/* �ϒ������p�o�b�t�@�̍ő�o�C�g�� */

int vlcode(short int data_in[], int no, char vlc_out[]);
void dpcm1(unsigned char image_in[Y_SIZE][X_SIZE], 
	int line, short data_out[X_SIZE]);
void dpcm2(unsigned char image_in[Y_SIZE][X_SIZE], 
	int line, short data_out[X_SIZE]);
int event(short dt);

/*--- dpcm_vlcode --- ������ --------------------------------------------------
	image_in:	���͉摜�z��
	image_buf:	�o�͉摜�z��i��������j
-----------------------------------------------------------------------------*/
int dpcm_vlcode(unsigned char image_in[Y_SIZE][X_SIZE], 
	unsigned char image_buf[])
{
	int			i, j, leng;
	long		ptr, size;
	char		vlc[MAX_LENG];		/* �ϒ����� */
	short int	data[X_SIZE];		/* �P���C�����̂c�o�b�l�f�[�^ */

	size = (long)X_SIZE*Y_SIZE;
	for (i = 0; i < size; i++) image_buf[i] = 0;
	ptr = 0;
	for (i = 0; i < Y_SIZE; i++) {				/* �P���C�����Ƃɕ���������	*/
		DPCM(image_in, i, data);				/* �\��������				*/
		for(j = 0; j < X_SIZE; j++)
			data[j] = event(data[j]);			/* �u�k�b�f�[�^�֕ϊ�����	*/
		leng = vlcode(data, X_SIZE, vlc);		/* �ϒ�������				*/
		image_buf[ptr] = (unsigned char)((leng >> 8) & 0x00ff);
		ptr++;		/* �������f�[�^�����摜���傫���Ȃ�ƃG���[��-1��Ԃ�	*/
		if (ptr > size) return -1;
		image_buf[ptr] = (unsigned char)(leng & 0x00ff);
		ptr++;		/* �������f�[�^�����摜���傫���Ȃ�ƃG���[��-1��Ԃ�	*/
		if (ptr > size) return -1;
		for (j = 0; j < leng; j++) {
			image_buf[ptr] = vlc[j];
			ptr++;
			if (ptr > size) return -1;
		}			/* �������f�[�^�����摜���傫���Ȃ�ƃG���[��-1��Ԃ�	*/
	}
	return 0;
}

