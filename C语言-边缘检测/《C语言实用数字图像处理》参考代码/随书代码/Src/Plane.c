#include "Params.h"

#define BUFF_MAX 10000	/* ƒqƒXƒgƒOƒ‰ƒ€•½’R‰»‚Ég—p‚·‚éƒoƒbƒtƒ@‚Ì‘å‚«‚³ */

struct xyw {
	int  x, y, w;		/* ‰æ‘f‚ÌˆÊ’u(x,y)‚Æü•Ó‰æ‘f‚Ì”Z“x‚Ì˜a(weight) */
} buf[BUFF_MAX];		/* ƒoƒbƒtƒ@”z—ñ */

void sort(unsigned char image_in[Y_SIZE][X_SIZE], struct xyw data[],
	int level);
void weight(unsigned char image_in[Y_SIZE][X_SIZE], int i, int j,int *wt);

/*--- plane --- ”Z“xƒqƒXƒgƒOƒ‰ƒ€‚ğ•½’R‰»‚·‚é ----------------------------------
	image_in:	“ü—Í‰æ‘œ”z—ñ
	image_out:	o—Í‰æ‘œ”z—ñ
	hist:		”Z“xƒqƒXƒgƒOƒ‰ƒ€”z—ñ
---------------------------------- ------------------------------------------*/
void plane(unsigned char image_in[Y_SIZE][X_SIZE], 
	unsigned char image_out[Y_SIZE][X_SIZE], long hist[256])
{
	int i, j, iy, jx, sum;
	int delt;              /* ü•Ó‰æ‘fƒŒƒxƒ‹‚É‚æ‚è‘I‚Î‚ê‚é‰æ‘f” */
	int low, high;         /* ˆ—ƒŒƒxƒ‹‚Ì”ÍˆÍ */
	int av;                /* •½’R‰»Œã‚Ì‚P”Z“xƒŒƒxƒ‹‚Ì‰æ‘f” */
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

/*--- sort --- ü‚è‚Ì‰æ‘f‚Ì”Z“x‚Ì‚‚¢‡‚É•À‚Ñ•Ï‚¦‚é ---------------------------
	image_in:	“ü—Í‰æ‘œ”z—ñ
	data:		ˆÊ’u‹y‚Ñü•Ó‰æ‘f‚Ì”Z“x˜a‚Ì”z—ñ
	level:		•À‚Ñ•Ï‚¦‚é‰æ‘f‚Ì”Z“x
-----------------------------------------------------------------------------*/
void sort(unsigned char image_in[Y_SIZE][X_SIZE], struct xyw data[], int level)
{
	int i, j, inum, wt;
	struct xyw temp;

	inum = 0;
	for (i = 0; i < Y_SIZE; i++ ){
		for (j = 0; j < X_SIZE; j++ ){
			if ((int)image_in[i][j] == level){
				weight(image_in, i, j, &wt);	/* ü•Ó‰æ‘f‚Ì”Z“x‚Ì˜a‚ğŒvZ */
				data[inum].y = i;
				data[inum].x = j;
				data[inum].w = wt;
				inum++;
			}
		}
	}

	for (i = 0; i < inum - 1; i++ ){         /* •À‚Ñ•Ï‚¦ */
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

/*--- weight --- ü•Ó‰æ‘f‚Ì”Z“x‚Ì˜a‚ğŒvZ‚·‚é ---------------------------------
	image_in:	“ü—Í‰æ‘œ”z—ñ
	i, j:		‰æ‘fˆÊ’u
	wt:			”Z“x˜a
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
