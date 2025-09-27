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
                       char*& resultadoFinal, int& resultadoLen,
                       int& metodoEncontrado, int& posicionEncontrada) {

    if (tamaño <= 0 || largoPista <= 0 || rotacion < 1 || rotacion > 7) return false;

    unsigned char* desencriptado = new unsigned char[tamaño + 1];
    for (int i = 0; i < tamaño; i++) {
        unsigned char byte = datosEncriptados[i];
        byte = byte ^ clave;
        byte = rotarDerecha(byte, rotacion);
        desencriptado[i] = byte;
    }
    desencriptado[tamaño] = '\0';

    char* resultadoRLE = nullptr;
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

        resultadoLen = lenRLE;
        resultadoFinal = new char[resultadoLen + 1];
        for (int i = 0; i < resultadoLen; i++) {
            resultadoFinal[i] = resultadoRLE[i];
        }
        resultadoFinal[resultadoLen] = '\0';

        metodoEncontrado = 1;

        delete[] resultadoRLE;
        delete[] desencriptado;
        return true;
    }

    delete[] resultadoRLE;

    char* resultadoLZ78 = nullptr;
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

        resultadoLen = lenLZ78;
        resultadoFinal = new char[resultadoLen + 1];
        for (int i = 0; i < resultadoLen; i++) {
            resultadoFinal[i] = resultadoLZ78[i];
        }
        resultadoFinal[resultadoLen] = '\0';

        metodoEncontrado = 2;

        delete[] resultadoLZ78;
        delete[] desencriptado;
        return true;
    }

    delete[] resultadoLZ78;
    delete[] desencriptado;
    return false;
}
