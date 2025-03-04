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
    private:
    // Método para obtener el final de la linea plantado por un punto y coma
    int getFinalNumber(int initialNumber, vector<string> &datos);

    // Método para sintaxis de Begin-End
    void getSyntaxBegin(vector<string> &datos, vector<string> &symbols, int &initialNumber);

    // Método para crear los árboles
    void recursiveTree(Node<string>* actual, Tree<string>&padre, vector<string> &symbols, vector<string> &datos, int initialNumber, int finalNumber);

    // Overload de recursive Tree con menos argumentos
    void recursiveTree(Tree<string>&padre, vector<string> &datos, int initialNumber, int finalNumber);

    //Sintaxis de Var en Pascal
    void getSyntaxVar(vector<string> &datos, vector<string> &symbols, int &initial);

    //sintaxis de Asignaciones x:=1;
    void getSyntaxAsignation(vector<string> &datos, vector<string> &symbols, int initial);

    // Analizador de comentarios
    void CommentAnalizer(const vector<string>& datos);

    // Analizador de for
    int forAnalizer(const vector<string>& datos);

    // Analizador de if
    int ifAnalizer(const vector<string>& datos);


    public:
    Syntax();

    //Método para la obteción principal del Syntax
    void getSyntax(vector<string> &datos);
    
};

