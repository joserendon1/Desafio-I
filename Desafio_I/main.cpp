#include <iostream>

int comprimirRLE(const char* entrada, int tamañoEntrada, char* salida){
    if(tamañoEntrada == 0){
        return 0;
    }
    int salida_i = 0;
    int contador = 1;
    char caracterActual = entrada[0];

    for (int i = 1; i <= tamañoEntrada; i++) {
        if (i < tamañoEntrada && entrada[i] == caracterActual) {
            contador++;
        } else {

            int digi = 0;
            int tempContador = contador;

            while (tempContador > 0) {
                digi++;
                tempContador /= 10;
            }

            tempContador = contador;
            for (int d = digi - 1; d >= 0; d--) {
                salida[salida_i + d] = '0' + (tempContador % 10);
                tempContador /= 10;
            }
            salida_i += digi;

            salida[salida_i++] = caracterActual;

            if (i < tamañoEntrada) {
                caracterActual = entrada[i];
                contador = 1;
            }
        }
    }

    return salida_i;
}

int descomprimirRLE(const char* entrada, int tamañoEntrada, char* salida) {

    if (tamañoEntrada == 0){
        return 0;
    }

    int salida_i = 0;
    int i = 0;

    while (i < tamañoEntrada) {
        int count = 0;
        while (i < tamañoEntrada && entrada[i] >= '0' && entrada[i] <= '9') {
            count = count * 10 + (entrada[i] - '0');
            i++;
        }

        if (i < tamañoEntrada) {
            char c = entrada[i++];

            for (int j = 0; j < count; j++) {
                salida[salida_i++] = c;
            }
        }
    }

    return salida_i;
}

void imprimirChar(const char* datos, int tamaño) {
    for (int i = 0; i < tamaño; i++) {
        std::cout << datos[i];
    }
    std::cout << std::endl;
}

int main(){

    const char testRLE[] = "AYYXXDDFFFFFA";
    int testRLETamaño = sizeof(testRLE) - 1;
    char comprimido[100] = {0};
    char descomprimido[100] = {0};
    int dimesionComprimido = comprimirRLE(testRLE, testRLETamaño, comprimido);
    int dimesionDescomprimido = descomprimirRLE(comprimido, dimesionComprimido, descomprimido);
    std::cout << "Original: ";
    imprimirChar(testRLE, testRLETamaño);
    std::cout << "Comprimido RLE: ";
    imprimirChar(comprimido, dimesionComprimido);
    std::cout << "Descomprimido RLE: ";
    imprimirChar(descomprimido, dimesionDescomprimido);

    return 0;
}
