#include "Params.h"

#define BUFF_MAX 10000	/* ヒストグラム平坦化に使用するバッファの大きさ */

struct xyw {
	int  x, y, w;		/* 画素の位置(x,y)と周辺画素の濃度の和(weight) */
} buf[BUFF_MAX];		/* バッファ配列 */

void sort(unsigned char image_in[Y_SIZE][X_SIZE], struct xyw data[],
	int level);
void weight(unsigned char image_in[Y_SIZE][X_SIZE], int i, int j,int *wt);

/*--- plane --- 濃度ヒストグラムを平坦化する ----------------------------------
	image_in:	入力画像配列
	image_out:	出力画像配列
	hist:		濃度ヒストグラム配列
---------------------------------- ------------------------------------------*/
void plane(unsigned char image_in[Y_SIZE][X_SIZE], 
	unsigned char image_out[Y_SIZE][X_SIZE], long hist[256])
{
	int i, j, iy, jx, sum;
	int delt;              /* 周辺画素レベルにより選ばれる画素数 */
	int low, high;         /* 処理レベルの範囲 */
	int av;                /* 平坦化後の１濃度レベルの画素数 */
	unsigned char image_buf[Y_SIZE][X_SIZE];

	av = (int)((Y_SIZE) * (X_SIZE) / 256);
	high = 255;
	low = 255;
	for (i = 0; i < Y_SIZE; i++){
		for (j = 0; j < X_SIZE; j++){
			image_out[i][j] = 0;
			image_buf[i][j] = image_in[i][j];
		}
	}

	for (i = 255; i > 0; i--){
		for (sum = 0; sum < av; low--) sum = sum + hist[low];
		low++;
		delt = hist[low] - (sum - av);
		sort(image_buf, buf, low);
		if (low < high){
			for (iy = 0; iy < Y_SIZE; iy++){
				for (jx = 0; jx < X_SIZE; jx++){
					if (((int)image_buf[iy][jx] >= low + 1 ) &&
						((int)image_buf[iy][jx] <= high))
					image_out[iy][jx] = (unsigned char)i;
				}
			}
		}
		for (j = 0; j < delt; j++){
			image_out[buf[j].y][buf[j].x] =(unsigned char)i;
			image_buf[buf[j].y][buf[j].x] =(unsigned char)0;
		}
		hist[low] = hist[low] - delt;
		high = low;
	}
}

/*--- sort --- 周りの画素の濃度の高い順に並び変える ---------------------------
	image_in:	入力画像配列
	data:		位置及び周辺画素の濃度和の配列
	level:		並び変える画素の濃度
-----------------------------------------------------------------------------*/
void sort(unsigned char image_in[Y_SIZE][X_SIZE], struct xyw data[], int level)
{
	int i, j, inum, wt;
	struct xyw temp;

	inum = 0;
	for (i = 0; i < Y_SIZE; i++ ){
		for (j = 0; j < X_SIZE; j++ ){
			if ((int)image_in[i][j] == level){
				weight(image_in, i, j, &wt);	/* 周辺画素の濃度の和を計算 */
				data[inum].y = i;
				data[inum].x = j;
				data[inum].w = wt;
				inum++;
			}
		}
	}

	for (i = 0; i < inum - 1; i++ ){         /* 並び変え */
		for (j = i + 1; j < inum; j++){
			if (data[i].w <= data[j].w){
				temp.y = data[i].y;
				temp.x = data[i].x;
				temp.w = data[i].w;
				data[i].y = data[j].y;
				data[i].x = data[j].x;
				data[i].w = data[j].w;
				data[j].y = temp.y;
				data[j].x = temp.x;
				data[j].w = temp.w;
			}
		}
	}
}

/*--- weight --- 周辺画素の濃度の和を計算する ---------------------------------
	image_in:	入力画像配列
	i, j:		画素位置
	wt:			濃度和
-----------------------------------------------------------------------------*/
void weight(unsigned char image_in[Y_SIZE][X_SIZE], int i, int j, int *wt)
{
	int dim, djm;
	int dip, djp;
	int k, d[8];

	dim = i - 1;
	djm = j - 1;
	dip = i + 1;
	djp = j + 1;
	if (dim < 0) dim = i;
	if (djm < 0) djm = j;
	if (dip > Y_SIZE-1) dip = i;
	if (djp > X_SIZE-1) djp = j;

	d[0] = (int)image_in[dim][djm];
	d[1] = (int)image_in[dim][j];
	d[2] = (int)image_in[dim][djp];
	d[3] = (int)image_in[i][djm];
	d[4] = (int)image_in[i][djp];
	d[5] = (int)image_in[dip][djm];
	d[6] = (int)image_in[dip][j];
	d[7] = (int)image_in[dip][djp];
	for (k = 0; k < 8; k++) *wt = *wt + d[i];
}
