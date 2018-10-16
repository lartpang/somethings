#include <math.h>
#include <stdio.h>
#include "Params.h"

#define IDPCM		idpcm1		/* (1)�̗\���@,(2)�̏ꍇ��idpcm2�ɑウ��	*/
#define MAX_LENG	X_SIZE*4	/* �ϒ������p�o�b�t�@�̍ő�o�C�g��		*/

void idpcm1(short data_in[X_SIZE], int line, 
	unsigned char image_out[Y_SIZE][X_SIZE]);
void idpcm2(short data_in[X_SIZE], int line, 
	unsigned char image_out[Y_SIZE][X_SIZE]);
void ivlcode(char vlc_in[], int no, short int data_out[]);
int ievent(short ev);

/*--- idpcm_vlcode --- ������ -------------------------------------------------
	image_buf:	���͉摜�z��i��������j
	image_out:	�o�͉摜�z��
-----------------------------------------------------------------------------*/
int idpcm_vlcode(unsigned char image_buf[],
	unsigned char image_out[Y_SIZE][X_SIZE])
{
	int			i, j, leng;
	long		ptr, size;
	char		vlc[MAX_LENG];	/* �ϒ����� */
	short int	data[X_SIZE];	/* �P���C�����̂c�o�b�l�f�[�^ */

	size = (long)X_SIZE*Y_SIZE;
	ptr = 0;
	for(i = 0; i < Y_SIZE; i++) {		/* �P���C�����Ƃɕ��������� */
		leng = (int)image_buf[ptr];
		ptr++;		/* �������f�[�^�����摜���傫���ꍇ�̓G���[��-1��Ԃ�	*/
		if (ptr > size) return -1;
		leng = (leng << 8) | image_buf[ptr];
		ptr ++;
		for (j = 0; j < leng; j++) {
			vlc[j] = image_buf[ptr];
			ptr++;
			if (ptr > size) return -1;
		}			/* �������f�[�^�����摜���傫���ꍇ�̓G���[��-1��Ԃ�	*/
		ivlcode(vlc, X_SIZE, data);		/* �ϒ������𕜍�����				*/
		for(j = 0; j < X_SIZE; j++)
			data[j] = ievent(data[j]);	/* �u�k�b�f�[�^�������f�[�^�ɖ߂�	*/
		IDPCM(data, i, image_out);		/* �����f�[�^����摜�𕜌�����		*/
	}
	return 0;
}

