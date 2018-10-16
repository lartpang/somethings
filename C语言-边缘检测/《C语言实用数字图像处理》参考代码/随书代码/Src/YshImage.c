#include "Params.h"

/*--- y_image --- 輝度データを濃淡画像化する ----------------------------------
	image_in_y:	輝度のデータ配列
	image_out:	出力画像配列
-----------------------------------------------------------------------------*/
void y_image(int image_in_y[Y_SIZE][X_SIZE],
	unsigned char image_out[Y_SIZE][X_SIZE])
{
	int i, j, d;

	for (i = 0; i < Y_SIZE; i++){
		for (j = 0; j < X_SIZE; j++ ){
			d = image_in_y[i][j];
			if (d <   0) d =   0;
			if (d > 255) d = 255;
			image_out[i][j] = (unsigned char)d;
		}
	}
}

/*--- sat_image --- 彩度データを濃淡画像化する --------------------------------
	image_in_sat:	彩度のデータ配列
	image_out:		出力画像配列
-----------------------------------------------------------------------------*/
void sat_image(int image_in_sat[Y_SIZE][X_SIZE],
	unsigned char image_out[Y_SIZE][X_SIZE])
{
	int i, j, d;

	for (i = 0; i < Y_SIZE; i++){
		for (j = 0; j < X_SIZE; j++ ){
			d = image_in_sat[i][j];
			if (d <   0) d =   0;
			if (d > 255) d = 255;
			image_out[i][j] = (unsigned char)d;
		}
	}
}

/*--- hue_image --- 色相データを画像化する ------------------------------------
	image_in_sat:	彩度のデータ配列
	image_in_hue:	色相のデータ配列
	image_out:		出力画像配列
	org:			基準色相(度)
-----------------------------------------------------------------------------*/
void hue_image(int image_in_sat[Y_SIZE][X_SIZE],
	int image_in_hue[Y_SIZE][X_SIZE],
	unsigned char image_out[Y_SIZE][X_SIZE], int org)
{
	int    i, j;
	double d;

	for (i = 0; i < Y_SIZE; i++){
		for (j = 0; j < X_SIZE; j++){
			if (image_in_sat[i][j] > 0) {
				d = image_in_hue[i][j];
				d = d - org;
				if (d <    0) d = -d;
				if (d >  180) d = 360.0 - d;
				d = 255.0 - d * 255.0 / 180.0;
				if (d <   0) d =   0;
				if (d > 255) d = 255;
				image_out[i][j] = (unsigned char)d;
			}
			else image_out[i][j]=0;
		}
	}
}
