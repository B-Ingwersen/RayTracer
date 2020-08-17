#ifndef _POST_PROCESSING___H_
#define _POST_PROCESSING___H_

#include <cstdint>

// perform a guassian blur on a pixel buffer of specified dimensions; the kernel
// size of the blur is hard-coded to 3x3 pixels
void postProcessing_3x3GuassianBlur(uint32_t * pixelBuffer,
    unsigned int windowWidth, unsigned int windowHeight);

#endif // _POST_PROCESSING___H_