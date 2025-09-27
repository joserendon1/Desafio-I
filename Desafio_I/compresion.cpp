#include "compresion.h"

int calcularTamañoDescomprimidoRLE(const unsigned char* compressed, int compressedLen) {
    int totalLength = 0;

    for (int i = 0; i <= compressedLen - 3; i += 3) {
        int length = (compressed[i] << 8) | compressed[i + 1];
        if (length > 0 && length <= 10000) {
            totalLength += length;
        }
    }

    return totalLength;
}

int estimarTamañoDescomprimidoLZ78(const unsigned char* compressed, int compressedLen) {

    int estimatedSize = compressedLen;

    if (compressedLen > 0) {
        estimatedSize = compressedLen * 3;
        if (estimatedSize > 1000000) estimatedSize = 1000000;
    }

    return estimatedSize;
}

void decompressRLE(const unsigned char* compressed, int compressedLen, char*& output, int& outputLen) {
    outputLen = 0;
    output = nullptr;

    if (compressedLen < 3) return;

    int totalLength = calcularTamañoDescomprimidoRLE(compressed, compressedLen);

    if (totalLength <= 0 || totalLength > 10000000) {
        return;
    }

    output = new char[totalLength + 1];

    outputLen = 0;
    for (int i = 0; i <= compressedLen - 3; i += 3) {
        int length = (compressed[i] << 8) | compressed[i + 1];
        char character = compressed[i + 2];

        if (length <= 0 || length > 10000) continue;

        for (int j = 0; j < length; j++) {
            if (outputLen < totalLength) {
                output[outputLen++] = character;
            }
        }
    }
    output[outputLen] = '\0';
}

void decompressLZ78(const unsigned char* compressed, int compressedLen, char*& output, int& outputLen) {
    const int MAX_DICT_ENTRIES = 65536;

    int dictCapacity;
    if (compressedLen < 100) {
        dictCapacity = 256;
    } else if (compressedLen < 1000) {
        dictCapacity = 1024;
    } else if (compressedLen < 10000) {
        dictCapacity = 4096;
    } else {
        dictCapacity = 16384;
    }

    if (dictCapacity > MAX_DICT_ENTRIES) {
        dictCapacity = MAX_DICT_ENTRIES;
    }

    char** dict = new char*[dictCapacity];
    int* dictLens = new int[dictCapacity];
    int dictSize = 1;

    dict[0] = nullptr;
    dictLens[0] = 0;
    outputLen = 0;

    int bufferSize = 1024;
    output = new char[bufferSize + 1];

    int redimensionamientosDict = 0;
    int redimensionamientosOutput = 0;

    for (int i = 0; i < compressedLen; i += 3) {
        if (i + 2 >= compressedLen) break;

        int prefixIndex = (compressed[i] << 8) | compressed[i + 1];
        char nextChar = compressed[i + 2];

        if (nextChar == 0) break;

        if (prefixIndex >= dictSize) {

            continue;
        }

        if (dictSize >= dictCapacity && dictCapacity < MAX_DICT_ENTRIES) {
            int newCapacity = dictCapacity * 2;
            if (newCapacity > MAX_DICT_ENTRIES) {
                newCapacity = MAX_DICT_ENTRIES;
            }

            char** newDict = new char*[newCapacity];
            int* newDictLens = new int[newCapacity];

            for (int j = 0; j < dictCapacity; j++) {
                newDict[j] = dict[j];
                newDictLens[j] = dictLens[j];
            }

            delete[] dict;
            delete[] dictLens;

            dict = newDict;
            dictLens = newDictLens;
            dictCapacity = newCapacity;
            redimensionamientosDict++;
        }

        if (prefixIndex == 0) {
            if (outputLen + 1 >= bufferSize) {
                int newBufferSize = bufferSize * 2;
                char* newOutput = new char[newBufferSize + 1];
                for (int j = 0; j < outputLen; j++) {
                    newOutput[j] = output[j];
                }
                delete[] output;
                output = newOutput;
                bufferSize = newBufferSize;
                redimensionamientosOutput++;
            }

            output[outputLen++] = nextChar;

            if (dictSize < dictCapacity) {
                dictLens[dictSize] = 1;
                dict[dictSize] = new char[1];
                dict[dictSize][0] = nextChar;
                dictSize++;
            }
        } else if (prefixIndex < dictSize) {
            int prefixLen = dictLens[prefixIndex];
            int neededSpace = prefixLen + 1;

            if (outputLen + neededSpace >= bufferSize) {
                int newBufferSize = (outputLen + neededSpace) * 2;
                char* newOutput = new char[newBufferSize + 1];
                for (int j = 0; j < outputLen; j++) {
                    newOutput[j] = output[j];
                }
                delete[] output;
                output = newOutput;
                bufferSize = newBufferSize;
                redimensionamientosOutput++;
            }

            for (int j = 0; j < prefixLen; j++) {
                output[outputLen++] = dict[prefixIndex][j];
            }
            output[outputLen++] = nextChar;

            if (dictSize < dictCapacity) {
                dictLens[dictSize] = prefixLen + 1;
                dict[dictSize] = new char[prefixLen + 1];
                for (int j = 0; j < prefixLen; j++) {
                    dict[dictSize][j] = dict[prefixIndex][j];
                }
                dict[dictSize][prefixLen] = nextChar;
                dictSize++;
            }
        }
    }
    output[outputLen] = '\0';

    for (int i = 1; i < dictSize; i++) {
        delete[] dict[i];
    }
    delete[] dict;
    delete[] dictLens;
}
