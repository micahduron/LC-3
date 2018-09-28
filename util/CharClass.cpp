#include "CharClass.h"

namespace Util {

CharClass CharClass::combine(const CharClass& classOne, const CharClass& classTwo) {
    return CharClass {
        [&classOne, &classTwo] (char c) {
            return classOne(c) || classTwo(c);
        }
    };
}
CharClass CharClass::intersect(const CharClass& classOne, const CharClass& classTwo) {
    return CharClass {
        [&classOne, &classTwo] (char c) {
            return classOne(c) && classTwo(c);
        }
    };
}
CharClass CharClass::complement(const CharClass& charClass) {
    return CharClass {
        [&charClass] (char c) {
            return !charClass(c);
        }
    };
}

CharClass operator ~ (const CharClass& charClass) {
    return CharClass::complement(charClass);
}
CharClass operator & (const CharClass& classOne, const CharClass& classTwo) {
    return CharClass::intersect(classOne, classTwo);
}
CharClass operator | (const CharClass& classOne, const CharClass& classTwo) {
    return CharClass::combine(classOne, classTwo);
}

} // namespace Util
