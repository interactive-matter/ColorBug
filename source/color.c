#include "color.h"

/* TODO This function is only used by the fade to random HSV. It might be possible to eliminate */
void rgb_to_hsv(uint8_t red, uint8_t green, uint8_t blue, uint8_t* hue, uint8_t* sat, uint8_t* val) {
    uint8_t rgb_min, rgb_max;
    rgb_min = MIN3(red, green, blue);
    rgb_max = MAX3(red, green, blue);

    *val = rgb_max;
    if (*val == 0) {
        *hue = *sat = 0;
        return;
    }

    *sat = 255*(rgb_max - rgb_min)/ *val;
    if (*sat == 0) {
        *hue = 0;
        return;
    }

    /* Compute hue */
    if (rgb_max == red) {
        *hue = 0 + 43*(green - blue)/(rgb_max - rgb_min);
    } else if (rgb_max == green) {
        *hue = 85 + 43*(blue - red)/(rgb_max - rgb_min);
    } else /* rgb_max == rgb.b */ {
        *hue = 171 + 43*(red - green)/(rgb_max - rgb_min);
    }
}

/**
 * Modified algorithm at http://www.tecgraf.puc-rio.br/~mgattass/color/HSVtoRGB.htm to work with uint8_t
 * Some precision is lost as intermediate values are scaled down by 16 to avoid 8 bit overflow.
 * HSV to RGB conversion theory here: http://en.wikipedia.org/wiki/HSV_color_space#Conversion_from_HSV_to_RGB
 */
void hsv_to_rgb(uint8_t hue, uint8_t sat, uint8_t val, uint8_t* red, uint8_t* green, uint8_t* blue)
{
        if ( sat == 0 )
        {
                *blue = *green = *red = val;
        }
        else
        {

                uint8_t var_i = hue/43;                                 // Hue quadrant (sixths) - 43 is (256/6)
                uint8_t var_h = (var_i*43)/16;                        // Caculate quadrant floor/16:
                                                                      // H Input of 0..255 should produce 0, 2, 5, 8, 10, 13
                sat /= 16;                                              // Pre-scale S
                uint8_t var_v = val/16;                                 // Pre-scale V
                                                                      // These interpolations using uint8 range 0..255 are equivalent
                                                                      // to using the below algorithm with floating point range 0..1
                uint8_t var_1 = var_v * ( 16 - sat );                   // V*(1-S)
                uint8_t var_2 = var_v * ( 16 - (sat * var_h)/16 );      // V*(1-(S*H_Floor))
                uint8_t var_3 = var_v * ( 16 - (sat * (16-var_h))/16 ); // V*(1-(S*(1-H_Floor)))

                if      ( var_i == 0 ) { *red = val     ; *green = var_3 ; *blue = var_1; } // 0   deg (r)   to 60  deg (r+g)
                else if ( var_i == 1 ) { *red = var_2 ; *green = val     ; *blue = var_1; } // 60  deg (r+g) to 120 deg (g)
                else if ( var_i == 2 ) { *red = var_1 ; *green = val     ; *blue = var_3; } // 120 deg (g)   to 180 deg (g+b)
                else if ( var_i == 3 ) { *red = var_1 ; *green = var_2 ; *blue = val;     } // 180 deg (g+b) to 240 deg (b)
                else if ( var_i == 4 ) { *red = var_3 ; *green = var_1 ; *blue = val;     } // 240 deg (b)   to 300 deg (b+r)
                else                   { *red = val     ; *green = var_1 ; *blue = var_2; } // 300 deg (b+r) to 0   deg (r)
        }
}
