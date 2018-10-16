#include <stdio.h>
#include "Params.h"
#include "Proto.h"

#define WMAIN	0		/* メイン画像用ウィンドウ	*/
#define WPREV 	1		/* プリ画像用ウィンドウ		*/
#define WSUB 	2		/* サブ画像用ウィンドウ	*/
#define WKEY 	3		/* キー画像用ウィンドウ		*/

void InitDisplay(void);
void DisplayImage(unsigned char image[Y_SIZE][X_SIZE], int position);

unsigned char image_main[Y_SIZE][X_SIZE];		/* メイン画像配列			*/
unsigned char image_prev[Y_SIZE][X_SIZE];		/* プリ画像配列				*/
unsigned char image_sub[Y_SIZE][X_SIZE];		/* サブ画像配列				*/
unsigned char image_key[Y_SIZE][X_SIZE];		/* キー画像配列				*/
unsigned char image_buf[Y_SIZE][X_SIZE];		/* バッファ配列				*/
unsigned char image_work[Y_SIZE*2][X_SIZE*2];	/* モザイク用画像配列		*/
unsigned char image_rgb[3][Y_SIZE][X_SIZE];		/* カラー画像配列(RGB)		*/
long	hist[256];								/* ヒストグラム配列			*/
long	histbuf[256];							/* ヒストグラム配列    		*/
double	ratio[128], size[128];					/* 特徴パラメータ配列 		*/
char	text_buf[TEXT_BUF_LEN];					/* 出力用文字配列			*/

/*--- main --- 画像処理メインプログラム（モノクロ）--------------------------*/
main(void)
{
	static int	command = -1;
	char		source[80], destin[80];     
	int			command_sub, nwin, ncol, num, m, n, ret;
    int 		thres, type, smt, mode, lvl;
    double   	a, b, c, d, e, f, g, h, x0, y0, deg;
	double		ratio_min, ratio_max;
	double		size_min, size_max, amp;
	int			fmax, fmin, cnt;
    int 		i, mx, my;
	int         x1, y1, x2, y2, x3, y3, x4, y4, xd, yd, xb, yb;
	int         u1, v1, u2, v2, u3, v3, u4, v4;

	InitDisplay();
	while (command) {
		printf("\n*** Image Processing Menu  ***\n");
		printf(" 1: File\n");
		printf(" 2: Edit\n");
		printf(" 3: Region Extraction\n");
		printf(" 4: Contour Extraction\n");
		printf(" 5: Noise Reduction\n");
		printf(" 6: Gray scale transform\n");
		printf(" 7: Feature Extraction\n");
		printf(" 8: Geometrical transform\n");
		printf(" 9: FFT & Filter\n");
		printf("10: Compression\n");
		printf("11: Depth Estimation\n");
		printf("12: Mosaic\n");
		printf(" 0: End\n");
		printf("Process Number   ?  ");
		scanf("%d", &command);
		switch (command) {
			case 1:
				printf(" 1: BMP File read\n");
				printf(" 2: BMP File write\n");
				printf(" 3: RAW File read\n");
				printf(" 4: RAW File write   ?  ");
				scanf("%d", &command_sub);
				if (command_sub < 1 || command_sub > 4) break;
				printf("  Which window (1:MAIN, 2:PREV, 3:SUB, 4:KEY) ?  ");
				scanf("%d", &nwin);
				if (nwin < 1 || nwin > 4) break;
				if (command_sub == 1 || command_sub  == 3) {
					printf("  Filename (read) ?  ");
					scanf("%s", source);
					if (command_sub == 1) {
						num = check_bmp_file(source);
						if (num == 8)
							ret = read_bmp_mono(image_buf, source);
						else if (num == 24)
							ret = read_bmp_color(image_rgb, source);
						if  (ret == -1)
							printf("--File read error !\n");
						else {
							if (num == 8)
								switch (nwin) {
									case 1: 
										image_copy(image_buf, image_main);
										DisplayImage(image_main, WMAIN);
										break;
									case 2: 
										image_copy(image_buf, image_prev);
										DisplayImage(image_prev, WPREV);
										break;
									case 3: 
										image_copy(image_buf, image_sub);
										DisplayImage(image_sub, WSUB);
										break;
									case 4: 
										image_copy(image_buf, image_key);
										DisplayImage(image_key, WKEY);
										break;
							} 
							else if (num ==24) {
								printf("  Which color (1:R, 2:G, 3:B) ?  ");
								scanf("%d", &ncol);
								switch (nwin) {
									case 1: 
										image_copy(image_rgb[ncol-1], image_main);
										DisplayImage(image_main, WMAIN);
										break;
									case 2: 
										image_copy(image_rgb[ncol-1], image_prev);
										DisplayImage(image_prev, WPREV);
										break;
									case 3: 
										image_copy(image_rgb[ncol-1], image_sub);
										DisplayImage(image_sub, WSUB);
										break;
									case 4: 
										image_copy(image_rgb[ncol-1], image_key);
										DisplayImage(image_key, WKEY);
										break;
								}
							}
						}
					} else {
						if (read_mono(image_buf, source) == -1)
							printf("--File read error !\n");
						else 
							switch (nwin) {
								case 1: 
									image_copy(image_buf, image_main);
									DisplayImage(image_main, WMAIN);
									break;
								case 2: 
									image_copy(image_buf, image_prev);
									DisplayImage(image_prev, WPREV);
									break;
								case 3: 
									image_copy(image_buf, image_sub);
									DisplayImage(image_sub, WSUB);
									break;
								case 4: 
									image_copy(image_buf, image_key);
									DisplayImage(image_key, WKEY);
									break;
						}
					}
				} else if (command_sub == 2 || command_sub == 4) {
					printf("  Filename (write) ?  ");
					scanf("%s", destin);
					switch (nwin) {
						case 1:
							if (command_sub == 2)
								ret = write_bmp_mono(image_main, destin);
							else
								ret = write_mono(image_main, destin);
							break;
						case 2:
							if (command_sub == 2)
								ret = write_bmp_mono(image_prev, destin);
							else
								ret = write_mono(image_prev, destin);
							break;
						case 3:
							if (command_sub == 2)
								ret = write_bmp_mono(image_sub, destin);
							else
								ret = write_mono(image_sub, destin);
							break;
						case 4:
							if (command_sub == 2)
								ret = write_bmp_mono(image_key, destin);
							else
								ret = write_mono(image_key, destin);
							break;
						default:
							ret = -1;
							break;
					}
					if (ret == -1)
						printf("--File write error !\n");
				}
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
				printf("10: MAIN + SUB\n");
				printf("11: MAIN - SUB\n");
				printf("12: |MAIN + SUB|\n");
				printf("13: MAIN * SUB\n");
				printf("14: MAIN | SUB\n");
				printf("15: MAIN & SUB   ?  ");
				scanf("%d", &command_sub);
				switch (command_sub) {
					case 1:
						image_copy(image_prev, image_main);
						DisplayImage(image_main, WMAIN);
						break;
					case 2:
						image_clear(image_main);
						DisplayImage(image_main, WMAIN);
						break;
					case 3:
						image_clear(image_sub);
						DisplayImage(image_sub, WSUB);
						break;
					case 4:
						image_clear(image_key);
						DisplayImage(image_key, WKEY);
						break;
					case 5:
						image_copy(image_main, image_sub);
						DisplayImage(image_sub, WSUB);
						break;
					case 6:
						image_copy(image_main, image_key);
						DisplayImage(image_key, WKEY);
						break;
					case 7:
						image_copy(image_sub, image_main);
						DisplayImage(image_main, WMAIN);
						break;
					case 8:
						image_copy(image_key, image_main);
						DisplayImage(image_main, WMAIN);
						break;
					case 9:
						image_copy(image_main, image_prev);
						DisplayImage(image_prev, WPREV);
						image_negative(image_main);
						DisplayImage(image_main, WMAIN);
						break;
					case 10:
						image_copy(image_main, image_prev);
		                DisplayImage(image_prev, WPREV);
						image_addition(image_prev, image_sub, image_main);
						DisplayImage(image_main, WMAIN);
						break;
					case 11:
						image_copy(image_main, image_prev);
						DisplayImage(image_prev, WPREV);
						image_subtraction(image_prev, image_sub, image_main);
						DisplayImage(image_main, WMAIN);
						break;
					case 12:
						image_copy(image_main, image_prev);
						DisplayImage(image_prev, WPREV);
						image_difference(image_prev, image_sub, image_main);
						DisplayImage(image_main, WMAIN);
						break;
					case 13:
						image_copy(image_main, image_prev);
						DisplayImage(image_prev, WPREV);
						image_multiplication(image_prev, image_sub, image_main);
						DisplayImage(image_main, WMAIN);
						break;
					case 14:
						image_copy(image_main, image_prev);
						DisplayImage(image_prev, WPREV);
						image_or(image_prev, image_sub, image_main);
						DisplayImage(image_main, WMAIN);
						break;
					case 15:
						image_copy(image_main, image_prev);
						DisplayImage(image_prev, WPREV);
						image_and(image_prev, image_sub, image_main);
						DisplayImage(image_main, WMAIN);
						break;
				}
				break;
			case 3:
				printf(" 1:Histgram \n");
				printf(" 2:Histgram Print \n");
				printf(" 3:Histgram Smooth\n");
				printf(" 4:Thresholding (Manual)\n");
				printf(" 5:Thresholding (Mode)\n");
				printf(" 6:Thresholding (Discriminant Analysis)\n");
				printf(" 7:Thresholding (Dynamic)   ?  ");
				scanf("%d", &command_sub);
				switch (command_sub) {
					case 1:
						histgram(image_main, hist);
						histimage(hist,image_sub);
						DisplayImage(image_sub, WSUB);
						break;
					case 2:
						histprint(hist, text_buf);
						printf("************ Histgram  ************\n");
						printf("%s", text_buf);
						break;
					case 3:
						histsmooth(hist, histbuf);
						for (i = 0; i < 256; i++) hist[i] = histbuf[i];
						histimage(hist, image_sub);
						DisplayImage(image_sub, WSUB);
						break;
					case 4:
						printf("  Threshold ?  ");
						scanf("%d", &thres);
						printf("  Type (1:Normal, 2:Inverse) ?  "); 
						scanf("%d", &type);
						image_copy(image_main, image_prev);
						DisplayImage(image_prev, WPREV);
						threshold(image_prev, image_main, thres, type);
						DisplayImage(image_main, WMAIN);
						break;
					case 5:
						printf("  Smooth Count ?  ");
						scanf("%d", &smt);
						printf("  Type (1:Normal, 2:Inverse) ?  "); 
						scanf("%d", &type);
						image_copy(image_main, image_prev);
						DisplayImage(image_prev, WPREV);
						threshold_mode(image_prev, image_main, smt, type);
						DisplayImage(image_main, WMAIN);
						break;
					case 6:
						printf("  Type (1:Normal, 2:Inverse) ?  "); 
						scanf("%d", &type);
						image_copy(image_main, image_prev);
						DisplayImage(image_prev, WPREV);
						threshold_discrim(image_prev, image_main, type);
						DisplayImage(image_main, WMAIN);
						break;
					case 7:
						printf("  Type (1:Normal, 2:Inverse) ?  "); 
						scanf("%d", &type);
						image_copy(image_main, image_prev);
						DisplayImage(image_prev, WPREV);
						threshold_dynamic(image_prev, image_main, type);
						DisplayImage(image_main, WMAIN);
						break;
				}
				break;
			case 4:
				printf(" 1:Gradient (Difference)\n");
				printf(" 2:Gradient (Roberts)\n");
				printf(" 3:Gradient (Sobel)\n");
				printf(" 4:Templete (Prewitt)\n");
				printf(" 5:Laplacian1\n");
				printf(" 6:Laplacian2\n");
				printf(" 7:Laplacian3\n");
				printf(" 8:Zero Cross\n");
				printf(" 9:Thinning\n");
				printf("10:Hough Transform\n");
				printf("11:Hough Cross Point   ?  ");
				scanf("%d", &command_sub);
				switch (command_sub) {
					case 1:
					case 2:
					case 3:
					case 4:
					case 5:
					case 6:
					case 7:
						printf("  Amplitude (double) ?  ");
						scanf("%lf", &amp);
						printf("amp = %lf\n", amp);
						image_copy(image_main, image_prev);
						DisplayImage(image_prev, WPREV);
						if (command_sub == 1)
							gradient_difference(image_prev, image_main, amp);
						if (command_sub == 2)
							gradient_roberts(image_prev, image_main, amp);
						if (command_sub == 3)
							gradient_sobel(image_prev, image_main, amp);
						if (command_sub == 4)
							prewitt(image_prev, image_main, amp);
						if (command_sub == 5)
							laplacian(image_prev, image_main, amp, 1);
						if (command_sub == 6)
							laplacian(image_prev, image_main, amp, 2);
						if (command_sub == 7)
							laplacian(image_prev, image_main, amp, 3);
						DisplayImage(image_main, WMAIN);
					    break;
					case 8:
						image_copy(image_main, image_prev);
						DisplayImage(image_prev, WPREV);
						zero_cross(image_prev, image_main);
						DisplayImage(image_main, WMAIN);
					    break;
					case 9:
						image_copy(image_main, image_prev);
						DisplayImage(image_prev, WPREV);
						thinning(image_prev, image_main);
						DisplayImage(image_main, WMAIN);
					    break;
					case 10:
 						printf("  Threshold ?  ");
						scanf("%d", &thres);
						image_copy(image_main, image_prev);
						DisplayImage(image_prev, WPREV);
						hough(image_prev, image_main, image_sub, thres, text_buf);
						printf("%s", text_buf);
						DisplayImage(image_main, WMAIN);
						DisplayImage(image_sub, WSUB);
					    break;
					case 11:
 						printf("  theta1, rho1, theta2, rho2 ?  ");
 						scanf("%lf %lf %lf %lf", &a, &b, &c, &d);
						hough_cross(a, b, c, d, &e, &f);
						printf("x = %10.3f, y= %10.3f \n", 
							e + X_SIZE/2, Y_SIZE/2 - f);
						break;
				}
				break;
			case 5:
				printf(" 1:Smooth\n");
				printf(" 2:Smooth (with weight)\n");
				printf(" 3:Smooth (edge preserve)\n");
				printf(" 4:median\n");
				printf(" 5:Dilation\n");
				printf(" 6:Erosion\n");
				printf(" 7:Laplacian of Gaussian\n");
				printf(" 8:Noise Add (rand)\n");
				printf(" 9:Noise Add (spike)   ?  ");
				scanf("%d", &command_sub);
				switch (command_sub) {
					case 1:
						printf("  Type (1:3x3, 2:5x5, 3:7x7) ?  "); 
						scanf("%d", &type);
						image_copy(image_main, image_prev);
						DisplayImage(image_prev, WPREV);
						smooth(image_prev, image_main, type*2+1);
						DisplayImage(image_main, WMAIN);
						break;
					case 2:
						printf("  Weight factor (1-3) ?  "); 
						scanf("%d", &type);
						image_copy(image_main, image_prev);
						DisplayImage(image_prev, WPREV);
						smooth_weighted(image_prev, image_main, type-1);
						DisplayImage(image_main, WMAIN);
						break;
					case 3:
						image_copy(image_main, image_prev);
						DisplayImage(image_prev, WPREV);
						smooth_edge_preserve(image_prev, image_main);
						DisplayImage(image_main, WMAIN);
						break;
					case 4:
						image_copy(image_main, image_prev);
						DisplayImage(image_prev, WPREV);
						median(image_prev, image_main);
						DisplayImage(image_main, WMAIN);
						break;
					case 5:
						image_copy(image_main, image_prev);
						DisplayImage(image_prev, WPREV);
						dilation(image_prev, image_main);
						DisplayImage(image_main, WMAIN);
						break;
					case 6:
						image_copy(image_main, image_prev);
						DisplayImage(image_prev, WPREV);
						erosion(image_prev, image_main);
						DisplayImage(image_main, WMAIN);
						break;
					case 7:
						printf("  Variance (float) ?  "); 
						scanf("%lf", &a);
						image_copy(image_main, image_prev);
						DisplayImage(image_prev, WPREV);
						log_zero_cross(image_prev, image_main, a);
						DisplayImage(image_main, WMAIN);
						break;
					case 8:
						printf("  Noise Level ?  "); 
						scanf("%d", &lvl);
						image_copy(image_main, image_prev);
						DisplayImage(image_prev, WPREV);
						noise_rand(image_prev, image_main, lvl);
						DisplayImage(image_main, WMAIN);
						break;
					case 9:
						printf("  Noise Number, Level ? ?  "); 
						scanf("%d %d", &num, &lvl);
						image_copy(image_main, image_prev);
						DisplayImage(image_prev, WPREV);
						noise_spike(image_prev, image_main, num, lvl);
						DisplayImage(image_main, WMAIN);
						break;
				}
				break;
			case 6:
				printf(" 1:Amplify\n");
				printf(" 2:Expand\n");
				printf(" 3:Plane\n");
				printf(" 4:Dither (Ordered)\n");
				printf(" 5:Dither (Minimized)\n");
				printf(" 6:Dither (Minimized Multi)\n");
				printf(" 7:Quantize   ?  ");
				scanf("%d", &command_sub);
				switch (command_sub) {
					case 1:
						printf("  Amplitude, Bias ? ?  ");
						scanf("%lf %lf", &a, &b);
						image_copy(image_main, image_prev);
						DisplayImage(image_prev, WPREV);
						amplify(image_prev, image_main, a, b);
						DisplayImage(image_main, WMAIN);
						break;
					case 2:
						image_copy(image_main, image_prev);
						DisplayImage(image_prev, WPREV);
						range(image_prev, &fmax, &fmin);
						expand(image_prev, image_main, fmax,fmin);
						DisplayImage(image_main, WMAIN);
						break;
					case 3:
						image_copy(image_main, image_prev);
						DisplayImage(image_prev, WPREV);
						histgram(image_prev, hist);
						plane(image_prev, image_main, hist);
						DisplayImage(image_main, WMAIN);
						break;
					case 4:
						image_copy(image_main, image_prev);
						DisplayImage(image_prev, WPREV);
						dither_ordered(image_prev, image_main);
						DisplayImage(image_main, WMAIN);
						break;
					case 5:
						image_copy(image_main, image_prev);
						DisplayImage(image_prev, WPREV);
						dither_minimized(image_prev, image_main);
						DisplayImage(image_main, WMAIN);
						break;
					case 6:
						printf("  Level Number ?  "); 
						scanf("%d", &num);
						image_copy(image_main, image_prev);
						DisplayImage(image_prev, WPREV);
						dither_minimized_multi(image_prev, image_main, num);
						DisplayImage(image_main, WMAIN);
						break;
					case 7:
						printf("  Level Number ?  "); 
						scanf("%d", &num);
						image_copy(image_main, image_prev);
						DisplayImage(image_prev, WPREV);
						quantize(image_prev, image_main, num);
						DisplayImage(image_main, WMAIN);
						break;
				}
				break;
			case 7:
				printf(" 1:Feature Parameter\n");
				printf(" 2:Key Mask\n");
				printf(" 3:Key Inverse   ?  ");
				scanf("%d", &command_sub);
				switch (command_sub) {
					case 1:
						image_copy(image_main, image_prev);
						DisplayImage(image_prev, WPREV);
						cnt = labeling(image_prev, image_sub);
						DisplayImage(image_sub, WSUB);
						features(image_sub, image_main, cnt, size, ratio, text_buf);
						printf("%s", text_buf);
						DisplayImage(image_main, WMAIN);
						image_copy(image_sub, image_buf);
						printf("  Extract by (1:shape, 2:size) ?  ");
						scanf("%d", &mode);
						if (mode == 1) {
							printf("  min max (double) ? ? ");
							scanf("%lf %lf", &ratio_min, &ratio_max);
							extract_ratio(image_buf, image_main, cnt, ratio, ratio_min, 
								ratio_max);
						}
						else if (mode == 2) {
 							printf("  min max (double) ? ?  ");
							scanf("%lf %lf", &size_min, &size_max);
							extract_size(image_buf, image_main, cnt, size, size_min, 
								size_max);
						}
						else break;
						threshold(image_main, image_main, 1, 1);
						DisplayImage(image_main, WMAIN);
						break;
					case 2:
						image_copy(image_main, image_prev);
						DisplayImage(image_prev, WPREV);
						masking(image_prev, image_main, image_key);
						DisplayImage(image_main, WMAIN);
						break;
					case 3:
						image_negative(image_key);
						DisplayImage(image_key, WKEY);
						break;
				}
				break;
			case 8:
				printf(" 1:Scale (bad method)\n");
				printf(" 2:Scale (near)\n");
				printf(" 3:Scale (bilinear)\n");
				printf(" 4:Shift\n");
				printf(" 5:Rotate\n");
				printf(" 6:Scale/Rotate/Shift\n");
				printf(" 7:Affine\n");
				printf(" 8:Affine (coef)\n");
				printf(" 9:Perspective\n");
				printf("10:Radial Distortion\n");
				printf("11:Lattice   ?  ");
				scanf("%d",&command_sub);
				switch (command_sub) {
					case 1:
						printf("  ax, ay (double) ? ?  ");
						scanf("%lf %lf", &a, &b);
						image_copy(image_main, image_prev);
						DisplayImage(image_prev, WPREV);
						scale_ng(image_prev, image_main, a, b);
						DisplayImage(image_main, WMAIN);
						break;
					case 2:
						printf("  ax, ay (double) ? ?  ");
						scanf("%lf %lf", &a, &b);
						image_copy(image_main, image_prev);
						DisplayImage(image_prev, WPREV);
						scale_near(image_prev, image_main, a, b);
						DisplayImage(image_main, WMAIN);
						break;
					case 3:
						printf("  ax, ay (double) ? ?  ");
						scanf("%lf %lf", &a, &b);
						image_copy(image_main, image_prev);
						DisplayImage(image_prev, WPREV);
						scale(image_prev, image_main, a, b);
						DisplayImage(image_main, WMAIN);
						break;
					case 4:
						printf("  x0, y0 (double) ? ?  ");
						scanf("%lf %lf", &x0, &y0);
						image_copy(image_main, image_prev);
						DisplayImage(image_prev, WPREV);
						shift(image_prev, image_main, x0, y0);
						DisplayImage(image_main, WMAIN);
						break;
					case 5:
						printf("  deg (double) ?  ");
						scanf("%lf", &deg);
						image_copy(image_main, image_prev);
						DisplayImage(image_prev, WPREV);
						rotation(image_prev, image_main, deg);
						DisplayImage(image_main, WMAIN);
						break;
					case 6:
						printf("  ax, ay, deg, x0, y0 (double) ? ? ? ? ?  ");
						scanf("%lf %lf %lf %lf %lf", &a, &b, &deg, &x0, &y0);
						image_copy(image_main, image_prev);
						DisplayImage(image_prev, WPREV);
						scale_rotate_shift(image_prev, image_main, a, b, deg, x0, y0);
						DisplayImage(image_main, WMAIN);
						break;
					case 7:
						printf("  a, b, c, d, e, f (double) ? ? ? ? ? ?  ");
						scanf("%lf %lf %lf %lf %lf %lf", &a, &b, &c, &d, &e, &f);
						image_copy(image_main, image_prev);
						DisplayImage(image_prev, WPREV);
						affine(image_prev, image_main, a, b, c, d, e, f);
						DisplayImage(image_main, WMAIN);
						break;
					case 8:
						printf("  x1, y1, X1, Y1 ? ? ? ?  ");
						scanf("%d %d %d %d", &x1, &y1, &u1, &v1);
						printf("  x2, y2, X2, Y2 ? ? ? ?  ");
						scanf("%d %d %d %d", &x2, &y2, &u2, &v2);
						printf("  x3, y3, X3, Y3 ? ? ? ?  ");
						scanf("%d %d %d %d", &x3, &y3, &u3, &v3);
						image_copy(image_main, image_prev);
						DisplayImage(image_prev, WPREV);
						affine_coef(x1, y1, u1, v1, x2, y2, u2, v2,
							x3, y3, u3, v3, &a, &b, &c, &d, &e, &f);
						affine(image_prev, image_main, a, b, c, d, e, f);
						DisplayImage(image_main, WMAIN);
						break;
					case 9:
						printf("  x1, y1, X1, Y1 ? ? ? ?  ");
						scanf("%d %d %d %d", &x1, &y1, &u1, &v1);
						printf("  x2, y2, X2, Y2 ? ? ? ?  ");
						scanf("%d %d %d %d", &x2, &y2, &u2, &v2);
						printf("  x3, y3, X3, Y3 ? ? ? ?  ");
						scanf("%d %d %d %d", &x3, &y3, &u3, &v3);
						printf("  x4, y4, X4, Y4 ? ? ? ?  ");
						scanf("%d %d %d %d", &x4, &y4, &u4, &v4);
						image_copy(image_main, image_prev);
						DisplayImage(image_prev, WPREV);
						perspect_coef(x1, y1, u1, v1, x2, y2, u2, v2,
							x3, y3, u3, v3, x4, y4, u4, v4,
							&a, &b, &c, &d, &e, &f, &g, &h);
						perspect(image_prev, image_main, a, b, c, d, e, f, g, h);
						DisplayImage(image_main, WMAIN);
						break;
					case 10:
						printf("  coef_a, coef_b (double) ? ?  ");
						scanf("%lf %lf", &a, &b);
						image_copy(image_main, image_prev);
						DisplayImage(image_prev, WPREV);
						radial_distortion(image_prev, image_main, a, b);
						DisplayImage(image_main, WMAIN);
						break;
					case 11:
						image_copy(image_main, image_prev);
						DisplayImage(image_prev, WPREV);
						lattice(image_main);
						DisplayImage(image_main, WMAIN);
						break;
				}
				break;
			case 9:
				printf(" 1:FFT\n");
				printf(" 2:FFT Filter   ?  ");
				scanf("%d", &command_sub);
				switch(command_sub) {
					case 1:
						if (fftimage(image_main, image_sub) != -1)
							DisplayImage(image_sub, WSUB);
						else
							printf("--Memory allocation error !\n");
						break;
					case 2:
						printf("  range min max ? ?  ");
						scanf("%d %d", &m, &n);
                    	image_copy(image_main, image_prev);
                    	DisplayImage(image_prev, WPREV);
						if (fftfilter(image_prev, image_main, m, n) != -1)
							DisplayImage(image_main, WMAIN);
						else
							printf("--Memory allocation error !\n");
						break;
				}
				break;
			case 10:
				printf(" 1:Encode\n");
				printf(" 2:Decode   ?  ");
				scanf("%d", &command_sub);
				switch (command_sub) {
					case 1:
						if (dpcm_vlcode(image_main, image_sub) != -1)
							DisplayImage(image_sub, WSUB);
						else
							printf("--Buffer overflow error !\n");
						break;
					case 2:
						image_copy(image_main, image_prev);
						DisplayImage(image_prev, WPREV);
						if (idpcm_vlcode(image_sub, image_main) != -1)
							DisplayImage(image_main, WMAIN);
						else
							printf("--Buffer overflow error !\n");
						break;
				}
				break;
			case 11:
				printf(" 1:Difference\n");
				printf(" 2:Correlation   ?  ");
				scanf("%d", &command_sub);
				switch (command_sub) {
					case 1:
						image_copy(image_main, image_prev);
						DisplayImage(image_prev, WPREV);
						printf("  Block Size ?  ");
						scanf("%d", &n);
						printf("  Amplitude (double) ?  ");
						scanf("%lf", &amp);
						stereo_diff(image_prev, image_sub, image_main, n);
						amplify(image_main, image_main, amp, 0);
						DisplayImage(image_main, WMAIN);
						break;
					case 2:
						image_copy(image_main, image_prev);
						DisplayImage(image_prev, WPREV);
						printf("  Block Size ?  ");
						scanf("%d", &n);
						printf("  Amplitude (double) ?  ");
						scanf("%lf", &amp);
						stereo_corre(image_prev, image_sub, image_main, n);
						amplify(image_main, image_main, amp, 0);
						DisplayImage(image_main, WMAIN);
						break;
				}
				break;
			case 12:
				printf(" 1:Cursor\n");
				printf(" 2:Mosaic\n");
				printf(" 3:Mosaic (coef)\n");
				printf(" 4:Mosaic (block)   ?  ");
				scanf("%d", &command_sub);
				switch (command_sub) {
					case 1:
						printf("  x, y ?  ");
						scanf("%d %d", &xd, &yd);
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
					case 2:
						printf("  dx, dy, scale, deg (double) ? ? ? ?  ");
						scanf("%lf %lf %lf %lf", &a, &b, &c, &d);
						printf("  Type (1:Overwrite, 2:Gradation) ?  ");
						scanf("%d", &type);
						image_copy(image_main, image_prev);
						DisplayImage(image_prev, WPREV);
						mosaic(image_prev, image_sub, image_work, a, b, c, d, type-1);
						image_halfsize(image_work, image_main);
						DisplayImage(image_main, WMAIN);
						break;
					case 3:
						printf("  x1, y1, X1, Y1 ? ? ? ?  ");
						scanf("%d %d %d %d", &u1, &v1, &x1, &y1);
						printf("  x2, y2, X2, Y2 ? ? ? ?  ");
						scanf("%d %d %d %d", &u2, &v2, &x2, &y2);
						printf("  x3, y3, X3, Y3 ? ? ? ?  ");
						scanf("%d %d %d %d", &u3, &v3, &x3, &y3);
						printf("  Type (1:Overwrite, 2:Gradation) ?  ");
						scanf("%d", &type);
						image_copy(image_main, image_prev);
						DisplayImage(image_prev, WPREV);
						affine_coef(x1, y1, u1, v1, x2, y2, u2, v2,
							x3, y3, u3, v3, &a, &b, &c, &d, &e, &f);
						mosaic_affine(image_prev, image_sub, image_work,
							a, b, c, d, e, f, type-1);
						image_halfsize(image_work, image_main);
						DisplayImage(image_main, WMAIN);
						break;
					case 4:
						printf("  main:x, main:y, sub:x, sub:y, ");
						printf("range:±x, range:y, matching:x, matching:y ");
						printf("? ? ? ? ? ? ? ?  ");
						scanf("%d %d %d %d %d %d %d %d", &x1, &y1, &x2, &y2,
							&xd, &yd, &xb, &yb);
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
			case 0:
				printf("...Bye-bye ........\n"); 
				break;
			default:
				printf("--Not defined number !\n"); 
				break;
		}
	}
}

