#ifndef IMGC_BMP_H_
#define IMGC_BMP_H_

#include <stdbool.h>
#include <stdint.h>
#include <stdio.h>
#include <stdlib.h>

#include "img.h"

bool write_bmp(const char* file_name, img_t img) {
  uint32_t fs = 54 + 3 * img.width * img.height;
  uint8_t* byte_data = (uint8_t*)malloc(3 * img.width * img.height);
  if (byte_data == NULL) {
    return false;
  }
  memset(byte_data, 0, 3 * img.width * img.height);
  for (uint32_t i = 0; i < img.width * img.height; ++i) {
    uint8_t r = ((img.data[i] >> 16) & 0xFF);
    uint8_t g = ((img.data[i] >> 8) & 0xFF);
    uint8_t b = ((img.data[i]) & 0xFF);
    byte_data[i * 3 + 0] = b;
    byte_data[i * 3 + 1] = g;
    byte_data[i * 3 + 2] = r;
  }
  uint8_t file_header[14] = {'B', 'M', 0, 0, 0, 0, 0, 0, 0, 0, 54, 0, 0, 0};
  uint8_t info_header[40] = {40, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 1, 0, 24, 0};
  uint8_t pad[3] = {0, 0, 0};
  file_header[2] = (uint8_t)(fs);
  file_header[3] = (uint8_t)(fs >> 8);
  file_header[4] = (uint8_t)(fs >> 16);
  file_header[5] = (uint8_t)(fs >> 24);

  info_header[4] = (uint8_t)(img.width);
  info_header[5] = (uint8_t)(img.width >> 8);
  info_header[6] = (uint8_t)(img.width >> 16);
  info_header[7] = (uint8_t)(img.width >> 24);
  info_header[8] = (uint8_t)(img.height);
  info_header[9] = (uint8_t)(img.height >> 8);
  info_header[10] = (uint8_t)(img.height >> 16);
  info_header[11] = (uint8_t)(img.height >> 24);

  FILE* out = fopen(file_name, "wb");
  if (out == NULL) {
    free(byte_data);
    return false;
  }

  fwrite(file_header, 1, 14, out);
  fwrite(info_header, 1, 40, out);
  for (uint32_t i = 0; i < img.height; ++i) {
    fwrite(byte_data + (img.width * (img.height - i - 1) * 3), 3, img.width,
           out);
    fwrite(pad, 1, (4 - (img.width * 3) % 4) % 4, out);
  }

  fclose(out);
  free(byte_data);
  return true;
}

#endif  // IMGC_BMP_H_
