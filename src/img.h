#ifndef IMGC_IMG_H_
#define IMGC_IMG_H_

#include <stdint.h>
#include <stdlib.h>
#include <string.h>

typedef struct imgc_img {
  uint32_t* data;
  uint32_t width, height;
} img_t;

img_t gen_img(uint32_t width, uint32_t height) {
  img_t img;
  img.width = width;
  img.height = height;
  img.data = (uint32_t*)malloc(sizeof(uint32_t) * img.width * img.height);
  memset(img.data, 0x00, sizeof(uint32_t) * img.width * img.height);
  return img;
}

void destroy_img(img_t image) { free(image.data); }

void fill(img_t img, uint32_t c) {
  for(uint32_t i = 0; i < img.width * img.height; ++i){
    img.data[i] = c;
  }
}

void set_pixel(img_t img, uint32_t x, uint32_t y, uint32_t c) {
  img.data[(y * img.width) + x] = c;
}


#endif  // IMGC_IMG_H_
