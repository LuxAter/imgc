#ifndef IMGC_GIF_H_
#define IMGC_GIF_H_

#include <stdint.h>
#include <stdlib.h>
#include <string.h>

#include "img.h"

typedef struct imgc_gif {
  uint32_t** frames;
  uint32_t pallet[255];
  uint32_t count, width, height;
} gif_t;

gif_t gen_gif(uint32_t width, uint32_t height, uint32_t count){
  gif_t gif;
  gif.width = width;
  gif.height = height;
  gif.count = count;
  gif.frames = (uint32_t**)malloc(sizeof(uint32_t*) * count);
  for(uint32_t i = 0; i < count; ++i){
    gif.frames[i] = (uint32_t*)malloc(sizeof(uint32_t) * width * height);
    memset(gif.frames[i], 0x00, sizeof(uint32_t) * width * height);
  }
  return gif;
}

void destroy_gif(gif_t gif){
  for(uint32_t i = 0; i < gif.count; ++i){
    free(gif.frames[i]);
  }
  free(gif.frames);
}

img_t get_frame(gif_t gif, uint32_t frame){
  img_t img;
  img.width = gif.width;
  img.height = gif.height;
  img.data = gif.frames[frame];
  return img;
}

bool write_gif(const char* file_name, gif_t gif, uint32_t delay){

}

#endif  // IMGC_GIF_H_
