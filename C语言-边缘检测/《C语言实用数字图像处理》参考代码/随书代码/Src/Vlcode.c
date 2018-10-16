#define	BYTESIZE	8		/* �P�o�C�g�̃r�b�g�� */
#define	LEN			4		/* �ϒP�ʁi�Q�܂��͂S�r�b�g�j */

/*--- vlcode --- �ϒ������� -------------------------------------------------
	data_in:	���f�[�^
	no:			�f�[�^��
	vlc_out:	�ϒ�����
-----------------------------------------------------------------------------*/
int vlcode(short int data_in[], int no, char vlc_out[])
{
	int  i;
	int  st = 0;
	int  num = 0;	                    /*  �������@(�o�C�g) */
	int  dl = BYTESIZE / LEN - 1;
	int  mask = (1 << LEN) - 1 ;
	int  dt, ms;

	vlc_out[num] = '\0';
	for(i = 0; i < no; i++) {
		dt = data_in[i];
		do {
			ms = dt >= mask ? mask : dt;
			vlc_out[num] |= (ms << (LEN * (dl - st)));
			dt -= mask;   st++;
			if(st > dl) {
				st = 0;   num++; vlc_out[num] = '\0';
			}
		} while(dt >= 0);
	}
	if(st != 0) {       /* �Ō�̃o�C�g�̎c��̃r�b�g��'1'�ɂ��� */
		ms = mask;
		for(i = (dl - st); i >= 0; i--) {
			vlc_out[num] |= ms;
			ms <<= LEN;
		}
		num++;
	}
	return num;
}
