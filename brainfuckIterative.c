#include <stdio.h>

void interpret(FILE *file);

int main(int argc, char *argv[]) {
    if (argc != 2) {
        printf("Usage: brainfuck <file-name>\n");
        return 0;
    }

    // open file
    FILE *file = fopen(argv[1], "r");

    if (file == NULL) {
        printf("Error opening file.\n");
        return 1;
    }

    // interpret program
    interpret(file);
}

void interpret(FILE *file) {
    char memory[30000] = {0};
    char *ptr = &memory[0]; // using a pointer makes it easier
    char c;
    int i;

    while (!feof(file)) {
        c = fgetc(file);

        switch (c) {
            case '<':
                --ptr;
                break;
            case '>':
                ++ptr;
                break;
            case '+':
                ++*ptr;
                break;
            case '-':
                --*ptr;
                break;
            case '.':
                putchar(*ptr);
                break;
            case ',':
                *ptr = getchar();
                break;
            case '[':
                // jump past matching ] if *ptr is 0
                if (*ptr == 0) {
                    i = 1;
                    do {
                        c = fgetc(file);
                        if (c == '[') {
                            i++;
                        } else if (c == ']') {
                            i--;
                        }
                    } while (i != 0);
                }
                // otherwise run the loop
                break;
            case ']':
                if (*ptr != 0) {
                    i = -1;
                    do {
                        fseek(file, -2, SEEK_CUR);
                        c = fgetc(file);
                        if (c == '[') {
                            i++;
                        } else if (c == ']') {
                            i--;
                        }
                    } while (i != 0);
                }
                break;
            default:
                // ignore all other characters
                break;
        }
    }
}