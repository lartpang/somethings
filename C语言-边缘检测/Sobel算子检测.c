// author：Lart Pang
// time  ：2018-10-16

#define X_SIZE 360
#define Y_SIZE 576
#define HIGH 255
#define LOW 0

/////////
// 灰度图 先左半个个图像进行灰度处理//
/////////
void GrayImage(Uint8 *in_data)
{
    Uint32 i,j;

    for(i = 0; i < 576; i++)
    {
        for(j=0; j <360; j++)
        {
            *(Uint8 *)(in_data + (i*720 + j) * 2) = 0x80;
        }
    }
}

/////////
// 二值化 //
/////////
void Threshold(Uint8 *image_in, int thresh, int type)
{
    int i, j;

    for (i = 0; i < Y_SIZE; i++)
    {
        for (j = 0; j < X_SIZE; j++)
        {
            switch (type)
            {
                case 1:
                    if ((*(Uint8 *)(image_in + (i*X_SIZE + j))) <= thresh)
                        (*(Uint8 *)(image_in + (i*X_SIZE + j))) = HIGH;
                    else
                        (*(Uint8 *)(image_in + (i*X_SIZE + j))) =  LOW;
                    break;
                default:
                    if ((*(Uint8 *)(image_in + (i*X_SIZE + j))) >= thresh)
                        (*(Uint8 *)(image_in + (i*X_SIZE + j))) = HIGH;
                    else
                        (*(Uint8 *)(image_in + (i*X_SIZE + j))) =  LOW;
                    break;
            }
        }
    }
}

///////////////
// Sobel算子检测 //
///////////////
int cx_sobel[9] =       {-1,   0,  1,
                         -2,   0,  2,
                         -1,   0,  1 };
int cy_sobel[9] =       {-1,  -2, -1,
                          0,   0,  0,
                          1,   2,  1 };
// 另一个算子
int cx_scharr[9] =      { -3,  0,  3,
                         -10,  0, 10,
                          -3,  0,  3 };
int cy_scharr[9] =      {-3, -10, -3,
                          0,   0,  0,
                          3,  10,  3 };

void GradientSobel(Uint8 *image_in, double amp)
{
    int     d[9];
    int     i, j, dat;
    double  xx, yy, zz;

    Uint8 *image_out = (Uint8 *)malloc(Y_SIZE * X_SIZE * sizeof(Uint8));

    for (i = 0; i < Y_SIZE; i++)
    {
        for (j = 0; j < X_SIZE; j++)
        {
            (*(Uint8 *)(image_out + (i*X_SIZE + j))) = (*(Uint8 *)(image_in + (i*X_SIZE + j)));
        }
    }

    for (i = 1; i < Y_SIZE-1; i++)
    {
        for (j = 1; j < X_SIZE-1; j++)
        {
            d[0] = (*(Uint8 *)(image_out + ((i-1)*X_SIZE + j-1)));
            d[1] = (*(Uint8 *)(image_out + ((i-1)*X_SIZE + j  )));
            d[2] = (*(Uint8 *)(image_out + ((i-1)*X_SIZE + j+1)));
            d[3] = (*(Uint8 *)(image_out + ((i  )*X_SIZE + j-1)));
            d[4] = (*(Uint8 *)(image_out + ((i  )*X_SIZE + j  )));
            d[5] = (*(Uint8 *)(image_out + ((i  )*X_SIZE + j+1)));
            d[6] = (*(Uint8 *)(image_out + ((i+1)*X_SIZE + j-1)));
            d[7] = (*(Uint8 *)(image_out + ((i+1)*X_SIZE + j  )));
            d[8] = (*(Uint8 *)(image_out + ((i+1)*X_SIZE + j+1)));

            xx = (double)(cx_sobel[0]*d[0] + cx_sobel[1]*d[1] + cx_sobel[2]*d[2]
                        + cx_sobel[3]*d[3] + cx_sobel[4]*d[4] + cx_sobel[5]*d[5]
                        + cx_sobel[6]*d[6] + cx_sobel[7]*d[7] + cx_sobel[8]*d[8]);
            yy = (double)(cy_sobel[0]*d[0] + cy_sobel[1]*d[1] + cy_sobel[2]*d[2]
                        + cy_sobel[3]*d[3] + cy_sobel[4]*d[4] + cy_sobel[5]*d[5]
                        + cy_sobel[6]*d[6] + cy_sobel[7]*d[7] + cy_sobel[8]*d[8]);
            zz = (double)(amp * sqrt(xx * xx + yy * yy));
            dat = (int)zz;
            if(dat > 255)
                dat = 255;

            (*(Uint8 *)(image_in + (i*X_SIZE + j))) = (Uint8 )dat;
        }
    }

    free(image_out);
}

/////////
//主要函数 //
/////////
void EageDetect(Uint8 *in_data)
{
    int     i, j;

    Uint8 *image_out = (Uint8 *)malloc(Y_SIZE * X_SIZE * sizeof(Uint8));

    printf("begin======================\n");

    GrayImage(in_data);
    printf("GrayImage\n");

    for (i = 0; i < Y_SIZE; i++)
    {
        for (j = 0; j < X_SIZE; j++)
        {
            // 只保存了左半图
            (*(Uint8 *)(image_out + (i*X_SIZE + j))) = (*(Uint8 *)(in_data + 1 + (i*720 + j) * 2));
        }
    }

    //Expand(image_out, 180, 80);
    //printf("Expand\n");

    //Median(in_data);
    //printf("Median\n");

    Threshold(image_out, 180, 1);
    printf("Threshold\n");

    //Erosion(in_data);
    //printf("Erosion\n");

    GradientSobel(image_out, 2.0);
    printf("GradientSobel\n");

    printf("end========================\n");
    for (i = 0; i < Y_SIZE; i++)
    {
        for (j = 0; j < X_SIZE; j++)
        {
            // 只保存了左半图
            (*(Uint8 *)(in_data + 1 + (i*720 + j+X_SIZE) * 2)) = (*(Uint8 *)(image_out + (i*X_SIZE + j)));
        }
    }
    free(image_out);
}
