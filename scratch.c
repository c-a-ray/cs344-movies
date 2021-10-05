#include <stdio.h>

int main() {
    char* t = "this is not a test";
    char m[50];

    for (int i = 0; i < 5; i++) {
        m[i] = t[i];
    }

    printf("%s", m);
    return 0;
}