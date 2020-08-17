#include "RTTypes.h"
#include <cstring>

// perform a guassian blur on a pixel buffer of specified dimensions; the kernel
// size of the blur is hard-coded to 3x3 pixels
void postProcessing_3x3GuassianBlur(uint32_t * pixelBuffer,
    unsigned int windowWidth, unsigned int windowHeight) {
    
    int x,y;

    // copy the buffer so that we still have access to the original values as
    // we modify the pixel contents
    uint32_t * copyBuffer = new uint32_t[windowWidth * windowHeight];
    memcpy(copyBuffer, pixelBuffer, windowWidth * windowHeight
        * sizeof(uint32_t));

    // iterate through each pixel, leaving off a one pixel border where the 3x3
    // kernel would extend past the buffer boundaries
    for (y = 1; y < windowHeight - 1; y++) {
        for (x = 1; x < windowWidth - 1; x++) {
            Color_Int center, top, bottom, left, right, topLeft, topRight,
                bottomLeft, bottomRight;

            // load the 9 source pixels into Color_Int objects, thus seperating
            // the red, green and blue components
            center.fromUint32(copyBuffer[windowWidth * y + x]);
            top.fromUint32(copyBuffer[windowWidth * (y - 1) + x]);
            bottom.fromUint32(copyBuffer[windowWidth * (y + 1) + x]);
            left.fromUint32(copyBuffer[windowWidth * y + x - 1]);
            right.fromUint32(copyBuffer[windowWidth * y + x + 1]);
            topLeft.fromUint32(copyBuffer[windowWidth * (y - 1) + x - 1]);
            topRight.fromUint32(copyBuffer[windowWidth * (y - 1) + x + 1]);
            bottomLeft.fromUint32(copyBuffer[windowWidth * (y + 1) + x - 1]);
            bottomRight.fromUint32(copyBuffer[windowWidth * (y + 1) + x + 1]);

            // average the 9 pixels according to the weights of the guassian
            // kernel, and save the result back in the original pixel buffer
            pixelBuffer[windowWidth * y + x] = ((
                topLeft * 16 + top * 32 + topRight * 16 + 
                left * 32 + center * 64 + right * 32 + 
                bottomLeft * 16 + bottom * 8 + bottomRight * 16
            ) / 256 ).toUint32();
        }
    }

    // cleanup allocated memory
    delete copyBuffer;
}