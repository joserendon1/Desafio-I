#ifndef ENCRIPTACION_H
#define ENCRIPTACION_H

unsigned char rotarDerecha(unsigned char byte, int n);
bool probarCombinacion(const unsigned char* datosEncriptados, int tamaño,
                       const char* pista, int largoPista,
                       int rotacion, unsigned char clave,
                       char*& resultadoFinal, int& resultadoLen,
                       int& metodoEncontrado, int& posicionEncontrada);

#endif
