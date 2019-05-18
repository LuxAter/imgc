#include "img.h"
#include "bmp.h"
#include "color.h"

int main(int argc, char *argv[])
{
  img_t img = gen_img(500, 500);
  uint32_t cs = 0xff0000;
  uint32_t ce = 0x00ffff;
  for(unsigned i = 0; i < 500; ++i){
    float perc = (float)i / 500.0;
    for(unsigned x = 0; x < 10; ++x){
      set_pixel(img, i, x, gradient_rgb(cs, ce, perc));
      set_pixel(img, i, x+10, gradient_cmyk(cs, ce, perc));
      set_pixel(img, i, x+20, gradient_hsv(cs, ce, perc));
      set_pixel(img, i, x+30, gradient_hsv_max(cs, ce, perc));
      set_pixel(img, i, x+40, gradient_hsl(cs, ce, perc));
      set_pixel(img, i, x+50, gradient_hsl_max(cs, ce, perc));
    }
  }
  write_bmp("test.bmp", img);
  destroy_img(img);
  return 0;
}
