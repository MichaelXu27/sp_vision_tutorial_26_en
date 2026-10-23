#include <iostream>
using namespace std;

class Material {
public:
    Material();
    ~Material();
    void print();
};

int main()
{
    cout << "--- Constructor (runs when the object is made) ---" << endl;
    Material m;

    cout << "\n--- print function ---" << endl;
    m.print();

    cout << "\n--- Destructor runs on its own (when the object goes away) ---" << endl;
    return 0;
}

// Constructor code
Material::Material() {
    cout << "Material Default Constructor!" << endl;
}

// Destructor code
Material::~Material() {
    cout << "Material Destructor!" << endl;
}

// print function code
void Material::print() {
    cout << "This is a Material object!" << endl;
}
