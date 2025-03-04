#pragma once
#include "lexico.h"
#include "arbol.h"
#include <iostream>
#include <string>
#include <cctype> //isanum isalpha isdigit
#include <stdexcept> // manejo de errores
#include <algorithm> 
#include <vector>

class Syntax{
    public:
    Syntax();

    void getSyntax(vector<string> &datos);

    void getSyntaxBegin(vector<string> &datos, vector<string> &symbols, int initialNumber, int finalNumber);

    void recursiveTree(Node<string>* actual, Tree<string>&padre, vector<string> &symbols, vector<string> &datos, int initialNumber, int finalNumber);

    void recursiveTree(Tree<string>&padre, vector<string> &datos, int initialNumber, int finalNumber);

    void getSyntaxVar(vector<string> &datos, vector<string> &symbols, int initial, int final);

    bool isNumber(string word);

    void getSyntaxAsignation(vector<string> &datos, vector<string> &symbols, int initial, int final);
    
    void CommentAnalizer(const vector<string>& datos);

    int forAnalizer(const vector<string>& datos);
    
    int ifAnalizer(const vector<string>& datos);

    // Helper function to match a keyword in a vector<string>
    bool matchKeyword(const vector<string>& datos, size_t& i, const vector<string>& keyword);

    // Helper function to check if a token is an identifier
    bool isIdentifier(const vector<string>& datos, size_t& i);

    bool isNumber(const vector<string>& datos, size_t& i);

    bool matchOperator(const vector<string>& datos, size_t& i, const vector<string>& op);
    
};

