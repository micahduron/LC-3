#include "CharClass.h"

CharClass combine(const CharClass& classOne, const CharClass& classTwo) {
    return {
        [classOne, classTwo] (char c) -> bool {
            return classOne(c) || classTwo(c);
        }
    };
}
CharClass intersect(const CharClass& classOne, const CharClass& classTwo) {
    return {
        [classOne, classTwo] (char c) -> bool {
            return classOne(c) && classTwo(c);
        }
    };
}
CharClass complement(const CharClass& charClass) {
    return {
        [charClass] (char c) -> bool {
            return !charClass(c);
        }
    };
}

CharClass operator | (const CharClass& classOne, const CharClass& classTwo) {
    return CharClass::combine(classOne, classTwo);
}
