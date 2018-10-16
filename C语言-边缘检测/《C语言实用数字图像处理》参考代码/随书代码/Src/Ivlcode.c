#define     BYTESIZE  8         /* �P�o�C�g�̃r�b�g�� */
#define     LEN       4         /* �ϒP�� �i�Q�܂��͂S�r�b�g�j */

/*--- ivlcode --- �ϒ������̕��� --------------------------------------------
	vlc_in:		�ϒ�����
	no:			�f�[�^��
	data_out:	�����f�[�^
-----------------------------------------------------------------------------*/
void ivlcode(char vlc_in[], int no, short int data_out[])
{
	int  i, j, k;
	int  ino = 0;           /*�@������ (�o�C�g)  */
	int  num = 0;
	int  dl = BYTESIZE / LEN - 1;
	int  mask = (1 << LEN) - 1;

	for(i = 0; i < no; i++) data_out[i] = 0;
		do {
			for(j = dl; j >= 0; j--) {
			k = vlc_in[ino] & (mask << (LEN * j));
			k >>= (LEN * j);
			data_out[num] += k;
			if(k != mask) num++;
			if (num >= no) break;
		}
		ino++;
	} while (num < no);
}
