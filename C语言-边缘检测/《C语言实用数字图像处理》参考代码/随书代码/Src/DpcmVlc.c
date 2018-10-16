#include "Params.h"

#define DPCM		dpcm1		/* (1)の予測法,(2)の場合はdpcm2に代える */
#define MAX_LENG	X_SIZE*4	/* 可変長符号用バッファの最大バイト数 */

int vlcode(short int data_in[], int no, char vlc_out[]);
void dpcm1(unsigned char image_in[Y_SIZE][X_SIZE], 
	int line, short data_out[X_SIZE]);
void dpcm2(unsigned char image_in[Y_SIZE][X_SIZE], 
	int line, short data_out[X_SIZE]);
int event(short dt);

/*--- dpcm_vlcode --- 符号化 --------------------------------------------------
	image_in:	入力画像配列
	image_buf:	出力画像配列（符号化列）
-----------------------------------------------------------------------------*/
int dpcm_vlcode(unsigned char image_in[Y_SIZE][X_SIZE], 
	unsigned char image_buf[])
{
	int			i, j, leng;
	long		ptr, size;
	char		vlc[MAX_LENG];		/* 可変長符号 */
	short int	data[X_SIZE];		/* １ライン分のＤＰＣＭデータ */

	size = (long)X_SIZE*Y_SIZE;
	for (i = 0; i < size; i++) image_buf[i] = 0;
	ptr = 0;
	for (i = 0; i < Y_SIZE; i++) {				/* １ラインごとに符号化する	*/
		DPCM(image_in, i, data);				/* 予測符号化				*/
		for(j = 0; j < X_SIZE; j++)
			data[j] = event(data[j]);			/* ＶＬＣデータへ変換する	*/
		leng = vlcode(data, X_SIZE, vlc);		/* 可変長符号化				*/
		image_buf[ptr] = (unsigned char)((leng >> 8) & 0x00ff);
		ptr++;		/* 符号化データが元画像より大きくなるとエラーで-1を返す	*/
		if (ptr > size) return -1;
		image_buf[ptr] = (unsigned char)(leng & 0x00ff);
		ptr++;		/* 符号化データが元画像より大きくなるとエラーで-1を返す	*/
		if (ptr > size) return -1;
		for (j = 0; j < leng; j++) {
			image_buf[ptr] = vlc[j];
			ptr++;
			if (ptr > size) return -1;
		}			/* 符号化データが元画像より大きくなるとエラーで-1を返す	*/
	}
	return 0;
}

