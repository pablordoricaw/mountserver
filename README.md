## Bash script that utilizes sshfs in order  your home directory.

Do you prefer using a text editor such as Sublime or Atom over Vim, but have 
to compile and execute your code on a server through the terminal? With this 
script you can mount the server directory onto your computer, and edit your code 
with the text editor of your choice while still compile and execute it on the 
server.

### Tested environments:
__macOS__ -> [FUSE for macOS](https://osxfuse.github.io/) required.

### Usage: 
1. Change the variables in psync file to the desired user, host domain and 
mount point.
    
2. Run the script as follows:
        
```
./psync hostname
```

