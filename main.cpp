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
    Syntax sintactico;


    
    sintactico.CommentAnalizer(datos);
    string forsito = sintactico.extractBlock(datos);
    //cout << forsito << "<- el for" << endl;
    if (forsito.empty()) {
        cout << "No hay for que triste" << endl;
    } else {
        cout << forsito << "<- el for" << endl;
        sintactico.forAnalizer(forsito);
            
    }
    
    sintactico.getsyntax(tokens);

    return 0;
}