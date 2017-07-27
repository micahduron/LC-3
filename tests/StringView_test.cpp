#include <iostream>
#include <string>

#include "../util/StringView.h"

using Util::StringView;

int main() {
    std::string text = "Hello world!";
    StringView view(text);

    if (view != text) {
        std::cerr << "Equality operator (" <<
            "View: " << view << ", " <<
            "String: " << text << ")\n";
        return 1;
    }
    for (size_t i = 0; i < text.size(); ++i) {
        StringView subView = view.subString(0, view.size() - i);

        if (subView.compare(text) == text[i]) {
            std::cerr << "StringView::compare() (" <<
                "Index: " << i << ", " <<
                "View: " << subView << ")\n";
            return 1;
        }
    }
    std::string tmpText;
    tmpText.reserve(text.size());

    for (const char& c : view) {
        tmpText += c;
    }
    if (view != tmpText) {
        std::cerr << "Ranged for-loop (" <<
            "View: " << view << ", " <<
            "String: " << tmpText << ")\n";
        return 1;
    }
    tmpText.clear();

    for (size_t k = 0; k < view.size(); ++k) {
        tmpText += view[k];
    }
    if (view != tmpText) {
        std::cerr << "Subscript operator (" <<
            "View: " << view << ", " <<
            "String: " << tmpText << ")\n";
        return 1;
    }
    return 0;
}
