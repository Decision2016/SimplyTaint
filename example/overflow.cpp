#include <unistd.h>

const char *INPUT_DATA = "input";

void func(char *str, int count) {
    char temp[15];
    printf("Run write function, source: %s\n", str);
    write(1, str, count);                   // taint sink
}

int main(int argc, char *argv[]) {
    char source[30];
    int readNum = read(0, source, 100);                // taint source
    if (strlen(source) < 30) {
        func(source, readNum);
    }
    else {
        printf("String too long: %s\n", source);
    }
    return 0;
}