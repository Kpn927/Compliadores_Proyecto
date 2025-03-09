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
    void getSyntaxBegin(vector<string> &datos, vector<string> &symbols, int &initialNumber, bool isPrincipal);

    // Método para crear los árboles
    void recursiveTree(Node<string>* actual, Tree<string>&padre, vector<string> &symbols, vector<string> &datos, int initialNumber, int finalNumber);

    // Overload de recursive Tree con menos argumentos
    void recursiveTree(Tree<string>&padre, vector<string> &datos, int initialNumber, int finalNumber);

    //Sintaxis de Var en Pascal
    void getSyntaxVar(vector<string> &datos, vector<string> &symbols, int &initial);

    //sintaxis de Asignaciones x:=1;
    void getSyntaxAsignation(vector<string> &datos, vector<string> &symbols, int &initial);

    // Analizador de for
    int forAnalizer(vector<string>& datos, int &i);

    // Analizador de if
    int ifAnalizer(vector<string>& datos, int &i);

    // Analizador de writeln
    void getSyntaxwriteln(vector<string> &datos, vector<string> &symbols, int &initial);

    // Analizador del readln
    void getSyntaxreadln(vector<string> &datos, vector<string> &symbols, int &initial);

    string phraseAnalizer(Node<string> *nodo);

    public:
    Syntax();

    //Método para la obteción principal del Syntax
    void getSyntax(vector<string> &datos);
    
};

