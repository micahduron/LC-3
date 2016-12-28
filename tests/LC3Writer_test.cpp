#include <iostream>
#include "../LC3Writer.h"

int main() {
    LC3Writer writer("LC3Writer_test.obj");

    writer.put(0x3000);
    writer.put(0);
    writer.put(1);

    return 0;
}
