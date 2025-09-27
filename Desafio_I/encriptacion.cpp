#include "encriptacion.h"
#include "compresion.h"
#include "utils.h"

unsigned char rotarDerecha(unsigned char byte, int n) {
    n = n % 8;
    return (n == 0) ? byte : (byte >> n) | (byte << (8 - n));
}

bool probarCombinacion(const unsigned char* datosEncriptados, int tamaño,
                       const char* pista, int largoPista,
                       int rotacion, unsigned char clave,
                       char* resultadoFinal, int& metodoEncontrado, int& posicionEncontrada) {

    if (tamaño <= 0 || largoPista <= 0 || rotacion < 1 || rotacion > 7) return false;

    unsigned char* desencriptado = new unsigned char[tamaño + 1];
    for (int i = 0; i < tamaño; i++) {
        unsigned char byte = datosEncriptados[i];
        byte = byte ^ clave;
        byte = rotarDerecha(byte, rotacion);
        desencriptado[i] = byte;
    }
    desencriptado[tamaño] = '\0';

    char resultadoRLE[100000] = {0};
    int lenRLE = 0;
    decompressRLE(desencriptado, tamaño, resultadoRLE, lenRLE);

    if (lenRLE >= largoPista && buscarSubcadena(resultadoRLE, lenRLE, pista, largoPista)) {
        for (int i = 0; i <= lenRLE - largoPista; i++) {
            bool match = true;
            for (int j = 0; j < largoPista; j++) {
                if (resultadoRLE[i + j] != pista[j]) {
                    match = false;
                    break;
                }
            }
            if (match) {
                posicionEncontrada = i;
                break;
            }
        }

        int copiarLen = (lenRLE < 99999) ? lenRLE : 99999;
        for (int i = 0; i < copiarLen; i++) resultadoFinal[i] = resultadoRLE[i];
        resultadoFinal[copiarLen] = '\0';
        metodoEncontrado = 1;
        delete[] desencriptado;
        return true;
    }

    char resultadoLZ78[100000] = {0};
    int lenLZ78 = 0;
    decompressLZ78(desencriptado, tamaño, resultadoLZ78, lenLZ78);

    if (lenLZ78 >= largoPista && buscarSubcadena(resultadoLZ78, lenLZ78, pista, largoPista)) {
        for (int i = 0; i <= lenLZ78 - largoPista; i++) {
            bool match = true;
            for (int j = 0; j < largoPista; j++) {
                if (resultadoLZ78[i + j] != pista[j]) {
                    match = false;
                    break;
                }
            }
            if (match) {
                posicionEncontrada = i;
                break;
            }
        }

        int copiarLen = (lenLZ78 < 99999) ? lenLZ78 : 99999;
        for (int i = 0; i < copiarLen; i++) resultadoFinal[i] = resultadoLZ78[i];
        resultadoFinal[copiarLen] = '\0';
        metodoEncontrado = 2;
        delete[] desencriptado;
        return true;
    }

    delete[] desencriptado;
    return false;
}
