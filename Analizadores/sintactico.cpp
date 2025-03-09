#include "sintactico.h"
#include "verificaciones.h"
Syntax::Syntax() {}

void Syntax::getSyntax(vector<string> &datos) {

    std::ios::sync_with_stdio(false);
    try{
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
            getSyntaxBegin(datos, symbols, initialNumber, true);
        }
    } catch (const invalid_argument& e){
        cerr<<"Error: "<< e.what()<<endl;
    }
};

int Syntax::getFinalNumber(int finalNumber, vector<string> &datos){
    while (finalNumber < datos.size() && datos[finalNumber + 1] != ";") {
        finalNumber++;
    }
    return finalNumber;
}

void Syntax::getSyntaxBegin(vector<string> &datos, vector<string> &symbols, int &initialNumber, bool isPrincipal) {
    bool final = true;
    while (final) {
        if (datos[initialNumber] == "end") { 
            if (initialNumber + 1 < datos.size()) {
                if (datos[initialNumber + 1] == ".") {
                    cout << endl << "Codigo terminado";
                    return;
                } else if (datos[initialNumber + 1] != ";") {
                    throw invalid_argument("Begin-End mal declarado");
                } else {
                    cout << "Begin-End terminado" << endl;
                    initialNumber++;
                    if (!isPrincipal) return;
                }
            }
        } 
        else if (datos[initialNumber] == "begin") {
            initialNumber++;
            getSyntaxBegin(datos, symbols, initialNumber, false);
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
        }else if (datos[initialNumber] == ";") initialNumber++;
        else {
            int finalNumber = getFinalNumber(initialNumber, datos);
            int i = 0;
            while (i < variables.size()) {
                if (datos[initialNumber] == variables[i].name) {
                    getSyntaxAsignation(datos, symbols, initialNumber);
                    i = variables.size();
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
    Variables newVariable;

    if (a->getValue() != "")
    {
        for (int i = 0; i < variables.size(); i++)
        {
            if (variables[i].name == a->getValue())
            {
                cout<<"tumama";
                throw invalid_argument("Variable doblemente declarada");
            }                    
        }
        newVariable.name = a->getValue();
        variables.push_back(newVariable);
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
        if (flag)throw invalid_argument("ERROR: EN LA DECLARACION DEL DATATYPE VARIABLE.");

    }
    catch(const std::exception& e)
    {
        throw invalid_argument(e.what());
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
                    if ((variables[i].name == a->getLeft()->getValue()) || (variables[i].name == a->getRight()->getValue()))
                    {
                        flag = false;
                        throw invalid_argument("VARIABLE YA DECLARADA.");
                    }                    
                }
            }

            if (flag)  
            {
                newVariable.name = a->getLeft()->getValue();
                variables.push_back(newVariable);
                a = a->getRight();

                if (a->getValue() != "")
                {   
                    newVariable.name = a->getValue();
                    variables.push_back(newVariable);
                }
                
            } else {
                throw invalid_argument("ARBOL HAY UNA PALABRA RESERVADA");
            }    
        }
    }
    catch(const std::exception& e)
    {
        throw invalid_argument( e.what());
    }
    
        /* SIGUIENTE LINEA. */
        initial = final + 2;
}

string Syntax::phraseAnalizer(Node<string> *nodo){
    if (nodo->getRight()==NULL){
        return nodo->getValue();
    }

    if (nodo->getCenter()->getCenter() != NULL){
        string value = phraseAnalizer(nodo->getCenter());
        return value;
    }else {
        if (nodo->getRight()->getRight() != NULL){
            string value = phraseAnalizer(nodo->getRight());
            nodo->getRight()->setValue(value);
        }
        if (nodo->getLeft()->getLeft() != NULL){
            string value = phraseAnalizer(nodo->getLeft());
            nodo->getLeft()->setValue(value);
        }
    }
    int i = 0;
    float *left, *right , result;
    if(Check::isNumber(nodo->getLeft()->getValue())){
         left = new float;
        *left =std::stof(nodo->getLeft()->getValue());
    } 
    if(Check::isNumber(nodo->getRight()->getValue())){
        right = new float;
        *right =std::stof(nodo->getRight()->getValue());
   }  
    while(i<variables.size()){

        if(variables[i].name == nodo->getLeft()->getValue() && !variables[i].value.empty()){
            if (Check::isNumber(variables[i].value)){
                left = new float;
                *left =(float) std::stof(variables[i].value);
            }
        }
        if(variables[i].name == nodo->getRight()->getValue() && !variables[i].value.empty()){
            if (Check::isNumber(variables[i].value)){
                right = new float;
                *right =(float) std::stof(variables[i].value);
            }
        }
        i++;
    }
    if(left!=NULL && right!=NULL){
        if (nodo->getCenter()->getValue() == "+") result = *left + *right;
        if (nodo->getCenter()->getValue() == "-") result = *left - *right;
        if (nodo->getCenter()->getValue() == "*") result = *left * *right;
        if (nodo->getCenter()->getValue() == "/") result = *left / *right;
        return (string)std::to_string(result);
    } else{
        return nodo->getLeft()->getValue() + " " + nodo->getCenter()->getValue() + " " + nodo->getRight()->getValue();
    }
}

void Syntax::getSyntaxAsignation(vector<string> &datos, vector<string> &symbols, int &initial) {
    int final = getFinalNumber(initial, datos);
    Tree<string> arbolvar;
    
    arbolvar.insertarOrdenado("",0);
    
    recursiveTree(arbolvar, datos, initial, final);
    
    arbolvar.orden(arbolvar.getRaiz(), true);

    Node<string> *a = arbolvar.getRaiz()->getRight();
    Node<string> *b = arbolvar.getRaiz()->getLeft();

    for (int i = 0; i < variables.size(); i++){
        if (b->getValue() == variables[i].name) throw invalid_argument("ERROR: NO PUSISTE UNA FOKIN VARIABLE");
    }
    
    if (!Check::verifyAsignation(arbolvar.getRaiz())) throw invalid_argument("NO PUSISTE BIEN LA ASIGNACION DE VARIABLE");

    initial = final+2;
    string value = (string) phraseAnalizer(arbolvar.getRaiz()->getRight());
    int i = 0;
    while(i<variables.size()){
        if(arbolvar.getRaiz()->getLeft()->getValue() == variables[i].name) variables[i].value = value;
        i++;
    }
    cout<<"[resultado: "<<value<<"]"<<endl;
    return;
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
        throw invalid_argument("Hay un error en el bucle for (no hay for)");
    }    
    i++;

    // 2.- Check for identifier
    if (!Check::isIdentifier(datos[i])) {
        throw invalid_argument("Hay un error en el bucle for (no hay identificador)");
    }
    i++;
        
    // 3.- Check for assignment operator ":="
    if (i+1 >= datos.size() || datos[i] != ":=") throw invalid_argument("Hay un error en el bucle for (no hay :=)");
    i++;
    
    // 4.- Check for initial value (number)
    if (!Check::isNumber(datos[i])) throw invalid_argument("Hay un error en el bucle for (valor inicial no es un numero): "+datos[i]);
    i++;
    
    // 5.- Check for "to" or "downto"
    if (i >= datos.size() || (datos[i] != "to" && datos[i] != "downto")) throw invalid_argument("Hay un error en el bucle for (no hay downto/to)");
    i++;

    // 6.- Check for final value (number)
    if (!Check::isNumber(datos[i])) throw invalid_argument("Hay un error en el bucle for (valor final no es un numero)");
    i++;
    
    // 7.- Check for "do"
    if (i >= datos.size() || datos[i] != "do") throw invalid_argument("Hay un error en el bucle for (no hay do)");
    i++;
    
    
    // Return the index where the analysis ends
    // cout << "Analysis of 'for' statement ended at index: " << i << endl;
    // cout << "Last token analyzed: " << datos[i - 1] << endl;
    return i;
}

int Syntax::ifAnalizer(vector<string>& datos, int &i) {
    //size_t i = 0;

    if (i >= datos.size() || datos[i] != "if") throw invalid_argument("Hay un error en el if (no hay if)");
    i++;
    if (datos[i] == "(") {
        i++;
    }

    while (i < datos.size() && datos[i] != ")" && datos[i] != "then") {
        if (!Check::isIdentifier(datos[i]) &&
            !isOperator(datos[i]) &&
            !Check::isNumber(datos[i])) throw invalid_argument("la condicion dentro del if esta mal");
            i++;
    }
    if (i >= datos.size() || datos[i] == ")") {
        i++;
    }

    // 3.- Check for "then"
    if (i >= datos.size() || datos[i] != "then") throw invalid_argument("Hay un error en el if (no hay then)");
    i++;

    if (i < datos.size() && datos[i] != ";") {
        if (datos[i] == "begin") {
            i++;
            getSyntaxBegin(datos, symbols, i, false);
            
        } else if (datos[i] == "if"){
            int result = ifAnalizer(datos, i);
            if (result == -1) return -1;
            i = result;
        } else {
            for(const Variables var : variables) {
                if (datos[i] == var.name) {
                    getSyntaxAsignation(datos, symbols, i);
                    break;
                }
            }
            // int final = getFinalNumber(i, datos);
            // i = final + 1;
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
    int symbol, j, lastParentesis;
    if (actual==NULL) return;
    if (datos[initialNumber]=="("){
        lastParentesis=initialNumber+2;
        while(datos[lastParentesis]!=")"){
            lastParentesis++;
        }
        if (lastParentesis ==finalNumber){
            padre.insertarOrdenado(datos[initialNumber],0,actual);
            padre.insertarOrdenado(datos[lastParentesis],2,actual);
            if(finalNumber-initialNumber== 1) cout<<"NADA ENTRE LOS PARENTESIS"<<endl;
            else if(finalNumber-initialNumber== 2) padre.insertarOrdenado(datos[finalNumber],1,actual);
            else if(finalNumber-initialNumber> 2) {
                padre.insertarOrdenado("", 1, actual);
                recursiveTree(actual->getCenter(), padre, symbols, datos, initialNumber+1, finalNumber-1);
            }
            return;
        }
        else j = lastParentesis;
    }
    else j=initialNumber;
    for(int i = 0; i< symbols.size(); i++){
        for(int k = j; k<=finalNumber;k++){
            if (symbols[i]==datos[k]){
                padre.insertarOrdenado(datos[k], 1, actual);
                symbol=k;
                k= finalNumber+1;
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
            throw invalid_argument("Falta una comilla de cierre.");
        }
    } else {
        throw invalid_argument("No se encontró una comilla inicial");
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
            throw invalid_argument("Falta un parentesis izquierdo");
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
                        throw invalid_argument("NO PUEDEN EXISTIR 2 STRINGS PEGADOS.");

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
                        throw invalid_argument("EL MAS DEBE TENER VARIABLES O STRINGS ENTRE ELLA");

                    } else if ((datos[initial+1] == symbols[9]) || (datos[initial-1] == symbols[9]))
                    {
                        throw invalid_argument("NO PUEDE HABER MAS DE 2 + JUNTOS.");

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

                    if (datos[initial] == variables[i].name)
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
                    throw invalid_argument("LO INGRESADO NO ES VARIABLE O NO HAY NADA.");
                }
            }
            initial++;
        }
        

        if ((initial == final) && (datos[initial] == symbols[3])) {
            something.push_back(datos[initial]);
        } else {
            throw invalid_argument("Falta un parentesis derecho");
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

        throw invalid_argument("Falta un parentesis izquierdo");

    }

    bool flag = true;
    for (int i = 0; i < variables.size(); i++)
    {

        if (datos[initial] == variables[i].name)
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
        throw invalid_argument("LO INGRESADO NO ES VARIABLE O NO HAY NADA. ");
    }
    cout << datos[initial] << " and " << flag;
    if ((initial == final) && (datos[initial] == symbols[3])) {
        comforting.push_back(datos[initial]);
    } else if ((datos[initial] != symbols[3]) && (flag))
    {
        
        throw invalid_argument("SOLO DEBE HABER UNA VARIABLE.");

    } else {
        throw invalid_argument("Falta un parentesis derecho");
    }

    /*for (int i = 0; i < comforting.size(); i++)
    {
        cout << comforting[i] << " ";
    }*/
}