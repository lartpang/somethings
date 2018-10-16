///////////////
// 简单的边缘检测思路 //
///////////////
void EageDetection(Uint8 *image_in)
{
    int i, j, n = 0;
    Uint8 *image_out = (Uint8*)malloc((Y_SIZE * X_SIZE) * sizeof(Uint8));

    for (i = 0; i < Y_SIZE; i++) 
    {
        for (j = 0; j < X_SIZE; j++) 
        {
            (*(Uint8 *)(image_out + (i*720 + j))) = (*(Uint8 *)(image_in + 1 + (i*720 + j) * 2));
        }
    }
    
    for (i = 1; i < Y_SIZE-1; i++)
    {
        for (j = 1; j < X_SIZE-1; j++)
        {
            if ((*(Uint8 *)(image_out + ((i  )*720 + j  ))) != (*(Uint8 *)(image_out + ((i  )*720 + j+1)))) 

                n++;
            if ((*(Uint8 *)(image_out + ((i  )*720 + j  ))) != (*(Uint8 *)(image_out + ((i  )*720 + j-1)))) 
                n++;

            if ((*(Uint8 *)(image_out + ((i  )*720 + j  ))) != (*(Uint8 *)(image_out + ((i-1)*720 + j+1)))) 
                n++;

            if ((*(Uint8 *)(image_out + ((i  )*720 + j  ))) != (*(Uint8 *)(image_out + ((i-1)*720 + j-1))))
                n++;

            if ((*(Uint8 *)(image_out + ((i  )*720 + j  ))) != (*(Uint8 *)(image_out + ((i+1)*720 + j+1))))
                n++;

            if ((*(Uint8 *)(image_out + ((i  )*720 + j  ))) != (*(Uint8 *)(image_out + ((i+1)*720 + j-1))))
                n++;

            if ((*(Uint8 *)(image_out + ((i  )*720 + j  ))) != (*(Uint8 *)(image_out + ((i+1)*720 + j  ))))
                n++;

            if ((*(Uint8 *)(image_out + ((i  )*720 + j  ))) != (*(Uint8 *)(image_out + ((i-1)*720 + j  ))))
                n++;
            
            if (n >= 3)
            {
                (*(Uint8 *)(image_in + 1 + (i*720 + j) * 2)) = HIGH;
            }
            else
            {
                (*(Uint8 *)(image_in + 1 + (i*720 + j) * 2)) = LOW;
            }
            n = 0;
        }
    }

    free(image_out);
}