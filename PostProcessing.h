
void postProcessing_3x3GuassianBlur(uint32_t * pixelBuffer, unsigned int windowWidth, unsigned int windowHeight) {
    int x,y;

    uint32_t * copyBuffer = new uint32_t[windowWidth * windowHeight];
    memcpy(copyBuffer, pixelBuffer, windowWidth * windowHeight * sizeof(uint32_t));

    for (y = 1; y < windowHeight - 1; y++) {
        for (x = 1; x < windowWidth - 1; x++) {
            Color_Int center; center.fromUint32(copyBuffer[windowWidth * y + x]);
            Color_Int top; top.fromUint32(copyBuffer[windowWidth * (y - 1) + x]);
            Color_Int bottom; bottom.fromUint32(copyBuffer[windowWidth * (y + 1) + x]);
            Color_Int left; left.fromUint32(copyBuffer[windowWidth * y + x - 1]);
            Color_Int right; right.fromUint32(copyBuffer[windowWidth * y + x + 1]);
            Color_Int topLeft; topLeft.fromUint32(copyBuffer[windowWidth * (y - 1) + x - 1]);
            Color_Int topRight; topRight.fromUint32(copyBuffer[windowWidth * (y - 1) + x + 1]);
            Color_Int bottomLeft; bottomLeft.fromUint32(copyBuffer[windowWidth * (y + 1) + x - 1]);
            Color_Int bottomRight; bottomRight.fromUint32(copyBuffer[windowWidth * (y + 1) + x + 1]);

            pixelBuffer[windowWidth * y + x] = ((
                topLeft * 16 + top * 32 + topRight * 16 + 
                left * 32 + center * 64 + right * 32 + 
                bottomLeft * 16 + bottom * 8 + bottomRight * 16
            ) / 256 ).toUint32();
        }
    }

    delete copyBuffer;
}