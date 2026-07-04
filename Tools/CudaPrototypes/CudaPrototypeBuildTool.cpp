#include <fstream>
#include <iostream>

int main(int argc, char* args[]) {
    if (argc < 3) {
        return -1;
    }
    for (size_t i = 0; i < argc; i++) {
        std::cout << args[i] << std::endl;
    }

    std::ofstream test("test.cpp");
    test << "Hello World!" << std::endl;
    test.close();

    return 0;
}
