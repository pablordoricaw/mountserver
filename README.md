##Bash script that utilizes sshfs in order to mount a server directory unto your home directory.

###Tested environments:
    macOS -> __[FUSE for macOS] (https://osxfuse.github.io/) required__.
    
###Usage: 
    1. Change variables in psync.sh file to desired user, host domain and mount point.
    
    1. Run the script as follows:
        ```
        ./psync hostname
        ```