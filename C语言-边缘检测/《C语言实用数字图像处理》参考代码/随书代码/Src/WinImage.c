#include	<stdio.h>
#include	<stdlib.h>
#include	<windows.h>
#include    "WinParams.h"
#include	"WinProto.h"
#include    "WinImage.h"

LRESULT CALLBACK MainWndProc(HWND, UINT, WPARAM, LPARAM);
BOOL InitApplication(HANDLE);
BOOL InitInstance(HANDLE, int);

char *wTitle[4] = {"MAIN", "PREV", "SUB", "KEY"};/* ウィンドウのタイトル	*/
unsigned char image_main[Y_SIZE][X_SIZE];		/* メイン画像配列			*/
unsigned char image_prev[Y_SIZE][X_SIZE];		/* プリ画像配列				*/
unsigned char image_sub[Y_SIZE][X_SIZE];		/* サブ画像配列				*/
unsigned char image_key[Y_SIZE][X_SIZE];		/* キー画像配列				*/
unsigned char image_buf[Y_SIZE][X_SIZE];		/* バッファ配列				*/
unsigned char image_work[Y_SIZE*2][X_SIZE*2];	/* モザイク用画像配列		*/
unsigned char image_prt1[P_Y_SIZE][P_X_SIZE];	/* 印刷用画像配列			*/
unsigned char image_prt2[P_Y_SIZE][P_X_SIZE];	/* 印刷用画像配列			*/
unsigned char image_r[Y_SIZE][X_SIZE];			/* 画像配列(R)				*/
unsigned char image_g[Y_SIZE][X_SIZE];			/* 画像配列(G)				*/
unsigned char image_b[Y_SIZE][X_SIZE];			/* 画像配列(B)				*/
unsigned char image_rgb[3][Y_SIZE][X_SIZE];		/* カラー画像配列(RGB)		*/
unsigned char image_hist[Y_SIZE][X_SIZE];		/* ヒストグラム画像配列		*/
int		bitspixel;								/* 一画素当たりのビット数	*/
long	hist[256];								/* ヒストグラム配列			*/
long	histbuf[256];							/* ヒストグラム配列			*/
double	ratio[128], size[128];					/* 特徴パラメータ配列		*/
char	text_buf[TEXT_BUF_LEN];					/* 出力用文字配列			*/

HANDLE   ghInst;		/*　インスタンスハンドル	*/
HWND     hGWnd;			/*　メインウィンドウ		*/
HBITMAP  hBitmap;		/*　ビットマップ			*/
HMENU    hSubMenu;		/*　サブメニュー			*/
ImageWin imageWin[4];	/*　画像用ウィンドウ		*/

OPENFILENAME ofn;
char szFilterSpecBmp[128] = "BMP画像ﾌｧｲﾙ (*.bmp)\0*.bmp\0";
char szFilterSpec[128] = "画像ﾌｧｲﾙ (*.*)\0*.*\0";
char szFileName[MAX_NAME_LENGTH];	/*　ファイル名（パス付き）	*/
char szFileTitle[MAX_NAME_LENGTH];	/*　ファイル名（パス無し）	*/

/*--- WinMain --- メイン ------------------------------------------------------
	hInstance:		アプリケーションへのインスタンスハンドル
	hPrevInstance:	直前のインスタンスハンドル
	lpCmdLine:		コマンドラインの文字列
	nCmdShow:		起動時の表示方法
-----------------------------------------------------------------------------*/
int APIENTRY WinMain(HINSTANCE hInstance, HINSTANCE hPrevInstance, 
	LPSTR lpCmdLine, int nCmdShow)
{
    MSG     msg;

    if (!hPrevInstance)
    if (!InitApplication(hInstance)) return 0;
    if (!InitInstance(hInstance, nCmdShow)) return 0;
    ghInst = hInstance;
    while (GetMessage(&msg, (HWND)NULL, (UINT)NULL, (UINT)NULL)) {
        TranslateMessage(&msg);
        DispatchMessage(&msg);
    }
    return msg.wParam;
}

/*--- MainWndProc --- メッセージの処理 ----------------------------------------
	hWnd:		ウィンドウ
	message:	メッセージ
	wParam:		ワードパラメータ
	Param:		ロングパラメータ
-----------------------------------------------------------------------------*/
LRESULT CALLBACK MainWndProc(HWND hWnd, UINT message, WPARAM wParam, 
	LPARAM lParam)
{
    HDC 			hMemDC, hDC, hDCs, hDCd;
    PAINTSTRUCT 	ps;
	HBITMAP			hBitmapd;
	BITMAP			bmp;
    POINT   		point;
    int 			wno, i, j, k, mx, my;
    int 			thres, type, smt, mode, lvl;
    double   		a, b, c, d, e, f, g, h, x0, y0, deg;
	int         	x1, y1, x2, y2, x3, y3, x4, y4, xd, yd, xb, yb;
	int         	u1, v1, u2, v2, u3, v3, u4, v4;
    double   		ratio_min, ratio_max;
    double   		size_min, size_max, amp;
    int 			fmax, fmin, cnt, num;
	char			filename[128];
	unsigned char	*rr, *p;

    switch (message) {
        case WM_RBUTTONDOWN:
            if (hWnd != imageWin[WMAIN].hWnd && hWnd != imageWin[WPREV].hWnd
				&& hWnd != imageWin[WSUB].hWnd && hWnd != imageWin[WKEY].hWnd)
 				break;
            point.x = LOWORD(lParam);
            point.y = HIWORD(lParam);
			ClientToScreen(hWnd, &point);
			TrackPopupMenu (hSubMenu, 0, point.x, point.y, 0, hWnd, NULL);
            break;
		case WM_LBUTTONDOWN:
			if (hWnd != imageWin[WMAIN].hWnd) break;
			point.x = LOWORD(lParam);
			point.y = HIWORD(lParam);
			hDC = GetDC(hGWnd);
			rr = &image_main[0][0] + X_SIZE*point.y + point.x;
			sprintf(text_buf, "%3d on (%3d, %3d) ", *rr, point.x, point.y);
			TextOut(hDC, 0, 0, text_buf, strlen(text_buf));
			break;
        case WM_COMMAND:
            switch(wParam) {
                case MENU_READ_BMP:
					ofn.lpstrFilter = szFilterSpecBmp;
					if (!GetOpenFileName ((LPOPENFILENAME)&ofn)) return FALSE;
					num = check_bmp_file(ofn.lpstrFileTitle);
					if (num == 8) {
						if (read_bmp_mono(image_main, ofn.lpstrFileTitle) != -1)
							DisplayImage(image_main, WMAIN);
						else
							MessageBox(GetFocus(), "File open error", "Error", 
								MB_OK | MB_APPLMODAL);
					}
					else if (num == 24) {
						if (read_bmp_color(image_rgb, ofn.lpstrFileTitle) != -1) {
							if (!GetSelection3("読み込みプレーン",
								"Ｒ", "Ｇ", "Ｂ", &type)) break;
							image_copy(image_rgb[type-1], image_main);
									DisplayImage(image_main, WMAIN);
						}
						else
							MessageBox(GetFocus(), "File open error", "Error", 
								MB_OK | MB_APPLMODAL);
					}
					else
							MessageBox(GetFocus(), "File open error", "Error", 
								MB_OK | MB_APPLMODAL);
					break;
                case MENU_WRITE_BMP:
					ofn.lpstrFilter = szFilterSpecBmp;
					if (!GetSaveFileName ((LPOPENFILENAME)&ofn)) return FALSE;
					sprintf(filename, "%s.bmp", ofn.lpstrFileTitle);
					if (write_bmp_mono(image_main, filename) == -1)
						MessageBox(GetFocus(), "File open error", 
							"Error", MB_OK | MB_APPLMODAL);
            	    break;
                case MENU_READ_RAW:
					ofn.lpstrFilter = szFilterSpec;
                    if (!GetOpenFileName ((LPOPENFILENAME)&ofn))
                        return FALSE;
                    if (read_mono(image_main, ofn.lpstrFileTitle) != -1)
                    	DisplayImage(image_main, WMAIN);
                    else
                    	MessageBox(GetFocus(), "File open error", "Error", 
                    		MB_OK | MB_APPLMODAL);
                    break;
                case MENU_WRITE_RAW:
					ofn.lpstrFilter = szFilterSpec;
                    if (!GetSaveFileName ((LPOPENFILENAME)&ofn))
                        return FALSE;
                    if (write_mono(image_main, ofn.lpstrFileTitle) == -1)
                    	MessageBox(GetFocus(), "File open error", "Error", 
                    		MB_OK | MB_APPLMODAL);
                    break;
                case MENU_PRINT:
					mx = P_X_SIZE/X_SIZE; my = P_Y_SIZE/Y_SIZE;
					for (i = 0; i < P_Y_SIZE; i++)
						for (j = 0; j < P_X_SIZE; j++)
							image_prt1[i][j] = image_main[i/my][j/mx];
					m_dither(image_prt1, image_prt2);
					if (ImgPrint(image_prt2, image_prt2, image_prt2) == -1)
						MessageBox(NULL, "Print error", "Error", 
							MB_OK | MB_APPLMODAL);
					break;
				case MENU_READ_BMP_SUB:
					ofn.lpstrFilter = szFilterSpecBmp;
					if (!GetOpenFileName ((LPOPENFILENAME)&ofn)) return FALSE;
					num = check_bmp_file(ofn.lpstrFileTitle);
					if (num == 8) {
						if (read_bmp_mono(image_buf, ofn.lpstrFileTitle) == -1)
							MessageBox(GetFocus(), "File open error", "Error", 
								MB_OK | MB_APPLMODAL);
					}
					else if (num == 24) {
						if (read_bmp_color(image_rgb, ofn.lpstrFileTitle) != -1) {
							if (!GetSelection3("読み込みプレーン",
								"Ｒ", "Ｇ", "Ｂ", &type)) break;
							image_copy(image_rgb[type-1], image_buf);
						}
						else
							MessageBox(GetFocus(), "File open error", "Error", 
								MB_OK | MB_APPLMODAL);
					}
					else
							MessageBox(GetFocus(), "File open error", "Error", 
								MB_OK | MB_APPLMODAL);
					if (hWnd == imageWin[WMAIN].hWnd) {
						image_copy(image_buf, image_main);
						DisplayImage(image_main, WMAIN);
					}
					else if (hWnd == imageWin[WPREV].hWnd){
						image_copy(image_buf, image_prev);
						DisplayImage(image_prev, WPREV);
					}
					else if (hWnd == imageWin[WSUB].hWnd){
						image_copy(image_buf, image_sub);
						DisplayImage(image_sub, WSUB);
					}
					else if (hWnd == imageWin[WKEY].hWnd){
						image_copy(image_buf, image_key);
						DisplayImage(image_key, WKEY);
					}
					break;
				case MENU_WRITE_BMP_SUB:
					ofn.lpstrFilter = szFilterSpecBmp;
					if (!GetSaveFileName ((LPOPENFILENAME)&ofn)) return FALSE;
					sprintf(filename, "%s.bmp", ofn.lpstrFileTitle);
					if (hWnd == imageWin[WMAIN].hWnd) {
						if (write_bmp_mono(image_main, filename) == -1)
							MessageBox(GetFocus(), "File open error", 
									"Error", MB_OK | MB_APPLMODAL);
					}
					else if (hWnd == imageWin[WPREV].hWnd){
						if (write_bmp_mono(image_prev, filename) == -1)
							MessageBox(GetFocus(), "File open error", 
									"Error", MB_OK | MB_APPLMODAL);
					}
					else if (hWnd == imageWin[WSUB].hWnd){
						if (write_bmp_mono(image_sub, filename) == -1) 
							MessageBox(GetFocus(), "File open error", 
									"Error", MB_OK | MB_APPLMODAL);
					}
					else if (hWnd == imageWin[WKEY].hWnd){
						if (write_bmp_mono(image_key, filename) == -1)
							MessageBox(GetFocus(), "File open error", 
									"Error", MB_OK | MB_APPLMODAL);
					}
            	    break;
                case MENU_QUIT:
                    DestroyWindow(hWnd);
                    break;
				case MENU_CLEAR:
					if (hWnd == imageWin[WMAIN].hWnd){
						image_clear(image_main);
						DisplayImage(image_main, WMAIN);
					}
					else if (hWnd == imageWin[WPREV].hWnd){
						image_clear(image_prev);
						DisplayImage(image_prev, WPREV);
					}
					else if (hWnd == imageWin[WSUB].hWnd){
						image_clear(image_sub);
						DisplayImage(image_sub, WSUB);
					}
					else if (hWnd == imageWin[WKEY].hWnd){
						image_clear(image_key);
						DisplayImage(image_key, WKEY);
					}
					break;
                case MENU_COPY:
                    hDC = GetDC(hWnd);
                    hBitmap = CreateCompatibleBitmap(hDC, X_SIZE, Y_SIZE);
                    if (hWnd == imageWin[WMAIN].hWnd)
                      RgbToBitmap(image_main, image_main, image_main, hBitmap);
                    else if (hWnd == imageWin[WPREV].hWnd)
                      RgbToBitmap(image_prev, image_prev, image_prev, hBitmap);
                    else if (hWnd == imageWin[WSUB].hWnd)
                      RgbToBitmap(image_sub, image_sub, image_sub, hBitmap);
                    else if (hWnd == imageWin[WKEY].hWnd)
                      RgbToBitmap(image_key, image_key, image_key, hBitmap);
                    else break;
                    OpenClipboard(NULL);
                    EmptyClipboard();
					SetClipboardData(CF_BITMAP, hBitmap);
                    CloseClipboard();
                    break;
                case MENU_PASTE:
					OpenClipboard(NULL);
					hBitmap = (HBITMAP)GetClipboardData(CF_BITMAP);
					if (hBitmap == 0) break;
					k = bitspixel / 8;
					p = (unsigned char *)malloc(X_SIZE * Y_SIZE * k);
					if (p == NULL) break;
					hBitmapd = CreateBitmap(X_SIZE, Y_SIZE, 1, bitspixel, p);
					hDCs = CreateCompatibleDC(NULL);
					hDCd = CreateCompatibleDC(NULL);
					SelectObject(hDCs, hBitmap);
					SelectObject(hDCd, hBitmapd);
					GetObject(hBitmap, sizeof(BITMAP), &bmp);
					SetStretchBltMode(hDCd, COLORONCOLOR);
					StretchBlt(hDCd, 0, 0, X_SIZE, Y_SIZE, hDCs, 0, 0, 
						bmp.bmWidth, bmp.bmHeight, SRCCOPY);
                    BitmapToRgb(hBitmapd, image_r, image_g, image_b);
                    CloseClipboard();
                    if (hWnd == imageWin[WMAIN].hWnd){
                        image_copy(image_g, image_main);
                        DisplayImage(image_main, WMAIN);
                    }
                    else if (hWnd == imageWin[WPREV].hWnd){
                        image_copy(image_g, image_prev);
                        DisplayImage(image_prev, WPREV);
                    }
                    else if (hWnd == imageWin[WSUB].hWnd){
                        image_copy(image_g, image_sub);
                        DisplayImage(image_sub, WSUB);
                    }
                    else if (hWnd == imageWin[WKEY].hWnd){
                        image_copy(image_g, image_key);
                        DisplayImage(image_key, WKEY);
                    }
                    break;
                case MENU_UNDO:
                    image_copy(image_prev, image_main);
                    DisplayImage(image_main, WMAIN);
                    break;
				case MENU_CLEAR_MAIN:
					image_clear(image_main);
					DisplayImage(image_main, WMAIN);
					break;
				case MENU_CLEAR_SUB:
					image_clear(image_sub);
					DisplayImage(image_sub, WSUB);
					break;
				case MENU_CLEAR_KEY:
					image_clear(image_key);
					DisplayImage(image_key, WKEY);
					break;
				case MENU_COPY_CM:
                    OpenClipboard(NULL);
                    hBitmap = (HBITMAP)GetClipboardData(CF_BITMAP);
                    BitmapToRgb(hBitmap, image_r, image_g, image_b);
                    CloseClipboard();
                    image_copy(image_g, image_main);
                    DisplayImage(image_main, WMAIN);
                    break;
                case MENU_COPY_MC:
                    hDC = GetDC(hWnd);
                    hBitmap = CreateCompatibleBitmap(hDC, X_SIZE, Y_SIZE);
                    RgbToBitmap(image_main, image_main, image_main, hBitmap);
                    OpenClipboard(NULL);
                    EmptyClipboard();
                    SetClipboardData(CF_BITMAP, hBitmap);
                    CloseClipboard();
                    break;
                case MENU_COPY_MS:
                    image_copy(image_main, image_sub);
                    DisplayImage(image_sub, WSUB);
                    break;
                case MENU_COPY_SM:
                    image_copy(image_sub, image_main);
                    DisplayImage(image_main, WMAIN);
                    break;
                case MENU_COPY_MK:
                    image_copy(image_main, image_key);
                    DisplayImage(image_key, WKEY);
                    break;
                case MENU_COPY_KM:
                    image_copy(image_key, image_main);
                    DisplayImage(image_main, WMAIN);
                    break;
                case MENU_INV:
                    image_copy(image_main, image_prev);
					DisplayImage(image_prev, WPREV);
					image_negative(image_main);
					DisplayImage(image_main, WMAIN);
					break;
                case MENU_ADD:
                    image_copy(image_main, image_prev);
                    DisplayImage(image_prev, WPREV);
                    image_addition(image_prev, image_sub, image_main);
                    DisplayImage(image_main, WMAIN);
                    break;
                case MENU_SUB:
                    image_copy(image_main, image_prev);
                    DisplayImage(image_prev, WPREV);
                    image_subtraction(image_prev, image_sub, image_main);
                    DisplayImage(image_main, WMAIN);
                    break;
                case MENU_DIF:
                    image_copy(image_main, image_prev);
                    DisplayImage(image_prev, WPREV);
                    image_difference(image_prev, image_sub, image_main);
                    DisplayImage(image_main, WMAIN);
                    break;
                case MENU_MPY:
                    image_copy(image_main, image_prev);
                    DisplayImage(image_prev, WPREV);
                    image_multiplication(image_prev, image_sub, image_main);
                    DisplayImage(image_main, WMAIN);
                    break;
                case MENU_OR:
                    image_copy(image_main, image_prev);
                    DisplayImage(image_prev, WPREV);
                    image_or(image_prev, image_sub, image_main);
                    DisplayImage(image_main, WMAIN);
                    break;
                case MENU_AND:
                    image_copy(image_main, image_prev);
                    DisplayImage(image_prev, WPREV);
                    image_and(image_prev, image_sub, image_main);
                    DisplayImage(image_main, WMAIN);
                    break;
				case MENU_HIST:
                    histgram(image_main, hist);
                    histimage(hist,image_sub);
                    DisplayImage(image_sub, WSUB);
                    break;
				case MENU_HISTPRIN:
					histprint(hist, text_buf);
					ShowText("ヒストグラム", text_buf);
                    break;
				case MENU_HIST_SMOTH:
					histsmooth(hist, histbuf);
					for (i = 0; i < 256; i++) hist[i] = histbuf[i];
					histimage(hist, image_sub);
                    DisplayImage(image_sub, WSUB);
                    break;
				case MENU_THRESH_MAN:
                    if (!GetParams1("閾値", &a)) break;
                    thres = (int)a;
                    if (thres < 0 || thres > 255) break;
                    if (!GetSelection2("表示画像", "そのまま", "反転", &type)) 
						break;
                    image_copy(image_main, image_prev);
                    DisplayImage(image_prev, WPREV);
                    threshold(image_prev, image_main, thres, type);
                    DisplayImage(image_main, WMAIN);
                    break;
				case MENU_THRESH_MODE:
                    if (!GetParams1("平滑化回数", &a)) break;
                    smt = (int)a;
                    if (!GetSelection2("表示画像", "そのまま", "反転", &type))
						break;
                    image_copy(image_main, image_prev);
                    DisplayImage(image_prev, WPREV);
                    threshold_mode(image_prev, image_main, smt, type);
                    DisplayImage(image_main, WMAIN);
                    break;
				case MENU_THRESH_DISCRIM:
					if (!GetSelection2("表示画像", "そのまま", "反転", &type))
						break;
                    image_copy(image_main, image_prev);
                    DisplayImage(image_prev, WPREV);
                    threshold_discrim(image_prev, image_main, type);
                    DisplayImage(image_main, WMAIN);
					break;
				case MENU_THRESH_DYNAMIC:
                    if (!GetSelection2("表示画像", "そのまま", "反転", &type))
						break;
                    image_copy(image_main, image_prev);
                    DisplayImage(image_prev, WPREV);
                    threshold_dynamic(image_prev, image_main, type);
                    DisplayImage(image_main, WMAIN);
                    break;
                 case MENU_GRADIENT_DIFFERENCE:
                 case MENU_GRADIENT_ROBERTS:
                 case MENU_GRADIENT_SOBEL:
                    if (!GetParams1("出力画像の利得", &amp)) break;
                    image_copy(image_main, image_prev);
                    DisplayImage(image_prev, WPREV);
					if (wParam == MENU_GRADIENT_DIFFERENCE) 
						gradient_difference(image_prev, image_main, amp);
					if (wParam == MENU_GRADIENT_ROBERTS) 
						gradient_roberts(image_prev, image_main, amp);
					if (wParam == MENU_GRADIENT_SOBEL) 
						gradient_sobel(image_prev, image_main, amp);
                    DisplayImage(image_main, WMAIN);
                    break;
                case MENU_TEMPLATE:
                    if (!GetParams1("出力画像の利得", &amp)) break;
                    image_copy(image_main, image_prev);
                    DisplayImage(image_prev, WPREV);
                    prewitt(image_prev, image_main, amp);
                    DisplayImage(image_main, WMAIN);
                    break;
                 case MENU_LAPLACIAN1:
                 case MENU_LAPLACIAN2:
                 case MENU_LAPLACIAN3:
                    if (!GetParams1("出力画像の利得", &amp)) break;
                    image_copy(image_main, image_prev);
                    DisplayImage(image_prev, WPREV);
					if (wParam == MENU_LAPLACIAN1) type = 1;
					if (wParam == MENU_LAPLACIAN2) type = 2;
					if (wParam == MENU_LAPLACIAN3) type = 3;
                    laplacian(image_prev, image_main, amp, type);
                    DisplayImage(image_main, WMAIN);
                    break;
                 case MENU_ZEROCROSS:
                    image_copy(image_main, image_prev);
                    DisplayImage(image_prev, WPREV);
                    zero_cross(image_prev, image_main);
                    DisplayImage(image_main, WMAIN);
                    break;
                 case MENU_THIN:
                    image_copy(image_main, image_prev);
                    DisplayImage(image_prev, WPREV);
                    thinning(image_prev, image_main);
                    DisplayImage(image_main, WMAIN);
                    break;
                  case MENU_HOUGH:
                    if (!GetParams1("閾値", &a)) break;
                    thres = (int)a;
                    if (thres < 0 || thres > 255) break;
                    image_copy(image_main, image_prev);
                    DisplayImage(image_prev, WPREV);
                    hough(image_prev, image_main, image_sub, thres, text_buf);
					ShowText("Houghパラメータ", text_buf);
                    DisplayImage(image_main, WMAIN);
                    DisplayImage(image_sub, WSUB);
                    break;
                  case MENU_HOUGH_CROSS:
					if (!GetParams4("直線の交点", "theta1", "rho1", "theta2", "rho2", 
						&a, &b, &c, &d)) break;
					hough_cross(a, b, c, d, &e, &f);
					sprintf(text_buf, "x = %10.3f, y= %10.3f \n", 
						e + X_SIZE/2, Y_SIZE/2 - f);
					ShowText("交点の座標", text_buf);
					break;
                case MENU_SMOOTH:
                    if (!GetSelection3("範囲", "3x3", "5x5", "7x7", &type)) break;
                    image_copy(image_main, image_prev);
                    DisplayImage(image_prev, WPREV);
                    smooth(image_prev, image_main, type*2+1);
                    DisplayImage(image_main, WMAIN);
                    break;
                 case MENU_SMOOTHWT:
                    if (!GetSelection3("重み係数", "タイプ1", "タイプ2",
						"タイプ3", &type)) break;
                    image_copy(image_main, image_prev);
                    DisplayImage(image_prev, WPREV);
                    smooth_weighted(image_prev, image_main, type-1);
                    DisplayImage(image_main, WMAIN);
                    break;
                 case MENU_MEDIAN:
                    image_copy(image_main, image_prev);
                    DisplayImage(image_prev, WPREV);
                    median(image_prev, image_main);
                    DisplayImage(image_main, WMAIN);
                    break;
                 case MENU_SMOOTHEDGE:
                    image_copy(image_main, image_prev);
                    DisplayImage(image_prev, WPREV);
                    smooth_edge_preserve(image_prev, image_main);
                    DisplayImage(image_main, WMAIN);
                    break;
                 case MENU_DILATION:
                    image_copy(image_main, image_prev);
                    DisplayImage(image_prev, WPREV);
                    dilation(image_prev, image_main);
                    DisplayImage(image_main, WMAIN);
                    break;
                case MENU_EROSION:
                    image_copy(image_main, image_prev);
                    DisplayImage(image_prev, WPREV);
                    erosion(image_prev, image_main);
                    DisplayImage(image_main, WMAIN);
                    break;
                case MENU_LOG:
                    if (!GetParams1("分散", &a)) break;
                    image_copy(image_main, image_prev);
                    DisplayImage(image_prev, WPREV);
                    log_zero_cross(image_prev, image_main, a);
                    DisplayImage(image_main, WMAIN);
                    break;
				case MENU_NOISE_RAND:
                    if (!GetParams1("ノイズレベル", &a)) break;
                    lvl = (int)a;
                    image_copy(image_main, image_prev);
                    DisplayImage(image_prev, WPREV);
					noise_rand(image_prev, image_main, lvl);
                    DisplayImage(image_main, WMAIN);
                    break;
				case MENU_NOISE_SPIKE:
					if (!GetParams2("ノイズ", "数", "レベル", &a, &b)) break;
					num = (int)a;
					lvl = (int)b;
					image_copy(image_main, image_prev);
					DisplayImage(image_prev, WPREV);
					noise_spike(image_prev, image_main, num, lvl);
					DisplayImage(image_main, WMAIN);
                    break;
                case MENU_AMP:
                    if (!GetParams2("ゲイン調整", "倍率", "バイアス", &a, &b))
						break;
                    image_copy(image_main, image_prev);
                    DisplayImage(image_prev, WPREV);
                    amplify(image_prev, image_main, a, b);
                    DisplayImage(image_main, WMAIN);
                    break;
                 case MENU_RANGE:
                    image_copy(image_main, image_prev);
                    DisplayImage(image_prev, WPREV);
                    range(image_prev, &fmax, &fmin);
                    expand(image_prev, image_main, fmax,fmin);
                    DisplayImage(image_main, WMAIN);
                    break;
                 case MENU_PLANE:
                    image_copy(image_main, image_prev);
                    DisplayImage(image_prev, WPREV);
                    histgram(image_prev, hist);
                    plane(image_prev, image_main, hist);
                    DisplayImage(image_main, WMAIN);
                    break;
                 case MENU_DITHER_O:
                    image_copy(image_main, image_prev);
                    DisplayImage(image_prev, WPREV);
                    dither_ordered(image_prev, image_main);
                    DisplayImage(image_main, WMAIN);
                    break;
                 case MENU_DITHER_M:
                    image_copy(image_main, image_prev);
                    DisplayImage(image_prev, WPREV);
                    dither_minimized(image_prev, image_main);
                    DisplayImage(image_main, WMAIN);
                    break;
                 case MENU_DITHER_M_M:
                    if (!GetParams1("階調数", &a)) break;
                    image_copy(image_main, image_prev);
                    DisplayImage(image_prev, WPREV);
                    dither_minimized_multi(image_prev, image_main, (int)a);
                    DisplayImage(image_main, WMAIN);
                    break;
                 case MENU_QUANTIZE:
                    if (!GetParams1("階調数", &a)) break;
                    image_copy(image_main, image_prev);
                    DisplayImage(image_prev, WPREV);
                    quantize(image_prev, image_main, (int)a);
                    DisplayImage(image_main, WMAIN);
                    break;
				case MENU_FEATURE:
                    image_copy(image_main, image_prev);
                    DisplayImage(image_prev, WPREV);
                    cnt = labeling(image_prev, image_sub);
                    DisplayImage(image_sub, WSUB);
                    features(image_sub, image_main, cnt, size, ratio, text_buf);
					ShowText("特徴パラメータ", text_buf);
					DisplayImage(image_main, WMAIN);
                    image_copy(image_sub, image_buf);
                    if (!GetSelection2("物体抽出", "円形度により物体を抽出", 
						"面積により物体を抽出", &mode)) break;
                    if (mode == 1) {
						if (!GetParams2("円形度", "最小", "最大", &ratio_min, 
							&ratio_max)) break;
						if (ratio_min >= ratio_max) break;
						extract_ratio(image_buf, image_main, cnt, ratio, ratio_min, 
							ratio_max);
                    }
                    else if (mode == 2) {
						if (!GetParams2("面積", "最小", "最大", &size_min, 
							&size_max)) break;
						if (size_min >= size_max) break;
						extract_size(image_buf, image_main, cnt, size, size_min, 
							size_max);
					}
                    else break;
                    threshold(image_main, image_main, 1, 1);
                    DisplayImage(image_main, WMAIN);
                    break;
				 case MENU_KEYMASK:
                    image_copy(image_main, image_prev);
                    DisplayImage(image_prev, WPREV);
                    masking(image_prev, image_main, image_key);
                    DisplayImage(image_main, WMAIN);
                    break;
				 case MENU_KEYINV:
                    image_negative(image_key);
                    DisplayImage(image_key, WKEY);
                    break;
                 case MENU_SCALE_NG:
                    if (!GetParams2("拡大率", "横", "縦", &a, &b)) break;
                    image_copy(image_main, image_prev);
                    DisplayImage(image_prev, WPREV);
                    scale_ng(image_prev, image_main, a, b);
                    DisplayImage(image_main, WMAIN);
                    break;
                 case MENU_SCALE_NE:
                    if (!GetParams2("拡大率", "横", "縦", &a, &b)) break;
                    image_copy(image_main, image_prev);
                    DisplayImage(image_prev, WPREV);
                    scale_near(image_prev, image_main, a, b);
                    DisplayImage(image_main, WMAIN);
                    break;
                 case MENU_SCALE:
                    if (!GetParams2("拡大率", "横", "縦", &a, &b)) break;
                    image_copy(image_main, image_prev);
                    DisplayImage(image_prev, WPREV);
                    scale(image_prev, image_main, a, b);
                    DisplayImage(image_main, WMAIN);
                    break;
                 case MENU_SHIFT:
                    if (!GetParams2("移動量", "横", "縦", &x0, &y0)) break;
                    image_copy(image_main, image_prev);
                    DisplayImage(image_prev, WPREV);
                    shift(image_prev, image_main, x0, y0);
                    DisplayImage(image_main, WMAIN);
                    break;
                 case MENU_ROTATE:
                    if (!GetParams1("回転角（度）", &deg)) break;
                    image_copy(image_main, image_prev);
                    DisplayImage(image_prev, WPREV);
                    rotation(image_prev, image_main, deg);
                    DisplayImage(image_main, WMAIN);
                    break;
                 case MENU_SCALEROTSHIFT:
                    if (!GetParams5("パラメータ", "拡大率(横)", "拡大率(縦)",
						"回転角(度)", "中心点(横)", "中心点(縦)",
						&a, &b, &deg, &x0, &y0)) break;
                    image_copy(image_main, image_prev);
                    DisplayImage(image_prev, WPREV);
                    scale_rotate_shift(image_prev, image_main, a, b, deg, x0, y0);
                    DisplayImage(image_main, WMAIN);
                    break;
                 case MENU_AFFINE:
                    if (!GetParams6("変換係数", "a", "b", "c", "d", "e", "f", 
						&a, &b, &c, &d, &e, &f)) break;
                    image_copy(image_main, image_prev);
                    DisplayImage(image_prev, WPREV);
                    affine(image_prev, image_main, a, b, c, d, e, f);
                    DisplayImage(image_main, WMAIN);
                    break;
				case MENU_AFFINECOEF:
					if (!GetParams4("対応点1", "x1", "y1", "X1", "Y1", 
						&a, &b, &c, &d)) break;
					x1 = (int)a;
					y1 = (int)b;
					u1 = (int)c;
					v1 = (int)d;
					if (!GetParams4("対応点2", "x2", "y2", "X2", "Y2", 
						&a, &b, &c, &d)) break;
					x2 = (int)a;
					y2 = (int)b;
					u2 = (int)c;
					v2 = (int)d;
					if (!GetParams4("対応点3", "x3", "y3", "X3", "Y3", 
						&a, &b, &c, &d)) break;
					x3 = (int)a;
					y3 = (int)b;
					u3 = (int)c;
					v3 = (int)d;
					image_copy(image_main, image_prev);
					DisplayImage(image_prev, WPREV);
					affine_coef(x1, y1, u1, v1, x2, y2, u2, v2,
						x3, y3, u3, v3, &a, &b, &c, &d, &e, &f);
					affine(image_prev, image_main, a, b, c, d, e, f);
					DisplayImage(image_main, WMAIN);
					break;
				 case MENU_PERSPECT:
					if (!GetParams4("対応点1", "x1", "y1", "X1", "Y1", 
						&a, &b, &c, &d)) break;
					x1 = (int)a;
					y1 = (int)b;
					u1 = (int)c;
					v1 = (int)d;
					if (!GetParams4("対応点2", "x2", "y2", "X2", "Y2", 
						&a, &b, &c, &d)) break;
					x2 = (int)a;
					y2 = (int)b;
					u2 = (int)c;
					v2 = (int)d;
					if (!GetParams4("対応点3", "x3", "y3", "X3", "Y3", 
						&a, &b, &c, &d)) break;
					x3 = (int)a;
					y3 = (int)b;
					u3 = (int)c;
					v3 = (int)d;
					if (!GetParams4("対応点4", "x4", "y4", "X4", "Y4", 
						&a, &b, &c, &d)) break;
					x4 = (int)a;
					y4 = (int)b;
					u4 = (int)c;
					v4 = (int)d;
                    image_copy(image_main, image_prev);
                    DisplayImage(image_prev, WPREV);
					perspect_coef(x1, y1, u1, v1, x2, y2, u2, v2,
						x3, y3, u3, v3, x4, y4, u4, v4,
						&a, &b, &c, &d, &e, &f, &g, &h);
					perspect(image_prev, image_main, a, b, c, d, e, f, g, h);
                    DisplayImage(image_main, WMAIN);
                    break;
				 case MENU_RADISTORT:
                    if (!GetParams2("歪曲収差", "係数a", "係数b", &a, &b)) break;
					image_copy(image_main, image_prev);
					DisplayImage(image_prev, WPREV);
					radial_distortion(image_prev, image_main, a, b);
					DisplayImage(image_main, WMAIN);
					break;
				 case MENU_LATTICE:
					image_copy(image_main, image_prev);
					DisplayImage(image_prev, WPREV);
					lattice(image_main);
					DisplayImage(image_main, WMAIN);
					break;
				case MENU_FFTIMAGE:
					if (fftimage(image_main, image_sub) != -1)
						DisplayImage(image_sub, WSUB);
					else
						MessageBox(GetFocus(), "Memory allocation error",
							"Error", MB_OK | MB_APPLMODAL);
					break;
				case MENU_FFTFILTER:
					if (!GetParams2("フィルタの範囲", "Low", "High", &a, &b)) 
						break;
                    image_copy(image_main, image_prev);
                    DisplayImage(image_prev, WPREV);
					if (fftfilter(image_prev, image_main, (int)a, (int)b) != -1)
						DisplayImage(image_main, WMAIN);
					else
						MessageBox(GetFocus(), "Memory allocation error",
							"Error", MB_OK | MB_APPLMODAL);
					break;
				case MENU_ENCODE:
					if (dpcm_vlcode(image_main, image_sub) != -1)
						DisplayImage(image_sub, WSUB);
					else
						MessageBox(GetFocus(), "Buffer overflow error",
							"Error", MB_OK | MB_APPLMODAL);
					break;
				case MENU_DECODE:
                    image_copy(image_main, image_prev);
                    DisplayImage(image_prev, WPREV);
					if (idpcm_vlcode(image_sub, image_main) != -1)
						DisplayImage(image_main, WMAIN);
					else
						MessageBox(GetFocus(), "Buffer overflow error",
							"Error", MB_OK | MB_APPLMODAL);
					break;
				case MENU_STEREO_DIFF:
                    image_copy(image_main, image_prev);
                    DisplayImage(image_prev, WPREV);
					if (!GetParams1("ブロックサイズ", &a)) break;
					if (!GetParams1("出力画像の利得", &amp)) break;
					stereo_diff(image_prev, image_sub, image_main, (int)a);
					amplify(image_main, image_main, amp, 0);
					DisplayImage(image_main, WMAIN);
					break;
				case MENU_STEREO_CORRE:
                    image_copy(image_main, image_prev);
                    DisplayImage(image_prev, WPREV);
					if (!GetParams1("ブロックサイズ", &a)) break;
					if (!GetParams1("出力画像の利得", &amp)) break;
					stereo_corre(image_prev, image_sub, image_main, (int)a);
					amplify(image_main, image_main, amp, 0);
					DisplayImage(image_main, WMAIN);
					break;
				case MENU_CSR:
                    if (!GetParams2("カーソル表示位置", "x座標", "y座標",
						&a, &b)) break;
					xd = (int)a;
					yd = (int)b;
					image_copy(image_main, image_prev);
					DisplayImage(image_prev, WPREV);
					for (i = -5; i <= 5; i++) {
						image_main[yd-1][xd+i] = 0;
						image_main[yd  ][xd+i] = 0;
						image_main[yd+1][xd+i] = 0;
						image_main[yd+i][xd-1] = 0;
						image_main[yd+i][xd  ] = 0;
						image_main[yd+i][xd+1] = 0;
					}
					DisplayImage(image_main, WMAIN);
					break;
				case MENU_MOSAIC:
					if (!GetParams4("Main画像に対するSub画像の位置",
						"ずれ量x", "ずれ量y",
						"拡大・縮小補正", "回転補正(度)",
						&a, &b, &c, &d)) break;
                    if (!GetSelection2("重複部分の処理", "上書き",
						"グラデーション", &type)) break;
					image_copy(image_main, image_prev);
					DisplayImage(image_prev, WPREV);
					mosaic(image_prev, image_sub, image_work, a, b, c, d, type-1);
					image_halfsize(image_work, image_main);
					DisplayImage(image_main, WMAIN);
					break;
				case MENU_MOSAICCOEF:
					if (!GetParams4("対応点1", "画像1 x", "画像1 y",
						"画像2 x", "画像2 y", &a, &b, &c, &d)) break;
					x1 = (int)c;
					y1 = (int)d;
					u1 = (int)a;
					v1 = (int)b;
					if (!GetParams4("対応点2", "画像1 x", "画像1 y",
						"画像2 x", "画像2 y", &a, &b, &c, &d)) break;
					x2 = (int)c;
					y2 = (int)d;
					u2 = (int)a;
					v2 = (int)b;
					if (!GetParams4("対応点3", "画像1 x", "画像1 y",
						"画像2 x", "画像2 y", &a, &b, &c, &d)) break;
					x3 = (int)c;
					y3 = (int)d;
					u3 = (int)a;
					v3 = (int)b;
                    if (!GetSelection2("重複部分の処理", "上書き",
						"グラデーション", &type)) break;
					image_copy(image_main, image_prev);
					DisplayImage(image_prev, WPREV);
					affine_coef(x1, y1, u1, v1, x2, y2, u2, v2,
						x3, y3, u3, v3, &a, &b, &c, &d, &e, &f);
					mosaic_affine(image_prev, image_sub, image_work,
						a, b, c, d, e, f, type-1);
					image_halfsize(image_work, image_main);
					DisplayImage(image_main, WMAIN);
					break;
				case MENU_MOSAIC_BLOCK:
                    if (!GetParams8("対応点探索の基準点", "Main画像x",
						"Main画像y", "Sub画像x", "Sub画像y",
						"探索の範囲±x", "探索の範囲±y", 
						"マッチング領域±x", "マッチング領域±y", 
						&a, &b, &c, &d, &e, &f, &g, &h)) break;
					x1 = (int)a;
					y1 = (int)b;
					x2 = (int)c;
					y2 = (int)d;
					xd = (int)e;
					yd = (int)f;
					xb = (int)g;
					yb = (int)h;
					image_copy(image_main, image_prev);
					DisplayImage(image_prev, WPREV);
					mosaic_coef_blockmatch(image_prev, image_sub,
						x1, y1, x2, y2, xd, yd, xb, yb, &mx, &my);
					mosaic(image_prev, image_sub, image_work,
						mx, my, 1, 0, 1);
					image_halfsize(image_work, image_main);
					DisplayImage(image_main, WMAIN);
					break;
			}
			break;
		case WM_PAINT:
            wno = -1;
            for (i = 0; i < 4; i++)
                if (hWnd == imageWin[i].hWnd) wno = i;
            if (wno == -1)
                return DefWindowProc(hWnd, message, wParam, lParam);
            BeginPaint(imageWin[wno].hWnd, &ps);
            hMemDC = CreateCompatibleDC(ps.hdc);
            SelectObject(hMemDC, imageWin[wno].hBitmap);
            BitBlt(ps.hdc, 0, 0, X_SIZE, Y_SIZE, hMemDC, 0, 0, SRCCOPY);
            DeleteDC(hMemDC);
            EndPaint(hWnd, &ps);
            break;
        case WM_DESTROY:
            PostQuitMessage(0);
            break;
        default:
            return DefWindowProc(hWnd, message, wParam, lParam);
    }
    return 0;
}

/*--- InitApplication --- ウィンドウクラスの登録 ------------------------------
	hInstance:	アプリケーションへのハンドル
-----------------------------------------------------------------------------*/
BOOL InitApplication(HANDLE hInstance)
{
    WNDCLASSEX    wc, wcm;

	wc.cbSize = sizeof(wc);
    wc.style = CS_HREDRAW | CS_VREDRAW;
    wc.lpfnWndProc = (WNDPROC)MainWndProc;
    wc.cbClsExtra = 0;
    wc.cbWndExtra = 0;
    wc.hInstance = hInstance;
    wc.hIcon = LoadIcon(hInstance, IDI_APPLICATION);
    wc.hCursor = LoadCursor(NULL, IDC_ARROW);
    wc.hbrBackground = (HBRUSH)GetStockObject(WHITE_BRUSH);
    wc.lpszMenuName = "MainMenu";
    wc.lpszClassName = "MainClass";
    wc.hIconSm = LoadIcon(NULL, IDI_APPLICATION);
    if (!RegisterClassEx(&wc)) return FALSE;
	wcm.cbSize = sizeof(wcm);
    wcm.style = CS_HREDRAW | CS_VREDRAW;
    wcm.lpfnWndProc = (WNDPROC)MainWndProc;
    wcm.cbClsExtra = 0;
    wcm.cbWndExtra = 0;
    wcm.hInstance = hInstance;
    wcm.hIcon = LoadIcon(hInstance, IDI_APPLICATION);
    wcm.hCursor = LoadCursor(NULL, IDC_ARROW);
    wcm.hbrBackground = (HBRUSH)GetStockObject(WHITE_BRUSH);
    wcm.lpszMenuName = "";
    wcm.lpszClassName = "ImageClass";
    wcm.hIconSm = LoadIcon(NULL, IDI_APPLICATION);
    return RegisterClassEx(&wcm);
}

/*--- InitInstance --- ウィンドウの作成 ---------------------------------------
	hInstance:	アプリケーションへのハンドル
	nCmdShow:	起動時の表示方法
-----------------------------------------------------------------------------*/
BOOL InitInstance(HANDLE hInstance, int nCmdShow)
{
    HWND    hWnd;
    HDC     hDC;
    RECT    rect;
    int     i;

    hGWnd = hWnd = CreateWindow("MainClass", "Image Processing",
					WS_OVERLAPPEDWINDOW, MENU_POSX, MENU_POSY, 
					MENU_SIZEX, MENU_SIZEY, NULL, NULL, hInstance, NULL);
    if (hWnd == NULL) return FALSE;
    ShowWindow(hWnd, nCmdShow);
    UpdateWindow(hWnd);
	hSubMenu = LoadMenu (ghInst, "SubMenu");
	hSubMenu = GetSubMenu(hSubMenu, 0);
    hDC = GetDC(hWnd);
    bitspixel = GetDeviceCaps (hDC, BITSPIXEL);
    if (bitspixel != 24 && bitspixel != 32) {
        MessageBox(GetFocus(),
			"表示をフルカラー（２４ビットか３２ビット）にしてください。",
			"エラー", MB_ICONHAND | MB_APPLMODAL);
        return FALSE;
    }
    for (i = 0; i < 4; i++){
        imageWin[i].hBitmap = CreateCompatibleBitmap(hDC, X_SIZE, Y_SIZE);
    }
    rect.top=0;
    rect.left = 0;
    rect.bottom = rect.top+Y_SIZE;
    rect.right = rect.left+X_SIZE;
    AdjustWindowRect(&rect, WS_OVERLAPPEDWINDOW, FALSE);
    imageWin[WMAIN].hWnd = CreateWindow("ImageClass", wTitle[WMAIN],
		WS_OVERLAPPED, WMAIN_POSX, WMAIN_POSY, 
		rect.right-rect.left, rect.bottom-rect.top, NULL, NULL, hInstance, 
		NULL);
    if (imageWin[WMAIN].hWnd == NULL) return FALSE;
    ShowWindow(imageWin[WMAIN].hWnd, nCmdShow);
    DisplayImage(image_main, WMAIN);
	imageWin[WPREV].hWnd = CreateWindow("ImageClass", wTitle[WPREV],
		WS_OVERLAPPED, WPREV_POSX, WPREV_POSY, 
		rect.right-rect.left, rect.bottom-rect.top, NULL, NULL, hInstance, 
		NULL);
    if (imageWin[WPREV].hWnd == NULL) return FALSE;
    ShowWindow(imageWin[WPREV].hWnd, nCmdShow);
    DisplayImage(image_prev, WPREV);
	imageWin[WSUB].hWnd = CreateWindow("ImageClass", wTitle[WSUB],
		WS_OVERLAPPED, WSUB_POSX, WSUB_POSY, 
		rect.right-rect.left, rect.bottom-rect.top, NULL, NULL, hInstance, 
		NULL);
    if (imageWin[WSUB].hWnd == NULL) return FALSE;
    ShowWindow(imageWin[WSUB].hWnd, nCmdShow);
    DisplayImage(image_sub, WSUB);
	imageWin[WKEY].hWnd = CreateWindow("ImageClass", wTitle[WKEY],
		WS_OVERLAPPED, WKEY_POSX, WKEY_POSY, 
		rect.right-rect.left, rect.bottom-rect.top, NULL, NULL, hInstance, 
		NULL);
    if (imageWin[WKEY].hWnd == NULL) return FALSE;
    ShowWindow(imageWin[WKEY].hWnd, nCmdShow);
    DisplayImage(image_key, WKEY);
    /* OPENFILENAME構造体のメンバを設定 */
    ofn.lStructSize         = sizeof(OPENFILENAME);
    ofn.hwndOwner           = hWnd;
    ofn.lpstrFilter         = szFilterSpec;
    ofn.lpstrCustomFilter   = NULL;
    ofn.nMaxCustFilter      = 0;
    ofn.nFilterIndex        = 1;
    ofn.lpstrFile           = szFileName;
    ofn.nMaxFile            = MAX_NAME_LENGTH;
    ofn.lpstrInitialDir     = NULL;
    ofn.lpstrFileTitle      = szFileTitle;
    ofn.nMaxFileTitle       = MAX_NAME_LENGTH;
    ofn.lpstrTitle          = NULL;
    ofn.lpstrDefExt         = NULL;
    ofn.Flags               = 0;
	rand();
    return TRUE;
}
