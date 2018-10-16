#include	<stdio.h>
#include	<stdlib.h>
#include	<windows.h>
#include	<string.h>
#include	<math.h>
#include	"WinParams.h"
#include	"WinProto.h"
#include	"WinCImage.h"

LRESULT CALLBACK MainWndProc(HWND, UINT, WPARAM, LPARAM);
BOOL InitApplication(HANDLE);
BOOL InitInstance(HANDLE, int);

char *wTitle[4] = {"MAIN", "PREV", "SUB", "KEY"};  /* �E�B���h�E�̃^�C�g��	  */
unsigned char image_main[3][Y_SIZE][X_SIZE];	   /* ���C���摜�z��		  */
unsigned char image_prev[3][Y_SIZE][X_SIZE];	   /* �ۑ��摜�z��			  */
unsigned char image_sub[3][Y_SIZE][X_SIZE];		   /* �T�u�摜�z��			  */
unsigned char image_key[3][Y_SIZE][X_SIZE];		   /* �L�[�摜�z��			  */
unsigned char image_buf[3][Y_SIZE][X_SIZE];		   /* �o�b�t�@�z��			  */
unsigned char image_work[3][Y_SIZE*2][X_SIZE*2];   /* ���U�C�N�p�摜�z��	  */
unsigned char image_prt1[3][P_Y_SIZE][P_X_SIZE];   /* ����p�摜�z��		  */
unsigned char image_prt2[3][P_Y_SIZE][P_X_SIZE];   /* ����p�摜�z��		  */
int           image_ysh[3][Y_SIZE][X_SIZE];		   /* YSH�摜�z��@		      */
int        	  bitspixel;						   /* ���f������̃r�b�g��  */
long          hist[3][256];						   /* �q�X�g�O�����z�� 		  */
long		  histbuf[256];						   /* �q�X�g�O�����z�� 		  */
double        ratio[128], size[128];			   /* �����p�����[�^�z��      */
char		  text_buf[TEXT_BUF_LEN];			   /* ����p�����z��		  */

HANDLE  	  ghInst;							   /* �C���X�^���X�n���h��	  */
HWND    	  hGWnd;							   /* ���C���E�B���h�E		  */
HBITMAP 	  hBitmap;							   /* �r�b�g�}�b�v			  */
HMENU		  hSubMenu;							   /* �T�u���j���[			  */
ImageWin      imageWin[4];						   /* �摜�p�E�B���h�E		  */

OPENFILENAME ofn;
char szFilterSpecBmp[128] = "BMP�摜̧�� (*.bmp)\0*.bmp\0";
char szFilterSpec [128] = "�摜̧�� (*.*)\0*.*\0";
char szFileName[MAX_NAME_LENGTH];				   /* �t�@�C�����i�p�X�t���j */
char szFileTitle[MAX_NAME_LENGTH];				   /* �t�@�C�����i�p�X�����j */

/*--- WinMain --- ���C�� ------------------------------------------------------
	hInstance:		�A�v���P�[�V�����ւ̃n���h��
	hPrevInstance:	���O�̃C���X�^���X�n���h��
	lpCmdLine:		�R�}���h���C���̕�����
	nCmdShow:		�N�����̕\�����@
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

/*--- MainWndProc --- ���b�Z�[�W�̏��� ----------------------------------------
	hWnd:		�E�B���h�E����
	message:	���b�Z�[�W
	wParam:		���[�h�p�����[�^
	lParam:		�����O�p�����[�^
-----------------------------------------------------------------------------*/
LRESULT CALLBACK MainWndProc(HWND hWnd, UINT message, WPARAM wParam, 
	LPARAM lParam)
{
    HDC 			hMemDC;
    PAINTSTRUCT		ps;
    HDC				hDC;
	HBITMAP			hBitmapd;
	BITMAP			bmp;
	HDC				hDCs, hDCd;
    POINT   		point;
    int 			wno, i, j, k, mx, my, lvl;
    int 			thres, type, org;
    double   		amp;
	int				num;
	int				smt;
    int 			thres_rmin, thres_rmax;
    int 			thres_gmin, thres_gmax;
    int 			thres_bmin, thres_bmax;
    int 			thres_ymin, thres_ymax;
    int 			thres_smin, thres_smax;
    int 			thres_hmin, thres_hmax;
    double   		ya, yb, sa, sb, hb;
    double   		a, b, c, d, e, f, g, h, x0, y0, deg;
	int				x1, y1, x2, y2, x3, y3, x4, y4, xd, yd, xm, ym;
	int				u1, v1, u2, v2, u3, v3, u4, v4;
	double			gain[3], bias[3];
	char			filename[128];
	unsigned char	*rr, *gg, *bb, *p;
	double			yy, cb, cr, ss, hh;

	switch (message) {
		case WM_RBUTTONDOWN:
			if (hWnd != imageWin[WMAIN].hWnd && hWnd != imageWin[WPREV].hWnd
				&& hWnd != imageWin[WSUB].hWnd
				&& hWnd != imageWin[WKEY].hWnd) break;
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
			rr = &image_main[0][0][0] + X_SIZE*point.y + point.x;
			gg = rr + X_SIZE*Y_SIZE;
			bb = gg + X_SIZE*Y_SIZE;
			yy = 0.2126 * (*rr) + 0.7152 * (*gg) + 0.0722 * (*bb);
			cb = (*bb - yy) / 1.8556;
			cr = (*rr - yy) / 1.5748;
			ss = sqrt(cb*cb + cr*cr);
			hh = atan2(cr, cb) * 180.0 / 3.141592;
			sprintf(text_buf, 
				"R:%3d, G:%3d, B:%3d, Y:%3.0f, S:%3.0f, H:%3.0f on (%3d, %3d) ",
				*rr, *gg, *bb, yy, ss, hh, point.x, point.y);
			TextOut(hDC, 0, 0, text_buf, strlen(text_buf));
			break;
		case WM_COMMAND:
			switch(wParam) {
				case MENU_READ_BMP:
					ofn.lpstrFilter = szFilterSpecBmp;
					if (!GetOpenFileName ((LPOPENFILENAME)&ofn)) return FALSE;
					num = check_bmp_file(ofn.lpstrFileTitle);
					if (num == 24) {
						if (read_bmp_color(image_main, ofn.lpstrFileTitle)
							!= -1) DisplayCImage(image_main, WMAIN);
						else
							MessageBox(GetFocus(), "File open error2", "Error", 
								MB_OK | MB_APPLMODAL);
					}
					else if (num == 8) {
						if (read_bmp_mono(image_main[0], ofn.lpstrFileTitle) != -1) {
							image_copy(image_main[0], image_main[1]);
							image_copy(image_main[0], image_main[2]);
							DisplayCImage(image_main, WMAIN);
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
					if (write_bmp_color(image_main, filename) 
						== -1) MessageBox(GetFocus(), "File open error", 
								"Error", MB_OK | MB_APPLMODAL);
            	    break;
				case MENU_READ_RGBPLANE:
					ofn.lpstrFilter = szFilterSpec;
					if (!GetOpenFileName ((LPOPENFILENAME)&ofn)) return FALSE;
						if (read_rgb_plane(image_main, ofn.lpstrFileTitle) 
							!= -1) DisplayCImage(image_main, WMAIN);
						else
							MessageBox(GetFocus(), "File open error", "Error", 
								MB_OK | MB_APPLMODAL);
					break;
				case MENU_READ_RGBLINE:
					ofn.lpstrFilter = szFilterSpec;
					if (!GetOpenFileName ((LPOPENFILENAME)&ofn)) return FALSE;
						if (read_rgb_line(image_main, ofn.lpstrFileTitle) 
							!= -1) DisplayCImage(image_main, WMAIN);
						else
							MessageBox(GetFocus(), "File open error", "Error", 
								MB_OK | MB_APPLMODAL);
					break;
				case MENU_READ_RGBPIXEL:
					ofn.lpstrFilter = szFilterSpec;
					if (!GetOpenFileName ((LPOPENFILENAME)&ofn)) return FALSE;
						if (read_rgb_pixel(image_main, ofn.lpstrFileTitle) 
							!= -1) DisplayCImage(image_main, WMAIN);
						else
							MessageBox(GetFocus(), "File open error", "Error", 
								MB_OK | MB_APPLMODAL);
					break;
				case MENU_WRITE_RGBPLANE:
					ofn.lpstrFilter = szFilterSpec;
					if (!GetSaveFileName ((LPOPENFILENAME)&ofn)) return FALSE;
						if (write_rgb_plane(image_main, ofn.lpstrFileTitle) 
							== -1) MessageBox(GetFocus(), "File open error", 
									"Error", MB_OK | MB_APPLMODAL);
            	    break;
				case MENU_WRITE_RGBLINE:
					ofn.lpstrFilter = szFilterSpec;
					if (!GetSaveFileName ((LPOPENFILENAME)&ofn)) return FALSE;
						if (write_rgb_line(image_main, ofn.lpstrFileTitle) 
							== -1) MessageBox(GetFocus(), "File open error", 
									"Error", MB_OK | MB_APPLMODAL);
            	    break;
				case MENU_WRITE_RGBPIXEL:
					ofn.lpstrFilter = szFilterSpec;
					if (!GetSaveFileName ((LPOPENFILENAME)&ofn)) return FALSE;
						if (write_rgb_pixel(image_main, ofn.lpstrFileTitle) 
							== -1) MessageBox(GetFocus(), "File open error", 
									"Error", MB_OK | MB_APPLMODAL);
            	    break;
                case MENU_READ_R:
					ofn.lpstrFilter = szFilterSpec;
                    if (!GetOpenFileName ((LPOPENFILENAME)&ofn)) return FALSE;
                    if (read_mono(image_main[0], ofn.lpstrFileTitle) != -1)
                    	DisplayCImage(image_main, WMAIN);
                    else
                    	MessageBox(GetFocus(), "File open error", "Error", 
                    		MB_OK | MB_APPLMODAL);
                    break;
                case MENU_READ_G:
					ofn.lpstrFilter = szFilterSpec;
                    if (!GetOpenFileName ((LPOPENFILENAME)&ofn)) return FALSE;
                    if (read_mono(image_main[1], ofn.lpstrFileTitle) != -1)
                    	DisplayCImage(image_main, WMAIN);
                    else
                    	MessageBox(GetFocus(), "File open error", "Error", 
                    		MB_OK | MB_APPLMODAL);
                    break;
                case MENU_READ_B:
					ofn.lpstrFilter = szFilterSpec;
                    if (!GetOpenFileName ((LPOPENFILENAME)&ofn)) return FALSE;
                    if (read_mono(image_main[2], ofn.lpstrFileTitle) != -1)
                    	DisplayCImage(image_main, WMAIN);
                    else
                    	MessageBox(GetFocus(), "File open error", "Error", 
                    		MB_OK | MB_APPLMODAL);
                    break;
				case MENU_WRITE_R:
					ofn.lpstrFilter = szFilterSpec;
					if (!GetSaveFileName ((LPOPENFILENAME)&ofn)) return FALSE;
						if (write_mono(image_main[0], ofn.lpstrFileTitle) 
							== -1) MessageBox(GetFocus(), "File open error", 
									"Error", MB_OK | MB_APPLMODAL);
            	    break;
				case MENU_WRITE_G:
					ofn.lpstrFilter = szFilterSpec;
					if (!GetSaveFileName ((LPOPENFILENAME)&ofn)) return FALSE;
						if (write_mono(image_main[1], ofn.lpstrFileTitle) 
							== -1) MessageBox(GetFocus(), "File open error", 
									"Error", MB_OK | MB_APPLMODAL);
            	    break;
				case MENU_WRITE_B:
					ofn.lpstrFilter = szFilterSpec;
					if (!GetSaveFileName ((LPOPENFILENAME)&ofn)) return FALSE;
						if (write_mono(image_main[2], ofn.lpstrFileTitle) 
							== -1) MessageBox(GetFocus(), "File open error", 
									"Error", MB_OK | MB_APPLMODAL);
            	    break;
				case MENU_WRITE_RGBAPART:
					ofn.lpstrFilter = szFilterSpec;
					if (!GetSaveFileName ((LPOPENFILENAME)&ofn)) return FALSE;
						if (write_rgb_apart(image_main, ofn.lpstrFileTitle) 
							== -1) MessageBox(GetFocus(), "File open error", 
									"Error", MB_OK | MB_APPLMODAL);
            	    break;
				case MENU_PRINT:
					mx = P_X_SIZE/X_SIZE; my = P_Y_SIZE/Y_SIZE;
					for (i = 0; i < 3; i++) {
						for (j = 0; j < P_Y_SIZE; j++)
							for (k = 0; k < P_X_SIZE; k++)
								image_prt1[i][j][k] = image_main[i][j/my][k/mx];
						m_dither(image_prt1[i], image_prt2[i]);
					}
					if (ImgPrint(image_prt2[0], image_prt2[1], image_prt2[2])
						== -1) MessageBox(NULL, "Print error", "Error", 
								MB_OK | MB_APPLMODAL);
					break;
				case MENU_QUIT:
					DestroyWindow(hWnd);
					break;
				case MENU_CLEAR:
					if (hWnd == imageWin[WMAIN].hWnd){
						image_clear_color(image_main);
						DisplayCImage(image_main, WMAIN);
					}
					else if (hWnd == imageWin[WPREV].hWnd){
						image_clear_color(image_prev);
						DisplayCImage(image_prev, WPREV);
					}
					else if (hWnd == imageWin[WSUB].hWnd){
						image_clear_color(image_sub);
						DisplayCImage(image_sub, WSUB);
					}
					else if (hWnd == imageWin[WKEY].hWnd){
						image_clear_color(image_key);
						DisplayCImage(image_key, WKEY);
					}
					break;
				case MENU_COPY:
					hDC = GetDC(hWnd);
					hBitmap = CreateCompatibleBitmap(hDC, X_SIZE, Y_SIZE);
					if (hWnd == imageWin[WMAIN].hWnd)
						RgbToBitmap(image_main[0], image_main[1], 
							image_main[2], hBitmap);
					else if (hWnd == imageWin[WPREV].hWnd)
						RgbToBitmap(image_prev[0], image_prev[1], 
							image_prev[2], hBitmap);
					else if (hWnd == imageWin[WSUB].hWnd)
						RgbToBitmap(image_sub[0], image_sub[1], image_sub[2],
							hBitmap);
					else if (hWnd == imageWin[WKEY].hWnd)
						RgbToBitmap(image_key[0], image_key[1], image_key[2],
							hBitmap);
					else
						break;
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
					BitmapToRgb(hBitmapd, image_buf[0], image_buf[1],
						image_buf[2]);
					CloseClipboard();
					if (hWnd == imageWin[WMAIN].hWnd){
						image_copy_color(image_buf, image_main);
						DisplayCImage(image_main, WMAIN);
					}
					else if (hWnd == imageWin[WPREV].hWnd){
						image_copy_color(image_buf, image_prev);
						DisplayCImage(image_prev, WPREV);
					}
					else if (hWnd == imageWin[WSUB].hWnd){
						image_copy_color(image_buf, image_sub);
						DisplayCImage(image_sub, WSUB);
					}
					else if (hWnd == imageWin[WKEY].hWnd){
						image_copy_color(image_buf, image_key);
						DisplayCImage(image_key, WKEY);
					}
					break;
				case MENU_READ_BMP_SUB:
					ofn.lpstrFilter = szFilterSpecBmp;
					if (!GetOpenFileName ((LPOPENFILENAME)&ofn)) return FALSE;
					num = check_bmp_file(ofn.lpstrFileTitle);
					if (num == 24) {
						if (read_bmp_color(image_buf, ofn.lpstrFileTitle) == -1)
							MessageBox(GetFocus(), "File open error2", "Error", 
								MB_OK | MB_APPLMODAL);
					}
					else if (num == 8) {
						if (read_bmp_mono(image_buf[0], ofn.lpstrFileTitle) != -1) {
							image_copy(image_buf[0], image_buf[1]);
							image_copy(image_buf[0], image_buf[2]);
						}
						else
							MessageBox(GetFocus(), "File open error", "Error", 
								MB_OK | MB_APPLMODAL);
					}
					else
							MessageBox(GetFocus(), "File open error", "Error", 
								MB_OK | MB_APPLMODAL);
					if (hWnd == imageWin[WMAIN].hWnd) {
						image_copy_color(image_buf, image_main);
						DisplayCImage(image_main, WMAIN);
					}
					else if (hWnd == imageWin[WPREV].hWnd){
						image_copy_color(image_buf, image_prev);
						DisplayCImage(image_prev, WPREV);
					}
					else if (hWnd == imageWin[WSUB].hWnd){
						image_copy_color(image_buf, image_sub);
						DisplayCImage(image_sub, WSUB);
					}
					else if (hWnd == imageWin[WKEY].hWnd){
						image_copy_color(image_buf, image_key);
						DisplayCImage(image_key, WKEY);
					}
					break;
				case MENU_WRITE_BMP_SUB:
					ofn.lpstrFilter = szFilterSpecBmp;
					if (!GetSaveFileName ((LPOPENFILENAME)&ofn)) return FALSE;
					sprintf(filename, "%s.bmp", ofn.lpstrFileTitle);
					if (hWnd == imageWin[WMAIN].hWnd) {
						if (write_bmp_color(image_main, filename) == -1)
							MessageBox(GetFocus(), "File open error", 
									"Error", MB_OK | MB_APPLMODAL);
					}
					else if (hWnd == imageWin[WPREV].hWnd){
						if (write_bmp_color(image_prev, filename) == -1)
							MessageBox(GetFocus(), "File open error", 
									"Error", MB_OK | MB_APPLMODAL);
					}
					else if (hWnd == imageWin[WSUB].hWnd){
						if (write_bmp_color(image_sub, filename) == -1) 
							MessageBox(GetFocus(), "File open error", 
									"Error", MB_OK | MB_APPLMODAL);
					}
					else if (hWnd == imageWin[WKEY].hWnd){
						if (write_bmp_color(image_key, filename) == -1)
							MessageBox(GetFocus(), "File open error", 
									"Error", MB_OK | MB_APPLMODAL);
					}
            	    break;
				case MENU_UNDO:
					image_copy_color(image_prev, image_main);
					DisplayCImage(image_main, WMAIN);
					break;
				case MENU_CLEAR_MAIN:
					image_clear_color(image_main);
					DisplayCImage(image_main, WMAIN);
					break;
				case MENU_CLEAR_SUB:
					image_clear_color(image_sub);
					DisplayCImage(image_sub, WSUB);
					break;
				case MENU_CLEAR_KEY:
					image_clear_color(image_key);
					DisplayCImage(image_key, WKEY);
					break;
				case MENU_COPY_CM:
					OpenClipboard(NULL);
					hBitmap = (HBITMAP)GetClipboardData(CF_BITMAP);
					BitmapToRgb(hBitmap, image_buf[0], image_buf[1],
						image_buf[2]);
					CloseClipboard();
					image_copy_color(image_buf, image_main);
					DisplayCImage(image_main, WMAIN);
					break;
				case MENU_COPY_MC:
					hDC = GetDC(hWnd);
					hBitmap = CreateCompatibleBitmap(hDC, X_SIZE, Y_SIZE);
					RgbToBitmap(image_main[0], image_main[1], image_main[2],
						hBitmap);
					OpenClipboard(NULL);
					EmptyClipboard();
					SetClipboardData(CF_BITMAP, hBitmap);
					CloseClipboard();
					break;
				case MENU_COPY_MS:
					image_copy_color(image_main, image_sub);
					DisplayCImage(image_sub, WSUB);
					break;
				case MENU_COPY_SM:
					image_copy_color(image_sub, image_main);
					DisplayCImage(image_main, WMAIN);
					break;
				case MENU_COPY_MK:
					image_copy_color(image_main, image_key);
					DisplayCImage(image_key, WKEY);
					break;
				case MENU_COPY_KM:
					image_copy_color(image_key, image_main);
					DisplayCImage(image_main, WMAIN);
					break;
                case MENU_NEGATIVE:
                    image_copy_color(image_main, image_prev);
                    DisplayCImage(image_prev, WPREV);
                    image_negative_color(image_main);
                    DisplayCImage(image_main, WMAIN);
                    break;
                case MENU_CTOM:
                    image_copy_color(image_main, image_prev);
                    DisplayCImage(image_prev, WPREV);
                    color_to_mono(image_main);
                    DisplayCImage(image_main, WMAIN);
                    break;
				case MENU_CLEAR_R:
                    image_copy_color(image_main, image_prev);
                    DisplayCImage(image_prev, WPREV);
                    image_clear(image_main[0]);
                    DisplayCImage(image_main, WMAIN);
                    break;
				case MENU_CLEAR_G:
                    image_copy_color(image_main, image_prev);
                    DisplayCImage(image_prev, WPREV);
                    image_clear(image_main[1]);
                    DisplayCImage(image_main, WMAIN);
                    break;
				case MENU_CLEAR_B:
                    image_copy_color(image_main, image_prev);
                    DisplayCImage(image_prev, WPREV);
                    image_clear(image_main[2]);
                    DisplayCImage(image_main, WMAIN);
                    break;
				case MENU_COPY_R:
                    image_copy_color(image_main, image_prev);
                    DisplayCImage(image_prev, WPREV);
                    image_copy(image_main[0], image_main[1]);
                    image_copy(image_main[0], image_main[2]);
                    DisplayCImage(image_main, WMAIN);
                    break;
				case MENU_COPY_G:
                    image_copy_color(image_main, image_prev);
                    DisplayCImage(image_prev, WPREV);
                    image_copy(image_main[1], image_main[0]);
                    image_copy(image_main[1], image_main[2]);
                    DisplayCImage(image_main, WMAIN);
                    break;
				case MENU_COPY_B:
                    image_copy_color(image_main, image_prev);
                    DisplayCImage(image_prev, WPREV);
                    image_copy(image_main[2], image_main[0]);
                    image_copy(image_main[2], image_main[1]);
                    DisplayCImage(image_main, WMAIN);
                    break;
				case MENU_ADD_RGB:
                    image_copy_color(image_main, image_prev);
                    DisplayCImage(image_prev, WPREV);
                    image_addition_rgb(image_main);
                    DisplayCImage(image_main, WMAIN);
                    break;
				case MENU_OR_RGB:
                    image_copy_color(image_main, image_prev);
                    DisplayCImage(image_prev, WPREV);
                    image_or_rgb(image_main);
                    DisplayCImage(image_main, WMAIN);
                    break;
				case MENU_AND_RGB:
                    image_copy_color(image_main, image_prev);
                    DisplayCImage(image_prev, WPREV);
                    image_and_rgb(image_main);
                    DisplayCImage(image_main, WMAIN);
                    break;
                case MENU_ADD:
                    image_copy_color(image_main, image_prev);
                    DisplayCImage(image_prev, WPREV);
                    image_addition_color(image_prev, image_sub, image_main);
                    DisplayCImage(image_main, WMAIN);
                    break;
                case MENU_SUB:
                    image_copy_color(image_main, image_prev);
                    DisplayCImage(image_prev, WPREV);
                    image_subtraction_color(image_prev, image_sub, image_main);
                    DisplayCImage(image_main, WMAIN);
                    break;
                case MENU_DIF:
                    image_copy_color(image_main, image_prev);
                    DisplayCImage(image_prev, WPREV);
                    image_difference_color(image_prev, image_sub, image_main);
                    DisplayCImage(image_main, WMAIN);
                    break;
                case MENU_MPY:
                    image_copy_color(image_main, image_prev);
                    DisplayCImage(image_prev, WPREV);
                    image_multiplication_color(image_prev, image_sub, image_main);
                    DisplayCImage(image_main, WMAIN);
                    break;
                case MENU_OR:
                    image_copy_color(image_main, image_prev);
                    DisplayCImage(image_prev, WPREV);
                    image_or_color(image_prev, image_sub, image_main);
                    DisplayCImage(image_main, WMAIN);
                    break;
                case MENU_AND:
                    image_copy_color(image_main, image_prev);
                    DisplayCImage(image_prev, WPREV);
                    image_and_color(image_prev, image_sub, image_main);
                    DisplayCImage(image_main, WMAIN);
                    break;
				case MENU_HSV_GEN:
					if (!GetParams1("��ƂȂ�F��(�x)", &a)) break;
					org = (int)a;
					image_copy_color(image_main, image_prev);
					DisplayCImage(image_prev, WPREV);
					rgb_to_ysh(image_prev, image_ysh);
					y_image(image_ysh[0], image_main[0]);
					sat_image(image_ysh[1], image_sub[0]);
					hue_image(image_ysh[1], image_ysh[2], image_key[0], org);
					image_copy(image_main[0], image_main[1]);
					image_copy(image_main[0], image_main[2]);
					DisplayCImage(image_main, WMAIN);
					image_copy(image_sub[0], image_sub[1]);
					image_copy(image_sub[0], image_sub[2]);
					DisplayCImage(image_sub, WSUB);
					image_copy(image_key[0], image_key[1]);
					image_copy(image_key[0], image_key[2]);
					DisplayCImage(image_key, WKEY);
					break;
				case MENU_RGB_CHANGE:
                    image_copy_color(image_main, image_prev);
                    if (!GetParams6("�q�f�a�F�ύX",
						"�q�Q�C��",	"�q�o�C�A�X", "�f�Q�C��",
						"�f�o�C�A�X","�a�Q�C��", "�a�o�C�A�X", 
						&gain[0], &bias[0], &gain[1], &bias[1],
						&gain[2], &bias[2])) break;
                    amplify(image_prev[0], image_main[0], gain[0], bias[0]);
                    amplify(image_prev[1], image_main[1], gain[1], bias[1]);
                    amplify(image_prev[2], image_main[2], gain[2], bias[2]);
                    DisplayCImage(image_prev, WPREV);
                    DisplayCImage(image_main, WMAIN);
					break;
				case MENU_YSH_CHANGE:
					if (!GetParams5("�F�ύX", "�P�x�Q�C��", "�P�x�o�C�A�X",
						"�ʓx�Q�C��", "�ʓx�o�C�A�X", "�F���o�C�A�X",
						&ya, &yb, &sa, &sb, &hb)) break;
					image_copy_color(image_main, image_prev);
					DisplayCImage(image_prev, WPREV);
					rgb_to_ysh(image_prev, image_ysh);
					tran_ysh(image_ysh,image_ysh, ya, yb, sa, sb, hb);
					ysh_to_rgb(image_ysh, image_main);
					DisplayCImage(image_main, WMAIN);
					break;
				case MENU_RGB_AUTO:
					if (!GetSelection2("�ύX����", "RGB�Ɨ�", "RGB�A��", &type)) 
						break;
					image_copy_color(image_main, image_prev);
					DisplayCImage(image_prev, WPREV);
					expand_rgb(image_prev, image_main, type-1);
					DisplayCImage(image_main, WMAIN);
                    break;
				case MENU_YSH_AUTO:
                    if (!GetSelection2("�ύX����", "�P�x�̂�", "�P�x�E�ʓx", &type)) 
						break;
					image_copy_color(image_main, image_prev);
					DisplayCImage(image_prev, WPREV);
					rgb_to_ysh(image_prev, image_ysh);
					expand_ysh(image_ysh, image_ysh, type-1);
					ysh_to_rgb(image_ysh, image_main);
					DisplayCImage(image_main, WMAIN);
					break;
				case MENU_PSEUDOCOLOR:
					if (!GetSelection2("�ύX����", "����a", "����b", &type)) 
						break;
					image_copy_color(image_main, image_prev);
					DisplayCImage(image_prev, WPREV);
					pseudo_color(image_prev[0], image_main[0], image_main[1],
						image_main[2], type);
					DisplayCImage(image_main, WMAIN);
					break;
				case MENU_COLORBAR:
					image_copy_color(image_main, image_prev);
					DisplayCImage(image_prev, WPREV);
					colorbar(image_main, 255);
					DisplayCImage(image_main, WMAIN);
					break;
				case MENU_RHIST:
					histgram(image_main[0], hist[0]);
					histimage(hist[0], image_sub[0]);
					image_clear(image_sub[1]);
					image_clear(image_sub[2]);
	                DisplayCImage(image_sub, WSUB);
					break;
				case MENU_RHISTPRIN:
					histprint(hist[0], text_buf);
					ShowText("�q�X�g�O����", text_buf);
                    break;
				case MENU_RHIST_SMOTH:
					histsmooth(hist[0], histbuf);
					for (i = 0; i < 256; i++) hist[0][i] = histbuf[i];
					histimage(hist[0], image_sub[0]);
					image_clear(image_sub[1]);
					image_clear(image_sub[2]);
					DisplayCImage(image_sub, WSUB);
                    break;
				case MENU_RTHRESH_MAN:
                    if (!GetParams1("臒l", &a)) break;
                    thres = (int)a;
                    if (thres < 0 || thres > 255) break;
                    if (!GetSelection2("�\���摜", "���̂܂�", "���]", &type)) 
						break;
                    image_copy_color(image_main, image_prev);
                    DisplayCImage(image_prev, WPREV);
                    threshold(image_prev[0], image_main[0], thres, type);
					image_copy(image_main[0], image_main[1]);
					image_copy(image_main[0], image_main[2]);
                    DisplayCImage(image_main, WMAIN);
					break;
				case MENU_RTHRESH_MODE:
                    if (!GetParams1("��������", &a)) break;
                    smt = (int)a;
                    if (!GetSelection2("�\���摜", "���̂܂�", "���]", &type))
						break;
                    image_copy_color(image_main, image_prev);
                    DisplayCImage(image_prev, WPREV);
                    threshold_mode(image_prev[0], image_main[0], smt, type);
					image_copy(image_main[0], image_main[1]);
					image_copy(image_main[0], image_main[2]);
                    DisplayCImage(image_main, WMAIN);
					break;
				case MENU_RTHRESH_DISCRIM:
                    if (!GetSelection2("�\���摜", "���̂܂�", "���]", &type))
						break;
                    image_copy_color(image_main, image_prev);
                    DisplayCImage(image_prev, WPREV);
                    threshold_discrim(image_prev[0], image_main[0], type);
					image_copy(image_main[0], image_main[1]);
					image_copy(image_main[0], image_main[2]);
                    DisplayCImage(image_main, WMAIN);
					break;
				case MENU_RTHRESH_DYNAMIC:
                    if (!GetSelection2("�\���摜", "���̂܂�", "���]", &type))
						break;
                    image_copy_color(image_main, image_prev);
                    DisplayCImage(image_prev, WPREV);
                    threshold_dynamic(image_prev[0], image_main[0], type);
					image_copy(image_main[0], image_main[1]);
					image_copy(image_main[0], image_main[2]);
                    DisplayCImage(image_main, WMAIN);
					break;
				case MENU_GHIST:
					histgram(image_main[1], hist[1]);
					histimage(hist[1], image_sub[1]);
					image_clear(image_sub[0]);
					image_clear(image_sub[2]);
	                DisplayCImage(image_sub, WSUB);
					break;
				case MENU_GHISTPRIN:
					histprint(hist[1], text_buf);
					ShowText("�q�X�g�O����", text_buf);
                    break;
				case MENU_GHIST_SMOTH:
					histsmooth(hist[1], histbuf);
					for (i = 0; i < 256; i++) hist[1][i] = histbuf[i];
					histimage(hist[1], image_sub[1]);
					image_clear(image_sub[0]);
					image_clear(image_sub[2]);
					DisplayCImage(image_sub, WSUB);
                    break;
				case MENU_GTHRESH_MAN:
                    if (!GetParams1("臒l", &a)) break;
                    thres = (int)a;
                    if (thres < 0 || thres > 255) break;
                    if (!GetSelection2("�\���摜", "���̂܂�", "���]", &type)) 
						break;
                    image_copy_color(image_main, image_prev);
                    DisplayCImage(image_prev, WPREV);
                    threshold(image_prev[1], image_main[0], thres, type);
					image_copy(image_main[0], image_main[1]);
					image_copy(image_main[0], image_main[2]);
                    DisplayCImage(image_main, WMAIN);
					break;
				case MENU_GTHRESH_MODE:
                    if (!GetParams1("��������", &a)) break;
                    smt = (int)a;
                    if (!GetSelection2("�\���摜", "���̂܂�", "���]", &type))
						break;
                    image_copy_color(image_main, image_prev);
                    DisplayCImage(image_prev, WPREV);
                    threshold_mode(image_prev[1], image_main[0], smt, type);
					image_copy(image_main[0], image_main[1]);
					image_copy(image_main[0], image_main[2]);
                    DisplayCImage(image_main, WMAIN);
					break;
				case MENU_GTHRESH_DISCRIM:
                    if (!GetSelection2("�\���摜", "���̂܂�", "���]", &type))
						break;
                    image_copy_color(image_main, image_prev);
                    DisplayCImage(image_prev, WPREV);
                    threshold_discrim(image_prev[1], image_main[0], type);
					image_copy(image_main[0], image_main[1]);
					image_copy(image_main[0], image_main[2]);
                    DisplayCImage(image_main, WMAIN);
					break;
				case MENU_GTHRESH_DYNAMIC:
                    if (!GetSelection2("�\���摜", "���̂܂�", "���]", &type))
						break;
                    image_copy_color(image_main, image_prev);
                    DisplayCImage(image_prev, WPREV);
                    threshold_dynamic(image_prev[1], image_main[0], type);
					image_copy(image_main[0], image_main[1]);
					image_copy(image_main[0], image_main[2]);
                    DisplayCImage(image_main, WMAIN);
					break;
				case MENU_BHIST:
					histgram(image_main[2], hist[2]);
					histimage(hist[2], image_sub[2]);
					image_clear(image_sub[0]);
					image_clear(image_sub[1]);
	                DisplayCImage(image_sub, WSUB);
					break;
				case MENU_BHISTPRIN:
					histprint(hist[2], text_buf);
					ShowText("�q�X�g�O����", text_buf);
                    break;
				case MENU_BHIST_SMOTH:
					histsmooth(hist[2], histbuf);
					for (i = 0; i < 256; i++) hist[2][i] = histbuf[i];
					histimage(hist[2], image_sub[2]);
					image_clear(image_sub[0]);
					image_clear(image_sub[1]);
					DisplayCImage(image_sub, WSUB);
                    break;
				case MENU_BTHRESH_MAN:
                    if (!GetParams1("臒l", &a)) break;
                    thres = (int)a;
                    if (thres < 0 || thres > 255) break;
                    if (!GetSelection2("�\���摜", "���̂܂�", "���]", &type)) 
						break;
                    image_copy_color(image_main, image_prev);
                    DisplayCImage(image_prev, WPREV);
                    threshold(image_prev[2], image_main[0], thres, type);
					image_copy(image_main[0], image_main[1]);
					image_copy(image_main[0], image_main[2]);
                    DisplayCImage(image_main, WMAIN);
					break;
				case MENU_BTHRESH_MODE:
                    if (!GetParams1("��������", &a)) break;
                    smt = (int)a;
                    if (!GetSelection2("�\���摜", "���̂܂�", "���]", &type))
						break;
                    image_copy_color(image_main, image_prev);
                    DisplayCImage(image_prev, WPREV);
                    threshold_mode(image_prev[2], image_main[0], smt, type);
					image_copy(image_main[0], image_main[1]);
					image_copy(image_main[0], image_main[2]);
                    DisplayCImage(image_main, WMAIN);
					break;
				case MENU_BTHRESH_DISCRIM:
                    if (!GetSelection2("�\���摜", "���̂܂�", "���]", &type))
						break;
                    image_copy_color(image_main, image_prev);
                    DisplayCImage(image_prev, WPREV);
                    threshold_discrim(image_prev[2], image_main[0], type);
					image_copy(image_main[0], image_main[1]);
					image_copy(image_main[0], image_main[2]);
                    DisplayCImage(image_main, WMAIN);
					break;
				case MENU_BTHRESH_DYNAMIC:
                    if (!GetSelection2("�\���摜", "���̂܂�", "���]", &type))
						break;
                    image_copy_color(image_main, image_prev);
                    DisplayCImage(image_prev, WPREV);
                    threshold_dynamic(image_prev[2], image_main[0], type);
					image_copy(image_main[0], image_main[1]);
					image_copy(image_main[0], image_main[2]);
                    DisplayCImage(image_main, WMAIN);
					break;
				case MENU_RGBHIST:
                    image_copy_color(image_main, image_prev);
                    DisplayCImage(image_prev, WPREV);
					histgram(image_prev[0], hist[0]);
					histimage(hist[0], image_main[0]);
					image_clear(image_main[1]);
					image_clear(image_main[2]);
					histgram(image_prev[1], hist[1]);
					histimage(hist[1], image_sub[1]);
					image_clear(image_sub[0]);
					image_clear(image_sub[2]);
					histgram(image_prev[2], hist[2]);
					histimage(hist[2], image_key[2]);
					image_clear(image_key[0]);
					image_clear(image_key[1]);
					DisplayCImage(image_main, WMAIN);
					DisplayCImage(image_sub, WSUB);
					DisplayCImage(image_key, WKEY);
					break;
				case MENU_RGBHIST_SMOTH:
					for (i = 0; i < 3; i++) {
						histsmooth(hist[i], histbuf);
						for (j = 0; j < 256; j++) hist[i][j] = histbuf[j];
					}
					histimage(hist[0], image_main[0]);
					image_clear(image_main[1]);
					image_clear(image_main[2]);
					histimage(hist[1], image_sub[1]);
					image_clear(image_sub[0]);
					image_clear(image_sub[2]);
					histimage(hist[2], image_key[2]);
					image_clear(image_key[0]);
					image_clear(image_key[1]);
					DisplayCImage(image_main, WMAIN);
					DisplayCImage(image_sub, WSUB);
					DisplayCImage(image_key, WKEY);
                    break;
				case MENU_HISTRB:
					hist2_image(image_main[0], image_main[2], image_sub[0]);
					image_copy(image_sub[0], image_sub[1]);
					image_copy(image_sub[0], image_sub[2]);
					DisplayCImage(image_sub, WSUB);
					break;
				case MENU_HISTGR:
					hist2_image(image_main[1], image_main[0], image_sub[0]);
					image_copy(image_sub[0], image_sub[1]);
					image_copy(image_sub[0], image_sub[2]);
					DisplayCImage(image_sub, WSUB);
					break;
				case MENU_HISTGB:
					hist2_image(image_main[1], image_main[2], image_sub[0]);
					image_copy(image_sub[0], image_sub[1]);
					image_copy(image_sub[0], image_sub[2]);
					DisplayCImage(image_sub, WSUB);
					break;
				case MENU_RGBTHRESH:
					if (!GetParams6("臒l", "�q��臒l(�ŏ�)", "�q��臒l(�ő�)",
						"�f��臒l(�ŏ�)", "�f��臒l(�ő�)",
						"�a��臒l(�ŏ�)", "�a��臒l(�ő�)",
						&a, &b, &c, &d, &e, &f)) break;
					thres_rmin = (int)a; thres_rmax = (int)b;
					thres_gmin = (int)c; thres_gmax = (int)d;
					thres_bmin = (int)e; thres_bmax = (int)f;
					image_copy_color(image_main, image_prev);
					DisplayCImage(image_prev, WPREV);
					thresh_rgb(image_prev, image_main[0], thres_rmin, thres_rmax,
						thres_gmin, thres_gmax, thres_bmin, thres_bmax);
					image_copy(image_main[0], image_main[1]);
					image_copy(image_main[0], image_main[2]);
					DisplayCImage(image_main, WMAIN);
					break;
				case MENU_YHIST:
					rgb_to_ysh(image_main, image_ysh);
					y_image(image_ysh[0], image_buf[0]);
					histgram(image_buf[0], hist[0]);
					histimage(hist[0], image_sub[0]);
					image_copy(image_sub[0], image_sub[1]);
					image_copy(image_sub[0], image_sub[2]);
	                DisplayCImage(image_sub, WSUB);
					break;
				case MENU_YHISTPRIN:
					histprint(hist[0], text_buf);
					ShowText("�q�X�g�O����", text_buf);
                    break;
				case MENU_YHIST_SMOTH:
					histsmooth(hist[0], histbuf);
					for (i = 0; i < 256; i++) hist[0][i] = histbuf[i];
					histimage(hist[0], image_sub[0]);
					image_copy(image_sub[0], image_sub[1]);
					image_copy(image_sub[0], image_sub[2]);
					DisplayCImage(image_sub, WSUB);
					break;
				case MENU_YTHRESH_MAN:
                    if (!GetParams1("臒l", &a)) break;
                    thres = (int)a;
                    if (thres < 0 || thres > 255) break;
                    if (!GetSelection2("�\���摜", "���̂܂�", "���]", &type)) 
						break;
                    image_copy_color(image_main, image_prev);
                    DisplayCImage(image_prev, WPREV);
					rgb_to_ysh(image_prev, image_ysh);
					y_image(image_ysh[0], image_buf[0]);
                    threshold(image_buf[0], image_main[0], thres, type);
					image_copy(image_main[0], image_main[1]);
					image_copy(image_main[0], image_main[2]);
                    DisplayCImage(image_main, WMAIN);
					break;
				case MENU_YTHRESH_MODE:
                    if (!GetParams1("��������", &a)) break;
                    smt = (int)a;
                    if (!GetSelection2("�\���摜", "���̂܂�", "���]", &type))
						break;
                    image_copy_color(image_main, image_prev);
                    DisplayCImage(image_prev, WPREV);
					rgb_to_ysh(image_prev, image_ysh);
					y_image(image_ysh[0], image_buf[0]);
                    threshold_mode(image_buf[0], image_main[0], smt, type);
					image_copy(image_main[0], image_main[1]);
					image_copy(image_main[0], image_main[2]);
                    DisplayCImage(image_main, WMAIN);
					break;
				case MENU_YTHRESH_DISCRIM:
                    if (!GetSelection2("�\���摜", "���̂܂�", "���]", &type))
						break;
                    image_copy_color(image_main, image_prev);
                    DisplayCImage(image_prev, WPREV);
					rgb_to_ysh(image_prev, image_ysh);
					y_image(image_ysh[0], image_buf[0]);
                    threshold_discrim(image_buf[0], image_main[0], type);
					image_copy(image_main[0], image_main[1]);
					image_copy(image_main[0], image_main[2]);
                    DisplayCImage(image_main, WMAIN);
					break;
				case MENU_YTHRESH_DYNAMIC:
                    if (!GetSelection2("�\���摜", "���̂܂�", "���]", &type))
						break;
                    image_copy_color(image_main, image_prev);
                    DisplayCImage(image_prev, WPREV);
					rgb_to_ysh(image_prev, image_ysh);
					y_image(image_ysh[0], image_buf[0]);
                    threshold_dynamic(image_buf[0], image_main[0], type);
					image_copy(image_main[0], image_main[1]);
					image_copy(image_main[0], image_main[2]);
                    DisplayCImage(image_main, WMAIN);
					break;
				case MENU_SHIST:
					rgb_to_ysh(image_main, image_ysh);
					sat_image(image_ysh[1], image_buf[0]);
					histgram(image_buf[0], hist[1]);
					histimage(hist[1], image_sub[0]);
					image_copy(image_sub[0], image_sub[1]);
					image_copy(image_sub[0], image_sub[2]);
	                DisplayCImage(image_sub, WSUB);
					break;
				case MENU_SHISTPRIN:
					histprint(hist[1], text_buf);
					ShowText("�q�X�g�O����", text_buf);
                    break;
				case MENU_SHIST_SMOTH:
					histsmooth(hist[1], histbuf);
					for (i = 0; i < 256; i++) hist[1][i] = histbuf[i];
					histimage(hist[1], image_sub[0]);
					image_copy(image_sub[0], image_sub[1]);
					image_copy(image_sub[0], image_sub[2]);
					DisplayCImage(image_sub, WSUB);
					break;
				case MENU_STHRESH_MAN:
                    if (!GetParams1("臒l", &a)) break;
                    thres = (int)a;
                    if (thres < 0 || thres > 255) break;
                    if (!GetSelection2("�\���摜", "���̂܂�", "���]", &type)) 
						break;
                    image_copy_color(image_main, image_prev);
                    DisplayCImage(image_prev, WPREV);
					rgb_to_ysh(image_prev, image_ysh);
					sat_image(image_ysh[1], image_buf[0]);
                    threshold(image_buf[0], image_main[0], thres, type);
					image_copy(image_main[0], image_main[1]);
					image_copy(image_main[0], image_main[2]);
                    DisplayCImage(image_main, WMAIN);
					break;
				case MENU_STHRESH_MODE:
                    if (!GetParams1("��������", &a)) break;
                    smt = (int)a;
                    if (!GetSelection2("�\���摜", "���̂܂�", "���]", &type))
						break;
                    image_copy_color(image_main, image_prev);
                    DisplayCImage(image_prev, WPREV);
					rgb_to_ysh(image_prev, image_ysh);
					sat_image(image_ysh[1], image_buf[0]);
                    threshold_mode(image_buf[0], image_main[0], smt, type);
					image_copy(image_main[0], image_main[1]);
					image_copy(image_main[0], image_main[2]);
                    DisplayCImage(image_main, WMAIN);
					break;
				case MENU_STHRESH_DISCRIM:
                    if (!GetSelection2("�\���摜", "���̂܂�", "���]", &type))
						break;
                    image_copy_color(image_main, image_prev);
                    DisplayCImage(image_prev, WPREV);
					rgb_to_ysh(image_prev, image_ysh);
					sat_image(image_ysh[1], image_buf[0]);
                    threshold_discrim(image_buf[0], image_main[0], type);
					image_copy(image_main[0], image_main[1]);
					image_copy(image_main[0], image_main[2]);
                    DisplayCImage(image_main, WMAIN);
					break;
				case MENU_STHRESH_DYNAMIC:
                    if (!GetSelection2("�\���摜", "���̂܂�", "���]", &type))
						break;
                    image_copy_color(image_main, image_prev);
                    DisplayCImage(image_prev, WPREV);
					rgb_to_ysh(image_prev, image_ysh);
					sat_image(image_ysh[1], image_buf[0]);
                    threshold_dynamic(image_buf[0], image_main[0], type);
					image_copy(image_main[0], image_main[1]);
					image_copy(image_main[0], image_main[2]);
                    DisplayCImage(image_main, WMAIN);
					break;
				case MENU_HHIST:
					if (!GetParams1("��ƂȂ�F��(�x)", &a)) break;
					org = (int)a;
					rgb_to_ysh(image_main, image_ysh);
					hue_image(image_ysh[1], image_ysh[2], image_buf[0], org);
					histgram(image_buf[0], hist[2]);
					histimage(hist[2], image_sub[0]);
					image_copy(image_sub[0], image_sub[1]);
					image_copy(image_sub[0], image_sub[2]);
	                DisplayCImage(image_sub, WSUB);
					break;
				case MENU_HHISTPRIN:
					histprint(hist[2], text_buf);
					ShowText("�q�X�g�O����", text_buf);
                    break;
				case MENU_HHIST_SMOTH:
					histsmooth(hist[2], histbuf);
					for (i = 0; i < 256; i++) hist[2][i] = histbuf[i];
					histimage(hist[2], image_sub[0]);
					image_copy(image_sub[0], image_sub[1]);
					image_copy(image_sub[0], image_sub[2]);
					DisplayCImage(image_sub, WSUB);
					break;
				case MENU_HTHRESH_MAN:
					if (!GetParams1("��ƂȂ�F��(�x)", &a)) break;
					org = (int)a;
                    if (!GetParams1("臒l", &a)) break;
                    thres = (int)a;
                    if (thres < 0 || thres > 255) break;
                    if (!GetSelection2("�\���摜", "���̂܂�", "���]", &type)) 
						break;
                    image_copy_color(image_main, image_prev);
                    DisplayCImage(image_prev, WPREV);
					rgb_to_ysh(image_prev, image_ysh);
					hue_image(image_ysh[1], image_ysh[2], image_buf[0], org);
                    threshold(image_buf[0], image_main[0], thres, type);
					image_copy(image_main[0], image_main[1]);
					image_copy(image_main[0], image_main[2]);
                    DisplayCImage(image_main, WMAIN);
					break;
				case MENU_HTHRESH_MODE:
                    if (!GetParams1("��������", &a)) break;
                    smt = (int)a;
					if (!GetParams1("��ƂȂ�F��(�x)", &a)) break;
					org = (int)a;
                    if (!GetSelection2("�\���摜", "���̂܂�", "���]", &type))
						break;
                    image_copy_color(image_main, image_prev);
                    DisplayCImage(image_prev, WPREV);
					rgb_to_ysh(image_prev, image_ysh);
					hue_image(image_ysh[1], image_ysh[2], image_buf[0], org);
                    threshold_mode(image_buf[0], image_main[0], smt, type);
					image_copy(image_main[0], image_main[1]);
					image_copy(image_main[0], image_main[2]);
                    DisplayCImage(image_main, WMAIN);
					break;
				case MENU_HTHRESH_DISCRIM:
					if (!GetParams1("��ƂȂ�F��(�x)", &a)) break;
					org = (int)a;
                    if (!GetSelection2("�\���摜", "���̂܂�", "���]", &type))
						break;
                    image_copy_color(image_main, image_prev);
                    DisplayCImage(image_prev, WPREV);
					rgb_to_ysh(image_prev, image_ysh);
					hue_image(image_ysh[1], image_ysh[2], image_buf[0], org);
                    threshold_discrim(image_buf[0], image_main[0], type);
					image_copy(image_main[0], image_main[1]);
					image_copy(image_main[0], image_main[2]);
                    DisplayCImage(image_main, WMAIN);
					break;
				case MENU_HTHRESH_DYNAMIC:
					if (!GetParams1("��ƂȂ�F��(�x)", &a)) break;
					org = (int)a;
                    if (!GetSelection2("�\���摜", "���̂܂�", "���]", &type))
						break;
                    image_copy_color(image_main, image_prev);
                    DisplayCImage(image_prev, WPREV);
					rgb_to_ysh(image_prev, image_ysh);
					hue_image(image_ysh[1], image_ysh[2], image_buf[0], org);
                    threshold_dynamic(image_buf[0], image_main[0], type);
					image_copy(image_main[0], image_main[1]);
					image_copy(image_main[0], image_main[2]);
                    DisplayCImage(image_main, WMAIN);
					break;
				case MENU_YSHHIST:
					if (!GetParams1("��ƂȂ�F��(�x)", &a)) break;
					org = (int)a;
                    image_copy_color(image_main, image_prev);
					DisplayCImage(image_prev, WPREV);
					rgb_to_ysh(image_prev, image_ysh);
					y_image(image_ysh[0], image_buf[0]);
					histgram(image_buf[0], hist[0]);
					sat_image(image_ysh[1], image_buf[0]);
					histgram(image_buf[0], hist[1]);
					hue_image(image_ysh[1], image_ysh[2], image_buf[0], org);
					histgram(image_buf[0], hist[2]);
					histimage(hist[0], image_main[0]);
					image_copy(image_main[0], image_main[1]);
					image_copy(image_main[0], image_main[2]);
					histimage(hist[1], image_sub[0]);
					image_copy(image_sub[0], image_sub[1]);
					image_copy(image_sub[0], image_sub[2]);
					histimage(hist[2], image_key[0]);
					image_copy(image_key[0], image_key[1]);
					image_copy(image_key[0], image_key[2]);
					DisplayCImage(image_main, WMAIN);
					DisplayCImage(image_sub, WSUB);
					DisplayCImage(image_key, WKEY);
					break;
				case MENU_YSHHIST_SMOTH:
					for (i = 0; i < 3; i++) {
						histsmooth(hist[i], histbuf);
						for (j = 0; j < 256; j++) hist[i][j] = histbuf[j];
					}
					histimage(hist[0], image_main[0]);
					image_copy(image_main[0], image_main[1]);
					image_copy(image_main[0], image_main[2]);
					histimage(hist[1], image_sub[1]);
					image_copy(image_sub[1], image_sub[0]);
					image_copy(image_sub[1], image_sub[2]);
					histimage(hist[2], image_key[2]);
					image_copy(image_key[2], image_key[0]);
					image_copy(image_key[2], image_key[1]);
					DisplayCImage(image_main, WMAIN);
					DisplayCImage(image_sub, WSUB);
					DisplayCImage(image_key, WKEY);
                    break;
				case MENU_YSHTHRESH:
					if (!GetParams6("臒l", "�x��臒l(�ŏ�)", "�x��臒l(�ő�)",
						"�r��臒l(�ŏ�)", "�r��臒l(�ő�)",
						"�g��臒l(�ŏ�,���w���)", "�g��臒l(�ő�)",
						&a, &b, &c, &d, &e, &f)) break;
					thres_ymin = (int)a; thres_ymax = (int)b;
					thres_smin = (int)c; thres_smax = (int)d;
					thres_hmin = (int)e; thres_hmax = (int)f;
					image_copy_color(image_main, image_prev);
					DisplayCImage(image_prev, WPREV);
					rgb_to_ysh(image_prev, image_ysh);
					thresh_ysh(image_ysh, image_main[0], thres_ymin, thres_ymax,
						thres_smin, thres_smax, thres_hmin, thres_hmax);
					image_copy(image_main[0], image_main[1]);
					image_copy(image_main[0], image_main[2]);
					DisplayCImage(image_main, WMAIN);
					break;
				case MENU_RKEY:
					if (!GetParams1("臒l", &a)) break;
					thres = (int)a;
                    image_copy_color(image_main, image_prev);
                    DisplayCImage(image_prev, WPREV);
					thresh_color_difference(image_prev, image_main[0], thres, 1);
					image_copy(image_main[0], image_main[1]);
					image_copy(image_main[0], image_main[2]);
					DisplayCImage(image_main, WMAIN);
					break;
				case MENU_GKEY:
					if (!GetParams1("臒l", &a)) break;
					thres = (int)a;
                    image_copy_color(image_main, image_prev);
                    DisplayCImage(image_prev, WPREV);
					thresh_color_difference(image_prev, image_main[0], thres, 2);
					image_copy(image_main[0], image_main[1]);
					image_copy(image_main[0], image_main[2]);
					DisplayCImage(image_main, WMAIN);
					break;
				case MENU_BKEY:
					if (!GetParams1("臒l", &a)) break;
					thres = (int)a;
                    image_copy_color(image_main, image_prev);
                    DisplayCImage(image_prev, WPREV);
					thresh_color_difference(image_prev, image_main[0], thres, 3);
					image_copy(image_main[0], image_main[1]);
					image_copy(image_main[0], image_main[2]);
					DisplayCImage(image_main, WMAIN);
					break;
				 case MENU_KEYMASK:
                    image_copy_color(image_main, image_prev);
                    DisplayCImage(image_prev, WPREV);
					for (i = 0; i < 3; i++)
	                    masking(image_prev[i], image_main[i], image_key[i]);
                    DisplayCImage(image_main, WMAIN);
                    break;
				 case MENU_KEYINV:
                    image_negative_color(image_key);
                    DisplayCImage(image_key, WKEY);
                    break;
				case MENU_SMOOTH:
                    if (!GetSelection2("�͈�", "3x3", "5x5", &type)) break;
					image_copy_color(image_main, image_prev);
					DisplayCImage(image_prev, WPREV);
					for (i = 0; i < 3; i++)
						smooth(image_prev[i], image_main[i], type*2+1);
					DisplayCImage(image_main, WMAIN);
					break;
				case MENU_SMOOTHWT:
                    if (!GetSelection3("�d�݌W��", "�^�C�v1", "�^�C�v2",
						"�^�C�v3", &type)) break;
					image_copy_color(image_main, image_prev);
					DisplayCImage(image_prev, WPREV);
					for (i = 0; i < 3; i++)
						smooth_weighted(image_prev[i], image_main[i], type-1);
					DisplayCImage(image_main, WMAIN);
					break;
				case MENU_MEDIAN:
					image_copy_color(image_main, image_prev);
					DisplayCImage(image_prev, WPREV);
					for (i = 0; i < 3; i++)
						median(image_prev[i], image_main[i]);
					DisplayCImage(image_main, WMAIN);
					break;
				case MENU_SMOOTHEDGE:
					image_copy_color(image_main, image_prev);
					DisplayCImage(image_prev, WPREV);
					for (i = 0; i < 3; i++)
						smooth_edge_preserve(image_prev[i], image_main[i]);
					DisplayCImage(image_main, WMAIN);
					break;
                 case MENU_DILATION:
					image_copy_color(image_main, image_prev);
                    DisplayCImage(image_prev, WPREV);
					for (i = 0; i < 3; i++)
	                    dilation(image_prev[i], image_main[i]);
                    DisplayCImage(image_main, WMAIN);
                    break;
                 case MENU_EROSION:
					image_copy_color(image_main, image_prev);
                    DisplayCImage(image_prev, WPREV);
					for (i = 0; i < 3; i++)
	                    erosion(image_prev[i], image_main[i]);
                    DisplayCImage(image_main, WMAIN);
                    break;
                 case MENU_NOISE_RAND:
                    if (!GetParams1("�m�C�Y���x��", &a)) break;
                    lvl = (int)a;
					image_copy_color(image_main, image_prev);
                    DisplayCImage(image_prev, WPREV);
					for (i = 0; i < 3; i++)
						noise_rand(image_prev[i], image_main[i], lvl);
                    DisplayCImage(image_main, WMAIN);
					break;
				case MENU_NOISE_SPIKE:
					if (!GetParams2("�m�C�Y", "��", "���x��", &a, &b)) break;
					num = (int)a;
					lvl = (int)b;
					image_copy_color(image_main, image_prev);
					DisplayCImage(image_prev, WPREV);
					for (i = 0; i < 3; i++)
						noise_spike(image_prev[i], image_main[i], num, lvl);
                    DisplayCImage(image_main, WMAIN);
					DisplayCImage(image_main, WMAIN);
                    break;
				 case MENU_GRADIENT:
                    if (!GetParams1("�o�͉摜�̗���", &amp)) break;
                    image_copy_color(image_main, image_prev);
                    DisplayCImage(image_prev, WPREV);
					for (i = 0; i < 3; i++)
	                    gradient_sobel(image_prev[i], image_main[i], amp);
                    DisplayCImage(image_main, WMAIN);
                    break;
                 case MENU_GRADIENTR:
                    if (!GetParams1("�o�͉摜�̗���", &amp)) break;
                    image_copy_color(image_main, image_prev);
                    DisplayCImage(image_prev, WPREV);
					gradient_sobel(image_prev[0], image_main[0], amp);
					image_copy(image_main[0], image_main[1]);
					image_copy(image_main[0], image_main[2]);
                    DisplayCImage(image_main, WMAIN);
                    break;
                 case MENU_GRADIENTG:
                    if (!GetParams1("�o�͉摜�̗���", &amp)) break;
                    image_copy_color(image_main, image_prev);
                    DisplayCImage(image_prev, WPREV);
					gradient_sobel(image_prev[1], image_main[0], amp);
					image_copy(image_main[0], image_main[1]);
					image_copy(image_main[0], image_main[2]);
                    DisplayCImage(image_main, WMAIN);
                    break;
                 case MENU_GRADIENTB:
                    if (!GetParams1("�o�͉摜�̗���", &amp)) break;
                    image_copy_color(image_main, image_prev);
                    DisplayCImage(image_prev, WPREV);
					gradient_sobel(image_prev[2], image_main[0], amp);
					image_copy(image_main[0], image_main[1]);
					image_copy(image_main[0], image_main[2]);
                    DisplayCImage(image_main, WMAIN);
                    break;
                 case MENU_GRADIENTRGB:
                    if (!GetParams1("�o�͉摜�̗���", &amp)) break;
                    image_copy_color(image_main, image_prev);
                    DisplayCImage(image_prev, WPREV);
					for (i = 0; i < 3; i++)
	                    gradient_sobel(image_prev[i], image_main[i], amp);
					image_addition_rgb(image_main);
					DisplayCImage(image_main, WMAIN);
                    break;
                case MENU_TEMPLATE:
                    if (!GetParams1("�o�͉摜�̗���", &amp)) break;
                    image_copy_color(image_main, image_prev);
                    DisplayCImage(image_prev, WPREV);
					for (i = 0; i < 3; i++)
	                    prewitt(image_prev[i], image_main[i], amp);
                    DisplayCImage(image_main, WMAIN);
                    break;
                case MENU_TEMPLATER:
                    if (!GetParams1("�o�͉摜�̗���", &amp)) break;
                    image_copy_color(image_main, image_prev);
                    DisplayCImage(image_prev, WPREV);
                    prewitt(image_prev[0], image_main[0], amp);
					image_copy(image_main[0], image_main[1]);
					image_copy(image_main[0], image_main[2]);
                    DisplayCImage(image_main, WMAIN);
                    break;
                case MENU_TEMPLATEG:
                    if (!GetParams1("�o�͉摜�̗���", &amp)) break;
                    image_copy_color(image_main, image_prev);
                    DisplayCImage(image_prev, WPREV);
                    prewitt(image_prev[1], image_main[0], amp);
					image_copy(image_main[0], image_main[1]);
					image_copy(image_main[0], image_main[2]);
                    DisplayCImage(image_main, WMAIN);
                    break;
                case MENU_TEMPLATEB:
                    if (!GetParams1("�o�͉摜�̗���", &amp)) break;
                    image_copy_color(image_main, image_prev);
                    DisplayCImage(image_prev, WPREV);
                    prewitt(image_prev[2], image_main[0], amp);
					image_copy(image_main[0], image_main[1]);
					image_copy(image_main[0], image_main[2]);
                    DisplayCImage(image_main, WMAIN);
                    break;
                case MENU_TEMPLATERGB:
                    if (!GetParams1("�o�͉摜�̗���", &amp)) break;
                    image_copy_color(image_main, image_prev);
                    DisplayCImage(image_prev, WPREV);
					for (i = 0; i < 3; i++)
	                    prewitt(image_prev[i], image_main[i], amp);
					image_addition_rgb(image_main);
                    DisplayCImage(image_main, WMAIN);
                    break;
                 case MENU_LAPLACIAN:
                    if (!GetSelection3("�W��", "�W��1", "�W��2", "�W��3", &type))
						break;
                    if (!GetParams1("�o�͉摜�̗���", &amp)) break;
                    image_copy_color(image_main, image_prev);
                    DisplayCImage(image_prev, WPREV);
					for (i = 0; i < 3; i++)
	                    laplacian(image_prev[i], image_main[i], amp, type);
                    DisplayCImage(image_main, WMAIN);
                    break;
                 case MENU_LAPLACIANR:
                    if (!GetSelection3("�W��", "�W��1", "�W��2", "�W��3", &type))
						break;
                    if (!GetParams1("�o�͉摜�̗���", &amp)) break;
                    image_copy_color(image_main, image_prev);
                    DisplayCImage(image_prev, WPREV);
                    laplacian(image_prev[0], image_main[0], amp, type);
					image_copy(image_main[0], image_main[1]);
					image_copy(image_main[0], image_main[2]);
                    DisplayCImage(image_main, WMAIN);
                    break;
                 case MENU_LAPLACIANG:
                    if (!GetSelection3("�W��", "�W��1", "�W��2", "�W��3", &type))
						break;
                    if (!GetParams1("�o�͉摜�̗���", &amp)) break;
                    image_copy_color(image_main, image_prev);
                    DisplayCImage(image_prev, WPREV);
                    laplacian(image_prev[1], image_main[0], amp, type);
					image_copy(image_main[0], image_main[1]);
					image_copy(image_main[0], image_main[2]);
                    DisplayCImage(image_main, WMAIN);
                    break;
                 case MENU_LAPLACIANB:
                    if (!GetSelection3("�W��", "�W��1", "�W��2", "�W��3", &type))
						break;
                    if (!GetParams1("�o�͉摜�̗���", &amp)) break;
                    image_copy_color(image_main, image_prev);
                    DisplayCImage(image_prev, WPREV);
                    laplacian(image_prev[2], image_main[0], amp, type);
					image_copy(image_main[0], image_main[1]);
					image_copy(image_main[0], image_main[2]);
                    DisplayCImage(image_main, WMAIN);
                    break;
                 case MENU_ZEROCROSS:
                    image_copy_color(image_main, image_prev);
                    DisplayCImage(image_prev, WPREV);
					for (i = 0; i < 3; i++)
	                    zero_cross(image_prev[i], image_main[i]);
                    DisplayCImage(image_main, WMAIN);
                    break;
                 case MENU_LOG:
                    if (!GetParams1("���U", &a)) break;
					image_copy_color(image_main, image_prev);
                    DisplayCImage(image_prev, WPREV);
					for (i = 0; i < 3; i++)
	                    log_zero_cross(image_prev[i], image_main[i], a);
                    DisplayCImage(image_main, WMAIN);
                    break;
                 case MENU_LOGR:
                    if (!GetParams1("���U", &a)) break;
					image_copy_color(image_main, image_prev);
                    DisplayCImage(image_prev, WPREV);
					log_zero_cross(image_prev[0], image_main[0], a);
					image_copy(image_main[0], image_main[1]);
					image_copy(image_main[0], image_main[2]);
                    DisplayCImage(image_main, WMAIN);
                    break;
                 case MENU_LOGG:
                    if (!GetParams1("���U", &a)) break;
					image_copy_color(image_main, image_prev);
                    DisplayCImage(image_prev, WPREV);
					log_zero_cross(image_prev[1], image_main[0], a);
					image_copy(image_main[0], image_main[1]);
					image_copy(image_main[0], image_main[2]);
                    DisplayCImage(image_main, WMAIN);
                    break;
                 case MENU_LOGB:
                    if (!GetParams1("���U", &a)) break;
					image_copy_color(image_main, image_prev);
                    DisplayCImage(image_prev, WPREV);
					log_zero_cross(image_prev[2], image_main[0], a);
					image_copy(image_main[0], image_main[1]);
					image_copy(image_main[0], image_main[2]);
                    DisplayCImage(image_main, WMAIN);
                    break;
                 case MENU_THIN:
                    image_copy_color(image_main, image_prev);
                    DisplayCImage(image_prev, WPREV);
					for (i = 0; i < 3; i++)
	                    thinning(image_prev[i], image_main[i]);
                    DisplayCImage(image_main, WMAIN);
                    break;
				case MENU_SCALE_NE:
					if (!GetParams2("�g�嗦", "��", "�c", &a, &b)) break;
					image_copy_color(image_main, image_prev);
					DisplayCImage(image_prev, WPREV);
					for (i = 0; i < 3; i++)
						scale_near(image_prev[i], image_main[i], a, b);
					DisplayCImage(image_main, WMAIN);
					break;
				case MENU_SCALE:
					if (!GetParams2("�g�嗦", "��", "�c", &a, &b)) break;
					image_copy_color(image_main, image_prev);
					DisplayCImage(image_prev, WPREV);
					for (i = 0; i < 3; i++)
						scale(image_prev[i], image_main[i], a, b);
					DisplayCImage(image_main, WMAIN);
					break;
				case MENU_SHIFT:
					if (!GetParams2("�ړ���", "��", "�c", &x0, &y0)) break;
					image_copy_color(image_main, image_prev);
					DisplayCImage(image_prev, WPREV);
					for (i = 0; i < 3; i++)
						shift(image_prev[i], image_main[i], x0, y0);
					DisplayCImage(image_main, WMAIN);
					break;
				case MENU_ROTATE:
					if (!GetParams1("��]�p�i�x�j", &deg)) break;
					image_copy_color(image_main, image_prev);
					DisplayCImage(image_prev, WPREV);
					for (i = 0; i < 3; i++)
						rotation(image_prev[i], image_main[i], deg);
					DisplayCImage(image_main, WMAIN);
					break;
                 case MENU_SCALEROTSHIFT:
                    if (!GetParams5("�p�����[�^", "�g�嗦(��)", "�g�嗦(�c)",
						"��]�p(�x)", "���S�_(��)", "���S�_(�c)",
						&a, &b, &deg, &x0, &y0)) break;
                    image_copy_color(image_main, image_prev);
                    DisplayCImage(image_prev, WPREV);
					for (i = 0; i < 3; i++)
						scale_rotate_shift(image_prev[i], image_main[i],
							a, b, deg, x0, y0);
                    DisplayCImage(image_main, WMAIN);
                    break;
				case MENU_AFFINE:
                    if (!GetParams6("�p�����[�^", "a", "b", "c", "d", "e", "f", 
						&a, &b, &c, &d, &e, &f)) break;
					image_copy_color(image_main, image_prev);
					DisplayCImage(image_prev, WPREV);
					for (i = 0; i < 3; i++)
	                    affine(image_prev[i], image_main[i], a, b, c, d, e, f);
					DisplayCImage(image_main, WMAIN);
					break;
				 case MENU_AFFINECOEF:
					if (!GetParams4("�Ή��_1", "x1", "y1", "X1", "Y1", 
						&a, &b, &c, &d)) break;
					x1 = (int)a;
					y1 = (int)b;
					u1 = (int)c;
					v1 = (int)d;
					if (!GetParams4("�Ή��_2", "x2", "y2", "X2", "Y2", 
						&a, &b, &c, &d)) break;
					x2 = (int)a;
					y2 = (int)b;
					u2 = (int)c;
					v2 = (int)d;
					if (!GetParams4("�Ή��_3", "x3", "y3", "X3", "Y3", 
						&a, &b, &c, &d)) break;
					x3 = (int)a;
					y3 = (int)b;
					u3 = (int)c;
					v3 = (int)d;
					image_copy_color(image_main, image_prev);
					DisplayCImage(image_prev, WPREV);
					affine_coef(x1, y1, u1, v1, x2, y2, u2, v2,
						x3, y3, u3, v3, &a, &b, &c, &d, &e, &f);
					for (i = 0; i < 3; i++)
	                    affine(image_prev[i], image_main[i], a, b, c, d, e, f);
					DisplayCImage(image_main, WMAIN);
					break;
				 case MENU_PERSPECT:
					if (!GetParams4("�Ή��_1", "x1", "y1", "X1", "Y1", 
						&a, &b, &c, &d)) break;
					x1 = (int)a;
					y1 = (int)b;
					u1 = (int)c;
					v1 = (int)d;
					if (!GetParams4("�Ή��_2", "x2", "y2", "X2", "Y2", 
						&a, &b, &c, &d)) break;
					x2 = (int)a;
					y2 = (int)b;
					u2 = (int)c;
					v2 = (int)d;
					if (!GetParams4("�Ή��_3", "x3", "y3", "X3", "Y3", 
						&a, &b, &c, &d)) break;
					x3 = (int)a;
					y3 = (int)b;
					u3 = (int)c;
					v3 = (int)d;
					if (!GetParams4("�Ή��_4", "x4", "y4", "X4", "Y4", 
						&a, &b, &c, &d)) break;
					x4 = (int)a;
					y4 = (int)b;
					u4 = (int)c;
					v4 = (int)d;
                    image_copy_color(image_main, image_prev);
                    DisplayCImage(image_prev, WPREV);
					perspect_coef(x1, y1, u1, v1, x2, y2, u2, v2,
						x3, y3, u3, v3, x4, y4, u4, v4,
						&a, &b, &c, &d, &e, &f, &g, &h);
					for (i = 0; i < 3; i++)
						perspect(image_prev[i], image_main[i],
							a, b, c, d, e, f, g, h);
                    DisplayCImage(image_main, WMAIN);
                    break;
				case MENU_RADISTORT:
                    if (!GetParams2("�c�Ȏ���", "�W��a", "�W��b", &a, &b)) break;
					image_copy_color(image_main, image_prev);
					DisplayCImage(image_prev, WPREV);
					for (i = 0; i < 3; i++)
						radial_distortion(image_prev[i], image_main[i], a, b);
					DisplayCImage(image_main, WMAIN);
					break;
				 case MENU_LATTICE:
					image_copy_color(image_main, image_prev);
					DisplayCImage(image_prev, WPREV);
					for (i = 0; i < 3; i++) lattice(image_main[i]);
					DisplayCImage(image_main, WMAIN);
					break;
				case MENU_FFTIMAGE:
					if ((fftimage(image_main[0], image_sub[0]) != -1) &&
						(fftimage(image_main[1], image_sub[1]) != -1) &&
						(fftimage(image_main[2], image_sub[2]) != -1))
						DisplayCImage(image_sub, WSUB);
					else
						MessageBox(GetFocus(), "Memory allocation error",
							"Error", MB_OK | MB_APPLMODAL);
					break;
				case MENU_FFTFILTER:
					if (!GetParams2("�t�B���^�͈̔�", "Low", "High", &a, &b)) 
						break;
                    image_copy_color(image_main, image_prev);
                    DisplayCImage(image_prev, WPREV);
					if ((fftfilter(image_prev[0], image_main[0], 
								(int)a, (int)b) != -1) &&
							(fftfilter(image_prev[1], image_main[1],
								(int)a, (int)b) != -1) &&
							(fftfilter(image_prev[2], image_main[2],
								(int)a, (int)b) != -1))
						DisplayCImage(image_main, WMAIN);
					else
						MessageBox(GetFocus(), "Memory allocation error",
							"Error", MB_OK | MB_APPLMODAL);
					break;
				case MENU_ENCODE:
					if ((dpcm_vlcode(image_main[0], image_sub[0]) != -1) &&
						(dpcm_vlcode(image_main[1], image_sub[1]) != -1) &&
						(dpcm_vlcode(image_main[2], image_sub[2]) != -1))
						DisplayCImage(image_sub, WSUB);
					else
						MessageBox(GetFocus(), "Buffer overflow error",
							"Error", MB_OK | MB_APPLMODAL);
					break;
				case MENU_DECODE:
                    image_copy_color(image_main, image_prev);
                    DisplayCImage(image_prev, WPREV);
					if ((idpcm_vlcode(image_sub[0], image_main[0]) != -1) &&
						(idpcm_vlcode(image_sub[1], image_main[1]) != -1) &&
						(idpcm_vlcode(image_sub[2], image_main[2]) != -1))
						DisplayCImage(image_main, WMAIN);
					else
						MessageBox(GetFocus(), "Buffer overflow error",
							"Error", MB_OK | MB_APPLMODAL);
					break;
				case MENU_STEREO_DIFF:
                    image_copy_color(image_main, image_prev);
                    DisplayCImage(image_prev, WPREV);
					if (!GetParams1("�u���b�N�T�C�Y", &a)) break;
					if (!GetParams1("�o�͉摜�̗���", &amp)) break;
					stereo_diff(image_prev[0], image_sub[0], image_main[0], (int)a);
					stereo_diff(image_prev[1], image_sub[1], image_main[1], (int)a);
					stereo_diff(image_prev[2], image_sub[2], image_main[2], (int)a);
					amplify(image_main[0], image_main[0], amp, 0);
					amplify(image_main[1], image_main[1], amp, 0);
					amplify(image_main[2], image_main[2], amp, 0);
					DisplayCImage(image_main, WMAIN);
					break;
				case MENU_STEREO_CORRE:
                    image_copy_color(image_main, image_prev);
                    DisplayCImage(image_prev, WPREV);
					if (!GetParams1("�u���b�N�T�C�Y", &a)) break;
					if (!GetParams1("�o�͉摜�̗���", &amp)) break;
					stereo_corre(image_prev[0], image_sub[0], image_main[0], (int)a);
					stereo_corre(image_prev[1], image_sub[1], image_main[1], (int)a);
					stereo_corre(image_prev[2], image_sub[2], image_main[2], (int)a);
					amplify(image_main[0], image_main[0], amp, 0);
					amplify(image_main[1], image_main[1], amp, 0);
					amplify(image_main[2], image_main[2], amp, 0);
					DisplayCImage(image_main, WMAIN);
					break;
				case MENU_CSR:
                    if (!GetParams2("�J�[�\���\���ʒu", "x���W", "y���W",
						&a, &b)) break;
					xd = (int)a;
					yd = (int)b;
					image_copy_color(image_main, image_prev);
					DisplayCImage(image_prev, WPREV);
					for (k = 0; k < 3; k++) {
						for (i = -5; i <= 5; i++) {
							image_main[k][yd-1][xd+i] = 0;
							image_main[k][yd  ][xd+i] = 0;
							image_main[k][yd+1][xd+i] = 0;
							image_main[k][yd+i][xd-1] = 0;
							image_main[k][yd+i][xd  ] = 0;
							image_main[k][yd+i][xd+1] = 0;
						}
					}
					DisplayCImage(image_main, WMAIN);
					break;
				case MENU_MOSAIC:
					if (!GetParams4("Main�摜�ɑ΂���Sub�摜�̈ʒu",
						"�����x", "�����y",
						"�g��E�k���␳", "��]�␳(�x)",
						&a, &b, &c, &d)) break;
                    if (!GetSelection2("�d�������̏���", "�㏑��",
						"�O���f�[�V����", &type)) break;
					image_copy_color(image_main, image_prev);
					DisplayCImage(image_prev, WPREV);
					for (i = 0; i < 3; i++)
						mosaic(image_prev[i], image_sub[i], image_work[i],
							a, b, c, d, type-1);
					image_halfsize_color(image_work, image_main);
					DisplayCImage(image_main, WMAIN);
					break;
				case MENU_MOSAICCOEF:
					if (!GetParams4("�Ή��_1", "�摜1 x", "�摜1 y",
						"�摜2 x", "�摜2 y", &a, &b, &c, &d)) break;
					x1 = (int)c;
					y1 = (int)d;
					u1 = (int)a;
					v1 = (int)b;
					if (!GetParams4("�Ή��_2", "�摜1 x", "�摜1 y",
						"�摜2 x", "�摜2 y", &a, &b, &c, &d)) break;
					x2 = (int)c;
					y2 = (int)d;
					u2 = (int)a;
					v2 = (int)b;
					if (!GetParams4("�Ή��_3", "�摜1 x", "�摜1 y",
						"�摜2 x", "�摜2 y", &a, &b, &c, &d)) break;
					x3 = (int)c;
					y3 = (int)d;
					u3 = (int)a;
					v3 = (int)b;
                    if (!GetSelection2("�d�������̏���", "�㏑��",
						"�O���f�[�V����", &type)) break;
					image_copy_color(image_main, image_prev);
					DisplayCImage(image_prev, WPREV);
					affine_coef(x1, y1, u1, v1, x2, y2, u2, v2,
						x3, y3, u3, v3, &a, &b, &c, &d, &e, &f);
					for (i = 0; i < 3; i++)
						mosaic_affine(image_prev[i], image_sub[i], image_work[i],
							a, b, c, d, e, f, type-1);
					image_halfsize_color(image_work, image_main);
					DisplayCImage(image_main, WMAIN);
					break;
				case MENU_MOSAIC_BLOCK:
                    if (!GetParams8("�Ή��_�T���̊�_", "Main�摜x",
						"Main�摜y", "Sub�摜x", "Sub�摜y",
						"�T���͈̔́}x", "�T���͈̔́}y", 
						"�}�b�`���O�̈�}x", "�}�b�`���O�̈�}y", 
						&a, &b, &c, &d, &e, &f, &g, &h)) break;
					x1 = (int)a;
					y1 = (int)b;
					x2 = (int)c;
					y2 = (int)d;
					xd = (int)e;
					yd = (int)f;
					xm = (int)g;
					ym = (int)h;
					image_copy_color(image_main, image_prev);
					DisplayCImage(image_prev, WPREV);
					mosaic_coef_blockmatch_rgb(image_prev, image_sub,
						x1, y1, x2, y2, xd, yd, xm, ym, &mx, &my);
					for (i = 0; i < 3; i++)
						mosaic(image_prev[i], image_sub[i], image_work[i],
							(double)mx, (double)my, 1, 0, 1);
					image_halfsize_color(image_work, image_main);
					DisplayCImage(image_main, WMAIN);
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

/*--- InitApplication --- �E�B���h�E�N���X�̓o�^ -----------------------------
	hInstance:		�A�v���P�[�V�����ւ̃n���h��
----------------------------------------------------------------------------*/
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
	wc.hbrBackground = GetStockObject(WHITE_BRUSH);
	wc.lpszMenuName = "MAINMENU";
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
	wcm.hbrBackground = GetStockObject(WHITE_BRUSH);
	wcm.lpszMenuName = "";
 	wcm.lpszClassName = "ImageClass";
    wcm.hIconSm = LoadIcon(NULL, IDI_APPLICATION);
	return RegisterClassEx(&wcm);
}

/*--- InitInstance --- �E�B���h�E�̍쐬 --------------------------------------
	hInstance:		�A�v���P�[�V�����ւ̃n���h��
	nCmdShow:		�N�����̕\�����@
----------------------------------------------------------------------------*/
BOOL InitInstance(HANDLE hInstance, int nCmdShow)
{
	HWND    hWnd;
	HDC     hDC;
	RECT    rect;
	int     i;

	hGWnd = hWnd = CreateWindow("MainClass", "Color Image Processing",
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
			"�\�����t���J���[�i�Q�S�r�b�g���R�Q�r�b�g�j�ɂ��Ă��������D", 
			"�G���[", MB_ICONHAND | MB_APPLMODAL);
		return FALSE;
	}
	for (i = 0; i < 4; i++)
		imageWin[i].hBitmap = CreateCompatibleBitmap(hDC, X_SIZE, Y_SIZE);
	rect.top= 0;
	rect.left = 0;
	rect.bottom = rect.top+Y_SIZE;
	rect.right = rect.left+X_SIZE;
	AdjustWindowRect(&rect, WS_OVERLAPPEDWINDOW, FALSE);
	imageWin[WMAIN].hWnd = CreateWindow("ImageClass", wTitle[WMAIN],
		WS_OVERLAPPED, WMAIN_POSX, WMAIN_POSY, 
		rect.right-rect.left, rect.bottom-rect.top, NULL, NULL,
		hInstance, NULL);
	if (imageWin[WMAIN].hWnd == NULL) return FALSE;
	ShowWindow(imageWin[WMAIN].hWnd, nCmdShow);
	DisplayCImage(image_main, WMAIN);
	imageWin[WPREV].hWnd = CreateWindow("ImageClass", wTitle[WPREV],
		WS_OVERLAPPED, WPREV_POSX, WPREV_POSY, 
		rect.right-rect.left, rect.bottom-rect.top, NULL, NULL,
		hInstance, NULL);
	if (imageWin[WPREV].hWnd == NULL) return FALSE;
	ShowWindow(imageWin[WPREV].hWnd, nCmdShow);
	DisplayCImage(image_prev, WPREV);
	imageWin[WSUB].hWnd = CreateWindow("ImageClass", wTitle[WSUB],
		WS_OVERLAPPED, WSUB_POSX, WSUB_POSY, 
		rect.right-rect.left, rect.bottom-rect.top, NULL, NULL,
		hInstance, NULL);
	if (imageWin[WSUB].hWnd == NULL) return FALSE;
	ShowWindow(imageWin[WSUB].hWnd, nCmdShow);
	DisplayCImage(image_sub, WSUB);
	imageWin[WKEY].hWnd = CreateWindow("ImageClass", wTitle[WKEY],
		WS_OVERLAPPED, WKEY_POSX, WKEY_POSY, 
		rect.right-rect.left, rect.bottom-rect.top, NULL, NULL,
		hInstance, NULL);
	if (imageWin[WKEY].hWnd == NULL) return FALSE;
	ShowWindow(imageWin[WKEY].hWnd, nCmdShow);
	DisplayCImage(image_key, WKEY);
	/* OPENFILENAME�\���̂̃����o��ݒ� */
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
