//////////
// 增强亮度 //
//////////
void Amplify(Uint8 *image_in, double a, double b)
{
    int i, j, d;

    for (i = 0; i < Y_SIZE; i++)
    {
        for (j = 0; j < X_SIZE; j++)
        {
            d = (int)(*(Uint8 *)(image_in + 1 + (i*720 + j) * 2) * a + b);

            if (d <   0) 
                d =   0;
            if (d > 255) 
                d = 255;

            *(Uint8 *)(image_in + 1 + (i*720 + j) * 2) = (Uint8)d;
        }
    }
}

void Expand(Uint8 *image_in, int fmax, int fmin)
{
    double a, b;

    a = 255.0 / (double)((fmax) - (fmin));
    b = -255.0 * (fmin) / (double)((fmax) - (fmin));

    Amplify(image_in, a, b);
}
