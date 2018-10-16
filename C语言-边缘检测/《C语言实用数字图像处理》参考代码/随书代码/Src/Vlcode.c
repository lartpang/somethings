#define	BYTESIZE	8		/* １バイトのビット数 */
#define	LEN			4		/* 可変単位（２または４ビット） */

/*--- vlcode --- 可変長符号化 -------------------------------------------------
	data_in:	原データ
	no:			データ数
	vlc_out:	可変長符号
-----------------------------------------------------------------------------*/
int vlcode(short int data_in[], int no, char vlc_out[])
{
	int  i;
	int  st = 0;
	int  num = 0;	                    /*  符号長　(バイト) */
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
	if(st != 0) {       /* 最後のバイトの残りのビットを'1'にする */
		ms = mask;
		for(i = (dl - st); i >= 0; i--) {
			vlc_out[num] |= ms;
			ms <<= LEN;
		}
		num++;
	}
	return num;
}
