#include <functional>

#pragma once

class CharClass {
public:
    CharClass(const CharClass& other) = default;
    CharClass(CharClass&& other) = default;
    CharClass(std::function<bool(char)> classFn) :
      m_classFn{ classFn }
    {}

    CharClass& operator = (const CharClass& other) = default;
    CharClass& operator = (CharClass&& other) = default;

    bool operator () (char c) const {
        return m_classFn(c);
    }

    static CharClass combine(const CharClass& classOne, const CharClass& classTwo);
    static CharClass intersect(const CharClass& classOne, const CharClass& classTwo);
    static CharClass complement(const CharClass& charClass);

    static CharClass matchSingle(char matchChar);

private:
    std::function<bool(char)>   m_classFn;
};

CharClass operator ~ (const CharClass& charClass);
CharClass operator & (const CharClass& classOne, const CharClass& classTwo);
CharClass operator | (const CharClass& classOne, const CharClass& classTwo);
