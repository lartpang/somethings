#include <stdio.h>
#include "Params.h"

/*--- histprint --- ヒストグラムを数字とグラフで印字する ----------------------
	hist:	ヒストグラム
	buf:	メッセージ用バッファ
-----------------------------------------------------------------------------*/
void histprint(long hist[256], char *buf)
{
	int i, j, k;
	double p, q, max;
	int posi, m;

	posi = 0;
	p = X_SIZE * Y_SIZE;
	max = 0;
	// 寻找频度最大的颜色值的频度（像素数目）
	// sprintf()如果成功，则返回写入的字符总数，不包括字符串追加在字符串末尾的空字符。如果失败，则返回一个负数。
	for (i = 0; i < 256; i++) if (hist[i] > max) max = hist[i];
	for (i = 0; i < 256; i++) {
		q = hist[i] / p * 100.0;
		m = sprintf(&buf[posi], "%3d:%5.1f%%|", i, q);
		posi += m;
		k = (int)(hist[i] / max * 60.0);
		for (j = 0; j < k; j++) {
			m = sprintf(&buf[posi], "*");
			posi += m;
		}
		m = sprintf(&buf[posi], "\n");
		posi += m;
	}
}
