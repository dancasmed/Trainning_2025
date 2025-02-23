#ifndef RGB_COLOR
#define RGB_COLOR

struct RGBColor {
    int red;
    int green;
    int blue;

    RGBColor(int r, int g, int b) : red(r), green(g), blue(b) {}
};

#endif
