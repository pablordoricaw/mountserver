## C program that utilizes sshfs in order to extend your home directory.

Do you prefer using a text editor such as Sublime or Atom over Vim, but have 
to compile and execute your code on a server through the terminal? With this 
program you can mount the server directory onto your computer, and edit your 
code with the text editor of your choice while still compiling and executing it 
on the server.

### Tested environments:
__macOS__ -> [FUSE for macOS](https://osxfuse.github.io/) required.

### Usage: 
1. Write an input file that holds the user@host:dir on the first line and the
mountpoint directory on the second line. An example can be found in medusa.txt
    
3. Run the program as follows:
    - To mount the directory
```
./mount_c -m[ount] filename
```

    - To unmount the directory
```
./mount_c -s[top] filename
```
or
```
./mount_c -u[nmount] filename
```
