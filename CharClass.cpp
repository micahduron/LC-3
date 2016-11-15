#include "CharClass.h"

CharClass operator | (const CharClass& classOne, const CharClass& classTwo) {
    return {
        [classOne, classTwo] (char c) -> bool {
            return classOne(c) || classTwo(c);
        }
    };
}
