#include <iostream>
#include <fstream>

void obtenerNombresArchivos(const char* numero, char* nombreArchivo, char* nombrePista) {
    const char prefijoArchivo[] = "Encriptado";
    const char prefijoPista[] = "pista";
    const char sufijo[] = ".txt";

    int pos = 0;

    for (int i = 0; prefijoArchivo[i] != '\0'; i++) {
        nombreArchivo[pos++] = prefijoArchivo[i];
    }
    for (int i = 0; numero[i] != '\0'; i++) {
        nombreArchivo[pos++] = numero[i];
    }
    for (int i = 0; sufijo[i] != '\0'; i++) {
        nombreArchivo[pos++] = sufijo[i];
    }
    nombreArchivo[pos] = '\0';

    pos = 0;
    for (int i = 0; prefijoPista[i] != '\0'; i++) {
        nombrePista[pos++] = prefijoPista[i];
    }
    for (int i = 0; numero[i] != '\0'; i++) {
        nombrePista[pos++] = numero[i];
    }
    for (int i = 0; sufijo[i] != '\0'; i++) {
        nombrePista[pos++] = sufijo[i];
    }
    nombrePista[pos] = '\0';
}

unsigned char* leerArchivoBinario(const char* nombreArchivo, int& tamaño) {
    std::ifstream archivo(nombreArchivo, std::ios::binary | std::ios::ate);
    if (!archivo.is_open()) {
        std::cout << "Error: No se pudo abrir " << nombreArchivo << std::endl;
        tamaño = 0;
        return nullptr;
    }

    tamaño = archivo.tellg();
    archivo.seekg(0, std::ios::beg);

    unsigned char* datos = new unsigned char[tamaño];
    archivo.read(reinterpret_cast<char*>(datos), tamaño);
    archivo.close();

    return datos;
}

unsigned char rotarDerecha(unsigned char byte, int n) {
    n = n % 8;
    if (n == 0) return byte;
    return (byte >> n) | (byte << (8 - n));
}

int calcularLongitud(const char* str) {
    int len = 0;
    while (str[len] != '\0' && len < 1000) {
        len++;
    }
    return len;
}

bool buscarSubcadena(const char* texto, int lenTexto, const char* subcadena, int lenSubcadena) {
    if (lenSubcadena <= 0 || lenTexto < lenSubcadena) return false;

    for (int i = 0; i <= lenTexto - lenSubcadena; i++) {
        bool encontrado = true;
        for (int j = 0; j < lenSubcadena; j++) {
            if (texto[i + j] != subcadena[j]) {
                encontrado = false;
                break;
            }
        }
        if (encontrado) return true;
    }
    return false;
}

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
                if (outputLen < 99999) {
                    output[outputLen++] = dict[prefixIndex][j];
                }
            }

            if (outputLen < 99999) {
                output[outputLen++] = nextChar;
            }

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

    for (int i = 1; i < dictSize; i++) {
        delete[] dict[i];
    }
    delete[] dict;
    delete[] dictLens;
}

bool probarCombinacion(const unsigned char* datosEncriptados, int tamaño,
                       const char* pista, int largoPista,
                       int rotacion, unsigned char clave,
                       char* resultadoFinal, int& metodoEncontrado, int& posicionEncontrada) {

    if (tamaño <= 0 || largoPista <= 0 || rotacion < 1 || rotacion > 7) {
        return false;
    }

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

    if (lenRLE >= largoPista) {
        if (buscarSubcadena(resultadoRLE, lenRLE, pista, largoPista)) {
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
            for (int i = 0; i < copiarLen; i++) {
                resultadoFinal[i] = resultadoRLE[i];
            }
            resultadoFinal[copiarLen] = '\0';
            metodoEncontrado = 1;
            delete[] desencriptado;
            return true;
        }
    }

    char resultadoLZ78[100000] = {0};
    int lenLZ78 = 0;
    decompressLZ78(desencriptado, tamaño, resultadoLZ78, lenLZ78);

    if (lenLZ78 >= largoPista) {
        if (buscarSubcadena(resultadoLZ78, lenLZ78, pista, largoPista)) {
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
            for (int i = 0; i < copiarLen; i++) {
                resultadoFinal[i] = resultadoLZ78[i];
            }
            resultadoFinal[copiarLen] = '\0';
            metodoEncontrado = 2;
            delete[] desencriptado;
            return true;
        }
    }

    delete[] desencriptado;
    return false;
}

int main() {

    char numero[10];
    char nombreArchivo[50];
    char nombrePista[50];

    std::cout << "Ingrese el numero del archivo: ";
    std::cin >> numero;

    obtenerNombresArchivos(numero, nombreArchivo, nombrePista);

    std::cout << "Buscando archivo: " << nombreArchivo << std::endl;
    std::cout << "Buscando pista: " << nombrePista << std::endl;
    std::cout << std::endl;

    int tamaño;
    unsigned char* datosEncriptados = leerArchivoBinario(nombreArchivo, tamaño);

    if (datosEncriptados == nullptr) {
        std::cout << "No se pudo cargar el archivo encriptado." << std::endl;
        return 1;
    }

    std::cout << "Archivo encriptado: " << nombreArchivo << " (" << tamaño << " bytes)" << std::endl;

    std::ifstream pistaFile(nombrePista);
    if (!pistaFile.is_open()) {
        std::cout << "Error: No se pudo abrir " << nombrePista << std::endl;
        delete[] datosEncriptados;
        return 1;
    }

    char pista[1000] = {0};
    pistaFile.getline(pista, 1000);
    pistaFile.close();

    int largoPista = calcularLongitud(pista);
    std::cout << "Pista conocida: \"" << pista << "\" (" << largoPista << " caracteres)" << std::endl;

    char resultadoFinal[100000] = {0};
    int rotacionEncontrada = -1;
    unsigned char claveEncontrada = 0;
    int metodoEncontrado = 0;
    int posicionPista = -1;
    bool encontrado = false;

    std::cout << "Rotaciones: 1-7, Claves: 0-255" << std::endl;
    std::cout << std::endl;

    for (int rotacion = 1; rotacion <= 7; rotacion++) {
        std::cout << "Rotacion " << rotacion << "/7." << std::endl;

        for (int clave = 0; clave <= 255; clave++) {
            int posicion = -1;
            int metodo = 0;

            if (probarCombinacion(datosEncriptados, tamaño, pista, largoPista,
                                  rotacion, (unsigned char)clave, resultadoFinal, metodo, posicion)) {
                rotacionEncontrada = rotacion;
                claveEncontrada = (unsigned char)clave;
                metodoEncontrado = metodo;
                posicionPista = posicion;
                encontrado = true;
                break;
            }
        }

        if (encontrado) break;
    }

    if (encontrado) {
        std::cout << "Parametros de encriptacion:" << std::endl;
        std::cout << "- Rotacion (n): " << rotacionEncontrada << " bits a la derecha" << std::endl;
        std::cout << "- Clave XOR (K): 0x" << std::hex << (int)claveEncontrada << std::dec;
        std::cout << " (" << (int)claveEncontrada << " decimal)" << std::endl;
        std::cout << "- Metodo de compresion: " << (metodoEncontrado == 1 ? "RLE" : "LZ78") << std::endl;
        std::cout << "- Posicion de la pista: caracter #" << posicionPista + 1 << std::endl;

        int longitudFinal = calcularLongitud(resultadoFinal);
        std::cout << "Longitud: " << longitudFinal << " caracteres" << std::endl;
        std::cout << "Texto:" << std::endl;
        std::cout << resultadoFinal << std::endl;

        char nombreSalida[50];
        if (metodoEncontrado == 1) {
            obtenerNombresArchivos(numero, nombreSalida, nombrePista);

            int len = calcularLongitud(nombreSalida);
            nombreSalida[len-4] = '_';
            nombreSalida[len-3] = 'r';
            nombreSalida[len-2] = 'l';
            nombreSalida[len-1] = 'e';
            nombreSalida[len] = '.';
            nombreSalida[len+1] = 't';
            nombreSalida[len+2] = 'x';
            nombreSalida[len+3] = 't';
            nombreSalida[len+4] = '\0';
        } else {
            obtenerNombresArchivos(numero, nombreSalida, nombrePista);
            int len = calcularLongitud(nombreSalida);
            nombreSalida[len-4] = '_';
            nombreSalida[len-3] = 'l';
            nombreSalida[len-2] = 'z';
            nombreSalida[len-1] = '7';
            nombreSalida[len] = '8';
            nombreSalida[len+1] = '.';
            nombreSalida[len+2] = 't';
            nombreSalida[len+3] = 'x';
            nombreSalida[len+4] = 't';
            nombreSalida[len+5] = '\0';
        }

        std::ofstream salida(nombreSalida);
        salida << resultadoFinal;
        salida.close();
        std::cout << "Resultado guardado en: " << nombreSalida << std::endl;

    } else {
        std::cout << "Posibles causas:" << std::endl;
        std::cout << "1. La pista no coincide con el texto" << std::endl;
        std::cout << "2. Los parametros estan fuera del rango probado" << std::endl;
        std::cout << "3. El formato del archivo es diferente" << std::endl;
        std::cout << "4. Se uso un metodo de compresion diferente" << std::endl;
    }

    delete[] datosEncriptados;

    return 0;
}
