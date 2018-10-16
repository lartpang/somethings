#include	<stdio.h>
#include	<stdlib.h>
#include	<windows.h>
#include	"WinParams.h"
#include	"Proto.h"

extern int 		bitspixel;				/*	���f������̃r�b�g��		*/
extern ImageWin	imageWin[4];

/*--- RgbToBitmap --- �摜�ϊ��iRGB�摜����Bitmap�摜�ցj---------------------
	pr:			���͂q�摜
	pg:			���͂f�摜
	pb:			���͂a�摜
	hBitmap:	�o��Bitmap�摜
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
	if (bitspixel == 24) {	/*	�Q�S�r�b�g�^��f�̃t���[��������	*/
		for (i = 0; i < (DWORD)X_SIZE*Y_SIZE; i++) {
			*pd++ = *psb++; *pd++ = *psg++; *pd++ = *psr++;
		}
		SetBitmapBits(hBitmap, (DWORD)X_SIZE*Y_SIZE*3, pd0);
	}
	else if (bitspixel == 32) {	/*	�R�Q�r�b�g�^��f�̃t���[��������	*/
		for (i = 0; i < (DWORD)X_SIZE*Y_SIZE; i++) {
			*pd++ = *psb++; *pd++ = *psg++; *pd++ = *psr++; *pd++;
		}
		SetBitmapBits(hBitmap, (DWORD)X_SIZE*Y_SIZE*4, pd0);
	}
	GlobalUnlock(hBitmap);
	GlobalUnlock(hDB);
	GlobalFree(hDB);
}

/*--- BitmapToRgb --- �摜�ϊ��iBitmap�摜����RGB�摜�ցj---------------------
	hBitmap:	����Bitmap�摜
	pr:			�o�͂q�摜
	pg:			�o�͂f�摜
	pb:			�o�͂a�摜
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
	if (bitspixel == 24) {	/*	�Q�S�r�b�g�^��f�̃t���[��������	*/
		GetBitmapBits(hBitmap, (DWORD)X_SIZE*Y_SIZE*3, ps0);
		for (i = 0; i < (DWORD)X_SIZE*Y_SIZE; i++) {
			*pdb++ = *ps++; *pdg++ = *ps++; *pdr++ = *ps++;
		}
	}
	else if (bitspixel == 32) {	/*	�R�Q�r�b�g�^��f�̃t���[��������	*/
		GetBitmapBits(hBitmap, (DWORD)X_SIZE*Y_SIZE*4, ps0);
		for (i = 0; i < (DWORD)X_SIZE*Y_SIZE; i++) {
			*pdb++ = *ps++; *pdg++ = *ps++; *pdr++ = *ps++; *ps++;
		}
	}
	GlobalUnlock(hBitmap);
	GlobalUnlock(hDB);
	GlobalFree(hDB);
}

/*--- DisplayImage --- �摜�̕\���i���m�N���j----------------------------------
	image:		�摜�f�[�^
	position:	�\���E�B���h�E
-----------------------------------------------------------------------------*/
void DisplayImage(unsigned char image[Y_SIZE][X_SIZE], int position)
{
	RgbToBitmap(image, image, image, imageWin[position].hBitmap);
	InvalidateRect(imageWin[position].hWnd, NULL, TRUE);
	SetFocus(imageWin[position].hWnd);
}

/*--- DisplayCImage --- �摜�̕\���i�J���[�j-----------------------------------
	image:		�摜�f�[�^
	position:	�\���E�B���h�E
-----------------------------------------------------------------------------*/
void DisplayCImage(unsigned char image[3][Y_SIZE][X_SIZE], int position)
{
	RgbToBitmap(image[0], image[1], image[2], imageWin[position].hBitmap);
	InvalidateRect(imageWin[position].hWnd, NULL, TRUE);
	SetFocus(imageWin[position].hWnd);
}
