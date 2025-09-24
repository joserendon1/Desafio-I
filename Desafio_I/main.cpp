#include <iostream>

char rotarDerecha(char byte, int n) {
    n = n % 8;
    if (n == 0) return byte;
    unsigned char ubyte = (unsigned char)byte;
    return (char)((ubyte >> n) | (ubyte << (8 - n)));
}

int calcularLongitud(const char* str) {
    int len = 0;
    while (str[len] != '\0' && len < 1000) {
        len++;
    }
    return len;
}

bool buscarSubcadena(const char* texto, int lenTexto, const char* subcadena, int lenSubcadena) {
    if (lenSubcadena <= 0 || lenTexto < lenSubcadena){
        return false;
    }

    for (int i = 0; i <= lenTexto - lenSubcadena; i++) {
        bool encontrado = true;
        for (int j = 0; j < lenSubcadena; j++) {
            if (texto[i + j] != subcadena[j]) {
                encontrado = false;
                break;
            }
        }
        if (encontrado){
            return true;
        }
    }
    return false;
}

void decompressRLE(const char* compressed, int compressedLen, char* output, int& outputLen) {
    outputLen = 0;

    if (compressedLen < 3) return;

    for (int i = 0; i <= compressedLen - 3; i += 3) {
        int length = ((unsigned char)compressed[i] << 8) | (unsigned char)compressed[i + 1];
        char character = compressed[i + 2];

        if (length <= 0 || length > 1000) continue;

        for (int j = 0; j < length && outputLen < 99999; j++) {
            output[outputLen++] = character;
        }

        if (outputLen >= 99999) break;
    }

    output[outputLen] = '\0';
}

void crearDatosRLE(unsigned char high, unsigned char low, char character, char* buffer) {
    buffer[0] = (char)high;
    buffer[1] = (char)low;
    buffer[2] = character;
}

int main() {

    char output[100000];
    int outputLen;

    {
        char compressed[3];
        crearDatosRLE(0, 3, 'A', compressed);

        decompressRLE(compressed, 3, output, outputLen);
        std::cout << "Entrada: high=0, low=3, char='A'" << std::endl;
        std::cout << "Salida: ";
        for (int i = 0; i < outputLen; i++) std::cout << output[i];
        std::cout << std::endl;
        std::cout << "Longitud: " << outputLen << std::endl;
    }

    {
        char problematico = 255;
        unsigned char correcto = 255;

        std::cout << "char 255 como signed: " << (int)problematico << std::endl;
        std::cout << "unsigned char 255: " << (int)correcto << std::endl;
    }

    {
        char compressed[3];
        crearDatosRLE(0, 255, 'X', compressed);

        decompressRLE(compressed, 3, output, outputLen);
        std::cout << "Entrada: high=0, low=255, char='X'" << std::endl;
        std::cout << "Longitud calculada: " << ((0 << 8) | 255) << std::endl;
        std::cout << "Longitud salida: " << outputLen << std::endl;
        if (outputLen > 0) {
            std::cout << "Primeros 5 caracteres: ";
            for (int i = 0; i < 5 && i < outputLen; i++) std::cout << output[i];
            std::cout << std::endl;
        }
    }

    {

        unsigned char high = 0;
        unsigned char low = 4;
        char character = 'B';

        char compressed[3] = {(char)high, (char)low, character};

        std::cout << "Valores originales: high=" << (int)high << ", low=" << (int)low << std::endl;
        std::cout << "Valores como char: high=" << (int)(char)high << ", low=" << (int)(char)low << std::endl;

        int length_calculada = ((unsigned char)compressed[0] << 8) | (unsigned char)compressed[1];
        std::cout << "Longitud calculada: " << length_calculada << std::endl;

        decompressRLE(compressed, 3, output, outputLen);
        std::cout << "Salida: ";
        for (int i = 0; i < outputLen; i++) std::cout << output[i];
        std::cout << std::endl;
    }

    {

        unsigned char compressed[] = {0, 3, 'C'};

        decompressRLE(reinterpret_cast<const char*>(compressed), 3, output, outputLen);

        std::cout << "Entrada: {0, 3, 'C'} como unsigned char[]" << std::endl;
        std::cout << "Salida: ";
        for (int i = 0; i < outputLen; i++) std::cout << output[i];
        std::cout << std::endl;
        std::cout << "Longitud: " << outputLen << std::endl;
    }

    {
        unsigned char compressed[] = {3, 232, 'Y'};

        decompressRLE(reinterpret_cast<const char*>(compressed), 3, output, outputLen);
        std::cout << "Entrada: {3, 232, 'Y'} (longitud=1000)" << std::endl;
        std::cout << "Longitud salida: " << outputLen << std::endl;
    }

    {

        unsigned char compressed[] = {3, 233, 'Z'};

        decompressRLE(reinterpret_cast<const char*>(compressed), 3, output, outputLen);
        std::cout << "Entrada: {3, 233, 'Z'} (longitud=1001 > 1000)" << std::endl;
        std::cout << "Longitud salida: " << outputLen << " (debe ser 0)" << std::endl;
    }

    return 0;
}
