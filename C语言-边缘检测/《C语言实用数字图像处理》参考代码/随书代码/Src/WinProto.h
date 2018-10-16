#include	"Proto.h"

void BitmapToRgb(HBITMAP hBitmap, unsigned char pr[Y_SIZE][X_SIZE], 
	unsigned char pg[Y_SIZE][X_SIZE], unsigned char pb[Y_SIZE][X_SIZE]);
void DisplayCImage(unsigned char image[3][Y_SIZE][X_SIZE], int position);
void DisplayImage(unsigned char image[Y_SIZE][X_SIZE], int position);
BOOL GetParams1(char *szQuestion, double *param1);
BOOL GetParams2(char *szQuestion, char *szQuestion1, char *szQuestion2,
	double *param1, double *param2);
BOOL GetParams3(char *szQuestion, char *szQuestion1, char *szQuestion2,
	char *szQuestion3, double *param1, double *param2, double *param3);
BOOL GetParams4(char *szQuestion, char *szQuestion1, char *szQuestion2,
	char *szQuestion3, char *szQuestion4,
	double *param1, double *param2, double *param3, double *param4);
BOOL GetParams5(char *szQuestion, char *szQuestion1, char *szQuestion2,
	char *szQuestion3, char *szQuestion4, char *szQuestion5,
	double *param1, double *param2, double *param3, double *param4, double *param5);
BOOL GetParams6(char *szQuestion, char *szQuestion1, char *szQuestion2,
	char *szQuestion3, char *szQuestion4, char *szQuestion5, char *szQuestion6,
	double *param1, double *param2, double *param3, double *param4, double *param5,
	double *param6);
BOOL GetParams8(char *szQuestion, char *szQuestion1, char *szQuestion2,
	char *szQuestion3, char *szQuestion4, char *szQuestion5, char *szQuestion6,
	char *szQuestion7, char *szQuestion8,
	double *param1, double *param2, double *param3, double *param4, double *param5,
	double *param6, double *param7, double *param8);
BOOL GetSelection2(char *szQuestion, char *szQuestion1, char *szQuestion2,
	int *param);
BOOL GetSelection3(char *szQuestion, char *szQuestion1, char *szQuestion2,
	char *szQuestion3,int *param);
BOOL ShowText(char *szQuestion, char *szResult);
int ImgPrint(unsigned char pr[P_Y_SIZE][P_X_SIZE], 
	unsigned char pg[P_Y_SIZE][P_X_SIZE], 
	unsigned char pb[P_Y_SIZE][P_X_SIZE]);
void m_dither(unsigned char image_in[P_Y_SIZE*4][P_X_SIZE], 
	unsigned char image_out[P_Y_SIZE][P_X_SIZE]);
void o_dither(unsigned char image_in[P_Y_SIZE][X_SIZE], 
	unsigned char image_out[P_Y_SIZE][P_X_SIZE]);
void RgbToBitmap(unsigned char pr[Y_SIZE][X_SIZE], 
	unsigned char pg[Y_SIZE][X_SIZE], 
	unsigned char pb[Y_SIZE][X_SIZE], HBITMAP hBitmap);
