#include	<stdio.h>
#include	<stdlib.h>
#include	"Params.h"

/*--- check_bmp_file --- BMPファイルの画像がモノクロかカラーか調べる ----------
	filename:	ファイル名
-----------------------------------------------------------------------------*/
int check_bmp_file(char *filename)
{
	FILE *fp;
	unsigned char header[54];

	if ((fp = fopen(filename, "rb")) == NULL) return -1;
	fread(header, sizeof(unsigned char), 54, fp);
	if (header[0] != 0x42) return -1;
	if (header[1] != 0x4d) return -1;
	if (header[30] != 0x00) return -1;
	if (header[31] != 0x00) return -1;
	if (header[32] != 0x00) return -1;
	if (header[33] != 0x00) return -1;
	if ((header[28] == 0x08) && (header[18] == 0x00)) return 8; 
	if ((header[28] == 0x18) && (header[18] == 0x00)) return 24; 
	return -1;
}

/*--- read_bmp_mono --- BMP形式のモノクロ画像をディスクから読み込む -----------
	image:		画像配列
	filename:	ファイル名
-----------------------------------------------------------------------------*/
int read_bmp_mono(unsigned char image[Y_SIZE][X_SIZE], char *filename)
{
	long i, j;
	FILE *fp;
	unsigned char *image_buf;
	unsigned char header[1024];

	image_buf = (unsigned char *)malloc((size_t)X_SIZE*Y_SIZE);
	if (image_buf == NULL) return -1;
	if ((fp = fopen(filename, "rb")) == NULL) return -1;
	fread(header, sizeof(unsigned char), 54, fp);
	fread(header, sizeof(unsigned char), 1024, fp);
	fread(image_buf, sizeof(unsigned char), (size_t)(long)X_SIZE*Y_SIZE, fp);
	fclose(fp);
	for (i = 0; i < Y_SIZE; i++)
		for (j = 0; j < X_SIZE; j++)
			image[Y_SIZE-i-1][j] = *(image_buf + X_SIZE*i + j);
	free(image_buf);
	return 0;
}

/*--- write_bmp_mono --- モノクロ画像をBMP形式でディスクに書き出す ------------
	image:		画像配列
	filename:	ファイル名
-----------------------------------------------------------------------------*/
int write_bmp_mono(unsigned char image[Y_SIZE][X_SIZE], char *filename)
{
	long i, j;
	FILE *fp;
	long file_size, width, height;
	unsigned char *image_buf;
	unsigned char header1[54] = {0x42, 0x4d, 0, 0, 0, 0, 0, 0, 0, 0,
		54, 4, 0, 0, 40, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 1, 0, 8, 0,
		0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
		0, 0, 0, 0};
	unsigned char header2[1024];
	
	file_size = (long)X_SIZE * (long)Y_SIZE + 54 + 1024;
	header1[2] = (unsigned char)(file_size & 0x000000ff);
	header1[3] = (unsigned char)((file_size >> 8) & 0x000000ff);
	header1[4] = (unsigned char)((file_size >> 16)  & 0x000000ff);
	header1[5] = (unsigned char)((file_size >> 24)  & 0x000000ff);
	width = X_SIZE;
	header1[18] = (unsigned char)(width & 0x000000ff);
	header1[19] = (unsigned char)((width >> 8) & 0x000000ff);
	header1[20] = (unsigned char)((width >> 16) & 0x000000ff);
	header1[21] = (unsigned char)((width >> 24) & 0x000000ff);
	height = Y_SIZE;
	header1[22] = (unsigned char)(height & 0x000000ff);
	header1[23] = (unsigned char)((height >> 8) & 0x000000ff);
	header1[24] = (unsigned char)((height >> 16) & 0x000000ff);
	header1[25] = (unsigned char)((height >> 24) & 0x000000ff);
	for (i= 0; i < 256; i++) {
		header2[i*4  ] = (unsigned char)i;
		header2[i*4+1] = (unsigned char)i;
		header2[i*4+2] = (unsigned char)i;
		header2[i*4+3] = 0;
	}
	image_buf = (unsigned char *)malloc((size_t)X_SIZE*Y_SIZE);
	if (image_buf == NULL) return -1;
	for (i = 0; i < Y_SIZE; i++)
		for (j = 0; j < X_SIZE; j++)
			*(image_buf + X_SIZE*i + j) = image[Y_SIZE-i-1][j];
	if ((fp = fopen(filename, "wb")) == NULL) return -1;
	fwrite(header1, sizeof(unsigned char), 54, fp);
	fwrite(header2, sizeof(unsigned char), 1024, fp);
	fwrite(image_buf, sizeof(unsigned char), (size_t)(long)X_SIZE*Y_SIZE, fp);
	fclose(fp);
	free(image_buf);
	return 0;
}

/*--- read_bmp_color --- BMP形式のカラー画像をディスクから読み込む ------------
	image:		画像配列（カラー）
	filename:	ファイル名（.bmp）
-----------------------------------------------------------------------------*/
int read_bmp_color(unsigned char image[3][Y_SIZE][X_SIZE], char *filename)
{
	long i, j;
	FILE *fp;
	unsigned char *image_buf;
	unsigned char header[54];

	image_buf = (unsigned char *)malloc((size_t)X_SIZE*Y_SIZE*3);
	if (image_buf == NULL) return -1;
	if ((fp = fopen(filename, "rb")) == NULL) return -1;
	fread(header, sizeof(unsigned char), 54, fp);
	fread(image_buf, sizeof(unsigned char), (size_t)(long)X_SIZE*Y_SIZE*3, fp);
	fclose(fp);
	for (i = 0; i < Y_SIZE; i++)
		for (j = 0; j < X_SIZE; j++) {
			image[0][Y_SIZE-i-1][j] = *(image_buf + 3 * (X_SIZE*i + j) + 2);
			image[1][Y_SIZE-i-1][j] = *(image_buf + 3 * (X_SIZE*i + j) + 1);
			image[2][Y_SIZE-i-1][j] = *(image_buf + 3 * (X_SIZE*i + j)    );
		}
	free(image_buf);
	return 0;
}

/*--- write_bmp_color --- カラー画像をBMP形式でディスクに書き出す -------------
	image:		画像配列（カラー）
	filename:	ファイル名（.bmp）
-----------------------------------------------------------------------------*/
int write_bmp_color(unsigned char image[3][Y_SIZE][X_SIZE], char *filename)
{
	long i, j;
	FILE *fp;
	long file_size, width, height;
	unsigned char *image_buf;
	unsigned char header[54] = {0x42, 0x4d, 0, 0, 0, 0, 0, 0, 0, 0,
		54, 0, 0, 0, 40, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 1, 0, 24, 0,
		0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
		0, 0, 0, 0};

	file_size = (long)X_SIZE * (long)Y_SIZE * 3 + 54;
	header[2] = (unsigned char)(file_size & 0x000000ff);
	header[3] = (unsigned char)((file_size >> 8) & 0x000000ff);
	header[4] = (unsigned char)((file_size >> 16)  & 0x000000ff);
	header[5] = (unsigned char)((file_size >> 24)  & 0x000000ff);
	width = X_SIZE;
	header[18] = (unsigned char)(width & 0x000000ff);
	header[19] = (unsigned char)((width >> 8) & 0x000000ff);
	header[20] = (unsigned char)((width >> 16) & 0x000000ff);
	header[21] = (unsigned char)((width >> 24) & 0x000000ff);
	height = Y_SIZE;
	header[22] = (unsigned char)(height & 0x000000ff);
	header[23] = (unsigned char)((height >> 8) & 0x000000ff);
	header[24] = (unsigned char)((height >> 16) & 0x000000ff);
	header[25] = (unsigned char)((height >> 24) & 0x000000ff);
	image_buf = (unsigned char *)malloc((size_t)X_SIZE*Y_SIZE*3);
	if (image_buf == NULL) return -1;
	for (i = 0; i < Y_SIZE; i++)
		for (j = 0; j < X_SIZE; j++) {
			*(image_buf + 3*(X_SIZE*i + j)    ) = image[2][Y_SIZE-i-1][j];
			*(image_buf + 3*(X_SIZE*i + j) + 1) = image[1][Y_SIZE-i-1][j];
			*(image_buf + 3*(X_SIZE*i + j) + 2) = image[0][Y_SIZE-i-1][j];
		}
	if ((fp = fopen(filename, "wb")) == NULL) return -1;
	fwrite(header, sizeof(unsigned char), 54, fp);
	fwrite(image_buf, sizeof(unsigned char), (size_t)(long)X_SIZE*Y_SIZE*3, fp);
	fclose(fp);
	free(image_buf);
	return 0;
}

/*--- read_mono --- モノクロ画像をディスクから読み込む ------------------------
	image:		画像配列
	filename:	ファイル名
-----------------------------------------------------------------------------*/
int read_mono(unsigned char image[Y_SIZE][X_SIZE], char *filename)
{
	FILE 	*fp;

	fp = fopen(filename, "rb");
	if (fp == NULL) return -1;
	else {
		fread(image, (size_t)X_SIZE, (size_t)Y_SIZE, fp);
		fclose(fp);
		return 0;
	}
}

/*--- write_mono --- モノクロ画像をディスクに書き出す -------------------------
	image:		画像配列
	filename:	ファイル名
-----------------------------------------------------------------------------*/
int write_mono(unsigned char image[Y_SIZE][X_SIZE], char *filename)
{
	FILE	*fp;

	fp = fopen(filename, "wb");
	if (fp == NULL) return -1;
	else {
		fwrite(image, (size_t)X_SIZE, (size_t)Y_SIZE, fp);
		fclose(fp);
		return 0;
	}
}

/*--- read_rgb_plane --- RGB面順次形式のカラー画像をディスクから読み込む ------
	image:		画像配列（カラー）
	filename:	ファイル名
-----------------------------------------------------------------------------*/
int read_rgb_plane(unsigned char image[3][Y_SIZE][X_SIZE], char *filename)
{
	FILE	*fp;
	int		i;

	fp = fopen(filename, "rb");
	if (fp == NULL) return -1;
	else {
		for (i = 0; i < 3; i++)
			fread(&image[i][0][0], (size_t)X_SIZE, (size_t)Y_SIZE, fp);
		fclose(fp);
		return 0;
	}
}

/*--- write_rgb_plane --- カラー画像をRGB面順次形式でディスクに書き出す -------
	image:		画像配列（カラー）
	filename:	ファイル名
-----------------------------------------------------------------------------*/
int write_rgb_plane(unsigned char image[3][Y_SIZE][X_SIZE], char *filename)
{
	FILE	*fp;
	int		i;

	fp = fopen(filename, "wb");
	if (fp == NULL) return -1;
	else {
		for (i = 0; i < 3; i++)
			fwrite(&image[i][0][0], (size_t)X_SIZE, (size_t)Y_SIZE, fp);
		fclose(fp);
		return 0;
	}
}

/*--- read_rgb_line --- RGB線順次形式のカラー画像をディスクから読み込む -------
	image:		画像配列（カラー）
	filename:	ファイル名
-----------------------------------------------------------------------------*/
int read_rgb_line(unsigned char image[3][Y_SIZE][X_SIZE], char *filename)
{
	FILE *fp;
	long i, j;
	unsigned char *image_buf;

	image_buf = (unsigned char *)malloc((size_t)X_SIZE*Y_SIZE*3);
	if (image_buf == NULL) return -1;
	if ((fp = fopen(filename, "rb")) == NULL) return -1;
	fread(image_buf, sizeof(unsigned char), (size_t)(long)X_SIZE*Y_SIZE*3, fp);
	fclose(fp);
	for (i = 0; i < Y_SIZE; i++)
		for (j = 0; j < X_SIZE; j++) {
			image[0][i][j] = *(image_buf + (3*i    )*X_SIZE + j);
			image[1][i][j] = *(image_buf + (3*i + 1)*X_SIZE + j);
			image[2][i][j] = *(image_buf + (3*i + 2)*X_SIZE + j);
		}
	free(image_buf);
	return 0;
}

/*--- write_rgb_line --- カラー画像をRGB線順次形式でディスクに書き出す --------
	image:		画像配列（カラー）
	filename:	ファイル名
-----------------------------------------------------------------------------*/
int write_rgb_line(unsigned char image[3][Y_SIZE][X_SIZE], char *filename)
{
	FILE *fp;
	long i, j;
	unsigned char *image_buf;

	image_buf = (unsigned char *)malloc((size_t)X_SIZE*Y_SIZE*3);
	if (image_buf == NULL) return -1;
	for (i = 0; i < Y_SIZE; i++)
		for (j = 0; j < X_SIZE; j++) {
			*(image_buf + (3*i    )*X_SIZE + j) = image[0][i][j];
			*(image_buf + (3*i + 1)*X_SIZE + j) = image[1][i][j];
			*(image_buf + (3*i + 2)*X_SIZE + j) = image[2][i][j];
		}
	if ((fp = fopen(filename, "wb")) == NULL) return -1;
	fwrite(image_buf, sizeof(unsigned char), (size_t)(long)X_SIZE*Y_SIZE*3, fp);
	fclose(fp);
	free(image_buf);
	return 0;
}

/*--- read_rgb_pixel --- RGB点順次形式のカラー画像をディスクから読み込む ------
	image:		画像配列（カラー）
	filename:	ファイル名
-----------------------------------------------------------------------------*/
int read_rgb_pixel(unsigned char image[3][Y_SIZE][X_SIZE], char *filename)
{
	FILE *fp;
	long i, j;
	unsigned char *image_buf;

	image_buf = (unsigned char *)malloc((size_t)X_SIZE*Y_SIZE*3);
	if (image_buf == NULL) return -1;
	if ((fp = fopen(filename, "rb")) == NULL) return -1;
	fread(image_buf, sizeof(unsigned char), (size_t)(long)X_SIZE*Y_SIZE*3, fp);
	fclose(fp);
	for (i = 0; i < Y_SIZE; i++)
		for (j = 0; j < X_SIZE; j++) {
			image[0][i][j] = *(image_buf + 3*(X_SIZE*i + j)    );
			image[1][i][j] = *(image_buf + 3*(X_SIZE*i + j) + 1);
			image[2][i][j] = *(image_buf + 3*(X_SIZE*i + j) + 2);
		}
	free(image_buf);
	return 0;
}

/*--- write_rgb_pixel --- カラー画像をRGB点順次形式でディスクに書き出す -------
	image:		画像配列（カラー）
	filename:	ファイル名
-----------------------------------------------------------------------------*/
int write_rgb_pixel(unsigned char image[3][Y_SIZE][X_SIZE], char *filename)
{
	FILE *fp;
	long i, j;
	unsigned char *image_buf;

	image_buf = (unsigned char *)malloc((size_t)X_SIZE*Y_SIZE*3);
	if (image_buf == NULL) return -1;
	for (i = 0; i < Y_SIZE; i++)
		for (j = 0; j < X_SIZE; j++) {
			*(image_buf + 3*(X_SIZE*i + j)    ) = image[0][i][j];
			*(image_buf + 3*(X_SIZE*i + j) + 1) = image[1][i][j];
			*(image_buf + 3*(X_SIZE*i + j) + 2) = image[2][i][j];
		}
	if ((fp = fopen(filename, "wb")) == NULL) return -1;
	fwrite(image_buf, sizeof(unsigned char), (size_t)(long)X_SIZE*Y_SIZE*3, fp);
	fclose(fp);
	free(image_buf);
	return 0;
}

/*--- read_rgb_apart --- R,G,B別ファイルのカラー画像をディスクから読み込む ----
	image:		画像配列（カラー）
	filename:	ファイル名（.r .g .b）
-----------------------------------------------------------------------------*/
int read_rgb_apart(unsigned char image[3][Y_SIZE][X_SIZE], char *filename)
{
	FILE *fp;
	char fname_r[128], fname_g[128], fname_b[128];

	sprintf(fname_r, "%s.r", filename);
	sprintf(fname_g, "%s.g", filename);
	sprintf(fname_b, "%s.b", filename);
	if ((fp = fopen(fname_r, "rb")) == NULL) return -1;
	fread(image[0], sizeof(unsigned char), (size_t)(long)X_SIZE*Y_SIZE, fp);
	fclose(fp);
	if ((fp = fopen(fname_g, "rb")) == NULL) return -1;
	fread(image[1], sizeof(unsigned char), (size_t)(long)X_SIZE*Y_SIZE, fp);
	fclose(fp);
	if ((fp = fopen(fname_b, "rb")) == NULL) return -1;
	fread(image[2], sizeof(unsigned char), (size_t)(long)X_SIZE*Y_SIZE, fp);
	fclose(fp);
	return 0;
}

/*--- write_RGB_apart --- カラー画像をR,G,B別ファイルでディスクに書き出す -----
	image:		画像配列（カラー）
	filename:	ファイル名（.r .g .b）
-----------------------------------------------------------------------------*/
int write_rgb_apart(unsigned char image[3][Y_SIZE][X_SIZE], char *filename)
{
	FILE *fp;
	char fname_r[128], fname_g[128], fname_b[128];

	sprintf(fname_r, "%s.r", filename);
	sprintf(fname_g, "%s.g", filename);
	sprintf(fname_b, "%s.b", filename);
	if ((fp = fopen(fname_r, "wb")) == NULL) return -1;
	fwrite(image[0], sizeof(unsigned char), (size_t)(long)X_SIZE*Y_SIZE, fp);
	fclose(fp);
	if ((fp = fopen(fname_g, "wb")) == NULL) return -1;
	fwrite(image[1], sizeof(unsigned char), (size_t)(long)X_SIZE*Y_SIZE, fp);
	fclose(fp);
	if ((fp = fopen(fname_b, "wb")) == NULL) return -1;
	fwrite(image[2], sizeof(unsigned char), (size_t)(long)X_SIZE*Y_SIZE, fp);
	fclose(fp);
	return 0;
}
