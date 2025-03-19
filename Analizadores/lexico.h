#pragma once

#include <iostream>
#include <fstream>
#include <sstream>
#include <string>
#include <vector>
#include <cctype> //isanum isalpha isdigit

using namespace std;

enum class Tokens {
    RESERVED, // if, else, while
    OPERATOR, // + - / *
    DATA_TYPE, // variables
    IDENTIFIER, // function names, class names   
    OTHERS, 
    VARIABLE,
    DELIMETER,
    NUMBER
};

struct Variables{
    string name;
    string value;
    string type;
};

struct Datos{
    string dato;
    int linea;
    int columna;
};

vector<string> reserved = {"if", "then", "to", "do", "downto", "writeln", "readln", "else"};
vector<string> operators = {"+", "-", "*", "/", ":=", "=", ">", "<", ">=", "<=", "==", "!=", "++",":"};
vector<string> dataType = {"integer", "real", "character", "array", "boolean", "double"};
vector<string> identifiers = {"var", "const", "procedure", "function", "begin", "end", "program"};
vector<Variables> variables = {};
vector<string> delimiters = {"(", ")", ";",  ",", ".", "{", "}"};
vector<string> symbols = {":=",":", "+", "-", "*", "/","(", ")", ";",  ",", ".", "{", "}", "=", ">", "<", ">=", "<=", "==", "!=", "++"};


void show_vector(const vector<string>& v);
void PrintingTokens(const vector<string>& v);
vector<Datos> tokenize(string& input);
void PushToVariable(vector<string>& variables, const string& v);
void deleteComments(string searchWord, string endWord, string &datos);

void show_vector(const vector<string>& v) {
    for (const auto& x : v)
        cout << x << " ";
    cout << endl;
}

bool isReserved(const string& word) {
    for (const auto& type : reserved) {
        if (word == type) return true;
    }
    return false;
}

bool isOperator(const string& word) {
    for (const auto& type : operators) {
        if (word == type) return true;
    }
    return false;
}

bool isDataType(const string& word) {
    for (const auto& type : dataType) {
        if (word == type) return true;
    }
    return false;
}

bool isIdentifer(const string& word) {
    for (const auto& type : identifiers) {
        if (word == type) return true;
    }
    return false;
}

bool isVariable(const string& word) {
    for (const auto& type : variables) {
        if (word == type.name) return true;
    }
    return false;
}

bool isDelimeter(const string& word) {
    for (const auto& type : delimiters) {
        if (word == type) return true;
    }
    return false;
}

bool isNumber(const string& word) {
    for (char ch : word) {
        if (!isdigit(ch)) return false;
    }
    return !word.empty();
}

void deleteComments(string searchWord, string endWord, string &datos, int position){
    size_t startPos;
    if (position==-1) startPos = datos.find(searchWord);
    else startPos = position;
    if (startPos != std::string::npos) {
        
        size_t finalPos = datos.find(endWord, startPos);
        if (finalPos != std::string::npos) {
            size_t lengthToRemove = finalPos - startPos;
            datos.erase(startPos, lengthToRemove+ endWord.size());
            if (position==-1) deleteComments(searchWord, endWord, datos, position);
        } else {
            cout << "'*/' not found after '/*'" << std::endl;
        }
    }
}

Tokens getTokenType(const string& word) {
    if (isReserved(word)) return Tokens::RESERVED;
    if (isOperator(word)) return Tokens::OPERATOR;
    if (isDataType(word)) return Tokens::DATA_TYPE;
    if (isIdentifer(word)) return Tokens::IDENTIFIER;
    if (isVariable(word)) return Tokens::VARIABLE;
    if (isDelimeter(word)) return Tokens::DELIMETER;
    if (isNumber(word)) return Tokens::NUMBER;
    return Tokens::OTHERS;
}

void PrintingTokens(const vector<string>& v) {
    for (const auto& word : v) {
        Tokens type = getTokenType(word);
        switch (type) {
            case Tokens::RESERVED:
                cout << "Reserved word: " << word << endl;
                break;
            case Tokens::OPERATOR:
                cout << "Operator: " << word << endl;
                break;
            case Tokens::DATA_TYPE:
                cout << "Data type: " << word << endl;
                break;
            case Tokens::IDENTIFIER:
                cout << "Identifier: " << word << endl;
                break;
            case Tokens::OTHERS:
                cout << "Other: " << word << endl;
                break;
            case Tokens::VARIABLE:
                cout << "Variable: " << word << endl;
                break;
            case Tokens::DELIMETER:
                cout << "Delimeter: " << word << endl;
                break;
            case Tokens::NUMBER:
                cout << "Number: " << word << endl;
                break;
        }
    }
}

vector<Datos> tokenize(string& input) {
    deleteComments("//","\n",input, -1);
    deleteComments("(*","*)",input, -1);
    deleteComments("{","}",input, -1);
    vector<Datos> tokens;
    Datos* token = new Datos;
    int linea=1, columna=0;
    for (size_t i = 0; i < input.size(); i++) {
        char ch = input[i];
        columna++;
        if (token==NULL) token = new Datos;
        if (isspace(ch)) {
            if (! token->dato.empty()) {
                token->linea = linea;
                token->columna = (columna - (token->dato.size()));
                tokens.push_back(*token);
                delete token;
                token = NULL;
            }
        } else if (isalnum(ch) || ch == '_') {
            token->dato += ch;
        } else {
            if (!token->dato.empty()) {
                token->linea = linea;
                token->columna = (columna - (token->dato.size()));
                tokens.push_back(*token);
                delete token;
                token = NULL;
            }
            if (i + 1 < input.size()) {
                Datos potentialOperator = {string(1, ch) + input[i + 1], linea, (int) (columna - potentialOperator.dato.size() + 1)};
                if (isOperator(potentialOperator.dato)) {
                    tokens.push_back(potentialOperator);
                    i++;
                    continue;
                }
            }
            tokens.push_back({string(1, ch), linea, columna});
        }
        if (ch=='\n'){
            columna = 0;
            linea++;
        }
    }
    if (token==NULL) {
        tokens.push_back(*token);
    }
    delete token;
    return tokens;
}

void PushToVariable(vector<string>& variables, const string& v) {
    variables.push_back(v);
}