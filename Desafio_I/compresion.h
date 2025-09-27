#ifndef COMPRESION_H
#define COMPRESION_H

void decompressRLE(const unsigned char* compressed, int compressedLen, char*& output, int& outputLen);
void decompressLZ78(const unsigned char* compressed, int compressedLen, char*& output, int& outputLen);

#endif
