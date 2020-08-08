#include "RTTypes.h"
#include <cstring>

void postProcessing_3x3GuassianBlur(uint32_t * pixelBuffer,
    unsigned int windowWidth, unsigned int windowHeight) {
    int x,y;

    uint32_t * copyBuffer = new uint32_t[windowWidth * windowHeight];
    memcpy(copyBuffer, pixelBuffer, windowWidth * windowHeight
        * sizeof(uint32_t));

    for (y = 1; y < windowHeight - 1; y++) {
        for (x = 1; x < windowWidth - 1; x++) {
            Color_Int center, top, bottom, left, right, topLeft, topRight,
                bottomLeft, bottomRight;

            center.fromUint32(copyBuffer[windowWidth * y + x]);
            top.fromUint32(copyBuffer[windowWidth * (y - 1) + x]);
            bottom.fromUint32(copyBuffer[windowWidth * (y + 1) + x]);
            left.fromUint32(copyBuffer[windowWidth * y + x - 1]);
            right.fromUint32(copyBuffer[windowWidth * y + x + 1]);
            topLeft.fromUint32(copyBuffer[windowWidth * (y - 1) + x - 1]);
            topRight.fromUint32(copyBuffer[windowWidth * (y - 1) + x + 1]);
            bottomLeft.fromUint32(copyBuffer[windowWidth * (y + 1) + x - 1]);
            bottomRight.fromUint32(copyBuffer[windowWidth * (y + 1) + x + 1]);

            pixelBuffer[windowWidth * y + x] = ((
                topLeft * 16 + top * 32 + topRight * 16 + 
                left * 32 + center * 64 + right * 32 + 
                bottomLeft * 16 + bottom * 8 + bottomRight * 16
            ) / 256 ).toUint32();
        }
    }

    delete copyBuffer;
}