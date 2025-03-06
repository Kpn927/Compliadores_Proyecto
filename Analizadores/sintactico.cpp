#include "sintactico.h"
#include "verificaciones.h"
Syntax::Syntax(){};

void Syntax::getSyntax(vector<string> &datos) {

    std::ios::sync_with_stdio(false);

    if (datos.empty() || datos[0] != identifiers[6]) {
        cout << "ERROR: Falta o esta escrito mal la palabra program'";
        return;
    }
    
    int initialNumber = 3;

    if (datos[initialNumber] == identifiers[0] || (datos[initialNumber] != identifiers[3] && datos[initialNumber] != identifiers[4])) {
        initialNumber++;
        while(datos[initialNumber] != identifiers[4]){
            getSyntaxVar(datos, symbols, initialNumber);
        }
    }

    if (datos[initialNumber] == identifiers[4]) {
        initialNumber++;
        getSyntaxBegin(datos, symbols, initialNumber);
    }
};

int Syntax::getFinalNumber(int finalNumber, vector<string> &datos){
    while (finalNumber < datos.size() && datos[finalNumber + 1] != ";") {
        finalNumber++;
    }
    return finalNumber;
}

void Syntax::getSyntaxBegin(vector<string> &datos, vector<string> &symbols, int &initialNumber){
    bool final = true;
    int finalNumber;
    while (final){
        finalNumber = getFinalNumber(initialNumber, datos);

        if (datos[initialNumber] == "end") {
            if (datos[initialNumber + 1] == "."){
                cout << endl << "Codigo terminado";
                return;
            }else if (datos[initialNumber + 1] != ";"){
                cout<<"Begin-End mal declarado"<<endl;
                return;
            } else{
                cout<<"Begin-End terminado"<<endl;
                return;
            }
        } else if (datos[initialNumber] == "begin") {
            initialNumber++;
            getSyntaxBegin(datos, symbols, initialNumber);
        }
        int i = 0;
        while (i < variables.size()) {
            if (datos[initialNumber] == variables[i]) {
                getSyntaxAsignation(datos, symbols, initialNumber);
                break;
            }
            i++;
        }

        for (int j = initialNumber; j <= finalNumber; j++) {
            if (datos[j] == reserved[5]) {	
                int temp_writeln = j;
                getSyntaxwriteln(datos, symbols, temp_writeln);
                break;
            }
        }
        initialNumber = finalNumber + 2;
    }

}

void Syntax::getSyntaxVar(vector<string> &datos, vector<string> &symbols, int &initial) {
    int final = getFinalNumber(initial, datos);
    Tree<string> arbolvar;
    
    if (datos[initial] == identifiers[0])
    {
        initial++;
    }
    
    arbolvar.insertarOrdenado("",0);

    recursiveTree(arbolvar, datos, initial, final);

    //arbolvar.orden(arbolvar.getRaiz(), true);

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
    
        /* SIGUIENTE LINEA. */
        initial = final + 2;
}

void Syntax::getSyntaxAsignation(vector<string> &datos, vector<string> &symbols, int initial) {
    int final = getFinalNumber(initial, datos);
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
    if (!Check::matchKeyword(datos, i, {"f", "o", "r"})) {
        cout << "ERROR: Hay un error en el bucle for (no hay for)" << endl;
        return -1;
    }    
    // 2.- Check for identifier
    if (!Check::isIdentifier(datos, i)) {
        cout << "ERROR: Hay un error en el bucle for (no hay identificador)" << endl;
        return -1;
    }
        
    // 3.- Check for assignment operator ":="
    if (!Check::matchOperator(datos, i, {":", "="})) {
        cout << "ERROR: Hay un error en el bucle for (no hay :=)" << endl;
        return -1;
    }
    
    // 4.- Check for initial value (number)
    if (!Check::isNumber(datos, i)) {
        cout << "ERROR: Hay un error en el bucle for (valor inicial no es un numero)" << endl;
        return -1;
    }
    
    // 5.- Check for "to" or "downto"
    bool isTo = Check::matchKeyword(datos, i, {"t", "o"});
    bool isDownTo = Check::matchKeyword(datos, i, {"d", "o", "w", "n", "t", "o"});

    if (!isTo && !isDownTo) {
        cout << "ERROR: Hay un error en el bucle for (no hay to/downto)" << endl;
        return -1;
    }
    
    // 6.- Check for final value (number)
    if (!Check::isNumber(datos, i)) {
        cout << "ERROR: Hay un error en el bucle for (valor final no es un numero)" << endl;
        return -1;
    }
    
    // 7.- Check for "do"
    if (!Check::matchKeyword(datos, i, {"d", "o"})) {
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

    if (!Check::matchKeyword(datos, i, {"i", "f"})) {
        cout << "ERROR: Hay un error en el if (no hay if)" << endl;
        return -1;
    }

    // condition (identifier)
    if (!Check::isIdentifier(datos, i)) {
        cout << "ERROR: Hay un error en el if (no hay identificador)" << endl;
        return -1;
    }

    // 3.- Check for "then"
    if (!Check::matchKeyword(datos, i, {"t", "h", "e", "n"})) {
        cout << "ERROR: Hay un error en el if (no hay then)" << endl;
        return -1;
    }
    
    cout << "Analysis of 'if' statement ended at index: " << i << endl;
    cout << "Last token analyzed: " << datos[i - 1] << endl;
    return i;
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

string comillasagrupadoras(vector<string> datos, int &initialNumber, int finalNumber) {

    if (initialNumber < 0 || initialNumber > finalNumber || finalNumber >= datos.size()) {
        cout << "ERROR: Índices fuera de rango." << endl;
        exit(1);
    }

    string value = "";

    if (datos[initialNumber] == "\"") {
        value += datos[initialNumber];
        initialNumber++;

        while (initialNumber <= finalNumber && datos[initialNumber] != "\"") {
            value += datos[initialNumber];
            initialNumber++;
        }

        if (initialNumber <= finalNumber && datos[initialNumber] == "\"") {
            value += datos[initialNumber];
        } else {
            cout << "ERROR: Falta una comilla de cierre." << endl;
            exit(1);
        }
    } else {
        cout << "ERROR: No se encontró una comilla inicial." << endl;
        exit(1);
    }

    return value;
}


void Syntax::recursiveTreeWriteln(Node<string>* actual, Tree<string>& padre, vector<string>& datos, int initialNumber, int finalNumber) {
    if (initialNumber > finalNumber || actual == nullptr) {
        return; // Condición base para detener la recursión
    }

    string value = "", temp_value = "";
    int contador = 0;

    // Primer ciclo para contar elementos
    for (int i = initialNumber; i < finalNumber; i++) {

        if (datos[i] == "\"") {
            temp_value = comillasagrupadoras(datos, i, finalNumber);
            contador++;
        }
        contador++;
    }
    contador--;

    cout << "CONTADOR: " << contador << endl;

    if (contador > 1) {
        padre.insertarOrdenado("", 1, actual);
        for (int i = initialNumber; i <= finalNumber; i++) {
            
            if (i > finalNumber) break; // Seguridad para evitar bucles infinitos

            if (datos[i] == "+") {
                padre.insertarOrdenado(datos[i], 2, actual);
            } else if (datos[i] == "\"") {
                value = comillasagrupadoras(datos, i, finalNumber);
                padre.insertarOrdenado(value, 0, actual);
            }
        }
    } else {
        padre.insertarOrdenado(temp_value, 1, actual);
    }
}

void Syntax::recursiveTreeWriteln(Tree<string>& padre, vector<string> &datos, int initialNumber, int finalNumber) {
    recursiveTreeWriteln(padre.getRaiz(), padre, datos, initialNumber, finalNumber);
}

void Syntax::getSyntaxwriteln(vector<string> &datos, vector<string> &symbols, int initial) {
    unsigned int final = getFinalNumber(initial, datos);
    try
    {
        initial++;

        Tree<string> arbolwriteln;

        arbolwriteln.insertarOrdenado("" , 0);

        if (datos[initial] == "(") {
            arbolwriteln.insertarOrdenado(datos[initial], 0);
            initial++;
        } else {
            cout << "ERROR: Falta un parentesis izquierdo" << endl;
            exit(1);
        }

        if (datos[final] == ")") {
            arbolwriteln.insertarOrdenado(datos[final], 2);
        } else {
            cout << "ERROR: Falta un parentesis derecho" << endl;
            exit(1);
        }

        recursiveTreeWriteln(arbolwriteln, datos, initial, final);

        Node<string> *a = arbolwriteln.getRaiz()->getCenter();

        arbolwriteln.orden(arbolwriteln.getRaiz(), true);

    }
    catch(const std::exception& e)
    {
        std::cerr << e.what() << '\n';
    }
}
