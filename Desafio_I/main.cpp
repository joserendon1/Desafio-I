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

int main(){

    // 'A' = 65 = 01000001
    // Rotar 1 bit a la derecha: 10100000 = 160
    char resultado = rotarDerecha('A', 1);
    std::cout << (int)(unsigned char)resultado; // Debe ser 160
    return 0;
}
