#include <stdio.h>
#include <stdlib.h>
#include "Params.h"
#include "Proto.h"

#define WMAIN	0		/* メイン画像用ウィンドウ	*/
#define WPREV 	1		/* プリ画像用ウィンドウ		*/
#define WSUB 	2		/* サブ画像用ウィンドウ		*/
#define WKEY 	3		/* キー画像用ウィンドウ		*/

void InitCDisplay(void);
void DisplayCImage(unsigned char image_r[3][Y_SIZE][X_SIZE], int position);

unsigned char	image_main[3][Y_SIZE][X_SIZE];		/* メイン画像配列		*/
unsigned char	image_prev[3][Y_SIZE][X_SIZE];		/* プリ画像配列			*/
unsigned char	image_sub[3][Y_SIZE][X_SIZE];		/* サブ画像配列 		*/
unsigned char	image_key[3][Y_SIZE][X_SIZE];		/* キー画像配列			*/
unsigned char 	image_buf[Y_SIZE][X_SIZE];			/* バッファ配列			*/
unsigned char 	image_work[3][Y_SIZE*2][X_SIZE*2];	/* モザイク用画像配列	*/
unsigned char 	image_rgb[3][Y_SIZE][X_SIZE];		/* カラー画像配列(RGB)	*/
int			 	image_ysh[3][Y_SIZE][X_SIZE];		/* カラー画像配列(YSH)	*/
long			hist[3][256];						/* ヒストグラム配列		*/
long			histbuf[256];						/* ヒストグラム配列 	*/
char			text_buf[TEXT_BUF_LEN];				/* 印刷用文字配列		*/


/*--- main --- 画像処理メインプログラム（カラー）----------------------------*/
main(void)
{
	static int	command = -1;
	char		source[80], destin[80];
	int			command_sub, nwin, nc, corder, num, m, n, ret;
    int 		i, j, k, mx, my, lvl;
    int 		thres, type, org;
    double   	amp;
	int			smt;
    int 		thres_rmin, thres_rmax;
    int 		thres_gmin, thres_gmax;
    int 		thres_bmin, thres_bmax;
    int 		thres_ymin, thres_ymax;
    int 		thres_smin, thres_smax;
    int 		thres_hmin, thres_hmax;
    double   	ya, yb, sa, sb, hb;
    double   	a, b, c, d, e, f, g, h, x0, y0, deg;
	int         x1, y1, x2, y2, x3, y3, x4, y4, xd, yd, xm, ym;
	int         u1, v1, u2, v2, u3, v3, u4, v4;
	double		gain[3], bias[3];

	InitCDisplay();
	while (command) {
		printf("\n*** Color Image Processing Menu ***\n");
		printf(" 1: File\n");
		printf(" 2: Edit\n");
		printf(" 3: Change Color\n");
		printf(" 4: Color Region Extraction\n");
		printf(" 5: Geometrical transform\n");
		printf(" 6: FFT & Filter\n");
		printf(" 7: Compression\n");
		printf(" 8: Depth Estimation\n");
		printf(" 9: Mosaic\n");
		printf(" 0: End\n");
		printf("Process number   ?  ");
		scanf("%d", &command);
		switch (command) {

			case 1:
				printf(" 1: BMP(color/mono) File read\n");
				printf(" 2: BMP(color) File write\n");
				printf(" 3: RAW(color) File read\n");
				printf(" 4: RAW(color) File write\n");
				printf(" 5: RAW(each plane) File read\n");
				printf(" 6: RAW(each plane) File write   ?  ");
				scanf("%d", &command_sub);
				if (command_sub < 0 || command_sub > 6) break;
				printf("  Which window (1:MAIN, 2:PREV, 3:SUB, 4:KEY) ?  ");
				scanf("%d", &nwin);
				if (command_sub == 3 || command_sub == 4) {
					printf("  Color Order (1:plane, 2:line, 3:pixel) ?  ");
					scanf("%d", &corder);
				}
				if (command_sub == 5 || command_sub == 6) {
					printf("  Which Color (1:R, 2:G, 3:B, 4:RGB) ?  ");
					scanf("%d", &nc);
				}
				if (command_sub == 1 || command_sub  == 3 || command_sub == 5) {
					printf("  Filename (read) ?  ");
					scanf("%s", source);
				}
				if (command_sub == 2 || command_sub  == 4 || command_sub == 6) {
					printf("  Filename (write) ?  ");
					scanf("%s", destin);
				}
				if (command_sub == 1) {
					num = check_bmp_file(source);
				}
				ret =  0;
				if (command_sub == 1 && num == 8) {
					ret = read_bmp_mono(image_buf, source);
					if (ret != -1) {
						image_copy(image_buf, image_rgb[0]);
						image_copy(image_buf, image_rgb[1]);
						image_copy(image_buf, image_rgb[2]);
					}
				}
				if (command_sub == 1 && num == 24)
					ret = read_bmp_color(image_rgb, source);
				if (command_sub == 3) {
					if (corder == 1)
						ret = read_rgb_plane(image_rgb, source);
					else if (corder == 2)
						ret = read_rgb_line(image_rgb, source);
					else if (corder == 3)
						ret = read_rgb_pixel(image_rgb, source);
					else break;
				}
				if  (command_sub == 5 && nc == 4)
					ret =  read_rgb_apart(image_rgb, source);
				if (ret == -1) break;
				if (command_sub == 1 || command_sub == 3 || 
					(command_sub == 5 && nc == 4)) {
					switch (nwin) {
						case 1: 
							image_copy_color(image_rgb, image_main);
							DisplayCImage(image_main, WMAIN);
							break;
						case 2: 
							image_copy_color(image_rgb, image_prev);
							DisplayCImage(image_prev, WPREV);
							break;
						case 3: 
							image_copy_color(image_rgb, image_sub);
							DisplayCImage(image_sub, WSUB);
							break;
						case 4: 
							image_copy_color(image_rgb, image_key);
							DisplayCImage(image_key, WKEY);
							break;
					}
				}
				if (command_sub == 5 && (nc == 1 || nc == 2 || nc == 3)) {
					ret = read_mono(image_buf, source);
					if (ret == -1) break;
					switch (nwin) {
						case 1: 
							if (nc == 1) 
								image_copy(image_buf, image_main[0]);
							else if (nc == 2) 
								image_copy(image_buf, image_main[1]);
							else if (nc == 3) 
								image_copy(image_buf, image_main[2]);
							DisplayCImage(image_main, WMAIN);
							break;
						case 2: 
							if (nc == 1) 
								image_copy(image_buf, image_prev[0]);
							else if (nc == 2) 
								image_copy(image_buf, image_prev[1]);
							else if (nc == 3) 
								image_copy(image_buf, image_prev[2]);
							DisplayCImage(image_prev, WPREV);
							break;
						case 3: 
							if (nc == 1) 
								image_copy(image_buf, image_sub[0]);
							else if (nc == 2) 
								image_copy(image_buf, image_sub[1]);
							else if (nc == 3) 
								image_copy(image_buf, image_sub[2]);
							DisplayCImage(image_sub, WSUB);
							break;
						case 4: 
							if (nc == 1) 
								image_copy(image_buf, image_key[0]);
							else if (nc == 2) 
								image_copy(image_buf, image_key[1]);
							else if (nc == 3) 
								image_copy(image_buf, image_key[2]);
							DisplayCImage(image_key, WKEY);
							break;
					}
				}
				if  (ret == -1) {
					printf("--File read error !\n");
					break;
				}
				if (command_sub == 2 || command_sub == 4 || 
					(command_sub == 6 && nc == 4)) {
					if  (nwin ==  1) 
						image_copy_color(image_main, image_rgb);
					else if  (nwin ==  2) 
						image_copy_color(image_prev, image_rgb);
					else if  (nwin ==  3) 
						image_copy_color(image_sub, image_rgb);
					else if  (nwin ==  4) 
						image_copy_color(image_key, image_rgb);
				}
				if (command_sub ==  2)
					ret = write_bmp_color(image_rgb, destin);
				if (command_sub == 4) {
					if (corder == 1)
						ret = write_rgb_plane(image_rgb, destin);
					else if (corder == 2)
						ret = write_rgb_line(image_rgb, destin);
					else if (corder == 3)
						ret = write_rgb_pixel(image_rgb, destin);
				}
				if (command_sub == 6 && nc == 4)
					ret = write_rgb_apart(image_rgb, destin);
				if (command_sub == 6 && (nc == 1 || nc == 2 || nc == 3)) {
					switch (nwin) {
						case 1: 
							if (nc == 1) 
								image_copy(image_main[0], image_buf);
							else if (nc == 2) 
								image_copy(image_main[1], image_buf);
							else if (nc == 3) 
								image_copy(image_main[2], image_buf);
							break;
						case 2: 
							if (nc == 1) 
								image_copy(image_prev[0], image_buf);
							else if (nc == 2) 
								image_copy(image_prev[1], image_buf);
							else if (nc == 3) 
								image_copy(image_prev[2], image_buf);
							break;
						case 3: 
							if (nc == 1) 
								image_copy(image_sub[0], image_buf);
							else if (nc == 2) 
								image_copy(image_sub[1], image_buf);
							else if (nc == 3) 
								image_copy(image_sub[2], image_buf);
							break;
						case 4: 
							if (nc == 1) 
								image_copy(image_key[0], image_buf);
							else if (nc == 2) 
								image_copy(image_key[1], image_buf);
							else if (nc == 3) 
								image_copy(image_key[2], image_buf);
							break;
					}
					ret = write_mono(image_buf, destin);
				}
				if (ret == -1)
					printf("--File write error !\n");
				break;
			case 2:
				printf(" 1: Undo\n");
				printf(" 2: Clear (MAIN)\n");
				printf(" 3: Clear (SUB)\n");
				printf(" 4: Clear (KEY)\n");
				printf(" 5: Copy (MAIN -> SUB)\n");
				printf(" 6: Copy (MAIN -> KEY)\n");
				printf(" 7: Copy (SUB  -> MAIN)\n");
				printf(" 8: Copy (KEY  -> MAIN)\n");
				printf(" 9: Inverse\n");
				printf("10: Color -> Mono\n");
				printf("11: Each Color\n");
				printf("12: MAIN + SUB\n");
				printf("13: MAIN - SUB\n");
				printf("14: |MAIN + SUB|\n");
				printf("15: MAIN * SUB\n");
				printf("16: MAIN | SUB\n");
				printf("17: MAIN & SUB   ?  ");
				scanf("%d", &command_sub);
				switch (command_sub) {
					case 1:
						image_copy_color(image_prev, image_main);
						DisplayCImage(image_main, WMAIN);
						break;
					case 2:
						image_clear_color(image_main);
						DisplayCImage(image_main, WMAIN);
						break;
					case 3:
						image_clear_color(image_sub);
						DisplayCImage(image_sub, WSUB);
						break;
					case 4:
						image_clear_color(image_key);
						DisplayCImage(image_key, WKEY);
						break;
					case 5:
						image_copy_color(image_main, image_sub);
						DisplayCImage(image_sub, WSUB);
						break;
					case 6:
						image_copy_color(image_main, image_key);
						DisplayCImage(image_key, WKEY);
						break;
					case 7:
						image_copy_color(image_sub, image_main);
						DisplayCImage(image_main, WMAIN);
						break;
					case 8:
						image_copy_color(image_key, image_main);
						DisplayCImage(image_main, WMAIN);
						break;
					case 9:
						image_copy_color(image_main, image_prev);
						DisplayCImage(image_prev, WPREV);
						image_negative_color(image_main);
						DisplayCImage(image_main, WMAIN);
						break;
					case 10:
						image_copy_color(image_main, image_prev);
						DisplayCImage(image_prev, WPREV);
						color_to_mono(image_main);
						DisplayCImage(image_main, WMAIN);
						break;
					case 11:
						printf(" 1:R clear \n");
						printf(" 2:G clear \n");
						printf(" 3:B clear \n");
						printf(" 4:R -> RGB\n");
						printf(" 5:G -> RGB\n");
						printf(" 6:B -> RGB\n");
						printf(" 7:R + G + B\n");
						printf(" 8:R | G | B\n");
						printf(" 9:R & G & B   ?  ");
						scanf("%d", &n);
						image_copy_color(image_main, image_prev);
						DisplayCImage(image_prev, WPREV);
						switch (n) {
							case 1:
								image_clear(image_main[0]);
								break;
							case 2:
								image_clear(image_main[1]);
								break;
							case 3:
								image_clear(image_main[2]);
								break;
							case 4:
								image_copy(image_main[0], image_main[1]);
								image_copy(image_main[0], image_main[2]);
								break;
							case 5:
								image_copy(image_main[1], image_main[0]);
								image_copy(image_main[1], image_main[2]);
								break;
							case 6:
								image_copy(image_main[2], image_main[0]);
								image_copy(image_main[2], image_main[1]);
								break;
							case 7:
								image_addition_rgb(image_main);
								break;
							case 8:
								image_or_rgb(image_main);
								break;
							case 9:
								image_and_rgb(image_main);
								break;
						}
						DisplayCImage(image_main, WMAIN);
						break;
					case 12:
						image_copy_color(image_main, image_prev);
		                DisplayCImage(image_prev, WPREV);
						image_addition_color(image_prev, image_sub, image_main);
						DisplayCImage(image_main, WMAIN);
						break;
					case 13:
						image_copy_color(image_main, image_prev);
						DisplayCImage(image_prev, WPREV);
						image_subtraction_color(image_prev, 
							image_sub, image_main);
						DisplayCImage(image_main, WMAIN);
						break;
					case 14:
						image_copy_color(image_main, image_prev);
						DisplayCImage(image_prev, WPREV);
						image_difference_color(image_prev, 
							image_sub, image_main);
						DisplayCImage(image_main, WMAIN);
						break;
					case 15:
						image_copy_color(image_main, image_prev);
						DisplayCImage(image_prev, WPREV);
						image_multiplication_color(image_prev, 
							image_sub, image_main);
						DisplayCImage(image_main, WMAIN);
						break;
					case 16:
						image_copy_color(image_main, image_prev);
						DisplayCImage(image_prev, WPREV);
						image_or_color(image_prev, image_sub, image_main);
						DisplayCImage(image_main, WMAIN);
						break;
					case 17:
						image_copy_color(image_main, image_prev);
						DisplayCImage(image_prev, WPREV);
						image_and_color(image_prev, image_sub, image_main);
						DisplayCImage(image_main, WMAIN);
						break;
				}
				break;
			case 3:
				printf(" 1:Display y, hue, sat\n");
				printf(" 2:Change R,  G, B (manual)\n");
				printf(" 3:Change Y, Sat, Hue (manual)\n");
				printf(" 4:Change R,  G, B (auto)\n");
				printf(" 5:Change Y, Sat, Hue (auto)\n");
				printf(" 6:Pseudo color\n");
				printf(" 7:Colorbar   ?  ");
				scanf("%d", &command_sub);
				image_copy_color(image_main, image_prev);
				DisplayCImage(image_prev, WPREV);
				switch (command_sub) {
					case 1:
						printf("  Initial hue ?  ");
						scanf("%d", &org);
						if (org < 0) break;
						rgb_to_ysh(image_prev, image_ysh);
						y_image(image_ysh[0], image_main[0]);
						sat_image(image_ysh[1], image_sub[0]);
						hue_image(image_ysh[1], image_ysh[2], 
							image_key[0], org);
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
					case 2:
						printf("  R-gain R-bias G-gain G-bias B-gain ");
						printf("B-bias (double) ? ? ? ? ? ?  ");
						scanf("%lf %lf %lf %lf %lf %lf", &gain[0], &bias[0], 
								&gain[1], &bias[1], &gain[2], &bias[2]);
						amplify(image_prev[0], image_main[0], gain[0], bias[0]);
						amplify(image_prev[1], image_main[1], gain[1], bias[1]);
						amplify(image_prev[2], image_main[2], gain[2], bias[2]);
						DisplayCImage(image_main, WMAIN);
						break;
					case 3:
						printf("  Y-gain Y-bias Sat-gain Sat-bias ");
						printf("Hue-bias (double) ? ? ? ? ?  ");
						scanf("%lf %lf %lf %lf %lf", &ya, &yb, &sa, &sb, &hb); 
						rgb_to_ysh(image_prev, image_ysh);
						tran_ysh(image_ysh,image_ysh, ya, yb, sa, sb, hb);
						ysh_to_rgb(image_ysh, image_main);
						DisplayCImage(image_main, WMAIN);
						break;
					case 4:
						printf("  Type (1:RGB each, 2:RGB same) ?  "); 
						scanf("%d", &type);
						expand_rgb(image_prev, image_main, type-1);
						DisplayCImage(image_main, WMAIN);
						break;
					case 5:
						printf("  Type (1:Y, 2:Y & Sat) ?  "); 
						scanf("%d", &type);
						rgb_to_ysh(image_prev, image_ysh);
						expand_ysh(image_ysh, image_ysh, type-1);
						ysh_to_rgb(image_ysh, image_main);
						DisplayCImage(image_main, WMAIN);
						break;
					case 6:
						printf("  Type (1:Method-a, 2:Method-b) ?  "); 
						scanf("%d", &type);
						pseudo_color(image_prev[0], image_main[0], 
							image_main[1], image_main[2], type);
						DisplayCImage(image_main, WMAIN);
						break;
					case 7:
						colorbar(image_main, 255);
						DisplayCImage(image_main, WMAIN);
						break;
				}
				break;
			case 4:
				printf(" 1:Thresolding R\n");
				printf(" 2:Thresolding G\n");
				printf(" 3:Thresolding B\n");
				printf(" 4:Thresolding RGB\n");
				printf(" 5:Thresolding Y\n");
				printf(" 6:Thresolding Sat\n");
				printf(" 7:Thresolding Hue\n");
				printf(" 8:Thresolding Y, Sat, Hue\n");
				printf(" 9:Thresolding Color Difference\n");
				printf("10:Masking\n");
				printf("11:Key Inverse\n");
				printf("12:Noise Reduction\n");
				printf("13:Noise Add\n");
				printf("14:Gradient\n");
				printf("15:Template\n");
				printf("16:Laplacian\n");
				printf("17:Zero Cross\n");
				printf("18:LoG\n");
				printf("19:Thinning   ?  ");
				scanf("%d", &command_sub);
				n = 0;
				if  (command_sub == 1 || command_sub == 2 || command_sub == 3
				  || command_sub == 5 || command_sub == 6 || command_sub == 7) {
					printf(" 1:Histgram\n");
					printf(" 2:Histgram Print\n");
					printf(" 3:Histgram Smooth\n");
					printf(" 4:Thresholding (Manual)\n");
					printf(" 5:Thresholding (Mode)\n");
					printf(" 6:Thresholding (Discriminant Analysis)\n");
					printf(" 7:Thresholding (Dynamic)   ?  ");
					scanf("%d", &n);
				}
				if (command_sub == 4) {
					printf(" 1:RGB Histgram\n");
					printf(" 2:RGB Histgram Smooth\n");
					printf(" 3:2D Histgram R-B\n");
					printf(" 4:2D Histgram G-R\n");
					printf(" 5:2D Histgram G-B\n");
					printf(" 6:RGB Thresholding   ?  ");
					scanf("%d", &n);
				}
				if (command_sub == 8) {
					printf(" 1:YSH Histgram\n");
					printf(" 2:YSH Histgram Smooth\n");
					printf(" 3:YSH Thresholding   ?  ");
					scanf("%d", &n);
				}
				if (command_sub == 9) {
					printf(" 1:Red\n");
					printf(" 2:Green\n");
					printf(" 3:Blue   ?  ");
					scanf("%d", &n);
				}
				if (command_sub == 12) {
					printf(" 1:Smooth\n");
					printf(" 2:Smooth (with weight)\n");
					printf(" 3:Smooth (edge preserve)\n");
					printf(" 4:median\n");
					printf(" 5:Dilation\n");
					printf(" 6:Erosion   ?  ");
					scanf("%d", &n);
				}
				if (command_sub == 13) {
					printf(" 1:Random\n");
					printf(" 2:Spike   ?  ");
					scanf("%d", &n);
				}
				if (command_sub == 14 || command_sub == 15) {
					printf(" 1:Red, Green, Blue\n");
					printf(" 2:Red\n");
					printf(" 3:Green\n");
					printf(" 4:Blue\n");
					printf(" 5:Red + Green + Blue   ?  ");
					scanf("%d", &n);
				}
				if (command_sub == 16 || command_sub == 18) {
					printf(" 1:Red, Green, Blue\n");
					printf(" 2:Red\n");
					printf(" 3:Green\n");
					printf(" 4:Blue   ?  ");
					scanf("%d", &n);
				}
				switch (command_sub * 100 + n) {
					case 101:
						histgram(image_main[0], hist[0]);
						histimage(hist[0], image_sub[0]);
						image_clear(image_sub[1]);
						image_clear(image_sub[2]);
						DisplayCImage(image_sub, WSUB);
	                	break;
					case 102:
						histprint(hist[0], text_buf);
						printf("************ Histgram  ************\n");
						printf("%s\n", text_buf);
						break;
					case 103:
						histsmooth(hist[0], histbuf);
						for (i = 0; i < 256; i++) hist[0][i] = histbuf[i];
						histimage(hist[0], image_sub[0]);
						image_clear(image_sub[1]);
						image_clear(image_sub[2]);
						DisplayCImage(image_sub, WSUB);
                    	break;
					case 104:
						printf("  Threshold ?  ");
						scanf("%d", &thres);
						printf("  Type (1:Normal, 2:Inverse) ?  "); 
						scanf("%d", &type);
                    	image_copy_color(image_main, image_prev);
                    	DisplayCImage(image_prev, WPREV);
                    	threshold(image_prev[0], image_main[0], thres, type);
						image_copy(image_main[0], image_main[1]);
						image_copy(image_main[0], image_main[2]);
                    	DisplayCImage(image_main, WMAIN);
						break;
					case 105:
						printf("  Smooth Count ?  ");
						scanf("%d", &smt);
						printf("  Type (1:Normal, 2:Inverse) ?  "); 
						scanf("%d", &type);
                    	image_copy_color(image_main, image_prev);
                    	DisplayCImage(image_prev, WPREV);
                    	threshold_mode(image_prev[0], image_main[0], smt, type);
						image_copy(image_main[0], image_main[1]);
						image_copy(image_main[0], image_main[2]);
                    	DisplayCImage(image_main, WMAIN);
						break;
					case 106:
						printf("  Type (1:Normal, 2:Inverse) ?  "); 
						scanf("%d", &type);
                    	image_copy_color(image_main, image_prev);
                    	DisplayCImage(image_prev, WPREV);
                    	threshold_discrim(image_prev[0], image_main[0], type);
						image_copy(image_main[0], image_main[1]);
						image_copy(image_main[0], image_main[2]);
                    	DisplayCImage(image_main, WMAIN);
						break;
					case 107:
						printf("  Type (1:Normal, 2:Inverse) ?  "); 
						scanf("%d", &type);
                    	image_copy_color(image_main, image_prev);
                    	DisplayCImage(image_prev, WPREV);
                    	threshold_dynamic(image_prev[0], image_main[0], type);
						image_copy(image_main[0], image_main[1]);
						image_copy(image_main[0], image_main[2]);
                    	DisplayCImage(image_main, WMAIN);
						break;
					case 201:
						histgram(image_main[1], hist[1]);
						histimage(hist[1], image_sub[1]);
						image_clear(image_sub[0]);
						image_clear(image_sub[2]);
	                	DisplayCImage(image_sub, WSUB);
	                	break;
					case 202:
						histprint(hist[1], text_buf);
						printf("************ Histgram  ************\n");
						printf("%s\n", text_buf);
						break;
					case 203:
						histsmooth(hist[1], histbuf);
						for (i = 0; i < 256; i++) hist[1][i] = histbuf[i];
						histimage(hist[1], image_sub[1]);
						image_clear(image_sub[0]);
						image_clear(image_sub[2]);
						DisplayCImage(image_sub, WSUB);
                    	break;
					case 204:
						printf("  Threshold ?  ");
						scanf("%d", &thres);
						printf("  Type (1:Normal, 2:Inverse) ?  "); 
						scanf("%d", &type);
                    	image_copy_color(image_main, image_prev);
                    	DisplayCImage(image_prev, WPREV);
                    	threshold(image_prev[1], image_main[1], thres, type);
						image_copy(image_main[1], image_main[0]);
						image_copy(image_main[1], image_main[2]);
                    	DisplayCImage(image_main, WMAIN);
						break;
					case 205:
						printf("  Smooth Count ?  ");
						scanf("%d", &smt);
						printf("  Type (1:Normal, 2:Inverse) ?  "); 
						scanf("%d", &type);
                    	image_copy_color(image_main, image_prev);
                    	DisplayCImage(image_prev, WPREV);
                    	threshold_mode(image_prev[1], image_main[1], smt, type);
						image_copy(image_main[1], image_main[0]);
						image_copy(image_main[1], image_main[2]);
                    	DisplayCImage(image_main, WMAIN);
						break;
					case 206:
						printf("  Type (1:Normal, 2:Inverse) ?  "); 
						scanf("%d", &type);
                    	image_copy_color(image_main, image_prev);
                    	DisplayCImage(image_prev, WPREV);
                    	threshold_discrim(image_prev[1], image_main[1], type);
						image_copy(image_main[1], image_main[0]);
						image_copy(image_main[1], image_main[2]);
                    	DisplayCImage(image_main, WMAIN);
						break;
					case 207:
						printf("  Type (1:Normal, 2:Inverse) ?  "); 
						scanf("%d", &type);
                    	image_copy_color(image_main, image_prev);
                    	DisplayCImage(image_prev, WPREV);
                    	threshold_dynamic(image_prev[1], image_main[1], type);
						image_copy(image_main[1], image_main[0]);
						image_copy(image_main[1], image_main[2]);
                    	DisplayCImage(image_main, WMAIN);
						break;
					case 301:
						histgram(image_main[2], hist[2]);
						histimage(hist[2], image_sub[2]);
						image_clear(image_sub[0]);
						image_clear(image_sub[1]);
	                	DisplayCImage(image_sub, WSUB);
	                	break;
					case 302:
						histprint(hist[2], text_buf);
						printf("************ Histgram  ************\n");
						printf("%s\n", text_buf);
						break;
					case 303:
						histsmooth(hist[2], histbuf);
						for (i = 0; i < 256; i++) hist[2][i] = histbuf[i];
						histimage(hist[2], image_sub[2]);
						image_clear(image_sub[0]);
						image_clear(image_sub[1]);
						DisplayCImage(image_sub, WSUB);
                    	break;
					case 304:
						printf("  Threshold ?  ");
						scanf("%d", &thres);
						printf("  Type (1:Normal, 2:Inverse) ?  "); 
						scanf("%d", &type);
                    	image_copy_color(image_main, image_prev);
                    	DisplayCImage(image_prev, WPREV);
                    	threshold(image_prev[2], image_main[2], thres, type);
						image_copy(image_main[2], image_main[0]);
						image_copy(image_main[2], image_main[1]);
                    	DisplayCImage(image_main, WMAIN);
						break;
					case 305:
						printf("  Smooth Count ?  ");
						scanf("%d", &smt);
						printf("  Type (1:Normal, 2:Inverse) ?  "); 
						scanf("%d", &type);
                    	image_copy_color(image_main, image_prev);
                    	DisplayCImage(image_prev, WPREV);
                    	threshold_mode(image_prev[2], image_main[2], smt, type);
						image_copy(image_main[2], image_main[0]);
						image_copy(image_main[2], image_main[1]);
                    	DisplayCImage(image_main, WMAIN);
						break;
					case 306:
						printf("  Type (1:Normal, 2:Inverse) ?  "); 
						scanf("%d", &type);
                    	image_copy_color(image_main, image_prev);
                    	DisplayCImage(image_prev, WPREV);
                    	threshold_discrim(image_prev[2], image_main[2], type);
						image_copy(image_main[2], image_main[0]);
						image_copy(image_main[2], image_main[1]);
                    	DisplayCImage(image_main, WMAIN);
						break;
					case 307:
						printf("  Type (1:Normal, 2:Inverse) ?  "); 
						scanf("%d", &type);
                    	image_copy_color(image_main, image_prev);
                    	DisplayCImage(image_prev, WPREV);
                    	threshold_dynamic(image_prev[2], image_main[2], type);
						image_copy(image_main[2], image_main[0]);
						image_copy(image_main[2], image_main[1]);
                    	DisplayCImage(image_main, WMAIN);
						break;
					case 401:
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
					case 402:
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
					case 403:
						hist2_image(image_main[0], image_main[2], image_sub[0]);
						image_copy(image_sub[0], image_sub[1]);
						image_copy(image_sub[0], image_sub[2]);
						DisplayCImage(image_sub, WSUB);
						break;
					case 404:
						hist2_image(image_main[1], image_main[0], image_sub[0]);
						image_copy(image_sub[0], image_sub[1]);
						image_copy(image_sub[0], image_sub[2]);
						DisplayCImage(image_sub, WSUB);
						break;
					case 405:
						hist2_image(image_main[1], image_main[2], image_sub[0]);
						image_copy(image_sub[0], image_sub[1]);
						image_copy(image_sub[0], image_sub[2]);
						DisplayCImage(image_sub, WSUB);
						break;
					case 406:
						printf("Threshold\n");
						printf("R-min R-max G-min G-max G-main G-max ");
						printf("? ? ? ? ? ?  ");
						scanf("%d %d %d %d %d %d", &thres_rmin, &thres_rmax, 
							&thres_gmin, &thres_gmax, &thres_bmin, &thres_bmax);
						image_copy_color(image_main, image_prev);
						DisplayCImage(image_prev, WPREV);
						thresh_rgb(image_prev, image_main[0], thres_rmin, 
							thres_rmax, thres_gmin, thres_gmax, 
							thres_bmin, thres_bmax);
						image_copy(image_main[0], image_main[1]);
						image_copy(image_main[0], image_main[2]);
						DisplayCImage(image_main, WMAIN);
						break;
					case 501:
						rgb_to_ysh(image_main, image_ysh);
						y_image(image_ysh[0], image_buf);
						histgram(image_buf, hist[0]);
						histimage(hist[0], image_sub[0]);
						image_copy(image_sub[0], image_sub[1]);
						image_copy(image_sub[0], image_sub[2]);
	                	DisplayCImage(image_sub, WSUB);
	                	break;
					case 502:
						histprint(hist[0], text_buf);
						printf("************ Histgram  ************\n");
						printf("%s\n", text_buf);
						break;
					case 503:
						histsmooth(hist[0], histbuf);
						for (i = 0; i < 256; i++) hist[0][i] = histbuf[i];
						histimage(hist[0], image_sub[0]);
						image_clear(image_sub[1]);
						image_clear(image_sub[2]);
						DisplayCImage(image_sub, WSUB);
                    	break;
					case 504:
						printf("  Threshold ?  ");
						scanf("%d", &thres);
						printf("  Type (1:Normal, 2:Inverse) ?  "); 
						scanf("%d", &type);
                    	image_copy_color(image_main, image_prev);
                    	DisplayCImage(image_prev, WPREV);
						rgb_to_ysh(image_prev, image_ysh);
						y_image(image_ysh[0], image_buf);
                    	threshold(image_buf, image_main[0], thres, type);
						image_copy(image_main[0], image_main[1]);
						image_copy(image_main[0], image_main[2]);
                    	DisplayCImage(image_main, WMAIN);
						break;
					case 505:
						printf("  Smooth Count ?  ");
						scanf("%d", &smt);
						printf("  Type (1:Normal, 2:Inverse) ?  "); 
						scanf("%d", &type);
                    	image_copy_color(image_main, image_prev);
                    	DisplayCImage(image_prev, WPREV);
						rgb_to_ysh(image_prev, image_ysh);
						y_image(image_ysh[0], image_buf);
                    	threshold_mode(image_buf, image_main[0], smt, type);
						image_copy(image_main[0], image_main[1]);
						image_copy(image_main[0], image_main[2]);
                    	DisplayCImage(image_main, WMAIN);
						break;
					case 506:
						printf("  Type (1:Normal, 2:Inverse) ?  "); 
						scanf("%d", &type);
                    	image_copy_color(image_main, image_prev);
                    	DisplayCImage(image_prev, WPREV);
						rgb_to_ysh(image_prev, image_ysh);
						y_image(image_ysh[0], image_buf);
                    	threshold_discrim(image_buf, image_main[0], type);
						image_copy(image_main[0], image_main[1]);
						image_copy(image_main[0], image_main[2]);
                    	DisplayCImage(image_main, WMAIN);
						break;
					case 507:
						printf("  Type (1:Normal, 2:Inverse) ?  "); 
						scanf("%d", &type);
                    	image_copy_color(image_main, image_prev);
                    	DisplayCImage(image_prev, WPREV);
						rgb_to_ysh(image_prev, image_ysh);
						y_image(image_ysh[0], image_buf);
                    	threshold_dynamic(image_buf, image_main[0], type);
						image_copy(image_main[0], image_main[1]);
						image_copy(image_main[0], image_main[2]);
                    	DisplayCImage(image_main, WMAIN);
						break;
					case 601:
						rgb_to_ysh(image_main, image_ysh);
						sat_image(image_ysh[1], image_buf);
						histgram(image_buf, hist[1]);
						histimage(hist[1], image_sub[0]);
						image_copy(image_sub[0], image_sub[1]);
						image_copy(image_sub[0], image_sub[2]);
	                	DisplayCImage(image_sub, WSUB);
	                	break;
					case 602:
						histprint(hist[1], text_buf);
						printf("************ Histgram  ************\n");
						printf("%s\n", text_buf);
						break;
					case 603:
						histsmooth(hist[1], histbuf);
						for (i = 0; i < 256; i++) hist[1][i] = histbuf[i];
						histimage(hist[1], image_sub[0]);
						image_copy(image_sub[0], image_sub[1]);
						image_copy(image_sub[0], image_sub[2]);
						DisplayCImage(image_sub, WSUB);
                    	break;
					case 604:
						printf("  Threshold ?  ");
						scanf("%d", &thres);
						printf("  Type (1:Normal, 2:Inverse) ?  "); 
						scanf("%d", &type);
                    	image_copy_color(image_main, image_prev);
                    	DisplayCImage(image_prev, WPREV);
						rgb_to_ysh(image_prev, image_ysh);
						sat_image(image_ysh[1], image_buf);
                    	threshold(image_buf, image_main[0], thres, type);
						image_copy(image_main[0], image_main[1]);
						image_copy(image_main[0], image_main[2]);
                    	DisplayCImage(image_main, WMAIN);
						break;
					case 605:
						printf("  Smooth Count ?  ");
						scanf("%d", &smt);
						printf("  Type (1:Normal, 2:Inverse) ?  "); 
						scanf("%d", &type);
                    	image_copy_color(image_main, image_prev);
                    	DisplayCImage(image_prev, WPREV);
						rgb_to_ysh(image_prev, image_ysh);
						sat_image(image_ysh[1], image_buf);
                    	threshold_mode(image_buf, image_main[0], smt, type);
						image_copy(image_main[0], image_main[1]);
						image_copy(image_main[0], image_main[2]);
                    	DisplayCImage(image_main, WMAIN);
						break;
					case 606:
						printf("  Type (1:Normal, 2:Inverse) ?  "); 
						scanf("%d", &type);
                    	image_copy_color(image_main, image_prev);
                    	DisplayCImage(image_prev, WPREV);
						rgb_to_ysh(image_prev, image_ysh);
						sat_image(image_ysh[1], image_buf);
                    	threshold_discrim(image_buf, image_main[0], type);
						image_copy(image_main[0], image_main[1]);
						image_copy(image_main[0], image_main[2]);
                    	DisplayCImage(image_main, WMAIN);
						break;
					case 607:
						printf("  Type (1:Normal, 2:Inverse) ?  "); 
						scanf("%d", &type);
	                    image_copy_color(image_main, image_prev);
	                    DisplayCImage(image_prev, WPREV);
						rgb_to_ysh(image_prev, image_ysh);
						sat_image(image_ysh[1], image_buf);
	                    threshold_dynamic(image_buf, image_main[0], type);
						image_copy(image_main[0], image_main[1]);
						image_copy(image_main[0], image_main[2]);
                    	DisplayCImage(image_main, WMAIN);
						break;
					case 701:
						printf("  Initial hue ?  ");
						scanf("%d", &org);
						rgb_to_ysh(image_main, image_ysh);
						hue_image(image_ysh[1], image_ysh[2], 
							image_buf, org);
						histgram(image_buf, hist[2]);
						histimage(hist[2], image_sub[0]);
						image_copy(image_sub[0], image_sub[1]);
						image_copy(image_sub[0], image_sub[2]);
	                	DisplayCImage(image_sub, WSUB);
	                	break;
					case 702:
						histprint(hist[2], text_buf);
						printf("************ Histgram  ************\n");
						printf("%s\n", text_buf);
						break;
					case 703:
						histsmooth(hist[2], histbuf);
						for (i = 0; i < 256; i++) hist[2][i] = histbuf[i];
						histimage(hist[2], image_sub[0]);
						image_copy(image_sub[0], image_sub[1]);
						image_copy(image_sub[0], image_sub[2]);
						DisplayCImage(image_sub, WSUB);
                    	break;
					case 704:
						printf("  Initial hue ?  ");
						scanf("%d", &org);
						printf("  Threshold ?  ");
						scanf("%d", &thres);
						printf("  Type (1:Normal, 2:Inverse) ?  "); 
						scanf("%d", &type);
                    	image_copy_color(image_main, image_prev);
                    	DisplayCImage(image_prev, WPREV);
						rgb_to_ysh(image_prev, image_ysh);
						hue_image(image_ysh[1], image_ysh[2], 
							image_buf, org);
                    	threshold(image_buf, image_main[0], thres, type);
						image_copy(image_main[0], image_main[1]);
						image_copy(image_main[0], image_main[2]);
                    	DisplayCImage(image_main, WMAIN);
						break;
					case 705:
						printf("  Smooth Count ?  ");
						scanf("%d", &smt);
						printf("  Initial hue ?  ");
						scanf("%d", &org);
						printf("  Type (1:Normal, 2:Inverse) ?  "); 
						scanf("%d", &type);
                    	image_copy_color(image_main, image_prev);
                    	DisplayCImage(image_prev, WPREV);
						rgb_to_ysh(image_prev, image_ysh);
						hue_image(image_ysh[1], image_ysh[2], 
							image_buf, org);
                    	threshold_mode(image_buf, image_main[0], smt, type);
						image_copy(image_main[0], image_main[1]);
						image_copy(image_main[0], image_main[2]);
                    	DisplayCImage(image_main, WMAIN);
						break;
					case 706:
						printf("  Initial hue ?  ");
						scanf("%d", &org);
						printf("  Type (1:Normal, 2:Inverse) ?  "); 
						scanf("%d", &type);
                    	image_copy_color(image_main, image_prev);
                    	DisplayCImage(image_prev, WPREV);
						rgb_to_ysh(image_prev, image_ysh);
						hue_image(image_ysh[1], image_ysh[2], 
							image_buf, org);
                    	threshold_discrim(image_buf, image_main[0], type);
						image_copy(image_main[0], image_main[1]);
						image_copy(image_main[0], image_main[2]);
                    	DisplayCImage(image_main, WMAIN);
						break;
					case 707:
						printf("  Initial hue ?  ");
						scanf("%d", &org);
						printf("  Type (1:Normal, 2:Inverse) ?  "); 
						scanf("%d", &type);
                    	image_copy_color(image_main, image_prev);
                    	DisplayCImage(image_prev, WPREV);
						rgb_to_ysh(image_prev, image_ysh);
						hue_image(image_ysh[1], image_ysh[2], 
							image_buf, org);
                    	threshold_dynamic(image_buf, image_main[0], type);
						image_copy(image_main[0], image_main[1]);
						image_copy(image_main[0], image_main[2]);
                    	DisplayCImage(image_main, WMAIN);
						break;
					case 801:
						printf("  Initial hue ?  ");
						scanf("%d", &org);
                    	image_copy_color(image_main, image_prev);
						DisplayCImage(image_prev, WPREV);
						rgb_to_ysh(image_prev, image_ysh);
						y_image(image_ysh[0], image_buf);
						histgram(image_buf, hist[0]);
						sat_image(image_ysh[1], image_buf);
						histgram(image_buf, hist[1]);
						hue_image(image_ysh[1], image_ysh[2], 
							image_buf, org);
						histgram(image_buf, hist[2]);
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
					case 802:
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
					case 803:
						printf("Threshold\n");
						printf("Y-min Y-max Sat-min Sat-max ");
						printf("Hue-main Hue-max  ? ");
						scanf("%d %d %d %d %d %d", &thres_ymin, &thres_ymax, 
							&thres_smin, &thres_smax, &thres_hmin, &thres_hmax);
						image_copy_color(image_main, image_prev);
						DisplayCImage(image_prev, WPREV);
						rgb_to_ysh(image_prev, image_ysh);
						thresh_ysh(image_ysh, image_main[0], thres_ymin, 
							thres_ymax, thres_smin, thres_smax, 
							thres_hmin, thres_hmax);
						image_copy(image_main[0], image_main[1]);
						image_copy(image_main[0], image_main[2]);
						DisplayCImage(image_main, WMAIN);
						break;
					case 901:
						printf("Threshold ?  ");
						scanf("%d", &thres);
                    	image_copy_color(image_main, image_prev);
                    	DisplayCImage(image_prev, WPREV);
						thresh_color_difference(image_prev, image_main[0], 
							thres, 1);
						image_copy(image_main[0], image_main[1]);
						image_copy(image_main[0], image_main[2]);
						DisplayCImage(image_main, WMAIN);
						break;
					case 902:
						printf("Threshold ?  ");
						scanf("%d", &thres);
                    	image_copy_color(image_main, image_prev);
                    	DisplayCImage(image_prev, WPREV);
						thresh_color_difference(image_prev, image_main[0], 
							thres, 2);
						image_copy(image_main[0], image_main[1]);
						image_copy(image_main[0], image_main[2]);
						DisplayCImage(image_main, WMAIN);
						break;
					case 903:
						printf("Threshold ?  ");
						scanf("%d", &thres);
                    	image_copy_color(image_main, image_prev);
                    	DisplayCImage(image_prev, WPREV);
						thresh_color_difference(image_prev, image_main[0], 
							thres, 3);
						image_copy(image_main[0], image_main[1]);
						image_copy(image_main[0], image_main[2]);
						DisplayCImage(image_main, WMAIN);
						break;
					case 1000:
                    	image_copy_color(image_main, image_prev);
                    	DisplayCImage(image_prev, WPREV);
						for (i = 0; i < 3; i++)
	                    	masking(image_prev[i], image_main[i], image_key[i]);
                    	DisplayCImage(image_main, WMAIN);
						break;
					case 1100:
                    	image_negative_color(image_key);
                    	DisplayCImage(image_key, WKEY);
						break;
					case 1201:
						printf("  Type (1:3x3, 2:5x5) ?  "); 
						scanf("%d", &type);
						image_copy_color(image_main, image_prev);
						DisplayCImage(image_prev, WPREV);
						for (i = 0; i < 3; i++)
							smooth(image_prev[i], image_main[i], type*2+1);
						DisplayCImage(image_main, WMAIN);
						break;
					case 1202:
						printf("  Weight Factor (1:type1, 2:type2, 3:type3) ");
						printf("?  ");
						scanf("%d", &type);
						image_copy_color(image_main, image_prev);
						DisplayCImage(image_prev, WPREV);
						for (i = 0; i < 3; i++)
							smooth_weighted(image_prev[i], image_main[i], 
								type-1);
						DisplayCImage(image_main, WMAIN);
						break;
					case 1203:
						image_copy_color(image_main, image_prev);
						DisplayCImage(image_prev, WPREV);
						for (i = 0; i < 3; i++)
							smooth_edge_preserve(image_prev[i], image_main[i]);
						DisplayCImage(image_main, WMAIN);
						break;
					case 1204:
						image_copy_color(image_main, image_prev);
						DisplayCImage(image_prev, WPREV);
						for (i = 0; i < 3; i++)
							median(image_prev[i], image_main[i]);
						DisplayCImage(image_main, WMAIN);
						break;
					case 1205:
						image_copy_color(image_main, image_prev);
                    	DisplayCImage(image_prev, WPREV);
						for (i = 0; i < 3; i++)
	                    	dilation(image_prev[i], image_main[i]);
                    	DisplayCImage(image_main, WMAIN);
						break;
					case 1206:
						image_copy_color(image_main, image_prev);
                    	DisplayCImage(image_prev, WPREV);
						for (i = 0; i < 3; i++)
	                    	erosion(image_prev[i], image_main[i]);
                    	DisplayCImage(image_main, WMAIN);
						break;
					case 1301:
						printf("Noise Level ?   ");
						scanf("%d", &lvl);
						image_copy_color(image_main, image_prev);
                    	DisplayCImage(image_prev, WPREV);
						for (i = 0; i < 3; i++)
							noise_rand(image_prev[i], image_main[i], lvl);
                    	DisplayCImage(image_main, WMAIN);
						break;
					case 1302:
						printf("Noise Number,  Level ? ?  ");
						scanf("%d %d", &num, &lvl);
						image_copy_color(image_main, image_prev);
						DisplayCImage(image_prev, WPREV);
						for (i = 0; i < 3; i++)
							noise_spike(image_prev[i], image_main[i], num, lvl);
                    	DisplayCImage(image_main, WMAIN);
						DisplayCImage(image_main, WMAIN);
						break;
					case 1401:
						printf("  Amplitude (double) ?  ");
						scanf("%lf", &amp);
                    	image_copy_color(image_main, image_prev);
                    	DisplayCImage(image_prev, WPREV);
						for (i = 0; i < 3; i++)
	                    	gradient_sobel(image_prev[i], image_main[i], amp);
                    	DisplayCImage(image_main, WMAIN);
						break;
					case 1402:
						printf("  Amplitude (double) ?  ");
						scanf("%lf", &amp);
                    	image_copy_color(image_main, image_prev);
                    	DisplayCImage(image_prev, WPREV);
						gradient_sobel(image_prev[0], image_main[0], amp);
						image_copy(image_main[0], image_main[1]);
						image_copy(image_main[0], image_main[2]);
                    	DisplayCImage(image_main, WMAIN);
						break;
					case 1403:
						printf("  Amplitude (double) ?  ");
						scanf("%lf", &amp);
                    	image_copy_color(image_main, image_prev);
                    	DisplayCImage(image_prev, WPREV);
						gradient_sobel(image_prev[1], image_main[0], amp);
						image_copy(image_main[0], image_main[1]);
						image_copy(image_main[0], image_main[2]);
                    	DisplayCImage(image_main, WMAIN);
						break;
					case 1404:
						printf("  Amplitude (double) ?  ");
						scanf("%lf", &amp);
                    	image_copy_color(image_main, image_prev);
                    	DisplayCImage(image_prev, WPREV);
						gradient_sobel(image_prev[2], image_main[0], amp);
						image_copy(image_main[0], image_main[1]);
						image_copy(image_main[0], image_main[2]);
                    	DisplayCImage(image_main, WMAIN);
						break;
					case 1405:
						printf("  Amplitude (double) ?  ");
						scanf("%lf", &amp);
                    	image_copy_color(image_main, image_prev);
                    	DisplayCImage(image_prev, WPREV);
						for (i = 0; i < 3; i++)
	                    	gradient_sobel(image_prev[i], image_main[i], amp);
						image_addition_rgb(image_main);
						DisplayCImage(image_main, WMAIN);
						break;
					case 1501:
						printf("  Amplitude (double) ?  ");
						scanf("%lf", &amp);
                    	image_copy_color(image_main, image_prev);
                    	DisplayCImage(image_prev, WPREV);
						for (i = 0; i < 3; i++)
	                    	prewitt(image_prev[i], image_main[i], amp);
                    	DisplayCImage(image_main, WMAIN);
						break;
					case 1502:
						printf("  Amplitude (double) ?  ");
						scanf("%lf", &amp);
                    	image_copy_color(image_main, image_prev);
                    	DisplayCImage(image_prev, WPREV);
                    	prewitt(image_prev[0], image_main[0], amp);
						image_copy(image_main[0], image_main[1]);
						image_copy(image_main[0], image_main[2]);
                    	DisplayCImage(image_main, WMAIN);
						break;
					case 1503:
						printf("  Amplitude (double) ?  ");
						scanf("%lf", &amp);
                    	image_copy_color(image_main, image_prev);
                    	DisplayCImage(image_prev, WPREV);
                    	prewitt(image_prev[1], image_main[0], amp);
						image_copy(image_main[0], image_main[1]);
						image_copy(image_main[0], image_main[2]);
                    	DisplayCImage(image_main, WMAIN);
						break;
					case 1504:
						printf("  Amplitude (double) ?  ");
						scanf("%lf", &amp);
                    	image_copy_color(image_main, image_prev);
                    	DisplayCImage(image_prev, WPREV);
                    	prewitt(image_prev[2], image_main[0], amp);
						image_copy(image_main[0], image_main[1]);
						image_copy(image_main[0], image_main[2]);
                    	DisplayCImage(image_main, WMAIN);
						break;
					case 1505:
						printf("  Amplitude (double) ?  ");
						scanf("%lf", &amp);
                    	image_copy_color(image_main, image_prev);
                    	DisplayCImage(image_prev, WPREV);
						for (i = 0; i < 3; i++)
	                    	prewitt(image_prev[i], image_main[i], amp);
						image_addition_rgb(image_main);
                    	DisplayCImage(image_main, WMAIN);
						break;
					case 1601:
						printf("  Coefficient (1:type1, 2:type2, 3:type3) ?  ");
						scanf("%d", &type);
						printf("  Amplitude (double) ?  ");
						scanf("%lf", &amp);
                    	image_copy_color(image_main, image_prev);
                    	DisplayCImage(image_prev, WPREV);
						for (i = 0; i < 3; i++)
	                    	laplacian(image_prev[i], image_main[i], amp, type);
                    	DisplayCImage(image_main, WMAIN);
						break;
					case 1602:
						printf("  Coefficient (1:type1, 2:type2, 3:type3) ?  ");
						scanf("%d", &type);
						printf("  Amplitude (double) ?  ");
						scanf("%lf", &amp);
                    	image_copy_color(image_main, image_prev);
                    	DisplayCImage(image_prev, WPREV);
                    	laplacian(image_prev[0], image_main[0], amp, type);
						image_copy(image_main[0], image_main[1]);
						image_copy(image_main[0], image_main[2]);
                    	DisplayCImage(image_main, WMAIN);
						break;
					case 1603:
						printf("  Coefficient (1:type1, 2:type2, 3:type3) ?  ");
						scanf("%d", &type);
						printf("  Amplitude (double) ?  ");
						scanf("%lf", &amp);
                    	image_copy_color(image_main, image_prev);
                    	DisplayCImage(image_prev, WPREV);
                    	laplacian(image_prev[1], image_main[0], amp, type);
						image_copy(image_main[0], image_main[1]);
						image_copy(image_main[0], image_main[2]);
                    	DisplayCImage(image_main, WMAIN);
						break;
					case 1604:
						printf("  Coefficient (1:type1, 2:type2, 3:type3) ?  ");
						scanf("%d", &type);
						printf("  Amplitude (double) ?  ");
						scanf("%lf", &amp);
                    	image_copy_color(image_main, image_prev);
                    	DisplayCImage(image_prev, WPREV);
                    	laplacian(image_prev[2], image_main[0], amp, type);
						image_copy(image_main[0], image_main[1]);
						image_copy(image_main[0], image_main[2]);
                    	DisplayCImage(image_main, WMAIN);
						break;
					case 1700:
                    	image_copy_color(image_main, image_prev);
                    	DisplayCImage(image_prev, WPREV);
						for (i = 0; i < 3; i++)
	                    	zero_cross(image_prev[i], image_main[i]);
                    	DisplayCImage(image_main, WMAIN);
						break;
					case 1801:
						printf("  Variance (double) ?  "); 
						scanf("%lf", &a);
						image_copy_color(image_main, image_prev);
                    	DisplayCImage(image_prev, WPREV);
						for (i = 0; i < 3; i++)
	                    	log_zero_cross(image_prev[i], image_main[i], a);
                    	DisplayCImage(image_main, WMAIN);
						break;
					case 1802:
						printf("  Variance (double) ?  "); 
						scanf("%lf", &a);
						image_copy_color(image_main, image_prev);
                    	DisplayCImage(image_prev, WPREV);
						log_zero_cross(image_prev[0], image_main[0], a);
						image_copy(image_main[0], image_main[1]);
						image_copy(image_main[0], image_main[2]);
                    	DisplayCImage(image_main, WMAIN);
						break;
					case 1803:
						printf("  Variance (double) ?  "); 
						scanf("%lf", &a);
						image_copy_color(image_main, image_prev);
                    	DisplayCImage(image_prev, WPREV);
						log_zero_cross(image_prev[1], image_main[0], a);
						image_copy(image_main[0], image_main[1]);
						image_copy(image_main[0], image_main[2]);
                    	DisplayCImage(image_main, WMAIN);
						break;
					case 1804:
						printf("  Variance (double) ?  "); 
						scanf("%lf", &a);
						image_copy_color(image_main, image_prev);
                    	DisplayCImage(image_prev, WPREV);
						log_zero_cross(image_prev[2], image_main[0], a);
						image_copy(image_main[0], image_main[1]);
						image_copy(image_main[0], image_main[2]);
                    	DisplayCImage(image_main, WMAIN);
						break;
					case 1900:
                    	image_copy_color(image_main, image_prev);
                    	DisplayCImage(image_prev, WPREV);
						for (i = 0; i < 3; i++)
	                    	thinning(image_prev[i], image_main[i]);
                    	DisplayCImage(image_main, WMAIN);
						break;
				}
				break;
			case 5:
				printf(" 1:Scale (near)\n");
				printf(" 2:Scale (bilinear)\n");
				printf(" 3:Shift\n");
				printf(" 4:Rotate\n");
				printf(" 5:Scale/Rotate/Shift\n");
				printf(" 6:Affine\n");
				printf(" 7:Affine (coef)\n");
				printf(" 8:Perspective\n");
				printf(" 9:Radial Distortion\n");
				printf("10:Lattice   ?  ");
				scanf("%d", &command_sub);
				switch (command_sub) {
					case 1:
						printf("  ax, ay (double) ? ?  ");
						scanf("%lf %lf", &a, &b);
						image_copy_color(image_main, image_prev);
						DisplayCImage(image_prev, WPREV);
						for (i = 0; i < 3; i++)
							scale(image_prev[i], image_main[i], a, b);
						DisplayCImage(image_main, WMAIN);
						break;
					case 2:
						printf("  ax, ay (double) ? ?  ");
						scanf("%lf %lf", &a, &b);
						image_copy_color(image_main, image_prev);
						DisplayCImage(image_prev, WPREV);
						for (i = 0; i < 3; i++)
							scale(image_prev[i], image_main[i], a, b);
						DisplayCImage(image_main, WMAIN);
						break;
					case 3:
						printf("  x0, y0 (double) ? ?  ");
						scanf("%lf %lf", &x0, &y0);
						image_copy_color(image_main, image_prev);
						DisplayCImage(image_prev, WPREV);
						for (i = 0; i < 3; i++)
							shift(image_prev[i], image_main[i], x0, y0);
						DisplayCImage(image_main, WMAIN);
						break;
					case 4:
						printf("  deg (double) ?  ");
						scanf("%lf", &deg);
						image_copy_color(image_main, image_prev);
						DisplayCImage(image_prev, WPREV);
						for (i = 0; i < 3; i++)
							rotation(image_prev[i], image_main[i], deg);
						DisplayCImage(image_main, WMAIN);
						break;
					case 5:
						printf("  ax, ay, deg, x0, y0 (double) ? ? ? ? ?  ");
						scanf("%lf %lf %lf %lf %lf", &a, &b, &deg, &x0, &y0);
                    	image_copy_color(image_main, image_prev);
                    	DisplayCImage(image_prev, WPREV);
						for (i = 0; i < 3; i++)
							scale_rotate_shift(image_prev[i], image_main[i],
								a, b, deg, x0, y0);
                    	DisplayCImage(image_main, WMAIN);
						break;
					case 6:
						printf("  a, b, c, d, e, f (double) ? ? ? ? ? ?  ");
						scanf("%lf %lf %lf %lf %lf %lf", &a, &b, &c, &d, &e, &f);
						image_copy_color(image_main, image_prev);
						DisplayCImage(image_prev, WPREV);
						for (i = 0; i < 3; i++)
	                    	affine(image_prev[i], image_main[i], 
	                    		a, b, c, d, e, f);
						DisplayCImage(image_main, WMAIN);
						break;
					case 7:
						printf("  x1, y1, X1, Y1 ? ? ? ?  ");
						scanf("%d %d %d %d", &x1, &y1, &u1, &v1);
						printf("  x2, y2, X2, Y2 ? ? ? ?  ");
						scanf("%d %d %d %d", &x2, &y2, &u2, &v2);
						printf("  x3, y3, X3, Y3 ? ? ? ?  ");
						scanf("%d %d %d %d", &x3, &y3, &u3, &v3);
						image_copy_color(image_main, image_prev);
						DisplayCImage(image_prev, WPREV);
						affine_coef(x1, y1, u1, v1, x2, y2, u2, v2,
							x3, y3, u3, v3, &a, &b, &c, &d, &e, &f);
						for (i = 0; i < 3; i++)
	                    	affine(image_prev[i], image_main[i], 
	                    		a, b, c, d, e, f);
						DisplayCImage(image_main, WMAIN);
						break;
					case 8:
						printf("  x1, y1, X1, Y1 ? ? ? ?  ");
						scanf("%d %d %d %d", &x1, &y1, &u1, &v1);
						printf("  x2, y2, X2, Y2 ? ? ? ?  ");
						scanf("%d %d %d %d", &x2, &y2, &u2, &v2);
						printf("  x3, y3, X3, Y3 ? ? ? ?  ");
						scanf("%d %d %d %d", &x3, &y3, &u3, &v3);
						printf("  x4, y4, X4, Y4 ? ? ? ?  ");
						scanf("%d %d %d %d", &x4, &y4, &u4, &v4);
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
					case 9:
						printf("  coef_a, coef_b (double) ? ?  ");
						scanf("%lf %lf", &a, &b);
						image_copy_color(image_main, image_prev);
						DisplayCImage(image_prev, WPREV);
						for (i = 0; i < 3; i++)
							radial_distortion(image_prev[i], 
								image_main[i], a, b);
						DisplayCImage(image_main, WMAIN);
						break;
					case 10:
						image_copy_color(image_main, image_prev);
						DisplayCImage(image_prev, WPREV);
						for (i = 0; i < 3; i++) lattice(image_main[i]);
						DisplayCImage(image_main, WMAIN);
						break;
				}
				break;
			case 6:
				printf(" 1:FFT\n");
				printf(" 2:FFT Filter  ?  ");
				scanf("%d", &command_sub);
				switch(command_sub) {
					case 1:
						if ((fftimage(image_main[0], image_sub[0]) != -1) &&
							(fftimage(image_main[1], image_sub[1]) != -1) &&
							(fftimage(image_main[2], image_sub[2]) != -1))
							DisplayCImage(image_sub, WSUB);
						else
							printf("--Memory allocation error !\n");
						break;
					case 2:
						printf("  range min max ? ?  ");
						scanf("%d %d", &m, &n);
                    	image_copy_color(image_main, image_prev);
                    	DisplayCImage(image_prev, WPREV);
						if ((fftfilter(image_prev[0], image_main[0], 
							m, n) != -1) &&
							(fftfilter(image_prev[1], image_main[1],
							m, n) != -1) &&
							(fftfilter(image_prev[2], image_main[2],
							m, n) != -1))
							DisplayCImage(image_main, WMAIN);
						else
							printf("--Memory allocation error !\n");
						break;
				}
				break;
			case 7:
				printf(" 1:Encode\n");
				printf(" 2:Decode   ?  ");
				scanf("%d", &command_sub);
				switch (command_sub) {
					case 1:
						if ((dpcm_vlcode(image_main[0], image_sub[0]) != -1)
						  && (dpcm_vlcode(image_main[1], image_sub[1]) != -1)
						  && (dpcm_vlcode(image_main[2], image_sub[2]) != -1))
							DisplayCImage(image_sub, WSUB);
						else
							printf("--Buffer overflow error !\n");
						break;
					case 2:
                    	image_copy_color(image_main, image_prev);
                    	DisplayCImage(image_prev, WPREV);
						if ((idpcm_vlcode(image_sub[0], image_main[0]) != -1) 
						  && (idpcm_vlcode(image_sub[1], image_main[1]) != -1) 
						  && (idpcm_vlcode(image_sub[2], image_main[2]) != -1))
							DisplayCImage(image_main, WMAIN);
						else
							printf("--Buffer overflow error !\n");
						break;
				}
				break;
			case 8:
				printf(" 1:Difference\n");
				printf(" 2:Correlation   ?  ");
				scanf("%d", &command_sub);
				switch (command_sub) {
					case 1:
                    	image_copy_color(image_main, image_prev);
                    	DisplayCImage(image_prev, WPREV);
						printf("  Block Size ?  ");
						scanf("%d", &n);
						printf("  Amplitude (double) ?  ");
						scanf("%lf", &amp);
						stereo_diff(image_prev[0], image_sub[0], 
							image_main[0], (int)a);
						stereo_diff(image_prev[1], image_sub[1], 
							image_main[1], (int)a);
						stereo_diff(image_prev[2], image_sub[2], 
							image_main[2], (int)a);
						amplify(image_main[0], image_main[0], amp, 0);
						amplify(image_main[1], image_main[1], amp, 0);
						amplify(image_main[2], image_main[2], amp, 0);
						DisplayCImage(image_main, WMAIN);
						break;
					case 2:
                    	image_copy_color(image_main, image_prev);
                    	DisplayCImage(image_prev, WPREV);
						printf("  Block Size ?  ");
						scanf("%d", &n);
						printf("  Amplitude (double) ?  ");
						scanf("%lf", &amp);
						stereo_corre(image_prev[0], image_sub[0], 
							image_main[0], (int)a);
						stereo_corre(image_prev[1], image_sub[1], 
							image_main[1], (int)a);
						stereo_corre(image_prev[2], image_sub[2], 
							image_main[2], (int)a);
						amplify(image_main[0], image_main[0], amp, 0);
						amplify(image_main[1], image_main[1], amp, 0);
						amplify(image_main[2], image_main[2], amp, 0);
						DisplayCImage(image_main, WMAIN);
						break;
				}
				break;
			case 9:
				printf(" 1:Cursor\n");
				printf(" 2:Mosaic\n");
				printf(" 3:Mosaic (coef)\n");
				printf(" 4:Mosaic (block)   ?  ");
				scanf("%d", &command_sub);
				switch (command_sub) {
					case 1:
						printf("  x, y ? ?  ");
						scanf("%d %d", &xd, &yd);
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
					case 2:
						printf("  dx, dy, scale, deg (double) ? ? ? ?  ");
						scanf("%lf %lf %lf %lf", &a, &b, &c, &d);
						printf("  Type (1:Overwrite, 2:Gradation) ?  ");
						scanf("%d", &type);
						image_copy_color(image_main, image_prev);
						DisplayCImage(image_prev, WPREV);
						for (i = 0; i < 3; i++)
							mosaic(image_prev[i], image_sub[i], image_work[i],
								a, b, c, d, type-1);
						image_halfsize_color(image_work, image_main);
						DisplayCImage(image_main, WMAIN);
						break;
					case 3:
						printf("  x1, y1, X1, Y1 ? ? ? ? ");
						scanf("%d %d %d %d", &u1, &v1, &x1, &y1);
						printf("  x2, y2, X2, Y2 ? ? ? ? ");
						scanf("%d %d %d %d", &u2, &v2, &x2, &y2);
						printf("  x3, y3, X3, Y3 ? ? ? ?  ");
						scanf("%d %d %d %d", &u3, &v3, &x3, &y3);
						printf("  Type (1:Overwrite, 2:Gradation) ?  ");
						scanf("%d", &type);
						image_copy_color(image_main, image_prev);
						DisplayCImage(image_prev, WPREV);
						affine_coef(x1, y1, u1, v1, x2, y2, u2, v2,
							x3, y3, u3, v3, &a, &b, &c, &d, &e, &f);
						for (i = 0; i < 3; i++)
							mosaic_affine(image_prev[i], image_sub[i], 
								image_work[i], a, b, c, d, e, f, type-1);
						image_halfsize_color(image_work, image_main);
						DisplayCImage(image_main, WMAIN);
						break;
					case 4:
						printf("  main:x, main:y, sub:x, sub:y, ");
						printf("range:±x, range:y, matching:x, matching:y");
						printf(" ? ? ? ? ? ? ? ?  ");
						scanf("%d %d %d %d %d %d %d %d", &x1, &y1, &x2, &y2,
							&xd, &yd, &xm, &ym);
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
			case 0:
				printf("...Bye-bye ........\n"); 
				break;
			default:
				printf("--Not defined number !\n"); 
				break;
		}
	}
}
