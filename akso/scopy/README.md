## special file copying

The program performs a modified operation of copying files.

Program recieves two parameters:  
the name of the input file and the name of the output file.  
Program creates the output file and for each read from the  
input file byte which is equal to 's' or 'S', it  
writes the byte to the output file. For each  
maximum, non-empty subsequence of the input file  
that does not contain letter 's', nor 'S', program writes  
to the output file a 16-bit number representing the  
length of that subsequence mod 65536.

Program uses Linux system functions and buffers for  
the input and te output file. In case of any error  
occuring during execution of a system call, or argument  
incompability, Program ends with a return value 1.

Program is implemented using assembly x86  
and it can be compiled using the following commands:  
nasm -f elf64 -w+all -w+error -o scopy.o scopy.asm  
ld --fatal-warnings -o scopy scopy.o
