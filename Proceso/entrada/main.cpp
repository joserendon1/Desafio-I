#include <iostream>

int main() {
    char numero[10];
    std::cout << "Ingrese el numero: ";
    std::cin >> numero;

    char nombreArchivo[50];
    char nombrePista[50];

    int i = 0;
    const char prefijoArchivo[] = "Encriptado";
    const char sufijo[] = ".txt";

    for (int j = 0; prefijoArchivo[j] != '\0'; j++) {
        nombreArchivo[i++] = prefijoArchivo[j];
    }

    for (int j = 0; numero[j] != '\0'; j++) {
        nombreArchivo[i++] = numero[j];
    }

    for (int j = 0; sufijo[j] != '\0'; j++) {
        nombreArchivo[i++] = sufijo[j];
    }
    nombreArchivo[i] = '\0';

    i = 0;
    const char prefijoPista[] = "pista";

    for (int j = 0; prefijoPista[j] != '\0'; j++) {
        nombrePista[i++] = prefijoPista[j];
    }

    for (int j = 0; numero[j] != '\0'; j++) {
        nombrePista[i++] = numero[j];
    }

    for (int j = 0; sufijo[j] != '\0'; j++) {
        nombrePista[i++] = sufijo[j];
    }
    nombrePista[i] = '\0';

    std::cout << "Archivo: " << nombreArchivo << std::endl;
    std::cout << "Pista: " << nombrePista << std::endl;

    return 0;
}
