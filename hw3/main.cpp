#include <iostream>
#include "GenScatterHierarchy.h"
#include <string>


template <class T>
struct Holder {
    T value;
};

using WidgetInfo = GenScatterHierarchy<TypeList<int, std::string, double>, Holder>;

int main() {
    WidgetInfo obj;
    std::string name = static_cast<Holder<std::string>&>(obj).value;
    std::string name1 = Field<std::string>(obj).value;
    std::cout << "Hello, World!" << std::endl;
    return 0;
}