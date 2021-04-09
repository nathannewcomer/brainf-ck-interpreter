#include <stdio.h>
#include <stdlib.h>

struct Instr {
    char instruction;
    struct Loop *loop;
    struct Instr *next;
};

struct Loop {
    struct Instr *instrs;
};

void parseInstructions(FILE *file, struct Instr *instr);
void parseLoop(FILE *file, struct Loop *loop);
void printTree(struct Instr *instr);
void executeInstructions(struct Instr *instr, char **ptr);
void executeLoop(struct Loop *loop, char **ptr);
void freeTree(struct Instr *instr);

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

    // create AST
    struct Instr *program = (struct Instr *)malloc(sizeof(struct Instr));
    parseInstructions(file, program);

    //printf("printing tree...\n");
    //printTree(program);

    // execute program
    char memory[30000] = {0};
    // have to use double pointer because we use recursion
    char *ptr = &memory[0];

    executeInstructions(program, &ptr);

    // free AST
    freeTree(program);
}

// turn the source file into an AST
void parseInstructions(FILE *file, struct Instr *instr) {
    char c = fgetc(file);
    struct Instr *currentInstr = instr;

    // see if there's an instruction
    while (c != EOF) {
        switch (c) {
            case '>':
            case '<':
            case '+':
            case '-':
            case '.':
            case ',':
                // regular instruction
                currentInstr->instruction = c;
                currentInstr->loop = NULL;
                break;
            case '[':
                // loop
                currentInstr->loop = (struct Loop *)malloc(sizeof(struct Loop));
                currentInstr->instruction = -1;
                parseLoop(file, currentInstr->loop);
                break;
            case ']':
                // this shouldn't happen
                printf("] happened!\n");
                return;
            default:
                // non-instruction: ignore
                break;
        }

        c = fgetc(file);

        if (c != EOF && c != ']') {
            currentInstr->next = (struct Instr *)malloc(sizeof(struct Instr));
            currentInstr = currentInstr->next;
        } else {
            currentInstr->next = NULL;
            return;
        }
    }

    // EOF reached, no next instruction
    currentInstr = NULL;
}

void parseLoop(FILE *file, struct Loop *loop) {
    loop->instrs = (struct Instr *)malloc(sizeof(struct Instr));
    parseInstructions(file, loop->instrs);
}

// prints the AST for debug purposes
void printTree(struct Instr *instr) {
    struct Instr *currentInstr = instr;

    while (currentInstr != NULL) {
        if (currentInstr->instruction != -1) {
            putchar(currentInstr->instruction);
        } else if (currentInstr->loop != NULL) {
            printf("[");
            printTree(currentInstr->loop->instrs);
            printf("]");
        }

        currentInstr = currentInstr->next;
    }
}

void executeInstructions(struct Instr *instr, char **ptr) {
    while (instr != NULL) {
        //printf("ptr = %d\n*ptr = %d\n", ptr, *ptr);
        // execute loop
        if (instr->loop != NULL) {
            executeLoop(instr->loop, ptr);
        // execute instruction
        } else {
            //printf("Executing instruction %c\n", instr->instruction);
            //printf("memory[%p] = %d\n", ptr, *ptr);

            switch (instr->instruction) {
                case '>':
                    ++*ptr;
                    break;
                case '<':
                    --*ptr;
                    break;
                case '+':
                    ++**ptr;
                    break;
                case '-':
                    --**ptr;
                    break;
                case '.':
                    putchar(**ptr);
                    break;
                case ',':
                    **ptr = getchar();
                    break;
                default:
                    // shouldn't happen but include anyway
                    //printf("bad instruction!\n");
                    break;
            }
        }

        instr = instr->next;
    }
}

void executeLoop(struct Loop *loop, char **ptr) {
    // loop until the cell is 0
    while (**ptr) {
        executeInstructions(loop->instrs, ptr);
    }
}

void freeTree(struct Instr *instr) {
    struct Instr *currentInstr = instr;
    struct Instr *nextInstr;

    while (currentInstr != NULL) {
        if (currentInstr->loop != NULL) {
            freeTree(currentInstr->loop->instrs);
        }

        nextInstr = currentInstr->next;
        free(currentInstr);
        currentInstr = nextInstr;
    }
}