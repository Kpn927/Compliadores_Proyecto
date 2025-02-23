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
            
            
            // WORKS (⊙_⊙;)
            bool flag = false;
            programOpener(datos, flag);
            if (flag) { cout << "ERROR: eL programa debe iniciar con la palabra 'program' "; return; }
            // WORKS (⊙_⊙;)

            /*int countId = std::count(identifiers.begin(), identifiers.end(), datos);
            if (countId > 0) {
                cout << " condicional Identifier " << endl;
            }*/

        };


        void temp_lines(vector<string> &datos){
            
        }

        void programOpener(vector<string> &datos, bool& flag) {

            if (datos.empty()) {
                throw runtime_error("ERROR: el programa esta vacío.");
            }
            
            string temp_word = datos[0];
            transform(temp_word.begin(), temp_word.end(), temp_word.begin(), ::tolower);

            if(temp_word != "program") {
                flag = true;
                return;
            }

            return;
        }

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

