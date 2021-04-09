# the compiler: gcc for C program, define as g++ for C++
CC = clang

# compiler flags:
#  -g    adds debugging information to the executable file
#  -Wall turns on most, but not all, compiler warnings
CFLAGS  = -g -Wall

# the build target executable:
BFITERATIVE = brainfuckIterative
BFAST = brainfuckAST

all: $(BFITERATIVE) $(BFAST)

$(BFITERATIVE): $(BFITERATIVE).c
	$(CC) $(CFLAGS) -o $(BFITERATIVE) $(BFITERATIVE).c

$(BFAST): $(BFAST).c
	$(CC) $(CFLAGS) -o $(BFAST) $(BFAST).c

clean:
	$(RM) $(BFITERATIVE) $(BFAST)