#include "sintactico.h"
Syntax::Syntax(){};

void Syntax::getSyntax(vector<string> &datos) {

    std::ios::sync_with_stdio(false);

    if (datos.empty() || datos[0] != identifiers[6]) {
        cout << "ERROR: Falta o esta escrito mal la palabra program'";
        return;
    }
    
    int finalNumber = 3, initialNumber;
    bool final = true, flag_begin = true;

    while (final) {
        /* MANEJO DE NUMEROS INICIALES Y FINALES */   
        initialNumber = finalNumber;
        
        if (datos[initialNumber] == "end" && datos[initialNumber + 1] == ".") {
            cout << endl << "Codigo terminado";
            return;
        }
        
        if (datos[initialNumber] == identifiers[4]) {
            // getsyntaxbegin
            if (flag_begin) {
                flag_begin = false;
            }
        }

        while (finalNumber < datos.size() && datos[finalNumber + 1] != ";") {
            finalNumber++;
        }
        
        if (flag_begin)
        {
            if (datos[initialNumber] == identifiers[0] || (datos[initialNumber] != identifiers[3] && datos[initialNumber] != identifiers[4])) {
                getSyntaxVar(datos, symbols, initialNumber, finalNumber);
            }
        }

        int i = 0;

        while (i < variables.size()) {
            if (datos[initialNumber] == variables[i]) {
                getSyntaxAsignation(datos, symbols, initialNumber, finalNumber);
                break;
            }
            i++;
        }

        /* SIGUIENTE LINEA. */
        initialNumber = finalNumber + 2;
        finalNumber = initialNumber;
    }
};

void Syntax::getSyntaxBegin(vector<string> &datos, vector<string> &symbols, int initialNumber, int finalNumber){
    bool final = true;
    while (final){
        
    }

}

void Syntax::getSyntaxVar(vector<string> &datos, vector<string> &symbols, int initial, int final) {
        
    Tree<string> arbolvar;

    if (datos[initial] == identifiers[0])
    {
        initial++;
    }
    
    arbolvar.insertarOrdenado("",0);

    recursiveTree(arbolvar, datos, initial, final);

    arbolvar.orden(arbolvar.getRaiz(), true);

    Node<string> *a = arbolvar.getRaiz()->getLeft();
    Node<string> *b = arbolvar.getRaiz()->getRight();

    if (a->getValue() != "")
    {
        for (int i = 0; i < variables.size(); i++)
        {
            if (variables[i] == a->getValue())
            {
                cout << "ERROR: VARIABLE DOBLEMENTE DECLARADA." << endl;
                exit(1);
            }                    
        }
        variables.push_back(a->getValue());
    }
    

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
            cout << endl << "ERROR: EN LA DECLARACION DEL DATATYPE VARIABLE. " << endl;
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
                for (int i = 0; i < variables.size(); i++)
                {
                    if ((variables[i] == a->getLeft()->getValue()) || (variables[i] == a->getRight()->getValue()))
                    {
                        flag = false;
                        cout << "ERROR: VARIABLE YA DECLARADA." << endl;
                        exit(1);
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

void Syntax::getSyntaxAsignation(vector<string> &datos, vector<string> &symbols, int initial, int final) {
    Tree<string> arbolvar;
    
    arbolvar.insertarOrdenado("",0);
    
    recursiveTree(arbolvar, datos, initial, final);
    
    // arbolvar.orden(arbolvar.getRaiz(), true);

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
    }
    catch(const std::exception& e)
    {
        std::cerr << e.what() << '\n';
    }
}

// Faltaria la logica del // para ver como terminariamos la linea
void Syntax::CommentAnalizer(const vector<string>& datos) {
    size_t i = 0;
    bool multiLine = false;

    while (i < datos.size()) {
        if (!multiLine && i + 1 < datos.size() && datos[i] == "/" && datos[i + 1] == "/") { // Single-line comment
            i += 2; // Skip "//"
            while (i < datos.size() && datos[i] != "\n") { // Go to the end of the line
                i++;
            }
            if (i < datos.size() && datos[i] == "\n") { // Skip the newline
                i++;
            }
        } else if (!multiLine && i + 1 < datos.size() && datos[i] == "(" && datos[i + 1] == "*") { // Start of multi-line comment
            multiLine = true;
            i += 2;
        } else if (multiLine && i + 1 < datos.size() && datos[i] == "*" && datos[i + 1] == ")") { // End of multi-line comment
            multiLine = false;
            i += 2;
        } else if (multiLine && i == datos.size() - 1) {
            cout << "ERROR: Comentario Multilineal no cerrado" << endl;
            break;
        } else {
            i++;
        }
    }
}

int Syntax::forAnalizer(const vector<string>& datos) {
    size_t i = 0;

    // 1.- Check for "for"
    if (!matchKeyword(datos, i, {"f", "o", "r"})) {
        cout << "ERROR: Hay un error en el bucle for (no hay for)" << endl;
        return -1;
    }    
    // 2.- Check for identifier
    if (!isIdentifier(datos, i)) {
        cout << "ERROR: Hay un error en el bucle for (no hay identificador)" << endl;
        return -1;
    }
        
    // 3.- Check for assignment operator ":="
    if (!matchOperator(datos, i, {":", "="})) {
        cout << "ERROR: Hay un error en el bucle for (no hay :=)" << endl;
        return -1;
    }
    
    // 4.- Check for initial value (number)
    if (!isNumber(datos, i)) {
        cout << "ERROR: Hay un error en el bucle for (valor inicial no es un numero)" << endl;
        return -1;
    }
    
    // 5.- Check for "to" or "downto"
    bool isTo = matchKeyword(datos, i, {"t", "o"});
    bool isDownTo = matchKeyword(datos, i, {"d", "o", "w", "n", "t", "o"});

    if (!isTo && !isDownTo) {
        cout << "ERROR: Hay un error en el bucle for (no hay to/downto)" << endl;
        return -1;
    }
    
    // 6.- Check for final value (number)
    if (!isNumber(datos, i)) {
        cout << "ERROR: Hay un error en el bucle for (valor final no es un numero)" << endl;
        return -1;
    }
    
    // 7.- Check for "do"
    if (!matchKeyword(datos, i, {"d", "o"})) {
        cout << "ERROR: Hay un error en el bucle for (no hay do)" << endl;
        return -1;
    }
    
    // Return the index where the analysis ends
    cout << "Analysis of 'for' statement ended at index: " << i << endl;
    cout << "Last token analyzed: " << datos[i - 1] << endl;
    return i;
}

int Syntax::ifAnalizer(const vector<string>& datos) {
    size_t i = 0;

    if (!matchKeyword(datos, i, {"i", "f"})) {
        cout << "ERROR: Hay un error en el if (no hay if)" << endl;
        return -1;
    }

    // condition (identifier)
    if (!isIdentifier(datos, i)) {
        cout << "ERROR: Hay un error en el if (no hay identificador)" << endl;
        return -1;
    }

    // 3.- Check for "then"
    if (!matchKeyword(datos, i, {"t", "h", "e", "n"})) {
        cout << "ERROR: Hay un error en el if (no hay then)" << endl;
        return -1;
    }
    
    cout << "Analysis of 'if' statement ended at index: " << i << endl;
    cout << "Last token analyzed: " << datos[i - 1] << endl;
    return i;
}

bool Syntax::matchKeyword(const vector<string>& datos, size_t& i, const vector<string>& keyword) {
    if (i + keyword.size() <= datos.size()) {
        for (size_t j = 0; j < keyword.size(); ++j) {
            if (datos[i + j] != keyword[j]) {
                return false;
            }
        }
        i += keyword.size();
        return true;
    }
    return false;
}

bool Syntax::isIdentifier(const vector<string>& datos, size_t& i) {
    if (i >= datos.size() || datos[i].empty() || !isalpha(datos[i][0])) {
        return false;
    }
    i++;
    return true;
}

bool Syntax::isNumber(const vector<string>& datos, size_t& i) {
    if (i >= datos.size() || datos[i].empty()) {
        return false;
    }
    for (char c : datos[i]) {
        if (!isdigit(c)) {
            return false;
        }
    }
    i++;
    return true;
}

bool Syntax::isNumber(string word){
    for (char c : word) {
        if (!isdigit(c)) {
            return false;
        }
    }
    return true;
}

bool Syntax::matchOperator(const vector<string>& datos, size_t& i, const vector<string>& op) {
    if (i + op.size() <= datos.size()) {
        for (size_t j = 0; j < op.size(); ++j) {
            if (datos[i + j] != op[j]) {
                return false;
            }
        }
        i += op.size();
        return true;
    }
    return false;
}

void Syntax::recursiveTree(Tree<string>&padre, vector<string> &datos, int initialNumber, int finalNumber){
    recursiveTree(padre.getRaiz(), padre, symbols, datos, initialNumber, finalNumber);
    return;
}

void Syntax::recursiveTree(Node<string>* actual, Tree<string>&padre, vector<string> &symbols, vector<string> &datos, int initialNumber, int finalNumber){
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