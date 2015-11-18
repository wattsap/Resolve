Lab 5: Unix commands, pipes, and linking

Unix commands, pipes, and linking
I. Overview
For this lab, you'll perform the symbol resolution step of a static linker using a sequence of .o files and archive files for input. Your program should output an error message for:

multiply defined symbols (two symbols with the same name), and
undefined symbols.
If there are no errors, the program should not produce any output.

/* main.c */
extern int global;
int main()
{
foobie();
}	/* sub.c */
int global;
void foobie()
{
}

For example, assume the name of your program is resolve. Using the code above, invoking resolve with the two object modules above, main.o and sub.o, will not cause any output to be produced since there are no errors:

% resolve main.o sub.o

On the other hand, if resolve is invoked with only main.o as input, resolve will indicate that global and foobie are undefined:

% resolve main.o
: undefined reference to global
: undefined reference to foobie

If resolve is invoked with only sub.o as input, resolve will indicate that main is undefined.

% resolve sub.o
: undefined reference to main

If resolve is invoked with main.o, sub.o and sub.o again, resolve will indicate that global and foobie are multiply defined.

% resolve main.o sub.o sub.o
: multiple definition of global
: multiple definition of foobie

The symbol resolution process is described on page 670 in your textbook. The linker scans the relocatable object files and archive files left to right, maintaining two sets, U - the set of unresolved symbols and D - the set of defined symbols. Initially, U and D are empty.

If the input file is an object file, the linker adds the defined symbols to the D set and the undefined symbols to the U set. Note that the linker can tell if a symbol is defined or undefined by examining the Type field in the symbol table.
If the input file is an archive, the linker uses the existing symbols in the U and D sets to determine whether to add the symbols of an archive member (archive members are object files) to the U and D sets. If a member is added, it is added in its entirety. The addition of an archive member may cause an addition to the U set, thus the linker iterates over the archive members until the U and D sets stop changing. There are two cases in which the linker decides to add the archive member:
A symbol in the U set (an undefined symbol) is defined by a member of the archive. The symbol in the archive member must be a strong symbol.
A symbol in the D set is a weak symbol and the member of the archive contains a strong symbol of the same name.
If during this process the linker discovers a symbol is multiply-defined (it finds a strong symbol to add to the D set and a strong symbol of that name is already in the D set), then an error message like above is output.

At the end of this process, the linker first checks to see if main is defined. If not, an error message is displayed. Next, the linker visits each element in the U set and outputs an error message for each symbol in the U set.

II. Unix Pipes

Rather than writing the code to read the symbol table from the elf file, we're going to use the Unix nm utility to obtain the symbol table. The nm utility produces output like this:

% nm sub.o

00000000

00000000 

00000004 
00000000	T
U 
D 
U 
C 
b	foo
goobie
initglobal
myextern
myglobal
mylocal

The leftmost column is the symbol value. In relocatable files, the symbol value holds alignment constraints for a symbol whose section index is COMMON. For defined symbols, the symbol value is a section offset. We do not need the symbol value for this assignment.

The middle column holds the symbol type. You'll need to add this type, along with the symbol name in the rightmost column, to your set of defined and undefined symbols. Note that U indicates the symbol is undefined. Symbols that are undefined will go in the list of undefined symbols (unless the symbol is defined already in the defined list in which case the undefined symbol is simply ignored). The strong symbols are those with a type of T (.text section) or a type of D (.data section). You'll use this type to determine whether a symbol is multiply defined. If there is already a symbol in the defined set with a type of D or T when your program encounters another symbol with the same name also of type D or T, then that symbol is multiply defined.

To use the output of nm from within your C program, you are going to need a Unix pipe. A pipe is a special kind of file that is shared by two processes. The two processes open the file, one for reading and the other for writing. This can allow the output of one process to be read by another process, or one process to provide input to another process. Pipes are unidirectional, thus one process can write to the pipe and another can read, but not both. In this case, we'll use the popen command to create our pipe and start the second process. Here's an example use of popen:
#include <stdio.h>
.
.
FILE *fp;
fp = popen("nm sub.o", "r");
//you need to write this displayMessageAndExit function
if (fp == NULL) displayMessageAndExit("popen failed\n");
while (fgets(buffer, sizeof(buffer), fp))
{

.
.
.
}
pclose(fp);

The code above creates a pipe from which my program can read. The output of the command executed (nm sub.o) is written to the pipe. The command itself is executed by creating a new shell process in which the command is run. Notice that the pipe is read from just like it is a regular file. The first fgets will read the first line of the nm output. You'll need to check the first character to see if the line contains a symbol value (it won't if the symbol is undefined) and then execute one of two sscanfs to obtain either the value, type and name or just the type and name. (Google sscanf.)

III. Unix commands

If you need to execute a Unix command from within your C program but you don't need to read the output of the command, you can use the system function. Here is an example of the use of the system function:
#include <stdlib.h>
.
.
if (system("mkdir tmp") == -1) displayMessageAndExit("mkdir tmp failed");

The system function creates a new shell process and passes the command passed to it to the shell process. 

IV. The U and D sets

Create a linked list in C to implement the U and D sets. You'll need functions for the following:

inserting into a linked list - to provide output that is consistent with mine, you'll always insert at the end
updating the type of a symbol in a linked list - if your defined symbols list contains a weak symbol, you'll want to update the type of that symbol (not D or T) to the type of a strong symbol (D or T) if a strong symbol of that name is later encountered. You don't want two symbols with the same name in your symbol lists.
getting the symbol type of a symbol with a particular name - you'll need this to check for multiply defined symbols. This function should return false if the symbol is not in the list, thus, you'll need to "return" two values -- one of those will be via the parameters.
remove a symbol from a list - if a symbol is at first undefined and then later defined, you'll need to remove it from the undefined list.
print symbols - this is useful for debugging
a symbol list iterator - this should take as input a pointer to a symbol element. It will "return" (via the parameter list) the name and type of the symbol element and increment the pointer. You'll need this to iterate through the U list after all the command line arguments are processed.
You should put all of these functions in their own file, symbolList.c, and have prototypes for the functions in the file symbolList.h. Since you are going to manage two linked lists, you'll need to declare two pointers, one to point to each of these lists.

Here is how you declare a linked list in C:
typedef struct symbolEntry
{

char type;
char name[31];
struct symbolEntry * next;
} symbolEntry;

The name array is declared to be of length 31 because this is the limit of the length of C identifiers. (I think. I read this in one location, but haven't actually verified it. However, this will work for our purposes.)

The variables that point to the first node in each of the linked lists would be declared like this:
symbolEntry * defined = NULL;
symbolEntry * undefined = NULL;

The functions that operate on the list will need to be passed a pointer to the first node of the list (either defined or undefined above). For example, here is the prototype for the function that looks for a symbol in the list with a particular name (symbolName) and sets the variable pointed to by the type parameter to the type of that symbol if it is in the list. It returns true or false depending upon whether the symbol is found.
bool getSymbolType(symbolEntry *, char * symbolName, char * type);

Some functions will need to be able to modify the pointer to the first node. In that case, you'll need to pass to the function the address of the pointer. For example, here is the prototype for the function that inserts a symbol into the list:
void insertSymbol(symbolEntry ** first, char * symbolName, char type);
Notice that the first parameter is a pointer to a pointer. You'll need to do this for the function that removes a symbol and the iterator, as well. 

You'll need to use malloc to allocate space for the linked list nodes and free to deallocate that space when a node is removed. For example, here is the code to allocate space for one node:
symbolEntry * entry = (symbolEntry *)malloc(sizeof(symbolEntry));

V. Handling Object Files

You'll need to build the appropriate nm command and use popen as described above to read the output of the nm command. Except in the case of a symbol being multiply defined, each symbol that is displayed by nm will be either added to the defined symbol list, added to the undefined symbol, or used to update the type of a symbol on the defined symbol list (from weak symbol to strong symbol). In addition, a defined symbol may cause an undefined symbol to be removed from the undefined list. You'll also need to check whether a symbol is multiply defined. (You've encountered two strong definitions.) If so, output an error message, and continue to process the symbols. Don't add the second definition to the symbol list.

VI. Handling Archive Files

A static library archive (.a) is a collection of relocatable object files. A relocatable object file within the library will either be added in its entirety to the executable being created or not added at all. It is added in its entirety if the object file provides a definition of a currently undefined symbol or if the object file contains a strong symbol with the same name as a weak symbol in the D set. Your program needs to examine each object file within the library to see whether it should be added. If the object file should be added, it can be handled just like a regular object file (using the same code). You will need to iterate through the object files multiple times because adding an object file may cause an undefined symbol to be added to the list and this undefined symbol could be defined by an earlier visited object file in the archive. Here's the idea:

do

changed = false
For each object file, Oi, within archive
If Oi should be included in executable
add symbols of Oi to defined/undefined lists (updating as necessary)
changed = true
while changed == true

How do you retrieve the object files from the archive? You'll need to use the system and popen functions described earlier. Make a call to system to create a tmp directory and another system call to copy the archive file into the tmp directory. Next, pass to a system call the ar command to extract the object files from the archive. In this case, you'll need to execute two commands in the context of one shell, one to get into the tmp directory and one to do the extract . This is the call for an archive named lib.a:
system("cd tmp; ar -x lib.a");
Note that you'll need to build the command using the actual name of the archive. Next, use the popen command to execute and get the output "ls tmp". This will provide you with the name of each object file, which you can now process as described above. When you are finished processing the archive, use the system command to delete the tmp directory and its contents ("rm -f -r tmp").

VII. Final Notes


I've provided some starting code and tests. Create a lab5 directory within your 3482 directory, get into the directory and type the following:
cp /u/css/classes/3482/154/lab5/lab5.tar .
tar xvf lab5.tar
The resolve.c file contains the main and code to handle the command line arguments. You'll need to write the functions to handle the object files and archive files. Don't do all of that work in two functions, however. For example, the handleObjectFile function can execute the nm on the object file and pass each symbol to another function to handle the processing of the symbol.
Create symbolList.c and symbolList.h files to provide the linked list functionality. Your undefined and defined pointers should be declared in the resolve.c file.
Create a makefile that will compile resolve.c, symbolList.c and create an executable called resolve.
You've been provided with an instrResolve executable that is my resolve implementation. You can also use gcc to determine what the output of your program should be. gcc will indicate when symbols are multiply defined or undefined; however, the gcc output is more complicated than the output your program should produce.
There are four test directories. Each of those contains a run.pl that can be used for automated testing. You can run all of the tests (all of the run.pl scripts) by executing the run.pl in the lab5 directory. To support automated testing, your program has to produce the exact same output as mine. It will produce the same output if:
Your multiply defined error message is of the form: ": multiple definition of symbolname"
Your undefined error message is of the form: ": undefined reference to symbolname"
You process the symbols displayed by nm in the order that they are displayed. (I don't know why you would process them in any other order.)
You always insert at the end of the linked list.
You output a "multiple definition" error message as soon as your program encounters it, and continue processing.
At the end of the processing of all command line arguments, you check whether main is defined (and output an error message if it is not) before you print error messages for the symbols in the undefined list.
