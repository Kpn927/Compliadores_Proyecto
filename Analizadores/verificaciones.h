#include <iostream>
#include <vector>
#include "lexico.h"
#include "error.h"
using namespace std;
class Check{
    public:
    static bool isNumber(string word);

    // Helper function to match a keyword in a vector<string>
    static bool matchKeyword(const vector<string>& datos, size_t& i, const vector<string>& keyword);

    // Helper function to check if a token is an identifier
    static bool isIdentifier(const string& datos);

    static bool isNumber(const vector<string>& datos, size_t& i);

    static bool matchOperator(const vector<string>& datos, size_t& i, const vector<string>& op);

    static bool verifyAsignation(Node<string>* actual, string type, int line, int column);

    static bool isReserved(string word);

    static bool isVariable(string word);

    static bool isDatatype(string word);

    static int getLastParentesis (int initial, vector<Datos> datos);

    static string getType(string variable);

    static int getLastQuotes(int initial, vector<Datos> datos);

    static bool isFunction(string word);

    static Variables getVariable(string word);

    static Function getFunction(string word);
};

int Check::getLastParentesis(int initial, vector<Datos> datos){
    initial++;
    while (datos[initial].dato != ")"){
        if (datos[initial].dato=="("){
            initial = Check::getLastParentesis(initial, datos);
        }
        initial++;
        if (initial>= datos.size()) return datos.size();
    }
    return initial;
}

int Check::getLastQuotes(int initial, vector<Datos> datos){
    initial++;
    while (datos[initial].dato != "'"){
        initial++;
    }
    return initial;
}

bool Check::isIdentifier(const string& datos) {
    return (!datos.empty() && isalpha(datos[0]));
}

bool Check::verifyAsignation(Node<string>* actual, string type, int line, int column){
    bool right = false, left = false, center;
    if((actual->getCenter()->getValue() == "")){
        center = Check::verifyAsignation(actual->getCenter(), type, line, column);
        return center;
    }else if (Check::isNumber(actual->getCenter()->getValue())){
        if (!(type == "integer" || type == "double" || type == "real")) throw CompilatorError("Asignación incluye error de tipos", line, column);
        return true;
    }
    if((actual->getLeft()->getValue() == "")) left = Check::verifyAsignation(actual->getLeft(), type, line, column);
    else if(Check::isNumber(actual->getLeft()->getValue())){
        if (!(type == "integer" || type == "double" || type == "real")) throw CompilatorError("Asignación incluye error de tipos", line, column);
        left = true;
    }
    if((actual->getRight()->getValue() == "")) right = Check::verifyAsignation(actual->getRight(), type, line, column);
    else if(Check::isNumber(actual->getRight()->getValue())){
        if (!(type == "integer" || type == "double" || type == "real")) throw CompilatorError("Asignación incluye error de tipos", line, column);
        right = true;
    }
    for (int i = 0; i < variables.size(); i++){
        if (left == false && variables[i].name == actual->getLeft()->getValue()){
            if (variables[i].type != type) throw CompilatorError("Asignación incluye error de tipos", line, column);
            left = true;
        }
        if (variables[i].name == actual->getCenter()->getValue()){
            if (variables[i].type != type) throw CompilatorError("Asignación incluye error de tipos", line, column);
            return true;
        }
        if (right == false && variables[i].name == actual->getRight()->getValue()){
            if (variables[i].type != type) throw CompilatorError("Asignación incluye error de tipos", line, column);
            right = true;     
        }
    }
    if (left==true && right==true) return true;
    return false;
}

string Check::getType(string variable){
    for (const Variables var: variables){
        if (var.name == variable) return var.type;
    }
    cout<<"ERROR EN GET TYPE";
    return "";
}

bool Check::isNumber(const vector<string>& datos, size_t& i) {
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

bool Check::isNumber(string word){
    bool hasDecimal = false;
    bool hasDigits = false;

    for (size_t i = 0; i < word.length(); ++i) {
        char c = word[i];

        if (isdigit(c)) {
            hasDigits = true;
        }
        else if (c == '.') {
            if (hasDecimal) return false;
            hasDecimal = true;
        }
        else if (c == '-' || c == '+') {
            if (i != 0) return false;
        }
        else {
            return false;
        }
    }
    return hasDigits;
}

bool Check::matchOperator(const vector<string>& datos, size_t& i, const vector<string>& op) {
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

bool Check::matchKeyword(const vector<string>& datos, size_t& i, const vector<string>& keyword) {
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

bool Check::isReserved(string word){
    // ¿como usar find con auto i?
    int i;
    i = count(reserved.begin(), reserved.end(), word);
    if (i>0) return true;
    i = count(dataType.begin(), dataType.end(), word);
    if (i>0) return true;
    i = count(identifiers.begin(), identifiers.end(), word);
    if (i>0) return true;
    i = count(symbols.begin(), symbols.end(), word);
    if (i>0) return true;
    return false;
}

bool Check::isVariable(string word){
    for (const Variables& variable : variables) {
        if (variable.name == word) return true;
    }
    return false;
}

bool Check::isFunction(string word){
    for (const Function& function : functions) {
        if (function.name == word) return true;
    }
    return false;
}

bool Check::isDatatype(string word){
    for (const string& type : dataType) {
        if (word == type) return true;
    }
    return false;
}

Variables Check::getVariable(string word) {
    for (const Variables var : variables) {
        if (word == var.name) return var;
    }
    Variables voidVariable;
    return voidVariable;
}

Function Check::getFunction(string word) {
    for (const Function func : functions) {
        if (word == func.name) return func;
    }
    Function voidFunction;
    return voidFunction;
}