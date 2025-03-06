#include "sintactico.h"
#include "verificaciones.h"
Syntax::Syntax() {}

void Syntax::getSyntax(vector<string> &datos) {

    std::ios::sync_with_stdio(false);

    if (datos.empty() || datos[0] != identifiers[6]) {
        cout << "ERROR: Falta o esta escrito mal la palabra 'program'";
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

void Syntax::getSyntaxBegin(vector<string> &datos, vector<string> &symbols, int &initialNumber) {
    bool final = true;
    while (final) {
        if (datos[initialNumber] == "end") { 
            if (initialNumber + 1 < datos.size()) {
                if (datos[initialNumber + 1] == ".") {
                    cout << endl << "Codigo terminado";
                    return;
                } else if (datos[initialNumber + 1] != ";") {
                    cout << "Begin-End mal declarado" << endl;
                    return;
                } else {
                    cout << "Begin-End terminado" << endl;
                    return;
                }
            }
        } 
        else if (datos[initialNumber] == "begin") {
            initialNumber++;
            getSyntaxBegin(datos, symbols, initialNumber);
        }
        else if (datos[initialNumber] == "if") {
            //cout << "Analizando el if" << endl;
            int result = ifAnalizer(datos, initialNumber);
            if (result == -1) return; 
            initialNumber = result;
        }
        else if (datos[initialNumber] == "for") {
            //cout << "Analizando el for" << endl;
            int result = forAnalizer(datos, initialNumber);
            if (result == -1) return; 
            initialNumber = result;
        }
        
        else {
            int finalNumber = getFinalNumber(initialNumber, datos);
            
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

            for (int j = initialNumber; j <= finalNumber; j++) {
                if (datos[j] == reserved[6]) {	    
                    int temp_readln = j;
                    getSyntaxreadln(datos, symbols, temp_readln);
                    break;
                }
            }
            initialNumber = finalNumber + 2; 
        }
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

int Syntax::forAnalizer(vector<string>& datos, int &i) {
    //size_t i = 0;

    // 1.- Check for "for"
    if (i >= datos.size() || datos[i] != "for") {
        cout << "ERROR: Hay un error en el bucle for (no hay for)" << endl;
        return -1;
    }    
    i++;

    // 2.- Check for identifier
    if (!Check::isIdentifier(datos[i])) {
        cout << "ERROR: Hay un error en el bucle for (no hay identificador)" << endl;
        return -1;
    }
    i++;
        
    // 3.- Check for assignment operator ":="
    if (i+1 >= datos.size() || datos[i] != ":=") {
        cout << "ERROR: Hay un error en el bucle for (no hay :=)" << endl;
        return -1;
    }
    i++;
    
    // 4.- Check for initial value (number)
    if (!Check::isNumber(datos[i])) {
        cout << "ERROR: Hay un error en el bucle for (valor inicial no es un numero)" << endl;
        cout << datos[i];
        return -1;
    }
    i++;
    
    // 5.- Check for "to" or "downto"
    if (i >= datos.size() || (datos[i] != "to" && datos[i] != "downto")) {
        cout << "Hay un error en le bulcer for (no hay downto/to)" << endl;
        return -1;
    }
    i++;

    // 6.- Check for final value (number)
    if (!Check::isNumber(datos[i])) {
        cout << "ERROR: Hay un error en el bucle for (valor final no es un numero)" << endl;
        return -1;
    }
    i++;
    
    // 7.- Check for "do"
    if (i >= datos.size() || datos[i] != "do") {
        cout << "ERROR: Hay un error en el bucle for (no hay do)" << endl;
        return -1;
    }
    i++;
    
    
    // Return the index where the analysis ends
    // cout << "Analysis of 'for' statement ended at index: " << i << endl;
    // cout << "Last token analyzed: " << datos[i - 1] << endl;
    return i;
}

int Syntax::ifAnalizer(vector<string>& datos, int &i) {
    //size_t i = 0;

    if (i >= datos.size() || datos[i] != "if") {
        cout << "ERROR: Hay un error en el if (no hay if)" << endl;
        return -1;
    }
    i++;
    if (datos[i] == "(") {
        i++;
    }

    while (i < datos.size() && datos[i] != ")" && datos[i] != "then") {
        if (!Check::isIdentifier(datos[i]) &&
            !isOperator(datos[i]) &&
            !Check::isNumber(datos[i])) {
                cout << "la condicion dentro del if esta mal" << endl;
                return -1;
            }
            i++;
    }
    if (i >= datos.size() || datos[i] == ")") {
        i++;
    }

    // 3.- Check for "then"
    if (i >= datos.size() || datos[i] != "then") {
        cout << "ERROR: Hay un error en el if (no hay then)" << endl;
        return -1;
    }
    i++;

    if (i < datos.size() && datos[i] != ";") {
        if (datos[i] == "if") {
            int result = ifAnalizer(datos, i);
            if (result == -1) return -1;
            i = result;
        } else if (datos[i] == "begin"){
            i++;
            getSyntaxBegin(datos, symbols, i);
        } else {
            int final = getFinalNumber(i, datos);
            i = final + 1;
        } 
    }
    
     //cout << "Analysis of 'if' statement ended at index: " << i << endl;
     //cout << "Last token analyzed: " << datos[i - 1] << endl;
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

string getString(vector<string> datos, int &initialNumber, int finalNumber) {

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

void Syntax::getSyntaxwriteln(vector<string> &datos, vector<string> &symbols, int initial) {
    unsigned int final = getFinalNumber(initial, datos);
    try
    {
        initial++;
        vector<string> something;

        if (datos[initial] == symbols[2]) {
            something.push_back(datos[initial]);
            initial++;
        } else {
            cout << "ERROR: Falta un parentesis izquierdo" << endl;
            exit(1);
        }

        while (initial != final)
        {
            bool flag = true;
            if (flag)
            {
                if (datos[initial] == "\"") {
                    string value = getString(datos, initial, final);
                    something.push_back(value);
                    flag = false;
    
                    if (datos[initial+1] == "\"")
                    {
                        cout << "ERROR: NO PUEDEN EXISTIR 2 STRINGS PEGADOS.";
                        exit(1);
                    }
                    // si  
                }
            }
            if (flag)
            {
                if (datos[initial] == symbols[9])
                {
                    if ((datos[initial-1] == symbols[2]) || (datos[initial+1] == symbols[3]))
                    {
                        cout << "ERROR: EL MAS DEBE TENER VARIABLES O STRINGS ENTRE ELLA";
                        exit(1);
                    } else if ((datos[initial+1] == symbols[9]) || (datos[initial-1] == symbols[9]))
                    {
                        cout << "ERROR: NO PUEDE HABER MAS DE 2 + JUNTOS.";
                        exit(1);
                    } else
                    {
                        something.push_back(datos[initial]);
                        flag = false;
                    }
                }
            }
            if (flag)
            {
                for (int i = 0; i < variables.size(); i++)
                {

                    if (datos[initial] == variables[i])
                    {
                        something.push_back(datos[initial]);
                        flag = true;
                        break;
                    } else
                    {
                        flag = false;
                    }
                    
                }
                if (!flag)
                {
                    cout << "ERROR: LO INGRESADO NO ES VARIABLE O NO HAY NADA." << endl;
                    exit(1);
                }
            }
            initial++;
        }
        

        if ((initial == final) && (datos[initial] == symbols[3])) {
            something.push_back(datos[initial]);
        } else {
            cout << "ERROR: Falta un parentesis derecho" << endl;
            exit(1);
        }

        /*for (int i = 0; i < something.size(); i++)
        {
            cout << something[i] << " ";
        }*/
        
    }
    catch(const std::exception& e)
    {
        std::cerr << e.what() << '\n';
    }
}

void Syntax::getSyntaxreadln(vector<string> &datos, vector<string> &symbols, int initial) { 
    int final = getFinalNumber(initial, datos);
    initial++;
    vector<string> comforting;

    if (datos[initial] == symbols[2]) {

        comforting.push_back(datos[initial]);
        initial++;

    } else {

        cout << "ERROR: Falta un parentesis izquierdo" << endl;
        exit(1);

    }

    bool flag = true;
    for (int i = 0; i < variables.size(); i++)
    {

        if (datos[initial] == variables[i])
        {
            comforting.push_back(datos[initial]);
            flag = true;
            initial++;
            break;
        } else {
            flag = false;
        }
    }

    if (!flag)
    {
        cout << "ERROR: LO INGRESADO NO ES VARIABLE O NO HAY NADA. " << endl;
        exit(1);
    }
    cout << datos[initial] << " and " << flag;
    if ((initial == final) && (datos[initial] == symbols[3])) {
        comforting.push_back(datos[initial]);
    } else if ((datos[initial] != symbols[3]) && (flag))
    {
        
        cout << "ERROR: SOLO DEBE HABER UNA VARIABLE. ";
        exit(1);

    } else {
        cout << "ERROR: Falta un parentesis derecho" << endl;
        exit(1);
    }

    /*for (int i = 0; i < comforting.size(); i++)
    {
        cout << comforting[i] << " ";
    }*/
}