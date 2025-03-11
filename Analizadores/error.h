#include <iostream>
#include <exception>
using namespace std;
class CompilatorError : public exception {
private:
    string mensaje;

public:
    CompilatorError(const string& msg, int ln, int col) {
        this->mensaje = ("Error: " + msg + " [Linea: " + std::to_string(ln) + ", Columna: " + std::to_string(col) + "]");
    }

    const char* what() const noexcept override {
        return mensaje.c_str();
    }
};