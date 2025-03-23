#include "sintactico.h"
#include "verificaciones.h"
Syntax::Syntax() {}

void Syntax::getSyntax(vector<Datos> &datos) {

    try{

        if (datos.empty() || datos[0].dato != identifiers[6]) throw CompilatorError("Falta o esta escrito mal la palabra 'program'", datos[0].linea, datos[0].columna);
        if (datos[2].dato != ";") throw CompilatorError("Falta ';' en declaración de program", datos[2].linea, datos[2].columna);
        if (Check::isReserved(datos[1].dato)) throw CompilatorError("declaración de Program tiene una palabra reservada", datos[1].linea, datos[1].columna);
        reserved.push_back(datos[1].dato);

        int initialNumber = 3;
        
        if (datos[initialNumber].dato == identifiers[3]) {
            getSyntaxfunction(datos, symbols, initialNumber);
        }
        
        if (datos[initialNumber].dato == identifiers[2]) {
            getSyntaxprocedure(datos, symbols, initialNumber);
        }
        
        if (datos[initialNumber].dato == identifiers[0] || (datos[initialNumber].dato != identifiers[3] && datos[initialNumber].dato != identifiers[4])) {
            initialNumber++;
            while(datos[initialNumber].dato != identifiers[4]){
                getSyntaxVar(datos, symbols, initialNumber, false);
            }
        }
    
        if (datos[initialNumber].dato == identifiers[4]) {
            initialNumber++;
            bool flag_function = false;
            getSyntaxBegin(datos, symbols, initialNumber, true, false, flag_function, "");
        }
    } catch (const CompilatorError& e){
        cerr<< e.what()<<endl;
    }
};

int Syntax::getFinalNumber(int finalNumber, vector<Datos> &datos){
    while (finalNumber < datos.size() && datos[finalNumber + 1].dato != ";") {
        finalNumber++;
    }
    return finalNumber;
}

void Syntax::getSyntaxBegin(vector<Datos> &datos, vector<string> &symbols, int &initialNumber, bool isPrincipal, bool isProcedure, bool &isFunction, string functionName) {
    bool final = true;

    if (isFunction) {
        if (datos[initialNumber].dato == identifiers[0] || (datos[initialNumber].dato != identifiers[3] && datos[initialNumber].dato != identifiers[4])) {
            initialNumber++;
            while(datos[initialNumber].dato != identifiers[4]){
                getSyntaxVar(datos, symbols, initialNumber, false);
            }
        }
        initialNumber++;
    }

    if (isProcedure) {
        if (datos[initialNumber].dato == identifiers[0] || (datos[initialNumber].dato != identifiers[3] && datos[initialNumber].dato != identifiers[4])) {
            initialNumber++;
            while(datos[initialNumber].dato != identifiers[4]){
                getSyntaxVar(datos, symbols, initialNumber, false);
            }
        }
        initialNumber++;
        isProcedure = false;
    }
    
    while (final) {
        if (datos[initialNumber].dato == "end") { 
            if (initialNumber + 1 < datos.size()) {
                if (datos[initialNumber + 1].dato == ".") {
                    cout << endl << "Codigo terminado";

                    for (const Variables& var : variables) {
                        if (var.value.empty()) {
                            cout << endl << "WARNING: Variable '" << var.name << "' no inicializada";
                        }
                    }
                    
                    return;
                } else if (datos[initialNumber + 1].dato != ";") {
                    throw CompilatorError("Begin-End mal declarado", datos[initialNumber+1].linea, datos[initialNumber+1].columna);
                } else {
                    // cout << "Begin-End terminado" << endl;
                    initialNumber++;
                    final = false;
                    if (!isPrincipal) return;
                }
            }
        }
        else if (datos[initialNumber].dato == "begin") {
            initialNumber++;
            bool flag_function = false;
            getSyntaxBegin(datos, symbols, initialNumber, false, false, flag_function, "");
        }
        else if (datos[initialNumber].dato == "if") {
            //cout << "Analizando el if" << endl;
            int result = ifAnalizer(datos, initialNumber);
            if (result == -1) return; 
            initialNumber = result;
        }
        else if (datos[initialNumber].dato == "for") {
            //cout << "Analizando el for" << endl;
            int result = forAnalizer(datos, initialNumber);
            if (result == -1) return; 
            initialNumber = result;
        }else if (datos[initialNumber].dato == ";") initialNumber++;
        else if (Check::isVariable(datos[initialNumber].dato)) getSyntaxAsignation(datos, symbols, initialNumber);
        else if (datos[initialNumber].dato=="writeln") getSyntaxwriteln(datos, symbols, initialNumber);
        else if (datos[initialNumber].dato=="readln") getSyntaxreadln(datos, symbols, initialNumber);
        else if (datos[initialNumber].dato==functionName) {isFunction = false; initialNumber++;
        } else {
            throw CompilatorError("Simbolo no reconocido", datos[initialNumber].linea, datos[initialNumber].columna);
        }
    }
}

void Syntax::getSyntaxVar(vector<Datos> &datos, vector<string> &symbols, int &initial, bool isFunction) {
    int final = getFinalNumber(initial, datos);
    Tree<string> arbolvar;

    // Avanzar si es un identificador válido
    if (datos[initial].dato == identifiers[0]) {
        initial++;
    }

    if (isFunction) {
        auto it = find_if(datos.begin() + initial, datos.end(), [](const Datos& d) {
            return Check::isDatatype(d.dato);
        });
        if (it != datos.end()) {
            final = std::distance(datos.begin(), it);
        }
    }
    

    arbolvar.insertarOrdenado("", 0);

    recursiveTree(arbolvar, datos, initial, final);

    Node<string> *a = arbolvar.getRaiz()->getLeft();
    Node<string> *b = arbolvar.getRaiz()->getRight();
    Variables newVariable;

    // Verificar si 'a' contiene un valor válido
    if (!a->getValue().empty()) {
        if (Check::isVariable(a->getValue())) {
            throw CompilatorError("variable doblemente declarada.", datos[initial].linea, datos[initial].columna);
        }
        
        newVariable.name = a->getValue();
        newVariable.type = b->getValue();
        variables.push_back(newVariable);
    }
    

    // Validar si el vSalor en 'b' es un tipo de dato válido
    if (!Check::isDatatype(b->getValue())) {
        throw CompilatorError("en la declaracion del datatype.", datos[initial].linea, datos[initial].columna);
    }

    if (Check::isReserved(a->getValue())) {
        throw CompilatorError("arbol hay una palabra reservada", datos[initial].linea, datos[initial].columna);
    }
    

    // Validar variables y palabras reservadas en el árbol
    while (a->getRight() != nullptr) {
        if (Check::isReserved(a->getLeft()->getValue()) || Check::isReserved(a->getRight()->getValue())) {
            throw CompilatorError("arbol hay una palabra reservada", datos[initial].linea, datos[initial].columna);
        }

        if (Check::isVariable(a->getLeft()->getValue()) || Check::isVariable(a->getRight()->getValue())) {
            throw CompilatorError("variable ya declarada.", datos[initial].linea, datos[initial].columna);
        }
        

        newVariable.name = a->getLeft()->getValue();
        newVariable.type = b->getValue();
        variables.push_back(newVariable);

        a = a->getRight();

        if (!a->getValue().empty()) {
            newVariable.name = a->getValue();
            newVariable.type = b->getValue();

            variables.push_back(newVariable);
        }
    }
    
    arbolvar.~Tree();
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
    if (Check::isNumber(nodo->getCenter()->getValue()) || Check::isVariable(nodo->getCenter()->getValue())){
        return (nodo->getCenter()->getValue());
    }
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
        delete left, right;
        return (string)std::to_string(result);
    } else{
        delete left, right;
        return nodo->getLeft()->getValue() + " " + nodo->getCenter()->getValue() + " " + nodo->getRight()->getValue();
    }
}

void Syntax::getSyntaxAsignation(vector<Datos> &datos, vector<string> &symbols, int &initial) {
    int final = getFinalNumber(initial, datos);
    Tree<string> arbolvar;
    
    arbolvar.insertarOrdenado("",0);
    
    recursiveTree(arbolvar, datos, initial, final);
    
    arbolvar.orden(arbolvar.getRaiz(), true);

    Node<string> *a = arbolvar.getRaiz()->getRight();
    Node<string> *b = arbolvar.getRaiz()->getLeft();

    if(!Check::isVariable(b->getValue())) throw CompilatorError("no pusiste una variable", datos[initial].linea, datos[initial+2].columna);
    string type = Check::getType(b->getValue());

    if (!Check::verifyAsignation(arbolvar.getRaiz(), type, datos[initial+2].linea, datos[initial+2].columna) && type != "character") throw CompilatorError("no pusiste bien la asignacion a la variable", datos[initial+2].linea, datos[initial+2].columna);

    initial = final+2;
    string value;
    if (type == "character"){
        value = datos[initial-3].dato;
        if (value.size() >1 || (datos[initial-2].dato != "'" && datos[initial-4].dato != "'")) throw CompilatorError("character tiene más de un valor", datos[initial-3].linea, datos[initial-3].columna);
    }
    else value = (string) phraseAnalizer(arbolvar.getRaiz()->getRight());
    int i = 0;
    while(i<variables.size()){
        if(arbolvar.getRaiz()->getLeft()->getValue() == variables[i].name) variables[i].value = value;
        i++;
    }
    cout<<"[resultado: "<<value<<"]"<<endl;
    arbolvar.~Tree();
    return;
}

int Syntax::forAnalizer(vector<Datos>& datos, int &i) {
    //size_t i = 0;

    // 1.- Check for "for"
    if (i >= datos.size() || datos[i].dato != "for") {
        throw CompilatorError("Hay un error en el bucle for (no hay for)", datos[i].linea, datos[i].columna);
    }    
    i++;

    // 2.- Check for identifier
    if (!Check::isIdentifier(datos[i].dato)) {
        throw CompilatorError("Hay un error en el bucle for (no hay identificador)", datos[i].linea, datos[i].columna);
    }
    i++;
        
    // 3.- Check for assignment operator ":="
    if (i+1 >= datos.size() || datos[i].dato != ":=") throw CompilatorError("Hay un error en el bucle for (no hay :=)", datos[i].linea, datos[i].columna);
    i++;
    
    // 4.- Check for initial value (number)
    if (!Check::isNumber(datos[i].dato)) throw CompilatorError("Hay un error en el bucle for (valor inicial no es un numero)", datos[i].linea, datos[i].columna);
    i++;
    
    // 5.- Check for "to" or "downto"
    if (i >= datos.size() || (datos[i].dato != "to" && datos[i].dato != "downto")) throw CompilatorError("Hay un error en el bucle for (no hay downto/to)", datos[i].linea, datos[i].columna);
    i++;

    // 6.- Check for final value (number)
    if (!Check::isNumber(datos[i].dato)) throw CompilatorError("Hay un error en el bucle for (valor final no es un numero)", datos[i].linea, datos[i].columna);
    i++;
    
    // 7.- Check for "do"
    if (i >= datos.size() || datos[i].dato != "do") throw CompilatorError("Hay un error en el bucle for (no hay do)", datos[i].linea, datos[i].columna);
    i++;
    
    return i;
}

int Syntax::ifAnalizer(vector<Datos>& datos, int &i) {
    //size_t i = 0;

    if (i >= datos.size() || datos[i].dato != "if") throw CompilatorError("Hay un error en el if (no hay if)", datos[i].linea, datos[i].columna);
    i++;
    if (datos[i].dato == "(") {
        i++;
    }

    while (i < datos.size() && datos[i].dato != ")" && datos[i].dato != "then") {
        if (!Check::isIdentifier(datos[i].dato) &&
            !isOperator(datos[i].dato) &&
            !Check::isNumber(datos[i].dato)) throw CompilatorError("la condicion dentro del if esta mal", datos[i].linea, datos[i].columna);
            i++;
    }
    if (i >= datos.size() || datos[i].dato == ")") {
        i++;
    }

    // 3.- Check for "then"
    if (i >= datos.size() || datos[i].dato != "then") throw CompilatorError("Hay un error en el if (no hay then)", datos[i].linea, datos[i].columna);
    i++;

    if (i < datos.size() && datos[i].dato != ";") {
        if (datos[i].dato == "begin") {
            i++;
            bool flag_function = false;
            getSyntaxBegin(datos, symbols, i, false, false, flag_function, "");
            
        } else if (datos[i].dato == "if"){
            int result = ifAnalizer(datos, i);
            if (result == -1) return -1;
            i = result;
            
        } else if (Check::isVariable(datos[i].dato)) {
                getSyntaxAsignation(datos, symbols, i);

            if (i < datos.size() && datos[i].dato == "else") {
                i++;

                if (datos[i].dato == "if") {
                    int result = ifAnalizer(datos, i);
                    if (result == -1) return -1;
                    i = result;
                } else if (Check::isVariable(datos[i].dato)) {
                    getSyntaxAsignation(datos, symbols, i);
                } else {
                    throw CompilatorError("Declaracion invalida despues de else", datos[i].linea, datos[i].columna);
                }
            }
        } 
        
    }
    return i;
}

void Syntax::recursiveTree(Tree<string>&padre, vector<Datos> &datos, int initialNumber, int finalNumber){
    recursiveTree(padre.getRaiz(), padre, symbols, datos, initialNumber, finalNumber);
    return;
}

void Syntax::recursiveTree(Node<string>* actual, Tree<string>&padre, vector<string> &symbols, vector<Datos> &datos, int initialNumber, int finalNumber){
    // detectar separador
    int symbol, j, lastWord;
    if (actual==NULL) return;
    if (initialNumber+1 == finalNumber) throw CompilatorError("Asignacion mal colocada", datos[initialNumber].linea, datos[initialNumber].columna);
    if (datos[initialNumber].dato=="(" || datos[initialNumber].dato == "'"){
        if (datos[initialNumber].dato=="(") lastWord = Check::getLastParentesis(initialNumber, datos);
        else lastWord = Check::getLastQuotes(initialNumber, datos);
        if (lastWord ==finalNumber){
            padre.insertarOrdenado(datos[initialNumber].dato,0,actual);
            padre.insertarOrdenado(datos[lastWord].dato,2,actual);
            if(finalNumber-initialNumber== 1) throw CompilatorError("NADA ENTRE LOS PARENTESIS", datos[initialNumber].linea, datos[initialNumber].columna);
            else if(finalNumber-initialNumber== 2) padre.insertarOrdenado(datos[finalNumber-1].dato,1,actual);
            else if(finalNumber-initialNumber> 2) {
                padre.insertarOrdenado("", 1, actual);
                recursiveTree(actual->getCenter(), padre, symbols, datos, initialNumber+1, finalNumber-1);
            }
            return;
        }
        else j = lastWord;
    }
    else j=initialNumber;
    for(int i = 0; i< symbols.size(); i++){
        for(int k = j; k<=finalNumber;k++){
            if (symbols[i]==datos[k].dato){
                padre.insertarOrdenado(datos[k].dato, 1, actual);
                symbol=k;
                k= finalNumber+1;
                i = symbols.size() +1;
            }
        }
    }
        if (symbol==initialNumber+1) padre.insertarOrdenado(datos[symbol-1].dato, 0, actual);
        else {
            padre.insertarOrdenado("", 0, actual);
            recursiveTree(actual->getLeft(), padre, symbols, datos, initialNumber, symbol-1);
        }
    
        if (symbol==finalNumber-1) padre.insertarOrdenado(datos[symbol+1].dato, 2, actual);
        else {
            padre.insertarOrdenado("", 2, actual);
            recursiveTree(actual->getRight(), padre, symbols, datos, symbol+1, finalNumber);
        }
    return;
}

string getString(vector<Datos> datos, int &initialNumber, int &finalNumber) {

    string value = "";

    if (datos[initialNumber].dato == "\"") {
        value += datos[initialNumber].dato;
        initialNumber++;

        while (initialNumber <= finalNumber && datos[initialNumber].dato != "\"") {
            value += datos[initialNumber].dato;
            initialNumber++;
            if (datos[initialNumber].dato == "\"") {
                finalNumber = initialNumber;
                break;
            }
        }

        if (initialNumber <= finalNumber && datos[initialNumber].dato == "\"") {
            value += datos[initialNumber].dato;
        } else {
            throw CompilatorError("Falta una comilla de cierre.", datos[initialNumber].linea, datos[initialNumber].columna);
        }
    } else {
        throw CompilatorError("No se encontró una comilla inicial", datos[initialNumber].linea, datos[initialNumber].columna);
    }

    return value;
}

void Syntax::getSyntaxwriteln(vector<Datos> &datos, vector<string> &symbols, int &initial) {
    int final = getFinalNumber(initial, datos);
    initial++;
    vector<string> something;

    if (datos[initial].dato == symbols[6]) {
        something.push_back(datos[initial].dato);
        initial++;
    } else {
        throw CompilatorError("falta un parentesis izquierdo.", datos[initial].linea, datos[initial].columna);
    }
    if (datos[initial].dato == symbols[7]) {
        throw CompilatorError("no existe nada en el writeln", datos[initial].linea, datos[initial].columna);
    }
        
    while (initial != final) {
            
        if (datos[initial].dato == "\"") {
            string value = getString(datos, initial, final);
            initial = final;
            if (datos[initial].dato == "\"") {
                initial++;
            }
            final = getFinalNumber(initial, datos);
            something.push_back(value);
            if (datos[initial].dato == "\"") {
                throw CompilatorError("no pueden existir 2 strings pegados.", datos[initial].linea, datos[initial].columna);
            }
            continue;
        }
            
        if (datos[initial].dato == symbols[2]) {
            if ((datos[initial - 1].dato == symbols[6]) || (datos[initial + 1].dato == symbols[7])) {
                throw CompilatorError("el mas debe tener variables o 2 strings entre ella", datos[initial].linea, datos[initial].columna);
            } else if ((datos[initial + 1].dato == symbols[2]) || (datos[initial - 1].dato == symbols[2])) {
                throw CompilatorError("no puedes tener 2 + juntos. ", datos[initial].linea, datos[initial].columna);
            }
            something.push_back(datos[initial].dato);
            initial++;
            continue;
        }
            
        if ((datos[initial].dato == symbols[7])) {
            if (datos[initial+1].dato != symbols[8]) {
                throw CompilatorError("falta un ; .", datos[initial].linea, datos[initial].columna);
            }   
        }
        
        if (!Check::isVariable(datos[initial].dato)) {
            throw CompilatorError("lo ingresado no es variable o no hay nada.", datos[initial].linea, datos[initial].columna);
        }
        
        if (initial == final) {
            break;
        }
        initial++;
    }
        
    if ((initial == final) && (datos[initial].dato == symbols[7])) {
        something.push_back(datos[initial].dato);
    } else {
        throw CompilatorError("Falta un parentesis derecho", datos[initial].linea, datos[initial].columna);
    }

    initial++;
}

void Syntax::getSyntaxreadln(vector<Datos> &datos, vector<string> &symbols, int &initial) {
    int final = getFinalNumber(initial, datos);
    initial++;
    vector<string> comforting;

    // Verificar paréntesis izquierdo
    if (datos[initial].dato == symbols[6]) {
        comforting.push_back(datos[initial].dato);
        initial++;
    } else {
        throw CompilatorError("falta un parentesis izquierdo.", datos[initial].linea, datos[initial].columna);
    }
    
    // Verificar paréntesis derecho y si solo hay una variable
    if ((initial == final) && (datos[initial].dato == symbols[7])) {
        comforting.push_back(datos[initial].dato);
    } else if (datos[initial].dato== symbols[8]) {
        throw CompilatorError("falta un parentesis derecho", datos[initial].linea, datos[initial].columna);
    } else if (datos[initial+1].dato != symbols[7]) {
        throw CompilatorError("solo debe haber una variable.", datos[initial+1].linea, datos[initial+1].columna);
    }
    
    if (!Check::isVariable(datos[initial].dato)) {
        throw CompilatorError("lo ingresado no es variable o no hay nada.", datos[initial].linea, datos[initial].columna);
    }
    initial++;
    
}

void Syntax::getSyntaxprocedure(vector<Datos> &datos, vector<string> &symbols, int &initial) {
    int final = getFinalNumber(initial, datos);
    if (datos[final].dato != ")")
    {
        throw CompilatorError("falta un parentesis derecho", datos[final].linea, datos[final].columna);
    }

    if (datos[initial+1].dato != "(")
    {
        throw CompilatorError("falta un parentesis derecho", datos[initial+1].linea, datos[initial+1].columna);
    }
    initial = final + 2;
    bool flag_function = false;
    getSyntaxBegin(datos, symbols, initial, false, true, flag_function, "");
    variables.clear();
    initial++;
}

void Syntax::getSyntaxfunction(vector<Datos> &datos, vector<string> &symbols, int &initial) {
    int final = getFinalNumber(initial, datos);
    initial++;

    if (datos[initial].dato == "(") {
        throw CompilatorError("no existe el nombre de la funcion. ", datos[final].linea, datos[final].columna);
    }

    string functionName = datos[initial].dato;
    initial++;

    if (datos[initial].dato != "(") {
        throw CompilatorError("falta parentesis izquierdo '('.", datos[final].linea, datos[final].columna);
    }
    initial++;

    while (datos[initial].dato != ")") {
        if (Check::isReserved(datos[initial].dato)) {
            throw CompilatorError("palabra Reservada o identificador en el nombre de la funcion.", datos[initial].linea, datos[initial].columna);
        }

        getSyntaxVar(datos, symbols, initial, true);
        initial--;

        if (datos[initial].dato == ",") {
            initial++;
            if (datos[initial].dato == ")") {
                throw CompilatorError("identificador sin existir despues de una coma.", datos[initial].linea, datos[initial].columna);
            }
        } else if (datos[initial].dato == ")") {
            initial++;
            break;
        } else {
            throw CompilatorError("falta una coma o parentesis derecho ')'.", datos[initial].linea, datos[initial].columna);
        }
    }

    if (datos[initial].dato != ":") {
        throw CompilatorError("falta los ':' despues de la declaracion del datatype de la funcion.", datos[initial].linea, datos[initial].columna);
    }
    initial++;

    if (!Check::isDatatype(datos[initial].dato)) {
        throw CompilatorError("no existe o es incorrecto el datatype usado en la funcion.", datos[initial].linea, datos[initial].columna);
    }

    initial = final + 2;
    bool function_error = true;
    getSyntaxBegin(datos, symbols, initial, false, false, function_error, functionName);

    if (function_error) {
        throw CompilatorError("falta el return de la funcion. ", datos[initial].linea, datos[initial].columna);
    }

    variables.clear();
    initial++;
}


