//////////
// 收缩处理 //
//////////
void Erosion(Uint8 *image_in)
{
    int i, j;
    Uint8 *image_out = (Uint8*)malloc((Y_SIZE * X_SIZE) * sizeof(Uint8));

    for (i = 1; i < Y_SIZE-1; i++) 
    {
        for (j = 1; j < X_SIZE-1; j++) 
        {
            (*(Uint8 *)(image_out + (i*720 + j))) = (*(Uint8 *)(image_in + 1 + (i*720 + j) * 2));
        }
    }

    for (i = 1; i < Y_SIZE-1; i++) 
    {
        for (j = 1; j < X_SIZE-1; j++) 
        {
            if      ((*(Uint8 *)(image_out + ((i-1)*720 + j-1))) == LOW) 
                (*(Uint8 *)(image_in + 1 + (i*720 + j) * 2)) = LOW;

            else if ((*(Uint8 *)(image_out + ((i-1)*720 + j  ))) == LOW) 
                (*(Uint8 *)(image_in + 1 + (i*720 + j) * 2)) = LOW;

            else if ((*(Uint8 *)(image_out + ((i-1)*720 + j+1))) == LOW) 
                (*(Uint8 *)(image_in + 1 + (i*720 + j) * 2)) = LOW;

            else if ((*(Uint8 *)(image_out + ((i  )*720 + j-1))) == LOW) 
                (*(Uint8 *)(image_in + 1 + (i*720 + j) * 2)) = LOW;

            else if ((*(Uint8 *)(image_out + ((i  )*720 + j+1))) == LOW) 
                (*(Uint8 *)(image_in + 1 + (i*720 + j) * 2)) = LOW;

            else if ((*(Uint8 *)(image_out + ((i+1)*720 + j-1))) == LOW) 
                (*(Uint8 *)(image_in + 1 + (i*720 + j) * 2)) = LOW;

            else if ((*(Uint8 *)(image_out + ((i+1)*720 + j  ))) == LOW) 
                (*(Uint8 *)(image_in + 1 + (i*720 + j) * 2)) = LOW;

            else if ((*(Uint8 *)(image_out + ((i+1)*720 + j+1))) == LOW) 
                (*(Uint8 *)(image_in + 1 + (i*720 + j) * 2)) = LOW;
        }
    }
    free(image_out);
}


// 膨胀处理
void Dilation(Uint8 *image_in)
{
    int i, j;
    Uint8 image_out[Y_SIZE][X_SIZE];

    for (i = 0; i < Y_SIZE; i++) 
    {
        for (j = 0; j < X_SIZE; j++) 
        {
            image_out[i][j] = (*(Uint8 *)(image_in + 1 + (i*720 + j) * 2));
        }
    }

    for (i = 1; i < Y_SIZE-1; i++) 
    {
        for (j = 1; j < X_SIZE-1; j++) 
        {
            if (image_out[i-1][j-1] == HIGH)
                (*(Uint8 *)(image_in + 1 + (i*720 + j) * 2)) = HIGH;

            else if (image_out[i-1][j] == HIGH) 
                (*(Uint8 *)(image_in + 1 + (i*720 + j) * 2)) = HIGH;

            else if (image_out[i-1][j+1] == HIGH) 
                (*(Uint8 *)(image_in + 1 + (i*720 + j) * 2)) = HIGH;

            else if (image_out[i][j-1] == HIGH) 
                (*(Uint8 *)(image_in + 1 + (i*720 + j) * 2)) = HIGH;

            else if (image_out[i][j+1] == HIGH) 
                (*(Uint8 *)(image_in + 1 + (i*720 + j) * 2)) = HIGH;

            else if (image_out[i+1][j-1] == HIGH) 
                (*(Uint8 *)(image_in + 1 + (i*720 + j) * 2)) = HIGH;

            else if (image_out[i+1][j] == HIGH) 
                (*(Uint8 *)(image_in + 1 + (i*720 + j) * 2)) = HIGH;

            else if (image_out[i+1][j+1] == HIGH) 
                (*(Uint8 *)(image_in + 1 + (i*720 + j) * 2)) = HIGH;
        }
    }
}