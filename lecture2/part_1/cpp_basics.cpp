#include <iostream>
#include <vector>
// #include "tools.hpp"

// ====== Function declarations ======
// What it does: prints every item in a vector
void printVector(const std::vector<int> data);

int main()
{
    // 1. Basic kinds of variables
    int age = 20;           // whole number
    double pi = 3.14159;    // decimal number
    char grade = 'A';       // one letter or symbol
    bool is_student = true; // true or false

    std::cout << "Age: " << age << std::endl;
    std::cout << "Pi: " << pi << std::endl;
    std::cout << "Grade: " << grade << std::endl;
    std::cout << "Is student: " << is_student << std::endl;

    // 2. Make a list of whole numbers (a vector)
    std::vector<int> numbers = {12, 45, 200, 180, 255, 90, 30};

    // 3. Make a choice with if
    // Pretend to turn pixels black or white: above 128 -> 1, otherwise -> 0
    std::vector<int> binary;
    for (int n : numbers)
    {
        if (n > 128)
            binary.push_back(1);
        else
            binary.push_back(0);
    }
    // 4. Call the function to print the list
    std::cout << "Numbers: ";
    printVector(numbers);
    std::cout << "Binary: ";
    printVector(binary);

    return 0;
}

// ====== Function code ======
void printVector(const std::vector<int> data)
{
    for (int n : data)
    {
        std::cout << n << " ";
    }
    std::cout << std::endl;
}
