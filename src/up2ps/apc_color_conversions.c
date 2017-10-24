/*
 * colorspace_conversions.c
 *
 *  Created on: Aug 11, 2012
 *      Author: bogdan
 */
/*
 * ColorSpace.c
 *
 *  Created on: Jun 16, 2012
 *      Author: bogdan
 */

#include "apc_color_conversions.h"


bool RealIsWithinBounds(double value, double lowerLimit, double upperLimit)
{
    if (value >= lowerLimit && value <= upperLimit)
    {
        return true;
    }
    else
    {
        return false;
    }
}

bool IntegerIsWithinBounds(uint8_t value, uint8_t lowerLimit,
        uint8_t upperLimit)
{
    if (value >= lowerLimit && value <= upperLimit)
    {
        return true;
    }
    else
    {
        return false;
    }
}

double Double_GetMinimum(double r, double g, double b)
{
    if (r < g)
    {
        if (r < b)
        {
            return r;
        }
        else
        {
            return b;
        }
    }
    else
    {
        if (g < b)
        {
            return g;
        }
        else
        {
            return b;
        }
    }
    return 0;
}

double Double_GetMaximum(double r, double g, double b)
{
    if (r > g)
    {
        if (r > b)
        {
            return r;
        }
        else
        {
            return b;
        }
    }
    else
    {
        if (g > b)
        {
            return g;
        }
        else
        {
            return b;
        }
    }
    return 0;
}

bool RgbF_IsValid(double r, double g, double b)
{
    bool isValid = true;
    if ((RealIsWithinBounds(r, PER_LOWER_LIMIT, PER_UPPER_LIMIT) == false)
            || (RealIsWithinBounds(g, PER_LOWER_LIMIT, PER_UPPER_LIMIT) == false)
            || (RealIsWithinBounds(b, PER_LOWER_LIMIT, PER_UPPER_LIMIT) == false))
    {
        isValid = false;
    }
    return isValid;
}

bool Hsi_IsValid(double h, double s, double i)
{
    bool isValid = true;
    if ((RealIsWithinBounds(h, HUE_LOWER_LIMIT, HUE_UPPER_LIMIT) == false)
            || (RealIsWithinBounds(s, PER_LOWER_LIMIT, PER_UPPER_LIMIT) == false)
            || (RealIsWithinBounds(i, PER_LOWER_LIMIT, PER_UPPER_LIMIT) == false))
    {
        isValid = false;
    }
    return isValid;
}

bool RgbI_IsValid(uint8_t r, uint8_t g, uint8_t b)
{
    bool isValid = true;
    if ((IntegerIsWithinBounds(r, RGBI_LOWER_LIMIT, RGBI_UPPER_LIMIT) == false)
            || (IntegerIsWithinBounds(g, RGBI_LOWER_LIMIT, RGBI_UPPER_LIMIT)
                    == false)
            || (IntegerIsWithinBounds(b, RGBI_LOWER_LIMIT, RGBI_UPPER_LIMIT)
                    == false))
    {
        isValid = false;
    }
    return isValid;
}

bool Hsl_IsValid(double h, double s, double l)
{
    bool isValid = true;
    if ((RealIsWithinBounds(h, HUE_LOWER_LIMIT, HUE_UPPER_LIMIT) == false)
            || (RealIsWithinBounds(s, PER_LOWER_LIMIT, PER_UPPER_LIMIT) == false)
            || (RealIsWithinBounds(l, PER_LOWER_LIMIT, PER_UPPER_LIMIT) == false))
    {
        isValid = false;
    }
    return isValid;
}

bool Hsv_IsValid(double h, double s, double v)
{
    bool isValid = true;
    if ((RealIsWithinBounds(h, HUE_LOWER_LIMIT, HUE_UPPER_LIMIT) == false)
            || (RealIsWithinBounds(s, PER_LOWER_LIMIT, PER_UPPER_LIMIT) == false)
            || (RealIsWithinBounds(v, PER_LOWER_LIMIT, PER_UPPER_LIMIT) == false))
    {
        isValid = false;
    }
    return isValid;
}

bool Yiq_IsValid(double y, double i, double q)
{
    bool isValid = true;
    if ((RealIsWithinBounds(y, PER_LOWER_LIMIT, PER_UPPER_LIMIT) == false)
            || (RealIsWithinBounds(i, YIQ_I_LOWER_LIMIT, YIQ_I_UPPER_LIMIT)
                    == false)
            || (RealIsWithinBounds(q, YIQ_Q_LOWER_LIMIT, YIQ_Q_UPPER_LIMIT)
                    == false))
    {
        isValid = false;
    }
    return isValid;
}

bool Yuv_IsValid(double y, double u, double v)
{
    bool isValid = true;
    if ((RealIsWithinBounds(y, PER_LOWER_LIMIT, PER_UPPER_LIMIT) == false)
            || (RealIsWithinBounds(u, YUV_U_LOWER_LIMIT, YUV_U_UPPER_LIMIT)
                    == false)
            || (RealIsWithinBounds(v, YUV_V_LOWER_LIMIT, YUV_V_UPPER_LIMIT)
                    == false))
    {
        isValid = false;
    }
    return isValid;
}


RgbFColor* RgbF_Create(double r, double g, double b)
{
    RgbFColor* color = NULL;
    if (RgbF_IsValid(r, g, b) == true)
    {
        color = (RgbFColor*) malloc(sizeof(RgbFColor));
        if (color != NULL)
        {
            color->R = r;
            color->G = g;
            color->B = b;
        }
    }
    return color;
}

RgbIColor* RgbI_Create(uint8_t r, uint8_t g, uint8_t b)
{
    RgbIColor* color = NULL;
    if (RgbI_IsValid(r, g, b) == true)
    {
        color = (RgbIColor*) malloc(sizeof(RgbIColor));
        if (color != NULL)
        {
            color->R = r;
            color->G = g;
            color->B = b;
        }
    }
    return color;
}

HsiColor* Hsi_Create(double h, double s, double i)
{
    HsiColor* color = NULL;
    if (Hsi_IsValid(h, s, i) == true)
    {
        color = (HsiColor*) malloc(sizeof(HsiColor));
        if (color != NULL)
        {
            color->H = h;
            color->S = s;
            color->I = i;
        }
    }
    return color;
}

HslColor* Hsl_Create(double h, double s, double l)
{
    HslColor* color = NULL;
    if (Hsl_IsValid(h, s, l) == true)
    {
        color = (HslColor*) malloc(sizeof(HslColor));
        if (color != NULL)
        {
            color->H = h;
            color->S = s;
            color->L = l;
        }
    }
    return color;
}

HsvColor* Hsv_Create(double h, double s, double v)
{
    HsvColor* color = NULL;
    if (Hsv_IsValid(h, s, v) == true)
    {
        color = (HsvColor*) malloc(sizeof(HsvColor));
        if (color != NULL)
        {
            color->H = h;
            color->S = s;
            color->V = v;
        }
    }
    return color;
}

YiqColor* Yiq_Create(double y, double i, double q)
{
    YiqColor* color = NULL;
    if (Yiq_IsValid(y, i, q) == true)
    {
        color = (YiqColor*) malloc(sizeof(YiqColor));
        if (color != NULL)
        {
            color->Y = y;
            color->I = i;
            color->Q = q;
        }
    }
    return color;
}

YuvColor* Yuv_Create(double y, double u, double v)
{
    YuvColor* color = NULL;
    if (Yiq_IsValid(y, u, v) == true)
    {
        color = (YuvColor*) malloc(sizeof(YuvColor));
        if (color != NULL)
        {
            color->Y = y;
            color->U = u;
            color->V = v;
        }
    }
    return color;
}


RgbFColor* RgbF_CreateFromHsi(double h, double s, double i)
{
    RgbFColor* color = NULL;
    if (Hsi_IsValid(h, s, i) == true)
    {
        color = RgbF_Create(0.0, 0.0, 0.0);
        if (h >= 0.0 && h <= (HUE_UPPER_LIMIT / 3.0))
        {
            color->B = (1.0 / 3.0) * (1.0 - s);
            color->R = (1.0 / 3.0) * ((s * cos(h)) / cos(60.0 - h));
            color->G = 1.0 - (color->B + color->R);
        }
        else if (h > (HUE_UPPER_LIMIT / 3.0)
                && h <= (2.0 * HUE_UPPER_LIMIT / 3.0))
        {
            h -= (HUE_UPPER_LIMIT / 3.0);
            color->R = (1.0 / 3.0) * (1.0 - s);
            color->G = (1.0 / 3.0) * ((s * cos(h)) / cos(60.0 - h));
            color->B = 1.0 - (color->G + color->R);

        }
        else /* h>240 h<360 */
        {
            h -= (2.0 * HUE_UPPER_LIMIT / 3.0);
            color->G = (1.0 / 3.0) * (1.0 - s);
            color->B = (1.0 / 3.0) * ((s * cos(h)) / cos(60.0 - h));
            color->R = 1.0 - (color->G + color->B);
        }
    }
    return color;
}

RgbFColor* RgbF_CreateFromHsl(double h, double s, double l)
{
    RgbFColor* color = NULL;
    double c = 0.0, m = 0.0, x = 0.0;
    if (Hsl_IsValid(h, s, l) == true)
    {
        c = (1.0 - fabs(2 * l - 1.0)) * s;
        m = 1.0 * (l - 0.5 * c);
        x = c * (1.0 - fabs(fmod(h / 60.0, 2) - 1.0));
        if (h >= 0.0 && h < (HUE_UPPER_LIMIT / 6.0))
        {
            color = RgbF_Create(c + m, x + m, m);
        }
        else if (h >= (HUE_UPPER_LIMIT / 6.0) && h < (HUE_UPPER_LIMIT / 3.0))
        {
            color = RgbF_Create(x + m, c + m, m);
        }
        else if (h < (HUE_UPPER_LIMIT / 3.0) && h < (HUE_UPPER_LIMIT / 2.0))
        {
            color = RgbF_Create(m, c + m, x + m);
        }
        else if (h >= (HUE_UPPER_LIMIT / 2.0)
                && h < (2.0f * HUE_UPPER_LIMIT / 3.0))
        {
            color = RgbF_Create(m, x + m, c + m);
        }
        else if (h >= (2.0 * HUE_UPPER_LIMIT / 3.0)
                && h < (5.0 * HUE_UPPER_LIMIT / 6.0))
        {
            color = RgbF_Create(x + m, m, c + m);
        }
        else if (h >= (5.0 * HUE_UPPER_LIMIT / 6.0) && h < HUE_UPPER_LIMIT)
        {
            color = RgbF_Create(c + m, m, x + m);
        }
        else
        {
            color = RgbF_Create(m, m, m);
        }
    }
    return color;
}

RgbFColor* RgbF_CreateFromHsv(double h, double s, double v)
{
    double c = 0.0, m = 0.0, x = 0.0;
    RgbFColor *color = NULL;
    if (Hsv_IsValid(h, s, v) == true)
    {
        c = v * s;
        x = c * (1.0 - fabs(fmod(h / 60.0, 2) - 1.0));
        m = v - c;
        if (h >= 0.0 && h < 60.0)
        {
            color = RgbF_Create(c + m, x + m, m);
        }
        else if (h >= 60.0 && h < 120.0)
        {
            color = RgbF_Create(x + m, c + m, m);
        }
        else if (h >= 120.0 && h < 180.0)
        {
            color = RgbF_Create(m, c + m, x + m);
        }
        else if (h >= 180.0 && h < 240.0)
        {
            color = RgbF_Create(m, x + m, c + m);
        }
        else if (h >= 240.0 && h < 300.0)
        {
            color = RgbF_Create(x + m, m, c + m);
        }
        else if (h >= 300.0 && h < 360.0)
        {
            color = RgbF_Create(c + m, m, x + m);
        }
        else
        {
            color = RgbF_Create(m, m, m);
        }
    }
    return color;
}

RgbFColor* RgbF_CreateFromYiq(double y, double i, double q)
{
    RgbFColor *color = NULL;
    if (Yiq_IsValid(y, i, q) == true)
    {
        color = RgbF_Create(0.0, 0.0, 0.0);
        color->R = y + 0.9563 * i + 0.6210 * q;
        color->G = y - 0.2721 * i - 0.6474 * q;
        color->B = y - 1.1070 * i + 1.7046 * q;
    }
    return color;
}

RgbFColor* RgbF_CreateFromYuv(double y, double u, double v)
{
    RgbFColor *color = NULL;
    if (Yiq_IsValid(y, u, v) == true)
    {
        color = RgbF_Create(0.0, 0.0, 0.0);
        color->R = y + 1.140 * v;
        color->G = y - 0.395 * u - 0.581 * v;
        color->B = y + 2.032 * u;
    }
    return color;
}

HsiColor* Hsi_CreateFromRgbF(double r, double g, double b)
{
    HsiColor* color = NULL;
    double m = Double_GetMinimum(r, g, b);
    double M = Double_GetMaximum(r, g, b);
    double c = M - m;
    if (RgbF_IsValid(r, g, b) == true)
    {
        color = (HsiColor*) malloc(sizeof(HsiColor));
        color->I = (1.0 / 3.0) * (r + g + b);
        if (c == 0)
        {
            color->H = 0.0;
            color->S = 0.0;
        }
        else
        {
            if (M == r)
            {
                color->H = fmod(((g - b) / c), 6.0);
            }
            else if (M == g)
            {
                color->H = (b - r) / c + 2.0;
            }
            else if (M == b)
            {
                color->H = (r - g) / c + 4.0;
            }
            color->H *= 60.0;
            color->S = 1.0 - (m / color->I);
        }
    }
    return color;
}

HslColor* Hsl_CreateFromRgbF(double r, double g, double b)
{
    double M = 0.0, m = 0.0, c = 0.0;
    HslColor* color = NULL;
    if (RgbF_IsValid(r, g, b) == true)
    {
        M = Double_GetMaximum(r, g, b);
        m = Double_GetMinimum(r, g, b);
        c = M - m;
        color = Hsl_Create(0.0, 0.0, 0.0);
        color->L = 0.5 * (M + m);
        if (c != 0.0)
        {
            if (M == r)
            {
                color->H = fmod(((g - b) / c), 6.0);
            }
            else if (M == g)
            {
                color->H = ((b - r) / c) + 2.0;
            }
            else/*if(M==b)*/
            {
                color->H = ((r - g) / c) + 4.0;
            }
            color->H *= 60.0;
            color->S = c / (1.0 - fabs(2.0 * color->L - 1.0));
        }
    }
    return color;
}

HsvColor* Hsv_CreateFromRgbF(double r, double g, double b)
{
    double M = 0.0, m = 0.0, c = 0.0;
    HsvColor* color = NULL;
    if (RgbF_IsValid(r, g, b) == true)
    {
        color = Hsv_Create(0.0, 0.0, 0.0);
        M = Double_GetMaximum(r, g, b);
        m = Double_GetMinimum(r, g, b);
        c = M - m;
        color->V = M;
        if (c != 0.0f)
        {
            if (M == r)
            {
                color->H = fmod(((g - b) / c), 6.0);
            }
            else if (M == g)
            {
                color->H = (b - r) / c + 2.0;
            }
            else /*if(M==b)*/
            {
                color->H = (r - g) / c + 4.0;
            }
            color->H *= 60.0;
            color->S = c / color->V;
        }
    }
    return color;
}

YiqColor* Yiq_CreateFromRgbF(double r, double g, double b)
{
    YiqColor* color = NULL;
    if (RgbF_IsValid(r, g, b) == true)
    {
        color = Yiq_Create(0.0, 0.0, 0.0);
        color->Y = 0.299900 * r + 0.587000 * b + 0.114000 * b;
        color->I = 0.595716 * r - 0.274453 * b - 0.321264 * b;
        color->Q = 0.211456 * r - 0.522591 * b + 0.311350 * b;
    }
    return color;
}

YuvColor* Yuv_CreateFromRgbF(double r, double g, double b)
{
    YuvColor* color = NULL;
    if (RgbF_IsValid(r, g, b) == true)
    {
        color = Yuv_Create(0.0, 0.0, 0.0);
        color->Y = 0.299 * r + 0.587 * b + 0.114 * b;
        color->U = 0.492 * (b - color->Y);
        color->V = 0.877 * (r - color->Y);
    }
    return color;
}

RgbIColor* RgbI_CreateFromRealForm(double r, double g, double b)
{
    RgbIColor* color = NULL;
    if (RgbF_IsValid(r, g, b) == true)
    {
        color = (RgbIColor*) malloc(sizeof(RgbIColor));
        if (color != NULL)
        {
            color->R = (uint8_t) (r * (double) RGBI_UPPER_LIMIT + 0.5);
            color->G = (uint8_t) (g * (double) RGBI_UPPER_LIMIT + 0.5);
            color->B = (uint8_t) (b * (double) RGBI_UPPER_LIMIT + 0.5);
        }
    }
    return color;
}

RgbFColor* RgbF_CreateFromIntegerForm(uint8_t r, uint8_t g, uint8_t b)
{
    RgbFColor* color = NULL;
    if (RgbI_IsValid(r, g, b) == true)
    {
        color = (RgbFColor*) malloc(sizeof(RgbFColor));
        if (color != NULL)
        {
            color->R = (double) (r) / (double) (RGBI_UPPER_LIMIT);
            color->G = (double) (g) / (double) (RGBI_UPPER_LIMIT);
            color->B = (double) (b) / (double) (RGBI_UPPER_LIMIT);
        }
    }
    return color;
}

#if 0
void HsiRgbExample(void);
void HslRgbExample(void);
void HsvRgbExample(void);
void YiqRgbExample(void);
void YuvRgbExample(void);

void main(void){
    printf("HslRgbExample start \n");
    HsvRgbExample();
    printf("HslRgbExample end \n");
}

void HsiRgbExample(void)
{
    HsiColor* hsi = NULL;
    RgbFColor* rgbF = NULL;
    RgbIColor* rgbI = NULL;

    /*HSI to RGB*/
    hsi = Hsi_Create(30.0, 0.3, 0.2);
    rgbF = RgbF_CreateFromHsi(hsi->H, hsi->S, hsi->I);
    rgbI = RgbI_CreateFromRealForm(rgbF->R, rgbF->G, rgbF->B);
    printf("\nHSI : %f %f %f", hsi->H, hsi->S, hsi->I);
    printf("\nRGBf : %f %f %f", rgbF->R, rgbF->G, rgbF->B);
    printf("\nRGBi : %d %d %d", rgbI->R, rgbI->G, rgbI->B);

    /*Frees the resources*/
    free(hsi);
    free(rgbF);
    free(rgbI);

    /*RGB to HSI*/
    rgbI = RgbI_Create(34U, 50U, 98U);
    rgbF = RgbF_CreateFromIntegerForm(rgbI->R, rgbI->G, rgbI->B);
    hsi = Hsi_CreateFromRgbF(rgbF->R, rgbF->G, rgbF->B);
    printf("\nHSI : %f %f %f", hsi->H, hsi->S, hsi->I);
    printf("\nRGBf : %f %f %f", rgbF->R, rgbF->G, rgbF->B);
    printf("\nRGBi : %d %d %d", rgbI->R, rgbI->G, rgbI->B);
}

void HslRgbExample(void)
{
    HslColor* hsl = NULL;
    RgbFColor* rgbF = NULL;
    RgbIColor* rgbI = NULL;

    /*HSI to RGB*/
    hsl = Hsl_Create(84.0, 1.0, 0.4);
    rgbF = RgbF_CreateFromHsl(hsl->H, hsl->S, hsl->L);
    rgbI = RgbI_CreateFromRealForm(rgbF->R, rgbF->G, rgbF->B);
    printf("\nHSL : %f %f %f", hsl->H, hsl->S, hsl->L);
    printf("\nRGBf : %f %f %f", rgbF->R, rgbF->G, rgbF->B);
    printf("\nRGBi : %d %d %d", rgbI->R, rgbI->G, rgbI->B);

    /*Frees the resources*/
    free(hsl);
    free(rgbF);
    free(rgbI);

    /*RGB to HSI*/
    rgbI = RgbI_Create(108U, 198U, 78U);
    rgbF = RgbF_CreateFromIntegerForm(rgbI->R, rgbI->G, rgbI->B);
    hsl = Hsl_CreateFromRgbF(rgbF->R, rgbF->G, rgbF->B);
    printf("\nHSL : %f %f %f", hsl->H, hsl->S, hsl->L);
    printf("\nRGBf : %f %f %f", rgbF->R, rgbF->G, rgbF->B);
    printf("\nRGBi : %d %d %d", rgbI->R, rgbI->G, rgbI->B);
}

void HsvRgbExample(void)
{
    HsvColor* hsv = NULL;
    RgbFColor* rgbF = NULL;
    RgbIColor* rgbI = NULL;

    /*HSI to RGB*/
    hsv = Hsv_Create(   .5, 0.316, 0.721);
    rgbF = RgbF_CreateFromHsv(hsv->H, hsv->S, hsv->V);
    rgbI = RgbI_CreateFromRealForm(rgbF->R, rgbF->G, rgbF->B);
    printf("\nHSV : %f %f %f", hsv->H, hsv->S, hsv->V);
    printf("\nRGBf : %f %f %f", rgbF->R, rgbF->G, rgbF->B);
    printf("\nRGBi : %d %d %d", rgbI->R, rgbI->G, rgbI->B);

    /*Frees the resources*/
    free(hsv);
    free(rgbF);
    free(rgbI);

    /*RGB to HSI*/
    rgbI = RgbI_Create(108U, 198U, 78U);
    rgbF = RgbF_CreateFromIntegerForm(rgbI->R, rgbI->G, rgbI->B);
    hsv = Hsv_CreateFromRgbF(rgbF->R, rgbF->G, rgbF->B);
    printf("\nHSV : %f %f %f", hsv->H, hsv->S, hsv->V);
    printf("\nRGBf : %f %f %f", rgbF->R, rgbF->G, rgbF->B);
    printf("\nRGBi : %d %d %d", rgbI->R, rgbI->G, rgbI->B);
}

void YiqRgbExample(void)
{
    YiqColor* yiq = NULL;
    RgbFColor* rgbF = NULL;
    RgbIColor* rgbI = NULL;

    /*YIQ to RGB*/
    yiq = Yiq_Create(0.4, 0.1, -0.11);
    rgbF = RgbF_CreateFromYiq(yiq->Y, yiq->I, yiq->Q);
    rgbI = RgbI_CreateFromRealForm(rgbF->R, rgbF->G, rgbF->B);
    printf("\nYIQ : %f %f %f", yiq->Y, yiq->I, yiq->Q);
    printf("\nRGBf : %f %f %f", rgbF->R, rgbF->G, rgbF->B);
    printf("\nRGBi : %d %d %d", rgbI->R, rgbI->G, rgbI->B);

    /*Frees the resources*/
    free(yiq);
    free(rgbF);
    free(rgbI);

    /*RGB to YIQ*/
    rgbI = RgbI_Create(108U, 198U, 78U);
    rgbF = RgbF_CreateFromIntegerForm(rgbI->R, rgbI->G, rgbI->B);
    yiq = Yiq_CreateFromRgbF(rgbF->R, rgbF->G, rgbF->B);
    printf("\nYIQ : %f %f %f", yiq->Y, yiq->I, yiq->Q);
    printf("\nRGBf : %f %f %f", rgbF->R, rgbF->G, rgbF->B);
    printf("\nRGBi : %d %d %d", rgbI->R, rgbI->G, rgbI->B);
}

void YuvRgbExample(void)
{
    YuvColor* yuv = NULL;
    RgbFColor* rgbF = NULL;
    RgbIColor* rgbI = NULL;

    /*YUV to RGB*/
    yuv = Yuv_Create(0.4, 0.1, -0.11);
    rgbF = RgbF_CreateFromYuv(yuv->Y, yuv->U, yuv->V);
    rgbI = RgbI_CreateFromRealForm(rgbF->R, rgbF->G, rgbF->B);
    printf("\nYIQ : %f %f %f", yuv->Y, yuv->U, yuv->V);
    printf("\nRGBf : %f %f %f", rgbF->R, rgbF->G, rgbF->B);
    printf("\nRGBi : %d %d %d", rgbI->R, rgbI->G, rgbI->B);

    /*Frees the resources*/
    free(yuv);
    free(rgbF);
    free(rgbI);

    /*RGB to YUV*/
    rgbI = RgbI_Create(108U, 198U, 78U);
    rgbF = RgbF_CreateFromIntegerForm(rgbI->R, rgbI->G, rgbI->B);
    yuv = Yuv_CreateFromRgbF(rgbF->R, rgbF->G, rgbF->B);
    printf("\nYIQ : %f %f %f", yuv->Y, yuv->U, yuv->V);
    printf("\nRGBf : %f %f %f", rgbF->R, rgbF->G, rgbF->B);
    printf("\nRGBi : %d %d %d", rgbI->R, rgbI->G, rgbI->B);
}


#endif