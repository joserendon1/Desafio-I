#include <iostream>

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

    char texto1[] = "Hola";
    char texto2[10] = "Mundo";
    char texto3[] = "Desafio 1";

    std::cout << calcularLongitud(texto1) << std::endl;
    std::cout << calcularLongitud(texto2) << std::endl;
    std::cout << calcularLongitud(texto3) << std::endl;

    char texto4[] = "Quick brown fox";
    int lenTexto = 15;
    char subcadena[] = "zorro";
    int lenSubcadena = 5;

    std::cout << buscarSubcadena(texto4, lenTexto, subcadena, lenSubcadena) << std::endl;

    return 0;
}
