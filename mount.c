/******************************************************************************
* author: Pablo Ordorica <pablo_ordoricaw@hotmail.com>
* file: mount.c
* Description: C file to extend filesystem using sshfs"
******************************************************************************/

//header files
#include <stdio.h>
#include <getopt.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>

//max string length macro
#define MAX_STR_LEN (256)
#define SYS_CALL_ERR (-1)
#define RESOURCE_BSY (256)

//use to determine what unmount command to use based on the OS
#if defined(__APPLE__) || defined(__MACH__)
    #define UNMOUNT_CMD "umount "
#elif defined(__linux__) || defined(linux) || defined(__linux)
    #define UNMOUNT_CMD "fusermount -u "
#endif

//sshfs command print error macro
#define PRINT_CMD_ERROR(command){                                              \
		fprintf(stderr,"ERROR: executing command %s in %s:%d\n",command,       \
		        __FILE__, __LINE__);                                           \
}

//function declaration
static void mount();
static void unmount();
static void funmount();
static char* copyfilename(char * optarg);
static void openfile(char * filename);

//global static variables
static char readStr[2][MAX_STR_LEN];//readStr[0]: user@hostname:/dir into
                                    //readStr[1]: the mountpoint directory
static char buffer[MAX_STR_LEN];

int main(int argc, char* argv[]){
	int rc;
	int opt_index = 0;              //store option index
	char* getoptOptions = "m:s:u:"; //short commaand line parameter list
	char* filename = NULL;             //file pointer
	int moru = 0;               //used as boolean: 0 - try to unmount
	                            //                 1 - try to mount

	//long command line parameter list
	struct option long_options[] = {
		{"mount",   required_argument, 0, 'm'},
		{"stop",    required_argument, 0, 's'},
		{"unmount", required_argument, 0, 'u'},
		{0, 0, 0, 0}
	};
	opterr = 1; //enable automatic error reporting

	while(-1 != (rc = getopt_long_only(argc, argv, getoptOptions, long_options,
	                                   &opt_index))) {
		//printf("getopt_long_only() returned ='%c' index = '%d'\n",  rc, opt_index);
		switch(rc) {
		case 'm': //mount
			moru = 1;
			filename = copyfilename(optarg);
			break;
		case 's'://stop
			moru = 0;
			filename = copyfilename(optarg);
			break;
		case 'u'://unmount
			moru = 0;
			filename = copyfilename(optarg);
			break;
		case '?': // Handled by default error handler
			break;
		default:
			printf("ERROR: undefined option %0xX\n", rc);
			exit(99);
		}
	}
	if(optind < argc || (3 != argc)) {
		fprintf(stderr,"usage: ./mount_c -m[ount] -s[top]/-u[nmount] filename\n");
		exit(99);
	}

	if(NULL != filename) {
		//open file
		printf("Opening input file: %s\n", filename);
		openfile(filename);
		if(1 == moru) {
			//try to mount
			mount();
		} else if(0 == moru) {
			//try to unmount
			unmount();
		} else{
			fprintf(stderr, "ERROR: internal\n");
			exit(99);
		}


		//deallocate mem used for filename
		free(filename);
	} else{
		fprintf(stderr, "ERROR: internal error saving the inputfile name\n");
		return -1;
	}
	return 0;
}
/*******************************************************************************
 * Description: dynamically allocates and stores the filename
 * Parameters: optarg: char* contains the filename
 * Return:     filename: char* containing the filename
 ******************************************************************************/
static char* copyfilename(char * optarg){
	if(optarg) {
		//length of optarg
		int optlen = strlen(optarg);
		//allocate memory for filename
		char* filename = (char*)malloc(sizeof(char) * (optlen + 1));
		if(NULL == filename) {
			//throw error if not enough memory allocated
			fprintf(stderr, "ERROR: couldn't allocate memory for filename\n");
			exit(-99);
		}
		//store the filename
		strncpy(filename, optarg, optlen);
		filename[optlen] = '\0'; //null terminate string

		return filename;
	}
	return NULL; //error
}
/*******************************************************************************
 * Description: open file and store the two lines in global variable readStr
 * Parameters: filename: char* with the name of the file to open
 * Return: Nothing but exit if file not found
 ******************************************************************************/
static void openfile(char * filename){
	FILE * inputfile = fopen(filename, "r");
	if(inputfile) {
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
static void mount(){
	char command[MAX_STR_LEN] = "sshfs "; //sshfs command
	char mkdir[MAX_STR_LEN] = "mkdir -p "; //mkdir for mountpoint command

	//make directory for mount point in case it doesn't exist
	strcat(mkdir, readStr[1]);
	if(SYS_CALL_ERR == system(mkdir)) {
		PRINT_CMD_ERROR(mkdir);
		exit(-1);
	}

	//sshfs to extend directory
	strcat(command, readStr[0]);
	strcat(command, " ");
	strcat(command, readStr[1]);
	printf("Mounting server directory %s\n", readStr[0]);
	if(SYS_CALL_ERR == system(command)) {
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
static void unmount(){
	char command[MAX_STR_LEN] = UNMOUNT_CMD; //umount command

	//umount to unmount the server directory
	strcat(command, readStr[1]);
	printf("Unmounting server directory %s\n", readStr[0]);

	//try to unmount
	int syscall = system(command);

	if(SYS_CALL_ERR == syscall) {
		PRINT_CMD_ERROR(command);
		exit(-1);
	} else if(RESOURCE_BSY == syscall) { //if resource busy, then ask if force
		funmount();
	}
}
/*******************************************************************************
 * Description: use the umount bash with the data stored in readstr to force
 *              unmount the server
 * parameters: none
 * return: nothing
 ******************************************************************************/
static void funmount(){
	char command[MAX_STR_LEN] = UNMOUNT_CMD; //umount command

	printf("Do you want to force unmount? (y/n): ");
	if(NULL != fgets(buffer, MAX_STR_LEN, stdin)) {
		int bufflen = strlen(buffer)-1;
		//removing \n at end of response from user
		if(*buffer && '\n' == buffer[bufflen]) {
			buffer[bufflen] = '\0';
		}

		//put everything in lower case
		for(int i = 0; i <=bufflen; i++) {
			buffer[i] = tolower(buffer[i]);
		}

		if( 0 == strncmp(buffer, "y", bufflen) ||
		    0 == strncmp(buffer, "yes", bufflen)) {
			//creating force unmount command
			strcat(command, "-f "); //attach force option to unmount
			strcat(command, readStr[1]);
			printf("Forcing unmount of server directory %s\n", readStr[0]);
			if(-1 == system(command)) {
				PRINT_CMD_ERROR(command);
				exit(-1);
			}
		}
	}
}
