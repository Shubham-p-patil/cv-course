#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <math.h>
#include <assert.h>
#include "image.h"
#define TWOPI 6.2831853

void l1_normalize(image im)
{
    // TODO

    float factor = 0;
    for (int im_c = 0; im_c < im.c; im_c++)
    {
        factor = 0;
        for (int im_w = 0; im_w < im.w; im_w++)
        {
            for (int im_h = 0; im_h < im.h; im_h++)
            {
                factor += get_pixel(im, im_w, im_h, im_c);
            }
        }

        for (int im_w = 0; im_w < im.w; im_w++)
        {
            for (int im_h = 0; im_h < im.h; im_h++)
            {
                for (int im_c = 0; im_c < im.c; im_c++)
                {
                    set_pixel(im, im_w, im_h, im_c, get_pixel(im, im_w, im_h, im_c) / factor);
                }
            }
        }
    }
}

image make_box_filter(int w)
{
    // TODO
    image filter = make_image(w, w, 1);

    for (int im_w = 0; im_w < w; im_w++)
    {
        for (int im_h = 0; im_h < w; im_h++)
        {
            set_pixel(filter, im_w, im_h, 0, 1.0);
        }
    }

    l1_normalize(filter);
    return filter;
}

image convolve_image(image im, image filter, int preserve)
{
    // TODO
    // Note that filter's size can'n be even, i.e 1x1, 3x3, 5x5 etc
    assert(im.c == filter.c || filter.c == 1);

    int center_w = floor(filter.w / 2);
    int center_h = floor(filter.h / 2);
    float q = 0;

    int numOfChannel;
    if (im.c == filter.c)
    {
        if (preserve == 1)
        {
            numOfChannel = im.c;
        }
        else
        {
            numOfChannel = 1;
        }
    }
    else if (filter.c == 1)
    {
        numOfChannel = 1;
        if (preserve == 1)
        {
            numOfChannel = 3;
        }
    }

    image after_convolve = make_image(im.w, im.h, numOfChannel);
    for (int im_w = 0; im_w < im.w; im_w++)
    {
        for (int im_h = 0; im_h < im.h; im_h++)
        {
            q = 0;

            for (int im_c = 0; im_c < im.c; im_c++)
            {
                if (numOfChannel > 1)
                {
                    q = 0;
                }

                for (int fil_w = 0; fil_w < filter.w; fil_w++)
                {
                    for (int fil_h = 0; fil_h < filter.h; fil_h++)
                    {
                        if (numOfChannel > 1)
                        {
                            q += get_pixel(im, (im_w + fil_w - center_w), (im_h + fil_h - center_h), im_c) * get_pixel(filter, fil_w, fil_h, im_c);
                            // printf("%f", get_pixel(filter, fil_w, fil_h, im_c));
                        }
                        else
                        {
                            q += get_pixel(im, (im_w + fil_w - center_w), (im_h + fil_h - center_h), im_c) * get_pixel(filter, fil_w, fil_h, 0);
                            // printf("%f", get_pixel(filter, fil_w, fil_h, im_c));
                        }
                    }
                }
                if (numOfChannel > 1)
                    set_pixel(after_convolve, im_w, im_h, im_c, q);
                else
                    set_pixel(after_convolve, im_w, im_h, 0, q);
            }
        }
    }
    return after_convolve;
}

image make_highpass_filter()
{
    // TODO
    image filter = make_image(3, 3, 1);

    set_pixel(filter, 0, 0, 0, 0);
    set_pixel(filter, 1, 0, 0, -1);
    set_pixel(filter, 2, 0, 0, 0);

    set_pixel(filter, 0, 1, 0, -1);
    set_pixel(filter, 1, 1, 0, 4);
    set_pixel(filter, 2, 1, 0, -1);

    set_pixel(filter, 0, 2, 0, 0);
    set_pixel(filter, 1, 2, 0, -1);
    set_pixel(filter, 2, 2, 0, 0);

    return filter;
}

image make_sharpen_filter()
{
    // TODO
    image filter = make_image(3, 3, 1);

    set_pixel(filter, 0, 0, 0, 0);
    set_pixel(filter, 1, 0, 0, -1);
    set_pixel(filter, 2, 0, 0, 0);

    set_pixel(filter, 0, 1, 0, -1);
    set_pixel(filter, 1, 1, 0, 5);
    set_pixel(filter, 2, 1, 0, -1);

    set_pixel(filter, 0, 2, 0, 0);
    set_pixel(filter, 1, 2, 0, -1);
    set_pixel(filter, 2, 2, 0, 0);

    return filter;
}

image make_emboss_filter()
{
    // TODO
    image filter = make_image(3, 3, 1);

    set_pixel(filter, 0, 0, 0, -2);
    set_pixel(filter, 1, 0, 0, -1);
    set_pixel(filter, 2, 0, 0, 0);

    set_pixel(filter, 0, 1, 0, -1);
    set_pixel(filter, 1, 1, 0, 1);
    set_pixel(filter, 2, 1, 0, 1);

    set_pixel(filter, 0, 2, 0, 0);
    set_pixel(filter, 1, 2, 0, 1);
    set_pixel(filter, 2, 2, 0, 2);

    return filter;
}

// Question 2.2.1: Which of these filters should we use preserve when we run our convolution and which ones should we not? Why?
// Answer: TODO

// Question 2.2.2: Do we have to do any post-processing for the above filters? Which ones and why?
// Answer: TODO

image make_gaussian_filter(float sigma)
{
    // TODO

    int sizeofKernel = ceil(sigma) * 6;
    sizeofKernel = sizeofKernel % 2 == 0 ? sizeofKernel + 1 : sizeofKernel;
    int c_w = sizeofKernel / 2;
    int c_h = sizeofKernel / 2;
    image filter = make_image(sizeofKernel, sizeofKernel, 1);
    for (int im_w = 0; im_w < sizeofKernel; im_w++)
    {
        for (int im_h = 0; im_h < sizeofKernel; im_h++)
        {
            float gaussian_value = exp(-((pow(im_w - c_w, 2) + pow(im_h - c_h, 2)) / (2 * pow(sigma, 2)))) / (TWOPI * pow(sigma, 2));
            set_pixel(filter, im_w, im_h, 0, gaussian_value);
        }
    }
    l1_normalize(filter);
    return filter;
}

image add_image(image a, image b)
{
    // TODO
    assert(a.w == b.w && a.h == b.h);
    image newImage = make_image(a.w, a.h, a.c);
    for (int im_w = 0; im_w < a.w; im_w++)
    {
        for (int im_h = 0; im_h < a.h; im_h++)
        {
            for (int im_c = 0; im_c < a.c; im_c++)
            {
                set_pixel(newImage, im_w, im_h, im_c, get_pixel(a, im_w, im_h, im_c) + get_pixel(b, im_w, im_h, im_c));
            }
        }
    }

    return newImage;
}

image sub_image(image a, image b)
{
    // TODO
    assert(a.w == b.w && a.h == b.h);
    image newImage = make_image(a.w, a.h, a.c);
    for (int im_w = 0; im_w < a.w; im_w++)
    {
        for (int im_h = 0; im_h < a.h; im_h++)
        {
            for (int im_c = 0; im_c < a.c; im_c++)
            {
                set_pixel(newImage, im_w, im_h, im_c, get_pixel(a, im_w, im_h, im_c) - get_pixel(b, im_w, im_h, im_c));
            }
        }
    }

    return newImage;
}

image make_gx_filter()
{
    // TODO
    image filter = make_image(3, 3, 1);

    set_pixel(filter, 0, 0, 0, -1);
    set_pixel(filter, 1, 0, 0, 0);
    set_pixel(filter, 2, 0, 0, 1);

    set_pixel(filter, 0, 1, 0, -2);
    set_pixel(filter, 1, 1, 0, 0);
    set_pixel(filter, 2, 1, 0, 2);

    set_pixel(filter, 0, 2, 0, -1);
    set_pixel(filter, 1, 2, 0, 0);
    set_pixel(filter, 2, 2, 0, 1);

    return filter;
}

image make_gy_filter()
{
    // TODO
    image filter = make_image(3, 3, 1);

    set_pixel(filter, 0, 0, 0, -1);
    set_pixel(filter, 1, 0, 0, -2);
    set_pixel(filter, 2, 0, 0, -1);

    set_pixel(filter, 0, 1, 0, 0);
    set_pixel(filter, 1, 1, 0, 0);
    set_pixel(filter, 2, 1, 0, 0);

    set_pixel(filter, 0, 2, 0, 1);
    set_pixel(filter, 1, 2, 0, 2);
    set_pixel(filter, 2, 2, 0, 1);

    return filter;
    return make_image(1, 1, 1);
}

void feature_normalize(image im)
{
    // TODO
    float im_max;
    float im_min;
    for (int im_c = 0; im_c < im.c; im_c++)
    {
        im_max = -INFINITY;
        im_min = INFINITY;

        for (int im_w = 0; im_w < im.w; im_w++)
        {
            for (int im_h = 0; im_h < im.h; im_h++)
            {
                im_max = fmax(get_pixel(im, im_w, im_h, im_c), im_max);
                im_min = fmin(get_pixel(im, im_w, im_h, im_c), im_min);
            }
        }
        for (int im_w = 0; im_w < im.w; im_w++)
        {
            for (int im_h = 0; im_h < im.h; im_h++)
            {
                if (im_max - im_min > 0)
                    set_pixel(im, im_w, im_h, im_c, (get_pixel(im, im_w, im_h, im_c) - im_min) / (im_max - im_min));
                else
                {
                    printf("Everything Zero");
                    set_pixel(im, im_w, im_h, im_c, 0);
                }
            }
        }
    }
}

image *sobel_image(image im)
{
    // TODO
    image magnitude = make_image(im.w, im.h, 1);
    image direction = make_image(im.w, im.h, 1);
    image gx_image = convolve_image(im, make_gx_filter(), 0);
    image gy_image = convolve_image(im, make_gy_filter(), 0);
    image *images = calloc(2, sizeof(image));
    for (int im_w = 0; im_w < im.w; im_w++)
    {
        for (int im_h = 0; im_h < im.h; im_h++)
        {
            set_pixel(magnitude, im_w, im_h, 0, sqrt(pow(get_pixel(gx_image, im_w, im_h, 0), 2) + pow(get_pixel(gy_image, im_w, im_h, 0), 2)));
            set_pixel(direction, im_w, im_h, 0, atan2(get_pixel(gy_image, im_w, im_h, 0), get_pixel(gx_image, im_w, im_h, 0)));
        }
    }

    images[0] = magnitude;
    images[1] = direction;
    return images;
}

image colorize_sobel(image im)
{
    // TODO

    return make_image(1, 1, 1);
}
