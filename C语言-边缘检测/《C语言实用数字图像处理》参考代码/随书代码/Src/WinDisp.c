#include	<stdio.h>
#include	<stdlib.h>
#include	<windows.h>
#include	"WinParams.h"
#include	"Proto.h"

extern int 		bitspixel;				/*	一画素当たりのビット数		*/
extern ImageWin	imageWin[4];

/*--- RgbToBitmap --- 画像変換（RGB画像からBitmap画像へ）---------------------
	pr:			入力Ｒ画像
	pg:			入力Ｇ画像
	pb:			入力Ｂ画像
	hBitmap:	出力Bitmap画像
----------------------------------------------------------------------------*/
void RgbToBitmap(unsigned char pr[Y_SIZE][X_SIZE], 
	unsigned char pg[Y_SIZE][X_SIZE], 
	unsigned char pb[Y_SIZE][X_SIZE], HBITMAP hBitmap)
{
	HANDLE		hDB;
	unsigned char		*psr, *psg, *psb, *pd, *pd0;
	LONG		i;
	
	psr = (unsigned char *)pr; 
	psg = (unsigned char *)pg; 
	psb = (unsigned char *)pb;
	hDB = GlobalAlloc(GMEM_MOVEABLE, (DWORD)X_SIZE*Y_SIZE*4);
	pd = pd0 = GlobalLock(hDB);
	GlobalLock(hBitmap);
	if (bitspixel == 24) {	/*	２４ビット／画素のフレームメモリ	*/
		for (i = 0; i < (DWORD)X_SIZE*Y_SIZE; i++) {
			*pd++ = *psb++; *pd++ = *psg++; *pd++ = *psr++;
		}
		SetBitmapBits(hBitmap, (DWORD)X_SIZE*Y_SIZE*3, pd0);
	}
	else if (bitspixel == 32) {	/*	３２ビット／画素のフレームメモリ	*/
		for (i = 0; i < (DWORD)X_SIZE*Y_SIZE; i++) {
			*pd++ = *psb++; *pd++ = *psg++; *pd++ = *psr++; *pd++;
		}
		SetBitmapBits(hBitmap, (DWORD)X_SIZE*Y_SIZE*4, pd0);
	}
	GlobalUnlock(hBitmap);
	GlobalUnlock(hDB);
	GlobalFree(hDB);
}

/*--- BitmapToRgb --- 画像変換（Bitmap画像からRGB画像へ）---------------------
	hBitmap:	入力Bitmap画像
	pr:			出力Ｒ画像
	pg:			出力Ｇ画像
	pb:			出力Ｂ画像
----------------------------------------------------------------------------*/
void BitmapToRgb(HBITMAP hBitmap, unsigned char pr[Y_SIZE][X_SIZE], 
	unsigned char pg[Y_SIZE][X_SIZE], unsigned char pb[Y_SIZE][X_SIZE])
{
	HANDLE		hDB;
	unsigned char		*pdr, *pdg, *pdb, *ps, *ps0;
	LONG		i;

	pdr = (unsigned char *)pr; 
	pdg = (unsigned char *)pg; 
	pdb = (unsigned char *)pb;
	hDB = GlobalAlloc(GMEM_MOVEABLE, (DWORD)X_SIZE*Y_SIZE*4);
	ps = ps0 = GlobalLock(hDB);
	GlobalLock(hBitmap);
	if (bitspixel == 24) {	/*	２４ビット／画素のフレームメモリ	*/
		GetBitmapBits(hBitmap, (DWORD)X_SIZE*Y_SIZE*3, ps0);
		for (i = 0; i < (DWORD)X_SIZE*Y_SIZE; i++) {
			*pdb++ = *ps++; *pdg++ = *ps++; *pdr++ = *ps++;
		}
	}
	else if (bitspixel == 32) {	/*	３２ビット／画素のフレームメモリ	*/
		GetBitmapBits(hBitmap, (DWORD)X_SIZE*Y_SIZE*4, ps0);
		for (i = 0; i < (DWORD)X_SIZE*Y_SIZE; i++) {
			*pdb++ = *ps++; *pdg++ = *ps++; *pdr++ = *ps++; *ps++;
		}
	}
	GlobalUnlock(hBitmap);
	GlobalUnlock(hDB);
	GlobalFree(hDB);
}

/*--- DisplayImage --- 画像の表示（モノクロ）----------------------------------
	image:		画像データ
	position:	表示ウィンドウ
-----------------------------------------------------------------------------*/
void DisplayImage(unsigned char image[Y_SIZE][X_SIZE], int position)
{
	RgbToBitmap(image, image, image, imageWin[position].hBitmap);
	InvalidateRect(imageWin[position].hWnd, NULL, TRUE);
	SetFocus(imageWin[position].hWnd);
}

/*--- DisplayCImage --- 画像の表示（カラー）-----------------------------------
	image:		画像データ
	position:	表示ウィンドウ
-----------------------------------------------------------------------------*/
void DisplayCImage(unsigned char image[3][Y_SIZE][X_SIZE], int position)
{
	RgbToBitmap(image[0], image[1], image[2], imageWin[position].hBitmap);
	InvalidateRect(imageWin[position].hWnd, NULL, TRUE);
	SetFocus(imageWin[position].hWnd);
}
