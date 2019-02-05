#ifndef _RECT16_H_
#define _RECT16_H_

struct Rect16 {
  int16_t x, y, w, h;
  Rect16 (int16_t xpos = 0, int16_t ypos = 0, int16_t width = 0, int16_t height = 0): x(xpos), y(ypos), w(width), h(height) {}
  Rect16 (const Rect16& src) : x(src.x), y(src.y), w(src.w), h(src.h) {}
  bool equal(const Rect16& rhs) const { return x == rhs.x && y == rhs.y && w == rhs.w && h == rhs.h; }
  int16_t bottom() const { return y+h; }
  int16_t right() const { return x+w; }
  Rect16 mixRect(const Rect16& dst) const {
    return Rect16( ((x + (x < dst.x) - (x > dst.x))*2 + dst.x) / 3
               , ((y + (y < dst.y) - (y > dst.y))*2 + dst.y) / 3
               , ((w + (w < dst.w) - (w > dst.w))*2 + dst.w) / 3
               , ((h + (h < dst.h) - (h > dst.h))*2 + dst.h) / 3
               );
  }
  void inflate(int inc) { x-=inc; y-=inc; w+=inc*2; h+=inc*2; };

  bool intersectsWith(const Rect16& r) const {
     return (r.x < x + w) && (x < r.x + r.w) && (r.y < y + h) && (y < r.y + r.h);
  }
  Rect16 intersect(const Rect16& rhs) const {
    int t = rhs.y > y ? rhs.y : y;
    int l = rhs.x > x ? rhs.x : x;
    int r = rhs.right()  < right()  ? rhs.right()  : right();
    int b = rhs.bottom() < bottom() ? rhs.bottom() : bottom();
    return Rect16(l, t, r - l, b - t);
  }
};

#endif
