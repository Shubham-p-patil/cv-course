#include <math.h>
#include "image.h"

float nn_interpolate(image im, float x, float y, int c)
{
    // TODO Fill in
    return get_pixel(im, round(x), round(y), c);
}

image nn_resize(image im, int w, int h)
{
    // TODO Fill in (also fix that first line)
    float a_w = (float)im.w / w;
    float b_w = -0.5 + 0.5 * a_w;

    float a_h = (float)im.h / h;
    float b_h = -0.5 + 0.5 * a_h;

    image newImage = make_image(w, h, im.c);
    for (int i = 0; i < newImage.w; ++i)
    {
        for (int j = 0; j < newImage.h; ++j)
        {
            for (int k = 0; k < newImage.c; ++k)
            {
                float old_x = a_w * i + b_w;
                float old_y = a_h * j + b_h;
                set_pixel(newImage, i, j, k, nn_interpolate(im, old_x, old_y, k));
            }
        }
    }
    return newImage;
}

float bilinear_interpolate(image im, float x, float y, int c)
{
    // TODO
    int Xi = floor(x);
    int Yi = floor(y);
    int XiPlusOneLim = ceil(x);
    int YiPlusOneLim = ceil(y);
    float v1 = get_pixel(im, Xi, Yi, c);
    float v2 = get_pixel(im, Xi, YiPlusOneLim, c);
    float v3 = get_pixel(im, XiPlusOneLim, YiPlusOneLim, c);
    float v4 = get_pixel(im, XiPlusOneLim, Yi, c);

    float Yf = y - Yi;
    float Xf = x - Xi;

    float q1 = v1 * (1.0 - Yf) + v2 * Yf;
    float q2 = v3 * Yf + v4 * (1.0 - Yf);
    float q = q1 * (1.0 - Xf) + q2 * Xf;

    return q;
}

image bilinear_resize(image im, int w, int h)
{
    // TODO
    float a_w = (float)im.w / w;
    float b_w = -0.5 + 0.5 * a_w;

    float a_h = (float)im.h / h;
    float b_h = -0.5 + 0.5 * a_h;

    image newImage = make_image(w, h, im.c);
    for (int i = 0; i < newImage.w; ++i)
    {
        for (int j = 0; j < newImage.h; ++j)
        {
            for (int k = 0; k < newImage.c; ++k)
            {
                float old_x = a_w * i + b_w;
                float old_y = a_h * j + b_h;
                set_pixel(newImage, i, j, k, bilinear_interpolate(im, old_x, old_y, k));
            }
        }
    }
    return newImage;
}
