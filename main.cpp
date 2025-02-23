#include "Analizadores/lexico.h"
#include "Analizadores/sintactico.h"
#include <vector>

int main() {

    string filename = "archivo/programa.pas";

    ifstream pascal(filename);
    if (!pascal.is_open()) {
        cout << "ERROR: intento fallido al abrir el archivo. -> " << filename << endl;
        exit(EXIT_FAILURE);
    }

    stringstream buffer;
    buffer << pascal.rdbuf();
    string datos = buffer.str();

    vector<string> tokens = tokenize(datos);

    //PrintingTokens(tokens);
    Syntax tumama;
    
    tumama.getsyntax(tokens);
    
    

    return 0;
}