#include <iostream>
#include "BMP.h"
#include <cmath>

// Function to plot a pixel and its symmetric counterparts
void plotEllipsePoints(BMP &bmp, int x, int y, int cx, int cy, uint8_t B, uint8_t G, uint8_t R, uint8_t A) {
    bmp.set_pixel(cx - x, cy + y, B, G, R, A); // Quadrant 2
    bmp.set_pixel(cx - x, cy - y, B, G, R, A); // Quadrant 3
}

// Midpoint Ellipse Algorithm
void rasterizeEllipse(BMP &bmp, int cx, int cy, int a, int b, uint8_t B, uint8_t G, uint8_t R, uint8_t A) {
    int a2 = a * a;
    int b2 = b * b;
    int twoA2 = 2 * a2;
    int twoB2 = 2 * b2;

    int x = 0;
    int y = b;
    int px = 0;
    int py = twoA2 * y;

    // Region 1
    int p = round(b2 - (a2 * b) + (0.25 * a2));
    while (px < py) {
        plotEllipsePoints(bmp, x, y, cx, cy, B, G, R, A);
        x++;
        px += twoB2;
        if (p < 0) {
            p += b2 + px;
        } else {
            y--;
            py -= twoA2;
            p += b2 + px - py;
        }
    }

    // Region 2
    p = round(b2 * (x + 0.5) * (x + 0.5) + a2 * (y - 1) * (y - 1) - a2 * b2);
    while (y >= 0) {
        plotEllipsePoints(bmp, x, y, cx, cy, B, G, R, A);
        y--;
        py -= twoA2;
        if (p > 0) {
            p += a2 - py;
        } else {
            x++;
            px += twoB2;
            p += a2 - py + px;
        }
    }
}

int main() {
    // Create a 200x200 canvas
    BMP bmpNew(200, 200, false);

    // Set the canvas to black
    bmpNew.fill_region(0, 0, 200, 200, 0, 0, 0, 0);

    // Center of the ellipse
    int cx = 100, cy = 100;

    // Semi-major and semi-minor axes
    int a = 16 * 30; // a = 480
    int b = 25 * 30; // b = 750

    // Scale down the ellipse to fit the 200x200 canvas
    a /= 10;
    b /= 10;

    // Rasterize the ellipse
    rasterizeEllipse(bmpNew, cx, cy, a, b, 255, 255, 255, 0);

    // Save the output
    bmpNew.write("output.bmp");

    std::cout << "Ellipse rasterized and saved to output.bmp" << std::endl;
    return 0;
}