#include	"Params.h"

#define WMAIN		0
#define WPREV		1
#define WSUB		2
#define WKEY		3

#define	MENU_POSX	10	/* ���j���[�E�B���h�E�̉������\���ʒu�i���[�j	*/
#define	MENU_POSY	10	/* ���j���[�E�B���h�E�̏c�����\���ʒu�i��[�j	*/
#define	MENU_SIZEX	690 /* ���j���[�E�B���h�E�̉������\���T�C�Y			*/
#define	MENU_SIZEY	80	/* ���j���[�E�B���h�E�̏c�����\���T�C�Y			*/

#define	WMAIN_POSX	10	/* MAIN�摜�E�B���h�E�̉������\���ʒu�i���[�j	*/
#define	WMAIN_POSY	100	/* MAIN�摜�E�B���h�E�̏c�����\���ʒu�i��[�j	*/
#define	WPREV_POSX	10	/* PREV�摜�E�B���h�E�̉������\���ʒu�i���[�j	*/
#define	WPREV_POSY	405	/* PREV�摜�E�B���h�E�̏c�����\���ʒu�i��[�j	*/
#define	WSUB_POSX	295	/* SUB�摜�E�B���h�E�̉������\���ʒu�i���[�j	*/
#define	WSUB_POSY	100	/* SUB�摜�E�B���h�E�̏c�����\���ʒu�i��[�j	*/
#define	WKEY_POSX	295	/* KEY�摜�E�B���h�E�̉������\���ʒu�i���[�j	*/
#define	WKEY_POSY	405	/* KEY�摜�E�B���h�E�̏c�����\���ʒu�i��[�j	*/

#define MAX_NAME_LENGTH	256

#define P_X_SIZE	X_SIZE*4
#define P_Y_SIZE	Y_SIZE*4

typedef struct {
	HWND	hWnd;
	HBITMAP	hBitmap;
} ImageWin;

#include	"Params.h"
