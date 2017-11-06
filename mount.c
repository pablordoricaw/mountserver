/******************************************************************************
 * author: Pablo Ordorica <pablo_ordoricaw@hotmail.com>
 * file: mount.c
 * Description: C file to extend filesystem using sshfs"
 ******************************************************************************/

#include <stdio.h>
#include <getopt.h>
#include <stdlib.h>
#include <string.h>

#define MAX_STR_LEN (256)

#define PRINT_CMD_ERROR(command){                                              \
    fprintf(stderr,"ERROR: executing command %s in %s:%d\n",command, __FILE__, \
            __LINE__);                                                         \
}

void mount();
void unmount();
void openfile(char * filename);

char readStr[2][MAX_STR_LEN];

int main(int argc, char* argv[]){
    int rc;
    int opt_index = 0; //store option index
    char* getoptOptions = "m:s:u:";//short commaand line parameter list

    //long command line parameter list
    struct option long_options[] = {
        {"mount",   required_argument, 0, 'm'},
        {"stop",    required_argument, 0, 's'},
        {"unmount", required_argument, 0, 'u'},
        {0, 0, 0, 0}
    };
    opterr = 1; //enable automatic error reporting

    while((rc = getopt_long_only(argc, argv, getoptOptions, long_options,
                    &opt_index)) != -1){
        switch(rc){
            case 'm': //mount
                openfile(optarg);
                mount();
                break;
            case 's'://stop
                openfile(optarg);
                unmount();
                break;
            case 'u'://unmount
                openfile(optarg);
                unmount();
                break;
            case '?': // Handled by default error handler
                break;
            default:
                printf("ERROR: undefined option %0xX\n", rc);
                exit(99);
        }
    }
    if(optind < argc || argc == 1){
        fprintf(stderr,"usage: ./mount -m[ount] -s[top] -u[nmount] filename\n");
        exit(99);
    }
}

/*******************************************************************************
 * Description: open file and store the two lines in global variable readStr
 * Parameters: filename: char* with the name of the file to open
 * Return: Nothing but exit if file not found
 ******************************************************************************/
void openfile(char * filename){
    FILE * inputfile = fopen(filename, "r");
    if(inputfile){
        int i = 0;
        //read the user@hostname:/dir into readStr[0]
        //read the mountpoint into readStr[1]
        while(fscanf(inputfile, "%s", readStr[i++]) != EOF);
    }
    else{
        fprintf(stderr, "ERROR: file: %s not found\n", filename);
        exit(10);
    }
}

/*******************************************************************************
 * Description: use the sshfs bash with the data stored in readstr to mount
 *              the server
 * parameters: none
 * return: nothing
 ******************************************************************************/
void mount(){
    char command[MAX_STR_LEN] = "sshfs "; //sshfs command
    char mkdir[MAX_STR_LEN] = "mkdir -p "; //mkdir for mountpoint command

    //make directory for mount point in case it doesn't exist
    strcat(mkdir, readStr[1]);
    if(system(mkdir) == -1){
        PRINT_CMD_ERROR(mkdir);
        exit(-1);
    }

    //sshfs to extend directory
    strcat(command, readStr[0]);
    strcat(command, " ");
    strcat(command, readStr[1]);
    printf("Mounting server directory %s\n", readStr[0]);
    if(system(command) == -1){
        PRINT_CMD_ERROR(command);
        exit(-1);
    }
}
/*******************************************************************************
 * Description: use the umount bash with the data stored in readstr to unmount
 *              the server
 * parameters: none
 * return: nothing
 ******************************************************************************/
void unmount(){
    char command[MAX_STR_LEN] = "umount " ; //umount command

    //umount to unmount the server directory
    strcat(command, readStr[1]);
    printf("Unmounting server directory %s\n", readStr[0]);
    if(system(command) == -1){
        PRINT_CMD_ERROR(command);
        exit(-1);
    }
}
