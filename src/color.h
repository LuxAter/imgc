#ifndef IMGC_COLOR_H_
#define IMGC_COLOR_H_

#include <math.h>
#include <stdbool.h>
#include <stdint.h>
#include <stdlib.h>
#include <string.h>

float angle_diff_clockwise(float start, float end) {
  if (start <= end) {
    return end - start;
  } else {
    return 360.0 + end - start;
  }
}

float angle_diff_counter_clockwise(float start, float end) {
  if (end <= start) {
    return end - start;
  } else {
    return -360.0 - start + end;
  }
}

float angle_diff_min(float start, float end) {
  float cw = angle_diff_clockwise(start, end);
  float ccw = angle_diff_counter_clockwise(start, end);
  if (fabs(cw) > fabs(ccw)) {
    return ccw;
  } else {
    return cw;
  }
}
float angle_diff_max(float start, float end) {
  float cw = angle_diff_clockwise(start, end);
  float ccw = angle_diff_counter_clockwise(start, end);
  if (fabs(cw) > fabs(ccw)) {
    return cw;
  } else {
    return ccw;
  }
}

uint32_t rgb(float r, float g, float b) {
  return ((uint32_t)(255 * r) << 16) + ((uint32_t)(255 * g) << 8) +
         (uint32_t)(255 * b);
}
uint32_t rgbi(uint8_t r, uint8_t g, uint8_t b) {
  return ((uint32_t)(r) << 16) + ((uint32_t)(g) << 8) + (uint32_t)(b);
}
uint32_t rgbs(const char* str) {
  bool pound = (str[0] == '#');
  char rs[3], gs[3], bs[3];
  strncpy(rs, str + pound, 2);
  strncpy(gs, str + 2 + pound, 2);
  strncpy(bs, str + 4 + pound, 2);
  return rgbi((uint8_t)strtol(rs, NULL, 16), (uint8_t)strtol(gs, NULL, 16),
              (uint8_t)strtol(bs, NULL, 16));
}

uint32_t cmyk(float c, float m, float y, float k) {
  return rgb((1.0 - c) * (1.0 - k), (1.0 - m) * (1.0 - k),
             (1.0 - y) * (1.0 - k));
}
uint32_t cmyki(uint8_t c, uint8_t m, uint8_t y, uint8_t k) {
  return rgbi((255 - c) * (255 - k), (255 - m) * (255 - k),
              (255 - y) * (255 - k));
}

uint32_t hsv(float h, float s, float v) {
  h = fmod(h, 360.0);
  if (h < 0) h += 360.0;
  float c = v * s;
  float x = c * (1.0 - fabs(fmod(h / 60.0, 2.0) - 1.0));
  float m = v - c;
  if (h < 60.0) {
    return rgb(c + m, x + m, +m);
  } else if (h < 120.0) {
    return rgb(x + m, c + m, +m);
  } else if (h < 180.0) {
    return rgb(+m, c + m, x + m);
  } else if (h < 240.0) {
    return rgb(+m, x + m, c + m);
  } else if (h < 300.0) {
    return rgb(x + m, +m, c + m);
  } else {
    return rgb(c + m, +m, x + m);
  }
}
uint32_t hsvi(uint16_t h, uint8_t s, uint8_t v) {
  return hsv((float)h, (float)s / 255.0, (float)v / 255.0);
}

uint32_t hsl(float h, float s, float l) {
  h = fmod(h, 360.0);
  if (h < 0) h += 360.0;
  float c = (1.0 - fabs(2 * l - 1.0)) * s;
  float x = c * (1.0 - fabs(fmod(h / 60.0, 2.0) - 1.0));
  float m = l - c / 2.0;
  if (h < 60.0) {
    return rgb(c + m, x + m, +m);
  } else if (h < 120.0) {
    return rgb(x + m, c + m, +m);
  } else if (h < 180.0) {
    return rgb(+m, c + m, x + m);
  } else if (h < 240.0) {
    return rgb(+m, x + m, c + m);
  } else if (h < 300.0) {
    return rgb(x + m, +m, c + m);
  } else {
    return rgb(c + m, +m, x + m);
  }
}
uint32_t hsli(uint16_t h, uint8_t s, uint8_t l) {
  return hsl((float)h, (float)s / 255.0, (float)l / 255.0);
}

void get_rgb(uint32_t color, float* r, float* g, float* b) {
  *r = (float)((color >> 16) & 0xFF) / 255.0;
  *g = (float)((color >> 8) & 0xFF) / 255.0;
  *b = (float)(color & 0xFF) / 255.0;
}

void get_rgbi(uint32_t color, uint8_t* r, uint8_t* g, uint8_t* b) {
  *r = (color >> 16) & 0xFF;
  *g = (color >> 8) & 0xFF;
  *b = color & 0xFF;
}

void get_cmyk(uint32_t color, float* c, float* m, float* y, float* k) {
  float r, g, b;
  get_rgb(color, &r, &g, &b);
  *k = 1.0 - fmax(r, fmax(g, b));
  *c = (1.0 - r - *k) / (1.0 - *k);
  *m = (1.0 - g - *k) / (1.0 - *k);
  *y = (1.0 - b - *k) / (1.0 - *k);
}

void get_cmyki(uint32_t color, uint8_t* c, uint8_t* m, uint8_t* y, uint8_t* k) {
  float r, g, b;
  get_rgb(color, &r, &g, &b);
  float tk = ((1.0 - fmax(r, fmax(g, b))) * 255);
  *k = (uint8_t)(tk * 255);
  *c = (uint8_t)(((1.0 - r - tk) / (1.0 - tk)) * 255);
  *m = (uint8_t)(((1.0 - g - tk) / (1.0 - tk)) * 255);
  *y = (uint8_t)(((1.0 - b - tk) / (1.0 - tk)) * 255);
}

void get_hsv(uint32_t color, float* h, float* s, float* v) {
  float r, g, b;
  get_rgb(color, &r, &g, &b);
  float c_max = fmax(r, fmax(g, b));
  float c_min = fmin(r, fmin(g, b));
  float delta = c_max - c_min;
  if (delta == 0) {
    *h = 0;
  } else if (c_max == r) {
    *h = 60 * fmod((g - b) / delta, 6.0);
  } else if (c_max == g) {
    *h = 60 * ((b - r) / delta + 2);
  } else if (c_max == b) {
    *h = 60 * ((r - g) / delta + 4);
  }
  if (*h < 0) {
    *h += 360.0;
  }
  if (c_max == 0) {
    *s = 0;
  } else {
    *s = delta / c_max;
  }
  *v = c_max;
}

void get_hsvi(uint32_t color, uint16_t* h, uint8_t* s, uint8_t* v) {
  float th, ts, tv;
  get_hsv(color, &th, &ts, &tv);
  *h = (uint16_t)(th);
  *s = (uint8_t)(ts * 255.0);
  *v = (uint8_t)(tv * 255.0);
}

void get_hsl(uint32_t color, float* h, float* s, float* l) {
  float r, g, b;
  get_rgb(color, &r, &g, &b);
  float c_max = fmax(r, fmax(g, b));
  float c_min = fmin(r, fmin(g, b));
  float delta = c_max - c_min;
  if (delta == 0) {
    *h = 0;
  } else if (c_max == r) {
    *h = 60 * fmod((g - b) / delta, 6.0);
  } else if (c_max == g) {
    *h = 60 * ((b - r) / delta + 2);
  } else if (c_max == b) {
    *h = 60 * ((r - g) / delta + 4);
  }
  if (*h < 0) {
    *h += 360.0;
  }
  *l = (c_max + c_min) / 2.0;
  if (c_max == 0) {
    *s = 0;
  } else {
    *s = delta / (1.0 - fabs(2.0 * *l - 1.0));
  }
}

void get_hsli(uint32_t color, uint16_t* h, uint8_t* s, uint8_t* l) {
  float th, ts, tl;
  get_hsl(color, &th, &ts, &tl);
  *h = (uint16_t)(th);
  *s = (uint8_t)(ts * 255.0);
  *l = (uint8_t)(tl * 255.0);
}

uint32_t gradient_rgb(uint32_t start, uint32_t end, float t) {
  float sr, sg, sb, er, eg, eb;
  get_rgb(start, &sr, &sg, &sb);
  get_rgb(end, &er, &eg, &eb);
  return rgb((er - sr) * t + sr, (eg - sg) * t + sg, (eb - sb) * t + sb);
}

uint32_t gradient_cmyk(uint32_t start, uint32_t end, float t) {
  float sc, sm, sy, sk, ec, em, ey, ek;
  get_cmyk(start, &sc, &sm, &sy, &sk);
  get_cmyk(end, &ec, &em, &ey, &ek);
  return cmyk((ec - sc) * t + sc, (em - sm) * t + sm, (ey - sy) * t + sy,
              (ek - sk) * t + sk);
}

uint32_t gradient_hsv(uint32_t start, uint32_t end, float t) {
  float sh, ss, sv, eh, es, ev;
  get_hsv(start, &sh, &ss, &sv);
  get_hsv(end, &eh, &es, &ev);
  float dh = angle_diff_min(sh, eh);
  return hsv(dh * t + sh, (es - ss) * t + ss, (ev - sv) * t + sv);
}
uint32_t gradient_hsv_max(uint32_t start, uint32_t end, float t) {
  float sh, ss, sv, eh, es, ev;
  get_hsv(start, &sh, &ss, &sv);
  get_hsv(end, &eh, &es, &ev);
  float dh = angle_diff_max(sh, eh);
  return hsv(dh * t + sh, (es - ss) * t + ss, (ev - sv) * t + sv);
}

uint32_t gradient_hsl(uint32_t start, uint32_t end, float t) {
  float sh, ss, sv, eh, es, ev;
  get_hsl(start, &sh, &ss, &sv);
  get_hsl(end, &eh, &es, &ev);
  float dh = angle_diff_min(sh, eh);
  return hsl(dh * t + sh, (es - ss) * t + ss, (ev - sv) * t + sv);
}
uint32_t gradient_hsl_max(uint32_t start, uint32_t end, float t) {
  float sh, ss, sv, eh, es, ev;
  get_hsl(start, &sh, &ss, &sv);
  get_hsl(end, &eh, &es, &ev);
  float dh = angle_diff_max(sh, eh);
  return hsl(dh * t + sh, (es - ss) * t + ss, (ev - sv) * t + sv);
}

#endif  // IMGC_COLOR_H_
