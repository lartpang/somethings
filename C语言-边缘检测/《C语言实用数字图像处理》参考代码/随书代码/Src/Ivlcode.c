#define     BYTESIZE  8         /* １バイトのビット数 */
#define     LEN       4         /* 可変単位 （２または４ビット） */

/*--- ivlcode --- 可変長符号の復号 --------------------------------------------
	vlc_in:		可変長符号
	no:			データ数
	data_out:	復号データ
-----------------------------------------------------------------------------*/
void ivlcode(char vlc_in[], int no, short int data_out[])
{
	int  i, j, k;
	int  ino = 0;           /*　符号長 (バイト)  */
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
