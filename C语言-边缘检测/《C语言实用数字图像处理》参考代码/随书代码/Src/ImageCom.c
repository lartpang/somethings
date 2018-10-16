#include "Params.h"

/*--- image_copy --- モノクロ画像データをコピーする --------------------------
	image_in:	入力画像配列
	image_out:	出力画像配列
----------------------------------------------------------------------------*/
void image_copy(unsigned char image_in[Y_SIZE][X_SIZE], 
	unsigned char image_out[Y_SIZE][X_SIZE])
{
	int	i, j;

	for (i = 0; i < Y_SIZE; i++)
		for (j = 0; j < X_SIZE; j++)
			image_out[i][j] = image_in[i][j];
}

/*--- image_copy_color --- カラー画像データをコピーする ----------------------
	image_in:	入力画像配列（カラー）
	image_out:	出力画像配列（カラー）
----------------------------------------------------------------------------*/
void image_copy_color(unsigned char image_in[3][Y_SIZE][X_SIZE], 
	unsigned char image_out[3][Y_SIZE][X_SIZE])
{
	int	i, j, k;
	
	for (i = 0; i < 3; i++)
		for (j = 0; j < Y_SIZE; j++)
			for (k = 0; k < X_SIZE; k++)
				image_out[i][j][k] = image_in[i][j][k];
}

/*--- image_clear --- モノクロ画像データをクリアする -------------------------
	image:	画像配列
----------------------------------------------------------------------------*/
void image_clear(unsigned char image[Y_SIZE][X_SIZE])
{
	int	i, j;

	for (i = 0; i < Y_SIZE; i++)
		for (j = 0; j < X_SIZE; j++)
			image[i][j] = 0;
}

/*--- image_clear_color --- カラー画像データをクリアする ---------------------
	image:	画像配列（カラー）
----------------------------------------------------------------------------*/
void image_clear_color(unsigned char image[3][Y_SIZE][X_SIZE])
{
	int	i, j, k;

	for (i = 0; i < 3; i++)
		for (j = 0; j < Y_SIZE; j++)
			for (k = 0; k < X_SIZE; k++)
				image[i][j][k] = 0;
}

/*--- image_negative --- モノクロ画像の濃度反転 ------------------------------
	image:	画像配列
----------------------------------------------------------------------------*/
void image_negative(unsigned char image[Y_SIZE][X_SIZE])
{
	int i, j;

	for (i = 0; i < Y_SIZE; i++)
	    for (j = 0; j < X_SIZE; j++)
		  	image[i][j] = 255 - image[i][j];
}

/*--- image_negative_color --- カラー画像の濃度反転 --------------------------
	image:	画像配列（カラー）
----------------------------------------------------------------------------*/
void image_negative_color(unsigned char image[3][Y_SIZE][X_SIZE])
{
	int i, j, k;

	for (i = 0; i < 3; i++)
		for (j = 0; j < Y_SIZE; j++)
		    for (k = 0; k < X_SIZE; k++)
			  	image[i][j][k] = 255 - image[i][j][k];
}

/*--- color_to_mono --- カラー画像をモノクロ画像に変換する --------------------
	image:	画像配列（カラー）
-----------------------------------------------------------------------------*/
void color_to_mono(unsigned char image[3][Y_SIZE][X_SIZE])
{
	int		i, j;
	double	y;

	for (i = 0; i < Y_SIZE; i++) {
		for (j = 0; j < X_SIZE; j++) {
			y = 0.2126 * image[0][i][j] + 0.7152 * image[1][i][j]
				+ 0.0722 * image[2][i][j];
			if (y > 255) y = 255;
			if (y <   0) y =   0;
			image[0][i][j] = image[1][i][j] = image[2][i][j]
				= (unsigned char)y;
		}  
	}
}

/*--- image_addition --- モノクロ画像を加算する ------------------------------
	image_in1:	入力画像配列1
	image_in2:	入力画像配列2
	image_out:	出力画像配列
----------------------------------------------------------------------------*/
void image_addition(unsigned char image_in1[Y_SIZE][X_SIZE],
	unsigned char image_in2[Y_SIZE][X_SIZE],
	unsigned char image_out[Y_SIZE][X_SIZE])
{
	int	i, j, d;

	for (i = 0; i < Y_SIZE; i++) {
		for (j = 0; j < X_SIZE; j++) {
			d = image_in1[i][j] + image_in2[i][j];
			if (d > 255) d = 255;
			image_out[i][j] = (unsigned char)d;
		}
	}
}

/*--- image_addition_color --- カラー画像を加算する --------------------------
	image_in1:	入力画像配列1（カラー）
	image_in2:	入力画像配列2（カラー）
	image_out:	出力画像配列（カラー）
----------------------------------------------------------------------------*/
void image_addition_color(unsigned char image_in1[3][Y_SIZE][X_SIZE],
	unsigned char image_in2[3][Y_SIZE][X_SIZE],
	unsigned char image_out[3][Y_SIZE][X_SIZE])
{
	int	i, j, k, d;
	
	for (i = 0; i < 3; i++) {
		for (j = 0; j < Y_SIZE; j++) {
			for (k = 0; k < X_SIZE; k++) {
				d = image_in1[i][j][k] + image_in2[i][j][k];
				if (d > 255) d = 255;
				image_out[i][j][k] = (unsigned char)d;
			}
		}
	}
}

/*--- image_subtraction --- モノクロ画像を減算する ---------------------------
	image_in1:	入力画像配列1
	image_in2:	入力画像配列2
	image_out:	出力画像配列
----------------------------------------------------------------------------*/
void image_subtraction(unsigned char image_in1[Y_SIZE][X_SIZE],
	unsigned char image_in2[Y_SIZE][X_SIZE],
	unsigned char image_out[Y_SIZE][X_SIZE])
{
	int	i, j, d;

	for (i = 0; i < Y_SIZE; i++) {
		for (j = 0; j < X_SIZE; j++) {
			d = image_in1[i][j] - image_in2[i][j];
			if (d < 0) d = 0;
			image_out[i][j] = (unsigned char)d;
		}
	}
}

/*--- image_subtraction_color --- カラー画像を減算する -----------------------
	image_in1:	入力画像配列1（カラー）
	image_in2:	入力画像配列2（カラー）
	image_out:	出力画像配列（カラー）
----------------------------------------------------------------------------*/
void image_subtraction_color(unsigned char image_in1[3][Y_SIZE][X_SIZE],
	unsigned char image_in2[3][Y_SIZE][X_SIZE],
	unsigned char image_out[3][Y_SIZE][X_SIZE])
{
	int	i, j, k, d;
	
	for (i = 0; i < 3; i++) {
		for (j = 0; j < Y_SIZE; j++) {
			for (k = 0; k < X_SIZE; k++) {
				d = image_in1[i][j][k] - image_in2[i][j][k];
				if (d < 0) d = 0;
				image_out[i][j][k] = (unsigned char)d;
			}
		}
	}
}

/*--- image_difference --- モノクロ画像の差分をとる --------------------------
	image_in1:	入力画像配列1
	image_in2:	入力画像配列2
	image_out:	出力画像配列
----------------------------------------------------------------------------*/
void image_difference(unsigned char image_in1[Y_SIZE][X_SIZE],
	unsigned char image_in2[Y_SIZE][X_SIZE],
	unsigned char image_out[Y_SIZE][X_SIZE])
{
	int	i, j, d;

	for (i = 0; i < Y_SIZE; i++) {
		for (j = 0; j < X_SIZE; j++) {
			d = image_in1[i][j] - image_in2[i][j];
			if (d < 0) d = -d;
			image_out[i][j] = (unsigned char)d;
		}
	}
}

/*--- image_difference_color --- カラー画像の差分をとる ----------------------
	image_in1:	入力画像配列1（カラー）
	image_in2:	入力画像配列2（カラー）
	image_out:	出力画像配列（カラー）
----------------------------------------------------------------------------*/
void image_difference_color(unsigned char image_in1[3][Y_SIZE][X_SIZE],
	unsigned char image_in2[3][Y_SIZE][X_SIZE],
	unsigned char image_out[3][Y_SIZE][X_SIZE])
{
	int	i, j, k, d;
	
	for (i = 0; i < 3; i++) {
		for (j = 0; j < Y_SIZE; j++) {
			for (k = 0; k < X_SIZE; k++) {
				d = image_in1[i][j][k] - image_in2[i][j][k];
				if (d < 0) d = -d;
				image_out[i][j][k] = (unsigned char)d;
			}
		}
	}
}

/*--- image_multiplication --- モノクロ画像を乗算する ------------------------
	image_in1:	入力画像配列1
	image_in2:	入力画像配列2
	image_out:	出力画像配列
----------------------------------------------------------------------------*/
void image_multiplication(unsigned char image_in1[Y_SIZE][X_SIZE],
	unsigned char image_in2[Y_SIZE][X_SIZE],
	unsigned char image_out[Y_SIZE][X_SIZE])
{
	int	i, j;
	double d;

	for (i = 0; i < Y_SIZE; i++) {
		for (j = 0; j < X_SIZE; j++) {
			d = (double)image_in1[i][j] * image_in2[i][j] / 255.0;
			image_out[i][j] = (unsigned char)d;
		}
	}
}

/*--- image_multiplication_color --- カラー画像を乗算する --------------------------
	image_in1:	入力画像配列1（カラー）
	image_in2:	入力画像配列2（カラー）
	image_out:	出力画像配列（カラー）
----------------------------------------------------------------------------*/
void image_multiplication_color(unsigned char image_in1[3][Y_SIZE][X_SIZE],
	unsigned char image_in2[3][Y_SIZE][X_SIZE],
	unsigned char image_out[3][Y_SIZE][X_SIZE])
{
	int	i, j, k;
	double d;
	
	for (i = 0; i < 3; i++) {
		for (j = 0; j < Y_SIZE; j++) {
			for (k = 0; k < X_SIZE; k++) {
				d = (double)image_in1[i][j][k] * image_in2[i][j][k] / 255.0;
				image_out[i][j][k] = (unsigned char)d;
			}
		}
	}
}

/*--- image_or --- モノクロ画像の論理和をとる --------------------------------
	image_in1:	入力画像配列1
	image_in2:	入力画像配列2
	image_out:	出力画像配列
----------------------------------------------------------------------------*/
void image_or(unsigned char image_in1[Y_SIZE][X_SIZE],
	unsigned char image_in2[Y_SIZE][X_SIZE],
	unsigned char image_out[Y_SIZE][X_SIZE])
{
	int	i, j;

	for (i = 0; i < Y_SIZE; i++) {
		for (j = 0; j < X_SIZE; j++) {
			image_out[i][j] = image_in1[i][j];
			if (image_in2[i][j] > image_out[i][j])
				image_out[i][j] = image_in2[i][j];
		}
	}
}

/*--- image_or_color --- カラー画像の論理和をとる ----------------------------
	image_in1:	入力画像配列1（カラー）
	image_in2:	入力画像配列2（カラー）
	image_out:	出力画像配列（カラー）
----------------------------------------------------------------------------*/
void image_or_color(unsigned char image_in1[3][Y_SIZE][X_SIZE],
	unsigned char image_in2[3][Y_SIZE][X_SIZE],
	unsigned char image_out[3][Y_SIZE][X_SIZE])
{
	int	i, j, k;
	
	for (i = 0; i < 3; i++) {
		for (j = 0; j < Y_SIZE; j++) {
			for (k = 0; k < X_SIZE; k++) {
				image_out[i][j][k] = image_in1[i][j][k];
				if (image_in2[i][j][k] > image_out[i][j][k])
					image_out[i][j][k] = image_in2[i][j][k];
			}
		}
	}
}

/*--- image_and --- モノクロ画像の論理積をとる -------------------------------
	image_in1:	入力画像配列1
	image_in2:	入力画像配列2
	image_out:	出力画像配列
----------------------------------------------------------------------------*/
void image_and(unsigned char image_in1[Y_SIZE][X_SIZE],
	unsigned char image_in2[Y_SIZE][X_SIZE],
	unsigned char image_out[Y_SIZE][X_SIZE])
{
	int	i, j;

	for (i = 0; i < Y_SIZE; i++) {
		for (j = 0; j < X_SIZE; j++) {
			image_out[i][j] = image_in1[i][j];
			if (image_in2[i][j] < image_out[i][j])
				image_out[i][j] = image_in2[i][j];
		}
	}
}

/*--- image_and_color --- カラー画像の論理積をとる ----------------------------
	image_in1:	入力画像配列1（カラー）
	image_in2:	入力画像配列2（カラー）
	image_out:	出力画像配列（カラー）
----------------------------------------------------------------------------*/
void image_and_color(unsigned char image_in1[3][Y_SIZE][X_SIZE],
	unsigned char image_in2[3][Y_SIZE][X_SIZE],
	unsigned char image_out[3][Y_SIZE][X_SIZE])
{
	int	i, j, k;
	
	for (i = 0; i < 3; i++) {
		for (j = 0; j < Y_SIZE; j++) {
			for (k = 0; k < X_SIZE; k++) {
				image_out[i][j][k] = image_in1[i][j][k];
				if (image_in2[i][j][k] < image_out[i][j][k])
					image_out[i][j][k] = image_in2[i][j][k];
			}
		}
	}
}

/*--- image_add_rgb --- RGBを加算する ----------------------------------------
	image:	画像配列（カラー）
----------------------------------------------------------------------------*/
void image_addition_rgb(unsigned char image[3][Y_SIZE][X_SIZE])
{
	int	i, j, d;

	for (i = 0; i < Y_SIZE; i++) {
		for (j = 0; j < X_SIZE; j++) {
			d = (int)image[0][i][j]
			  + (int)image[1][i][j]
			  + (int)image[2][i][j];
			if (d <   0) d =   0;
			if (d > 255) d = 255;
			image[0][i][j] = (unsigned char)d;		   
			image[1][i][j] = (unsigned char)d;		   
			image[2][i][j] = (unsigned char)d;
		}
	}
}

/*--- image_or_rgb --- RGBの論理和をとる -------------------------------------
	image:	画像配列（カラー）
----------------------------------------------------------------------------*/
void image_or_rgb(unsigned char image[3][Y_SIZE][X_SIZE])
{
	int	i, j;
	unsigned char d;

	for (i = 0; i < Y_SIZE; i++) {
		for (j = 0; j < X_SIZE; j++) {
			d = image[0][i][j];
			if (image[1][i][j] > d) d = image[1][i][j];
			if (image[2][i][j] > d) d = image[2][i][j];
			image[0][i][j] = (unsigned char)d;		   
			image[1][i][j] = (unsigned char)d;		   
			image[2][i][j] = (unsigned char)d;
		}
	}
}

/*--- image_and_rgb --- RGBの論理積をとる ------------------------------------
	image:	画像配列（カラー）
----------------------------------------------------------------------------*/
void image_and_rgb(unsigned char image[3][Y_SIZE][X_SIZE])
{
	int	i, j;
	unsigned char d;

	for (i = 0; i < Y_SIZE; i++) {
		for (j = 0; j < X_SIZE; j++) {
			d = image[0][i][j];
			if (image[1][i][j] < d) d = image[1][i][j];
			if (image[2][i][j] < d) d = image[2][i][j];
			image[0][i][j] = (unsigned char)d;		   
			image[1][i][j] = (unsigned char)d;		   
			image[2][i][j] = (unsigned char)d;
		}
	}
}

/*--- image_halfsize --- モノクロ画像配列を半分サイズにする ------------------
	image_in:	入力画像配列
	image_out:	出力画像配列
----------------------------------------------------------------------------*/
void image_halfsize(unsigned char image_in[Y_SIZE*2][X_SIZE*2], 
	unsigned char image_out[Y_SIZE][X_SIZE])
{
	int	i, j, n;

	for (i = 0; i < Y_SIZE; i++) {
		for (j = 0; j < X_SIZE; j++) {
			n = (int)image_in[i*2  ][j*2  ]
			  + (int)image_in[i*2  ][j*2+1]
			  + (int)image_in[i*2+1][j*2  ]
			  + (int)image_in[i*2+1][j*2+1];
			image_out[i][j] = (unsigned char)(n / 4);
		}
	}
}

/*--- image_halfsize_color --- カラー画像配列を半分サイズにする --------------
	image_in:	入力画像配列（カラー）
	image_out:	出力画像配列（カラー）
----------------------------------------------------------------------------*/
void image_halfsize_color(unsigned char image_in[3][Y_SIZE*2][X_SIZE*2], 
	unsigned char image_out[3][Y_SIZE][X_SIZE])
{
	int	i, j, k, n;
	
	for (i = 0; i < 3; i++) {
		for (j = 0; j < Y_SIZE; j++) {
			for (k = 0; k < X_SIZE; k++) {
				n = (int)image_in[i][j*2  ][k*2  ]
				  + (int)image_in[i][j*2  ][k*2+1]
				  + (int)image_in[i][j*2+1][k*2  ]
				  + (int)image_in[i][j*2+1][k*2+1];
				image_out[i][j][k] = (unsigned char)(n / 4);
			}
		}
	}
}
