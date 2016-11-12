#include <utility>
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

private:
    std::function<bool(char)>   m_classFn;
};
CharClass operator | (const CharClass& classOne, const CharClass& classTwo) {
    return {
      [classOne, classTwo] (char c) -> bool {
          return classOne(c) || classTwo(c);
      }
    };
}
