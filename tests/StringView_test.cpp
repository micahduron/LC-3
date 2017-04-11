#include <iostream>
#include <string>

#include "../StringView.h"

int main() {
    std::string text = "Hello world!";
    StringView view(text, text.size());

    for (size_t i = 0; i < text.size(); ++i) {
        StringView subView = view.subString(0, view.size() - i);

        for (char c : subView) {
            std::cout << c;
        }
        std::cout << '\n';

        std::cout << subView << '\n';

        std::cout << subView.compare(text) << '\n';
    }
    return 0;
}
