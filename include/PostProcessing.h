#ifndef _POST_PROCESSING___H_
#define _POST_PROCESSING___H_

#include <cstdint>

void postProcessing_3x3GuassianBlur(uint32_t * pixelBuffer,
    unsigned int windowWidth, unsigned int windowHeight);

#endif // _POST_PROCESSING___H_