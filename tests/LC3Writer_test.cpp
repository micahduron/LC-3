#include <iostream>
#include <lc3/Word.h>
#include <LC3Writer.h>

int main() {
    LC3Writer writer("LC3Writer_test.obj");

    writer.putWord(0x3000);
    writer.putWord(0);
    writer.putWord(1);

    return 0;
}
