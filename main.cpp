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

    // BEGIN TESTING
    // if x then begin x := x + 1; end;
    vector<string> code = {"i", "f", "x", "t", "h", "e", "n"};
    int ifanalizer = sintactico.ifAnalizer(code);
    
    // for i := 0 to 150 do begin x := x + 1; writeln(x); end;
    vector<string> code2 = {"f", "o", "r", "i", ":", "=", "0", "t", "o", "150", "d", "o"};
    int analizer = sintactico.forAnalizer(code2);
    

    vector<string> code3 = {"/* Comentario"};
    sintactico.CommentAnalizer(code3);
    // END TESTING
    
    //sintactico.CommentAnalizer(datos);
    // vector<string> forsito = sintactico.extractBlock(datos);
    // if (forsito.empty()) {
    //     cout << "No hay bucle for que triste" << endl;
    // }
    // sintactico.forAnalizer(forsito);
    
    sintactico.getSyntax(tokens);

    return 0;
}