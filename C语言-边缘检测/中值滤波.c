//////////
// 中值滤波 //
//////////
int median_value(Uint8 c[9])
{
    int i, j, buf;

    for (j = 0; j < 8; j++) 
    {
        for (i = 0; i < 8; i++) 
        {
            if (c[i+1] < c[i]) 
            {
                buf = c[i+1];
                c[i+1] = c[i];
                c[i] = buf;
            }
        }
    }
    return c[4];
}

void Median(Uint8 *image_in)
{
    int i, j;
    Uint8 c[9];

    for (i = 1; i < Y_SIZE-1; i++) 
    {
        for (j = 1; j < X_SIZE-1; j++) 
        {
            c[0] = (*(Uint8 *)(image_in + 1 + ((i-1)*720 + j-1) * 2));
            c[1] = (*(Uint8 *)(image_in + 1 + ((i-1)*720 + j  ) * 2));
            c[2] = (*(Uint8 *)(image_in + 1 + ((i-1)*720 + j+1) * 2));
            c[3] = (*(Uint8 *)(image_in + 1 + ((i  )*720 + j-1) * 2));
            c[4] = (*(Uint8 *)(image_in + 1 + ((i  )*720 + j  ) * 2));
            c[5] = (*(Uint8 *)(image_in + 1 + ((i  )*720 + j+1) * 2));
            c[6] = (*(Uint8 *)(image_in + 1 + ((i+1)*720 + j-1) * 2));
            c[7] = (*(Uint8 *)(image_in + 1 + ((i+1)*720 + j  ) * 2));
            c[8] = (*(Uint8 *)(image_in + 1 + ((i+1)*720 + j+1) * 2));

            (*(Uint8 *)(image_in + 1 + ((i)*720 + j) * 2)) = median_value(c);
        }
    }
}