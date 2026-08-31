#include <iostream>
using namespace std;

int main(){
    try{
    throw 42;
} catch(int e){
        cout << "Program is running..." << endl;

    }
    

}
    return 0;
