#include <iostream>
#include <stdexcept>

using namespace std;

void divide (int a, int b){
    if(b==0){
        throw invalid_argument("Division by zero is invalid");
    }

    cout << "Result: " << a/b << endl;
}

int main() {
    try{
        divide(20,5);
        divide(15,0);
    }catch(...){
        cout<< "Caught: "<<endl;
    }
    divide(0,5);

    return 0;
    }