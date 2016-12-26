#include <iostream>
#include <string>

#include "../StringView.h"

int main() {
    std::string text = "Hello world!";

    for (size_t i = 0; i < text.size(); ++i) {
        StringView view{ text, text.size() - i };

        for (char c : view) {
            std::cout << c;
        }
        std::cout << '\n';

        std::cout << view << '\n';

        if (view == text) {
            std::cout << "Yep.\n";
        } else {
            std::cout << "Nope.\n";
        }
    }
    return 0;
}
