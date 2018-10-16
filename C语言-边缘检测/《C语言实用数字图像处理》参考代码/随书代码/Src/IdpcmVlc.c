#include <math.h>
#include <stdio.h>
#include "Params.h"

#define IDPCM		idpcm1		/* (1)の予測法,(2)の場合はidpcm2に代える	*/
#define MAX_LENG	X_SIZE*4	/* 可変長符号用バッファの最大バイト数		*/

void idpcm1(short data_in[X_SIZE], int line, 
	unsigned char image_out[Y_SIZE][X_SIZE]);
void idpcm2(short data_in[X_SIZE], int line, 
	unsigned char image_out[Y_SIZE][X_SIZE]);
void ivlcode(char vlc_in[], int no, short int data_out[]);
int ievent(short ev);

/*--- idpcm_vlcode --- 復号化 -------------------------------------------------
	image_buf:	入力画像配列（符号化列）
	image_out:	出力画像配列
-----------------------------------------------------------------------------*/
int idpcm_vlcode(unsigned char image_buf[],
	unsigned char image_out[Y_SIZE][X_SIZE])
{
	int			i, j, leng;
	long		ptr, size;
	char		vlc[MAX_LENG];	/* 可変長符号 */
	short int	data[X_SIZE];	/* １ライン分のＤＰＣＭデータ */

	size = (long)X_SIZE*Y_SIZE;
	ptr = 0;
	for(i = 0; i < Y_SIZE; i++) {		/* １ラインごとに復号化する */
		leng = (int)image_buf[ptr];
		ptr++;		/* 符号化データが元画像より大きい場合はエラーで-1を返す	*/
		if (ptr > size) return -1;
		leng = (leng << 8) | image_buf[ptr];
		ptr ++;
		for (j = 0; j < leng; j++) {
			vlc[j] = image_buf[ptr];
			ptr++;
			if (ptr > size) return -1;
		}			/* 符号化データが元画像より大きい場合はエラーで-1を返す	*/
		ivlcode(vlc, X_SIZE, data);		/* 可変長符号を復号する				*/
		for(j = 0; j < X_SIZE; j++)
			data[j] = ievent(data[j]);	/* ＶＬＣデータを差分データに戻す	*/
		IDPCM(data, i, image_out);		/* 差分データから画像を復元する		*/
	}
	return 0;
}

