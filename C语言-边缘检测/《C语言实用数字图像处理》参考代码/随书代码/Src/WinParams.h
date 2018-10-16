#include	"Params.h"

#define WMAIN		0
#define WPREV		1
#define WSUB		2
#define WKEY		3

#define	MENU_POSX	10	/* メニューウィンドウの横方向表示位置（左端）	*/
#define	MENU_POSY	10	/* メニューウィンドウの縦方向表示位置（上端）	*/
#define	MENU_SIZEX	690 /* メニューウィンドウの横方向表示サイズ			*/
#define	MENU_SIZEY	80	/* メニューウィンドウの縦方向表示サイズ			*/

#define	WMAIN_POSX	10	/* MAIN画像ウィンドウの横方向表示位置（左端）	*/
#define	WMAIN_POSY	100	/* MAIN画像ウィンドウの縦方向表示位置（上端）	*/
#define	WPREV_POSX	10	/* PREV画像ウィンドウの横方向表示位置（左端）	*/
#define	WPREV_POSY	405	/* PREV画像ウィンドウの縦方向表示位置（上端）	*/
#define	WSUB_POSX	295	/* SUB画像ウィンドウの横方向表示位置（左端）	*/
#define	WSUB_POSY	100	/* SUB画像ウィンドウの縦方向表示位置（上端）	*/
#define	WKEY_POSX	295	/* KEY画像ウィンドウの横方向表示位置（左端）	*/
#define	WKEY_POSY	405	/* KEY画像ウィンドウの縦方向表示位置（上端）	*/

#define MAX_NAME_LENGTH	256

#define P_X_SIZE	X_SIZE*4
#define P_Y_SIZE	Y_SIZE*4

typedef struct {
	HWND	hWnd;
	HBITMAP	hBitmap;
} ImageWin;

#include	"Params.h"
