#include "lexico.h"
#include "arbol.h"
#include <iostream>
#include <string>
#include <cctype> //isanum isalpha isdigit
#include <stdexcept> // manejo de errores
#include <algorithm>

class Syntax{
    public:
    Syntax(){};
        void getsyntax(vector<string> &datos){
            
            cout << datos[1];
            /*int countId = std::count(identifiers.begin(), identifiers.end(), datos);
            if (countId > 0) {
                cout << " condicional Identifier " << endl;
            }*/

        };

        void getsyntaxvar(vector<string> &datos)
        {
            /*try{
                bool flag = false;
                int i = 0;

                if(datos == ":")
                {
                    cout << " condicional : " << endl;
                    // agregar nodo del medio
                }

                int count = std::count(reserved.begin(), reserved.end(), datos);
                if (count > 0) {
                    cout << " condicional reservada " << endl;
                }

                int countType = std::count(dataType.begin(), dataType.end(), datos);
                if (countType > 0) {
                    cout << " condicional dataType " << endl;
                }

                int countId = std::count(identifiers.begin(), identifiers.end(), datos);
                if (countId > 0) {
                    cout << " condicional Identifier " << endl;
                }

            }
            catch(const runtime_error& e)
            {
                cout << " ERROR: ARBOL DE VARIABLES";
            }
        }*/
    }
};

