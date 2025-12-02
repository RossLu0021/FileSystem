
# Makefile for lab 1-4.

# Change the last dependency for each new lab
all: headers lab3

# Rule to compile lab3
lab3: lab3.o commands.o utility.o 
	gcc -g -o $@ $^

# Rule to compile commands.o into the commands executable
commands: commands.o
	gcc -g -o $@ $^

# Rule to compile utility.o into the utility executable
utility: utility.o
	gcc -g -o $@ $^


# Rule to create lab3.zip
lab3.zip: makefile *.c *.h *.awk readme
	zip lab3.zip makefile *.c *.h *.sh *.awk readme
	rm -rf install
	mkdir install
	unzip lab3.zip -d install
	make -C install lab3

# Rule to generate tags
tags: *.c
	ctags -R .

# Rule to generate headers
headers: *.c
	./headers.sh

# Rule to compile .c files into .o files
%.o: %.c *.h
	gcc -ansi -pedantic -Wimplicit-function-declaration -Wreturn-type -g -c $< -o $@
