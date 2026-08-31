#include <iostream>
#include <stdexcept>

int main(){

    try{

        bool condition = true;

        if(condition){
            throw std::runtime_error("Runtime error occured!")''
        }else{
            throw std::out_of_range("Index out of bounds!")
        }
    }catch(const std::runtime_error& e){
        std::cerr << "Runtime error: " << e.what() << std::endl;
    }catch(const std::out_of_range& e){
        std::cerr << "Out of range: " << e.what() << std::endl;
    }catch(const std::exception& e){
        std::cerr << "Caught Error: " << e.what() << std::endl;
    }
    std::cout << "Program continues...\n";
    return 0;
}