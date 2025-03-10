#include "sintactico.h"
#include "verificaciones.h"
Syntax::Syntax() {}

void Syntax::getSyntax(vector<string> &datos) {

    try{

        if (datos.empty() || datos[0] != identifiers[6]) throw invalid_argument("Falta o esta escrito mal la palabra 'program'");
        if (datos[2] != ";") throw invalid_argument("Falta ';' en declaración de program");
        if (Check::isReserved(datos[1])) throw invalid_argument("declaración de Program tiene una palabra reservada");
        reserved.push_back(datos[1]);

        int initialNumber = 3;
        
        if (datos[initialNumber] == identifiers[2]) {
            getSyntaxprocedure(datos, symbols, initialNumber);
        }
        
        if (datos[initialNumber] == identifiers[0] || (datos[initialNumber] != identifiers[3] && datos[initialNumber] != identifiers[4])) {
            initialNumber++;
            while(datos[initialNumber] != identifiers[4]){
                getSyntaxVar(datos, symbols, initialNumber);
            }
        }
    
        if (datos[initialNumber] == identifiers[4]) {
            initialNumber++;
            getSyntaxBegin(datos, symbols, initialNumber, true, false);
        }
    } catch (const invalid_argument& e){
        cerr<<"Error: "<< e.what()<<endl;
        exit(1);
    }
};

int Syntax::getFinalNumber(int finalNumber, vector<string> &datos){
    while (finalNumber < datos.size() && datos[finalNumber + 1] != ";") {
        finalNumber++;
    }
    return finalNumber;
}

void Syntax::getSyntaxBegin(vector<string> &datos, vector<string> &symbols, int &initialNumber, bool isPrincipal, bool isProcedure) {
    bool final = true;

    if (isProcedure) {
        if (datos[initialNumber] == identifiers[0] || (datos[initialNumber] != identifiers[3] && datos[initialNumber] != identifiers[4])) {
            initialNumber++;
            while(datos[initialNumber] != identifiers[4]){
                getSyntaxVar(datos, symbols, initialNumber);
            }
        }
        initialNumber++;
        isProcedure = false;
    }
    
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
                    final = false;
                    if (!isPrincipal) return;
                }
            }
        }
        else if (datos[initialNumber] == "begin") {
            initialNumber++;
            getSyntaxBegin(datos, symbols, initialNumber, false, false);
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
        else if (Check::isVariable(datos[initialNumber])) getSyntaxAsignation(datos, symbols, initialNumber);
        else if (datos[initialNumber]=="writeln") getSyntaxwriteln(datos, symbols, initialNumber);
        else if (datos[initialNumber]=="readln") getSyntaxreadln(datos, symbols, initialNumber);
        else {
            throw invalid_argument("Simbolo no reconocido");
        }
    }
}

void Syntax::getSyntaxVar(vector<string> &datos, vector<string> &symbols, int &initial) {
    int final = getFinalNumber(initial, datos);
    Tree<string> arbolvar;

    // Avanzar si es un identificador válido
    if (datos[initial] == identifiers[0]) {
        initial++;
    }

    arbolvar.insertarOrdenado("", 0);

    recursiveTree(arbolvar, datos, initial, final);

    Node<string> *a = arbolvar.getRaiz()->getLeft();
    Node<string> *b = arbolvar.getRaiz()->getRight();
    Variables newVariable;

    // Verificar si 'a' contiene un valor válido
    if (!a->getValue().empty()) {
        if (std::any_of(variables.begin(), variables.end(), [&](const Variables &var) { return var.name == a->getValue(); })) {
            throw invalid_argument("VARIABLE DOBLEMENTE DECLARADA.");
        }
        newVariable.name = a->getValue();
        variables.push_back(newVariable);
    }

    // Validar si el valor en 'b' es un tipo de dato válido
    if (std::none_of(dataType.begin(), dataType.end(), [&](const string &type) { return b->getValue() == type; })) {
        throw invalid_argument("EN LA DECLARACION DEL DATATYPE VARIABLE.");
    }

    // Validar variables y palabras reservadas en el árbol
    while (a->getRight() != nullptr) {
        if (isReservedOrInvalid(a->getLeft()->getValue()) || isReservedOrInvalid(a->getRight()->getValue())) {
            throw invalid_argument("ARBOL HAY UNA PALABRA RESERVADA");
        }

        if (std::any_of(variables.begin(), variables.end(), [&](const Variables &var) {
                return var.name == a->getLeft()->getValue() || var.name == a->getRight()->getValue();
            })) {
            throw invalid_argument("VARIABLE YA DECLARADA.");
        }

        newVariable.name = a->getLeft()->getValue();
        variables.push_back(newVariable);

        a = a->getRight();

        if (!a->getValue().empty()) {
            newVariable.name = a->getValue();
            variables.push_back(newVariable);
        }
    }

    // Avanzar a la siguiente línea
    initial = final + 2;
}

bool Syntax::isReservedOrInvalid(const string &value) {
    return std::any_of(reserved.begin(), reserved.end(), [&](const string &reservedWord) { return reservedWord == value; }) ||
           std::any_of(identifiers.begin(), identifiers.end(), [&](const string &identifier) { return identifier == value; }) ||
           std::any_of(dataType.begin(), dataType.end(), [&](const string &type) { return type == value; });
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

    if(!Check::isVariable(b->getValue())) throw invalid_argument("ERROR: NO PUSISTE UNA FOKIN VARIABLE");
    
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
            getSyntaxBegin(datos, symbols, i, false, false);
            
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

string getString(vector<string> datos, int &initialNumber, int &finalNumber) {

    string value = "";

    if (datos[initialNumber] == "\"") {
        value += datos[initialNumber];
        initialNumber++;

        while (initialNumber <= finalNumber && datos[initialNumber] != "\"") {
            value += datos[initialNumber];
            initialNumber++;
            if (datos[initialNumber] == "\"") {
                finalNumber = initialNumber;
                break;
            }
        }

        if (initialNumber <= finalNumber && datos[initialNumber] == "\"") {
            value += datos[initialNumber];
        } else {
            cout << "Falta una comilla de cierre." << endl;
            exit(1);
        }
    } else {
        cout << "No se encontró una comilla inicial" << endl;
    }

    return value;
}

void Syntax::getSyntaxwriteln(vector<string> &datos, vector<string> &symbols, int &initial) {
    int final = getFinalNumber(initial, datos);
    initial++;
    vector<string> something;

    if (datos[initial] == symbols[6]) {
        something.push_back(datos[initial]);
        initial++;
    } else {
        throw invalid_argument("ERROR: FALTA UN PARENTESIS IZQUIERDO.");
    }
    if (datos[initial] == symbols[7]) {
        throw invalid_argument("ERROR: NO HAY NADA EN EL WRITELN");
    }
        
    while (initial != final) {
            
        if (datos[initial] == "\"") {
            string value = getString(datos, initial, final);
            initial = final;
            if (datos[initial] == "\"") {
                initial++;
            }
            final = getFinalNumber(initial, datos);
            something.push_back(value);
            if (datos[initial] == "\"") {
                throw invalid_argument("NO PUEDEN EXISTIR 2 STRINGS PEGADOS.");
            }
            continue;
        }
            
        if (datos[initial] == symbols[2]) {
            if ((datos[initial - 1] == symbols[6]) || (datos[initial + 1] == symbols[7])) {
                throw invalid_argument("EL MAS DEBE TENER VARIABLES O STRINGS ENTRE ELLA");
            } else if ((datos[initial + 1] == symbols[2]) || (datos[initial - 1] == symbols[2])) {
                throw invalid_argument("NO PUEDE HABER MAS DE 2 + JUNTOS.");
            }
            something.push_back(datos[initial]);
            initial++;
            continue;
        }
            
        if ((datos[initial] == symbols[7])) {
            if (datos[initial+1]!= symbols[8]) {
                throw invalid_argument("ERROR: FALTA UN ; .");
            }   
        }
        bool isVariable = false;
        for (const auto &variable : variables) {
            if (datos[initial] == variable.name) {
                something.push_back(datos[initial]);
                isVariable = true;
                initial++;
                break;
            }
        }
        if (!isVariable) {
            throw invalid_argument("LO INGRESADO NO ES VARIABLE O NO HAY NADA.");
        }
        
        if (initial == final) {
            break;
        }
        initial++;
    }
        
    if ((initial == final) && (datos[initial] == symbols[7])) {
        something.push_back(datos[initial]);
    } else {
        throw invalid_argument("Falta un parentesis derecho");
    }

    initial++;
}

void Syntax::getSyntaxreadln(vector<string> &datos, vector<string> &symbols, int &initial) {
    int final = getFinalNumber(initial, datos);
    initial++;
    vector<string> comforting;

    // Verificar paréntesis izquierdo
    if (datos[initial] == symbols[6]) {
        comforting.push_back(datos[initial]);
        initial++;
    } else {
        throw invalid_argument("FALTA UN PARENTESIS IZQUIERDO.");
    }

    // Verificar si el siguiente dato es una variable válida
    bool isVariable = false;
    for (const auto &variable : variables) {
        if (datos[initial] == variable.name) {
            comforting.push_back(datos[initial]);
            isVariable = true;
            initial++;
            break;
        }
    }
    
    // Verificar paréntesis derecho y si solo hay una variable
    if ((initial == final) && (datos[initial] == symbols[7])) {
        comforting.push_back(datos[initial]);
    } else if (datos[initial] == symbols[8]) {
        throw invalid_argument("FALTA DE PARENTESIS DERECHO");
    } else if (datos[initial+1] != symbols[7]) {
        throw invalid_argument("SOLO DEBE HABER UNA VARIABLE.");
    }
    
    if (!isVariable) {
        throw invalid_argument("LO INGRESADO NO ES VARIABLE O NO HAY NADA.");
    }
    initial++;
    
}

void Syntax::getSyntaxprocedure(vector<string> &datos, vector<string> &symbols, int &initial) {
    int final = getFinalNumber(initial, datos);
    if (datos[final] != ")")
    {
        throw invalid_argument("Falta un parentesis derecho");
    }

    if (datos[initial+1] != "(")
    {
        throw invalid_argument("Falta un parentesis derecho");
    }
    initial = final + 2;
    getSyntaxBegin(datos, symbols, initial, false, true);
    variables.clear();
    initial++;
}
