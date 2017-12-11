## C program that utilizes sshfs in order to extend your home directory.

Do you prefer using a text editor such as Sublime or Atom over Vim, but have
to compile and execute your code on a server through the terminal? With this
program you can mount the server directory onto your computer. This allows you
to edit your code with the text editor of your choice, but compile and execute
it on the server.

Works even better if an ssh key pair has been setup between your computer and
the server. Here's [DigitalOcean](goo.gl/Vk5au) tutorial on how to generate and
setup the ssh keys.

### Tested environments:
__macOS__ -> [FUSE for macOS](https://osxfuse.github.io/) required.

**Tested functionality:**
Mount         :white_check_mark:  
Un-mount      :white_check_mark:  
Force unmount :white_check_mark:  
 List mounted  :white_check_mark:  

__Ubuntu__ -> sshfs required. Download by typing the following command in the
              terminal and inputting your user password:

              sudo apt-get install sshfs

**Tested functionality:**
Mount         :white_check_mark:  
Un-mount      :white_check_mark:  
Force unmount :x:  
List mounted  :x:  

### Usage:
1. Write an input file that holds the desired directory on the server
(user@host:dir) to mount onto your computer on the first line, and the mount
point directory on your computer in the second line. An example can be found in
medusa.txt .

2. Create the C executable with the Makefile by using the command `make`.

3. Run the program as follows:
    - To mount the directory
      ```
      ./mount_c -m[ount] filename
      ```
    - To display list of mounted directories
    ```
    ./mount_c -i[nfo] filename
    ```
    - To unmount the directory
      ```
      ./mount_c -s[top]/-u[nmount] filename
      ```
      or
      ```
      ./mount_c -u[nmount] filename
      ```
