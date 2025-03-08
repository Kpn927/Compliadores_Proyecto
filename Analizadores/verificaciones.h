#include <iostream>
#include <vector>
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

    static bool verifyAsignation(Node<string>* actual);
};

bool Check::isIdentifier(const string& datos) {
    return (!datos.empty() && isalpha(datos[0]));
}

bool Check::verifyAsignation(Node<string>* actual){
    bool right = false, left = false;
    if((actual->getLeft()->getValue() == "")) left = Check::verifyAsignation(actual->getLeft());
    if((actual->getRight()->getValue() == "")) right = Check::verifyAsignation(actual->getRight());
    if(Check::isNumber(actual->getLeft()->getValue())) left = true;
    if(Check::isNumber(actual->getRight()->getValue())) right = true;
    for (int i = 0; i < variables.size(); i++){
        if (left == false && variables[i].name == actual->getLeft()->getValue()) left = true;
        if (right == false && variables[i].name == actual->getRight()->getValue()) right = true;     
    }
    if (left && right) return true;
    else return false;
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
    for (char c : word) {
        if (!isdigit(c)) {
            return false;
        }
    }
    return true;
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
