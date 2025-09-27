#include "compresion.h"

void decompressRLE(const unsigned char* compressed, int compressedLen, char* output, int& outputLen) {
    outputLen = 0;
    if (compressedLen < 3) return;

    for (int i = 0; i <= compressedLen - 3; i += 3) {
        int length = (compressed[i] << 8) | compressed[i + 1];
        char character = compressed[i + 2];

        if (length <= 0 || length > 1000) continue;

        for (int j = 0; j < length && outputLen < 99999; j++) {
            output[outputLen++] = character;
        }

        if (outputLen >= 99999) break;
    }
    output[outputLen] = '\0';
}

void decompressLZ78(const unsigned char* compressed, int compressedLen, char* output, int& outputLen) {
    char** dict = new char*[65536];
    int* dictLens = new int[65536];
    int dictSize = 1;

    dict[0] = nullptr;
    dictLens[0] = 0;
    outputLen = 0;

    for (int i = 0; i < compressedLen; i += 3) {
        if (i + 2 >= compressedLen) break;

        int prefixIndex = (compressed[i] << 8) | compressed[i + 1];
        char nextChar = compressed[i + 2];

        if (nextChar == 0) break;

        if (prefixIndex == 0) {
            output[outputLen++] = nextChar;

            if (dictSize < 65536) {
                dictLens[dictSize] = 1;
                dict[dictSize] = new char[1];
                dict[dictSize][0] = nextChar;
                dictSize++;
            }
        } else if (prefixIndex < dictSize) {
            int prefixLen = dictLens[prefixIndex];

            for (int j = 0; j < prefixLen; j++) {
                if (outputLen < 99999) output[outputLen++] = dict[prefixIndex][j];
            }
            if (outputLen < 99999) output[outputLen++] = nextChar;

            if (dictSize < 65536 && outputLen < 99999) {
                dictLens[dictSize] = prefixLen + 1;
                dict[dictSize] = new char[prefixLen + 1];
                for (int j = 0; j < prefixLen; j++) {
                    dict[dictSize][j] = dict[prefixIndex][j];
                }
                dict[dictSize][prefixLen] = nextChar;
                dictSize++;
            }
        }
        if (outputLen >= 99999) break;
    }
    output[outputLen] = '\0';

    for (int i = 1; i < dictSize; i++) delete[] dict[i];
    delete[] dict;
    delete[] dictLens;
}
