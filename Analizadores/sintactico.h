#include "lexico.h"
#include "arbol.h"
#include <iostream>
#include <string>
#include <cctype> //isanum isalpha isdigit
#include <stdexcept> // manejo de errores
#include <algorithm>

class Syntax{
    public:
    Syntax(){};

    void getSyntax(vector<string> &datos) {
            
        if (datos.empty() || datos[0] != identifiers[6]) {
            cout << "ERROR: Falta o esta escrito mal la palabra program'";
            return;
        }
        
        int finalNumber = 3, initialNumber = 0;
        bool final = true;
        while (final){
            if (datos[initialNumber]=="end" && datos[initialNumber+1]=="."){
                cout<<"Codigo terminado";
                return;
            }
            lineaLeida(datos, finalNumber, initialNumber);
    
            if (datos[initialNumber] == identifiers[0])
            {
                getSyntaxVar(datos, symbols, initialNumber, finalNumber);
                
            } else if (datos[initialNumber] == identifiers[4])
            {
                // getsyntaxbegin
            }
            int i = 0;
            while (i<variables.size()){
                if (datos[initialNumber] == variables[i]) getSyntaxAsignation(datos,symbols, initialNumber, finalNumber);
                i++;
            }   
        }
    };
    
    // WORKS (⊙_⊙;)
    void lineaLeida(const vector<string>& datos, int& i, int& initial) {
        
        if (initial != 0) {
            i+=2;
            initial += 2;
        }

        initial = i;
        
        while (i < datos.size() && datos[i] != ";") {
            i++;
        }
        if (i > initial) {
            i--;
        }
    }

    void recursiveTree(Node<string>* actual, Tree<string>&padre, vector<string> &symbols, vector<string> &datos, int initialNumber, int finalNumber){
        // detectar separador
        int symbol;
        for(int i = 0; i< symbols.size(); i++){
            for(int j = initialNumber; j<=finalNumber;j++){
                if (symbols[i]==datos[j]){
                    padre.insertarOrdenado(datos[j], 1, actual);
                    symbol=j;
                    j= finalNumber+1;
                    i = symbols.size() +1;
                }
            }
        }
        if (symbol==initialNumber+1) padre.insertarOrdenado(datos[symbol-1], 0, actual);
        else {
            padre.insertarOrdenado("", 0, actual);
            recursiveTree(actual->getLeft(), padre, symbols, datos, initialNumber, symbol-1);
        }

        if (symbol==finalNumber-1) padre.insertarOrdenado(datos[symbol+1], 2, actual);
        else {
            padre.insertarOrdenado("", 2, actual);
            recursiveTree(actual->getRight(), padre, symbols, datos, symbol+1, finalNumber);
        }
        return;
    }

    void recursiveTree(Tree<string>&padre, vector<string> &datos, int initialNumber, int finalNumber){
        recursiveTree(padre.getRaiz(), padre, symbols, datos, initialNumber, finalNumber);
        return;
    }

    void getSyntaxVar(vector<string> &datos, vector<string> &symbols, int initial, int final) {
        Tree<string> arbolvar;
        
        initial++;
        arbolvar.insertarOrdenado("",0);
        
        recursiveTree(arbolvar, datos, initial, final);
        
        arbolvar.orden(arbolvar.getRaiz(), true);

        Node<string> *a = arbolvar.getRaiz()->getLeft();
        Node<string> *b = arbolvar.getRaiz()->getRight();

        try
        {
            bool flag = true;

            for (int i = 0; i < dataType.size(); i++)
            {
                if (b->getValue() == dataType[i])
                {
                    flag = false;
                } 
            }

            if (flag)
            {
                cout << endl << "ERROR: EN LA DECLARACION DEL FOKIN VARIABLE. " << endl;
            }
            
        }
        catch(const std::exception& e)
        {
            std::cerr << e.what() << '\n';
        }
        

        try
        {
            bool flag = true;
            while (a->getRight() != NULL)
            {    
                for (int i = 0; i < reserved.size(); i++)
                {
                    if ((reserved[i] == a->getLeft()->getValue()) || (reserved[i] == a->getRight()->getValue()))
                    {
                        flag = false;
                    }                    
                }
                if (flag)
                {
                    for (int i = 0; i < identifiers.size(); i++)
                    {
                        if ((identifiers[i] == a->getLeft()->getValue()) || (identifiers[i] == a->getRight()->getValue()))
                        {
                            flag = false;
                        }                    
                    }
                }
                if (flag)
                {
                    for (int i = 0; i < dataType.size(); i++)
                    {
                        if ((dataType[i] == a->getLeft()->getValue()) || (dataType[i] == a->getRight()->getValue()))
                        {
                            flag = false;
                        }                    
                    }
                }

                if (flag)
                {
                    variables.push_back(a->getLeft()->getValue());
                    a = a->getRight();
                    if (a->getValue() != "")
                    {
                        variables.push_back(a->getValue());
                    }
                } else {
                    cout << endl <<"ERROR: HP ARBOL HAY UNA FOKIN PALABRA RESERVADA" << endl;
                    return;
                }    
            }
        }
        catch(const std::exception& e)
        {
            std::cerr << e.what() << '\n';
        }
    }

    bool isNumber(string word){
        for (char c : word) {
            if (!isdigit(c)) {
                return false;
            }
        }
        return true;
    }

    void getSyntaxAsignation(vector<string> &datos, vector<string> &symbols, int initial, int final) {
        Tree<string> arbolvar;
        
        arbolvar.insertarOrdenado("",0);
        
        recursiveTree(arbolvar, datos, initial, final);
        
        arbolvar.orden(arbolvar.getRaiz(), true);

        Node<string> *a = arbolvar.getRaiz()->getRight();
        Node<string> *b = arbolvar.getRaiz()->getLeft();

        try
        {
            bool flag = true;

            for (int i = 0; i < variables.size(); i++)
            {
                if (b->getValue() == variables[i])
                {
                    flag = false;
                } 
            }

            if (flag)
            {
                cout << endl << "ERROR: NO PUSISTE UNA FOKIN VARIABLE" << endl;
            }
            
        }
        catch(const std::exception& e)
        {
            std::cerr << e.what() << '\n';
        }
        

        try
        {
            bool left = false;
            bool right = false;
            while (a->getRight() != NULL)
            {    
                left = isNumber(a->getLeft()->getValue());
                right = isNumber(a->getRight()->getValue());
                if (!right && a->getRight()->getValue() == "") right=true;
                if(!(left && right)){
                    for (int i = 0; i < variables.size(); i++)
                    {
                        if (variables[i] == a->getLeft()->getValue()) left = true;
                        if (variables[i] == a->getRight()->getValue()) right = true;     
                    }
                }
                if (!(left && right))
                {
                    cout<<"ERROR: NO PUSISTE BIEN LA ASIGNACION DE VARIABLE"<<endl;
                    return;
                }
                a = a->getRight();
            }
            cout<<"Todo bien"<<endl;
        }
        catch(const std::exception& e)
        {
            std::cerr << e.what() << '\n';
        }
    }
    
    void CommentAnalizer(const string& datos) {
        size_t i = 0;
        bool multiLine = false;
        //size_t multiLineStart = 0;

        while (i < datos.length()) {
            if (!multiLine && datos.substr(i, 2) == "//") { // Comentario de una Linea
                i += 2; // Saltamos por que no nos importa tbh
                while (i < datos.length() && datos[i] != '\n') { // nos vamos al salto de linea
                    i++;
                } 
                if (i < datos.length() && datos[i] == '\n') { // saltamos el salto de linea
                    i++;
                }
            } else if (!multiLine && datos.substr(i, 2) == "/*") { // Empezamos con los comentarios multilineales
                multiLine = true;
                //multiLineStart = i;
                i += 2;
            } else if (multiLine && datos.substr(i, 2) == "*/") {
                multiLine = true;
                i += 2;
            } else if (multiLine && i == datos.length() - 1) {
                cout << "ERROR: Comentario Multilineal no cerrado" << endl;
                break;
            } else {
                i++;
            }
        }
    };

    bool forAnalizer(const string& datos) {
        size_t i = 0;
    
        // 1.- Verificamos que este el for
        if (!skipWhiteSpace(datos, i) || !matchKeyword(datos, i, "for")) {
            cout << "ERROR: Hay un error en el bucle for'" << endl;
            return false;
        }
    
        // 2.- Verificamos que este el identificador
        if (!skipWhiteSpace(datos, i) || !isIdentifier(datos, i)) {
            cout << "ERROR: Hay un error en el bucle for" << endl;
            return false;
        }
    
        // 3.- Verificamos que este el operador de asignacion
        if (!skipWhiteSpace(datos, i) || !matchOperator(datos, i, ":=")) {
            cout << "ERROR: Hay un error en el bucle for" << endl;
            return false;
        }
    
        // 4.- Verificamos que existe el valor inicial (el numero pana)
        if (!skipWhiteSpace(datos, i) || !isNumber(datos, i)) {
            cout << "ERROR: Hay un error en el bucle for" << endl;
            return false;
        }
    
        // 5.- Verificamos que no haya un punto y coma después del valor inicial
        if (skipWhiteSpace(datos, i) && matchOperator(datos, i, ";")) {
            cout << "ERROR: Hay un error en el bucle for" << endl;
            return false;
        }
    
        // 6.- Verificamos que esten las palabras reservadas "to" o "downto"
        if (!skipWhiteSpace(datos, i)) {
            cout << "ERROR: Hay un error en el bucle for" << endl;
            return false;
        }
    
        bool isTo = matchKeyword(datos, i, "to");
        bool isDownTo = matchKeyword(datos, i, "downto");
    
        if (!isTo && !isDownTo) {
            cout << "ERROR: Hay un error en el bucle for'" << endl;
            return false;
        }
    
        // 7.- Verificamos que exista el valor final (otro numero)
        if (!skipWhiteSpace(datos, i) || !isNumber(datos, i)) {
            cout << "ERROR: Hay un error en el bucle for" << endl;
            return false;
        }
    
        // 8.- Verificamos que no haya un punto y coma después del valor final
        if (skipWhiteSpace(datos, i) && matchOperator(datos, i, ";")) {
            cout << "ERROR: Hay un error en el bucle for" << endl;
            return false;
        }
    
        // 9.- Verificamos el do
        if (!skipWhiteSpace(datos, i) || !matchKeyword(datos, i, "do")) {
            cout << "ERROR: Hay un error en el bucle for" << endl;
            return false;
        }
    
        // 10.- Verificamos que no haya un punto y coma después del do
        if (skipWhiteSpace(datos, i) && matchOperator(datos, i, ";")) {
            cout << "ERROR: Hay un error en el bucle for" << endl;
            return false;
        }
    
        // 11.- Verificamos que el cuerpo del bucle esté dentro de un bloque "begin ... end"
        if (!skipWhiteSpace(datos, i) || !matchKeyword(datos, i, "begin")) {
            cout << "ERROR: Hay un error en el bucle for" << endl;
            return false;
        }
    
        // // Buscamos el "end" que cierra el bloque
        // while (i < datos.length() && !matchKeyword(datos, i, "end")) {
        //     i++;
        // }
        // if (i >= datos.length()) {
        //     cout << "ERROR: Falta 'end' para cerrar el cuerpo del bucle 'for'." << endl;
        //     return false;
        // }
        return true;
    }
    // Extraemos el bloque del bucle for por que aja, cosas
    string extractBlock(const string& datos) {
        size_t start = datos.find("for");
        if (start == string::npos) {
            return ""; // No hay for
        }

        size_t end = start;
        int count = 0; // Contador para los bloques begin/end

        while (end < datos.length()) {
            if (datos.substr(end, 5) == "begin") {
                count++;
            } else if (datos.substr(end, 3) == "end") {
                count--;
                if (count == 0) {
                    end += 3;
                    break;
                }
            }
            end++;
        }

        if (end >= datos.length()) {
            return ""; // No hay bloque 
        }
        return datos.substr(start, end - start);
    }

    bool skipWhiteSpace(const string& datos, size_t& i) {
        while (i < datos.length() && isspace(datos[i])) {
            i++;
        }
        return i < datos.length();
    };
    // coincide con palabra "x"
    bool matchKeyword(const string& datos, size_t& i, const string& keyword) {
        if (datos.substr(i, keyword.length()) == keyword) {
            i += keyword.length();
            return true;
        }
        return false;
    };

    bool isIdentifier(const string& datos, size_t& i) {
        if (i >= datos.length() || !isalpha(datos[i])) {
            return false;
        }
        i++;
        while (i < datos.length() && isalnum(datos[i])) {
            i++;
        }
        return true;
    };

    bool isNumber(const string& datos, size_t& i) {
        if (i >= datos.length() || !isdigit(datos[i])) {
            return false;
        }
        
        i++;
        while (i < datos.length() && isdigit(datos[i])) {
            i++;
        }
        return true;
    };
    
    bool matchOperator(const string& datos, size_t &i, const string& op) {
        if (datos.substr(i, op.length()) == op) {
            i += op.length();
            return true;
        } 
        return false;
    }

    bool ifAnalizer(const string& datos) {
        size_t i = 0;

        if (!skipWhiteSpace(datos, i) || !matchKeyword(datos, i, "if")) {
            cout << "ERROR: Hay un error en el if (el if)" << endl;
            return false;
        }
        // Condicion
        if (!skipWhiteSpace(datos, i) || !isIdentifier(datos, i)) {
            cout << "ERROR: Hay un error en el if (la condicion)" << endl;
            return false;
        }

        if (!skipWhiteSpace(datos, i) || !matchKeyword(datos, i, "then")) {
            cout << "ERROR: Hay un error en el if (no hay then)" << endl;
            return false;
        }

        if (!skipWhiteSpace(datos, i) || !matchKeyword(datos, i, "begin")){
            cout << "ERROR: Hay un error en el if (no hay begin)" << endl;
            return false;
        }

        if (!skipWhiteSpace(datos, i) || !isBlock(datos, i)) {
            cout << "ERROR: Hay un error en el if (mal bloque)" << endl;
            return false;
        }
        

        if (!skipWhiteSpace(datos, i) || !matchKeyword(datos, i, "else")) {
            if (!skipWhiteSpace(datos, i) || !isBlock(datos, i)) {
                cout << "ERROR: Hay un error en el if (el bloque dentro del else)" << endl;
                return false;
            }
        }
        // if (!skipWhiteSpace(datos, i) || !matchKeyword(datos, i, "end")){
        //     cout << "ERROR: Hay un error en el if (no hay end)" << endl;
        //     return false;
        // }
        //cout << "Todo esta bien en el if \n";
        return true;
    }

    bool isBlock(const string& datos, size_t& i) {
        if (matchKeyword(datos, i, "begin")) {
            int count = 1;
            while (i < datos.length() && count > 0) {
                if (matchKeyword(datos, i, "begin")) {
                    count++;
                } else if (matchKeyword(datos, i, "end")) {
                    count--;
                } else {
                    i++;
                }
            }
            return count == 0;
        } else {
            while (i < datos.length() && datos[i] != ';') {
                i++;
            }
            return true;
        }
    } 

    
};

