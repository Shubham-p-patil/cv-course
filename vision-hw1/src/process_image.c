#include <stdio.h>
#include <string.h>
#include <assert.h>
#include <math.h>
#include "image.h"

int clip_value(int v, int min, int max)
{
    // clip value v
    assert(min <= max);
    if (v < min)
        v = min;
    if (v >= max)
        v = max - 1;
    return v;
}

float get_pixel_with_pad(image im, int x, int y, int c)
{
    // use zero padding strategies
    if (x < 0 || y < 0)
    {
        return 0.;
    }
    else if (x > im.w || y > im.h)
    {
        return 0.;
    }
    else
    {
        return im.data[x + im.w * y + im.w * im.h * c];
    }
}

float get_pixel(image im, int x, int y, int c)
{
    // TODO Fill this in
    // // use clamp padding strategies
    c = clip_value(c, 0, im.c);
    y = clip_value(y, 0, im.h);
    x = clip_value(x, 0, im.w);
    return im.data[x + im.w * y + im.w * im.h * c];
}

void set_pixel(image im, int x, int y, int c, float v)
{
    // TODO Fill this in
    im.data[x + im.w * y + im.w * im.h * c] = v;
}

image copy_image(image im)
{
    image copy = make_image(im.w, im.h, im.c);
    // TODO Fill this in
    memcpy(copy.data, im.data, (im.w * im.h * im.c) * sizeof(float));
    return copy;
}

image rgb_to_grayscale(image im)
{
    assert(im.c == 3);
    image gray = make_image(im.w, im.h, 1);
    // TODO Fill this in
    for (int i = 0; i < im.w; i++)
    {
        for (int j = 0; j < im.h; j++)
        {
            set_pixel(gray, i, j, 0, (0.299 * get_pixel(im, i, j, 0) + 0.587 * get_pixel(im, i, j, 1) + 0.114 * get_pixel(im, i, j, 2)));
        }
    }
    return gray;
}

void shift_image(image im, int c, float v)
{
    // TODO Fill this in
    for (int i = 0; i < im.w; i++)
    {
        for (int j = 0; j < im.h; j++)
        {
            float pixel_value = get_pixel(im, i, j, c);
            set_pixel(im, i, j, c, pixel_value + v);
        }
    }
}

void clamp_image(image im)
{
    // TODO Fill this in
    for (int w = 0; w < im.w; w++)
    {
        for (int h = 0; h < im.h; h++)
        {
            for (int c = 0; c < im.c; c++)
            {
                float pixel_value = get_pixel(im, w, h, c);
                pixel_value<0.0 ? set_pixel(im, w, h, c, 0.0) : pixel_value> 1.0 ? set_pixel(im, w, h, c, 1.0) : pixel_value;
            }
        }
    }
}

// These might be handy
float three_way_max(float a, float b, float c)
{
    return (a > b) ? ((a > c) ? a : c) : ((b > c) ? b : c);
}

float three_way_min(float a, float b, float c)
{
    return (a < b) ? ((a < c) ? a : c) : ((b < c) ? b : c);
}

void rgb_to_hsv(image im)
{

    // TODO Fill this in
    for (int w = 0; w < im.w; w++)
    {
        for (int h = 0; h < im.h; h++)
        {
            float R = get_pixel(im, w, h, 0);
            float G = get_pixel(im, w, h, 1);
            float B = get_pixel(im, w, h, 2);
            float V = three_way_max(R, G, B);
            float m = three_way_min(R, G, B);
            float C = V - m;
            float S = 0;
            float h_ = 0;

            if (V > 0)
                S = C / V;

            if (C != 0)
            {

                if (V == R)
                    h_ = (G - B) / C;
                else if (V == G)
                    h_ = ((B - R) / C) + 2;
                else if (V == B)
                    h_ = ((R - G) / C) + 4;
            }

            set_pixel(im, w, h, 1, S);
            set_pixel(im, w, h, 2, V);

            if (h_ < 0)
                set_pixel(im, w, h, 0, ((h_ / 6) + 1));
            else
                set_pixel(im, w, h, 0, (h_ / 6));
        }
    }
}

void hsv_to_rgb(image im)
{
    // TODO Fill this in
    for (int w = 0; w < im.w; w++)
    {
        for (int h = 0; h < im.h; h++)
        {
            float H = get_pixel(im, w, h, 0);
            float S = get_pixel(im, w, h, 1);
            float V = get_pixel(im, w, h, 2);

            float C = S * V;
            float m = V - C;

            float h_ = H * 6.0;
            float X = (1 - fabs(fmod(h_, 2) - 1)) * C;

            float R = 0., G = 0., B = 0.;
            if (0.0 <= h_ && h_ <= 1.)
            {
                R = C;
                G = X;
                B = 0;
            }
            else if (1. <= h_ && h_ <= 2.)
            {
                R = X;
                G = C;
                B = 0;
            }
            else if (2. <= h_ && h_ <= 3.)
            {
                R = 0;
                G = C;
                B = X;
            }
            else if (3. <= h_ && h_ <= 4.)
            {
                R = 0;
                G = X;
                B = C;
            }
            else if (4. <= h_ && h_ <= 5.)
            {
                R = X;
                G = 0;
                B = C;
            }
            else if (5. <= h_ && h_ <= 6.)
            {
                R = C;
                G = 0;
                B = X;
            }
        set_pixel(im, w, h, 0, R + m);
        set_pixel(im, w, h, 1, G + m);
        set_pixel(im, w, h, 2, B + m);

        // if (h_ >= 5)
        // {
        //     h_ = (H - 1) * 6;
        // }
        // else
        // {
        // }
        // if (-1 <= h_ && h_ < 1)
        // {
        //     if (h_ < 0)
        //     {
        //         set_pixel(im, w, h, 0, V);
        //         set_pixel(im, w, h, 1, m);
        //         set_pixel(im, w, h, 2, (m - (h_ * C)));
        //     }
        //     else
        //     {
        //         set_pixel(im, w, h, 0, V);
        //         set_pixel(im, w, h, 1, (m + (h_ * C)));
        //         set_pixel(im, w, h, 2, m);
        //     }
        // }
        // if (1 <= h_ && h_ < 3)
        // {
        //     if ((h_ - 2) < 0)
        //     {
        //         set_pixel(im, w, h, 0, (m - ((h_ - 2) * C)));
        //         set_pixel(im, w, h, 1, V);
        //         set_pixel(im, w, h, 2, m);
        //     }
        //     else
        //     {
        //         set_pixel(im, w, h, 0, m);
        //         set_pixel(im, w, h, 1, V);
        //         set_pixel(im, w, h, 2, (m + ((h_ - 2) * C)));
        //     }
        // }
        // if (3 <= h_ && h_ < 5)
        // {
        //     if ((h_ - 4) < 0)
        //     {
        //         set_pixel(im, w, h, 0, m);
        //         set_pixel(im, w, h, 1, (m - ((h_ - 4) * C)));
        //         set_pixel(im, w, h, 2, V);
        //     }
        //     else
        //     {
        //         set_pixel(im, w, h, 0, (m + ((h_ - 4) * C)));
        //         set_pixel(im, w, h, 1, m);
        //         set_pixel(im, w, h, 2, V);
        //     }
        // }
    }
}
}
