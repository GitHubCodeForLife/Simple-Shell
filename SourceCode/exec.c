#include"exec.h"
#include"tokenString.h"
#include"history.h"
	/* Colors */
#define ANSI_COLOR_CYAN_BOLD  "\033[1m\033[36m"  
#define ANSI_COLOR_YELLOW_BOLD   "\033[1m\033[33m"
#define ANSI_COLOR_BIRGHT_BLUE "\033[1m\033[34m"
#define ANSI_COLOR_BRIGHT_RED "\033[1m\033[31m"
#define ANSI_COLOR_RESET  "\033[0m"
//Copy Deep String
int copyString(char* target, char* source){
	  int len = strlen(source);
  //target=malloc(sizeof(char)*len);
  int i;
  for( i=0;i<len;i++)
    target[i]=source[i];
  target[len] = '\0';
return 1;
}

//Open Help
void OpenHelp(){
	//Mo file 
	FILE* f = fopen("help.txt","r");
	if(f==NULL){
		printf("\nKhong the mo file help.txt\n");	
	}
	//Doc file
	 char ch;	
	while((ch = fgetc(f)) != -1)
     	 printf("%c", ch);
	
	//Dong file
	fclose(f);
}

//Change director
void cd(char** argv){
	if(argv[1]!=NULL){
		if(chdir(argv[1])==0){//check for valid path
			setenv("PWD", argv[1],1);	
		}else{
			printf("Bash: cd: %s: no such file or directory\n", argv[1]);

		}
	}else{
		chdir("..");
		//printf("%s\n",getenv("PWD"));
	}
}
/*0: thoat ra va khong luu lich su
//2: thoat va luu lich su
//1: Luu lich su va tiep tuc chuong trinh
//Special command : history, !!, hello,...*/
int execArgsBuiltIn(char* lineInput,char** history,int historycount){
	int len = strlen(lineInput);
	char* copy=malloc(sizeof(char)*len);
	copyString(copy, lineInput);
	char* argv[100];
	//printf("\nChuoi copy: %s",copy);
	int counter = tokenWord(copy, argv);
	if(counter ==0) return 0;
	if(strcmp(argv[0],"history")==0||strcmp(argv[0],"History")==0){
		if(argv[1] !=NULL){
		 OpenHelp();
		 return 0;}
		printHistory(history, historycount);
		return 2;
	} 	
	if(strcmp(argv[0],"exit")==0){
		if(argv[1] !=NULL){
		 OpenHelp();
		 return 0;}
		printf("\nGoodBye. Hen gap lai.\n");
		exit(0);
	}
	if(strcmp(argv[0],"hello")==0||strcmp(argv[0],"Hello")==0){
		if(argv[1] !=NULL){
		 OpenHelp();
		 return 2;}
		 printf("\n--------Chao mung den voi MYSHELL---------\nThanh Vien Nhom: \n1: Nguyen Phu Bao Tien\n2: Tran Van Tu\n3: Tran Ngoc Tuan\nGo 'help' neu ban muon giup do\n");
		return 2;
	}
	if(strcmp(argv[0],"cd")==0){
		//cd in here
		//printf("file: %s",argv[1]);
		cd(argv);
		return 2;
	}
	if(strcmp(argv[0],"help")==0){
		OpenHelp();
		//printf("\nhelp\n");
		return 2;
	}
	if(strcmp(argv[0],"!!")==0){
		if(historycount==0) {
			printf("No command in history");return 0;
		}
		printf("%s", history[historycount-1]);
		copyString(lineInput, history[historycount-1]);
		return 1;
	}
	if(strcmp(argv[0],"clear")==0){
		execArgs(argv,0);
		return 2;
	}	
	
	free(copy);
	//free(argv);
return 1;
} 

//Run simple shell
void execArgs(char** parsed, int background) 
{ 
    // Forking a child 

  pid_t pid = fork();  
	switch ( pid ) {
	 case -1: // fork không tạo được tiến trình mới
		 printf("Khong the tao tien trinh moi");
		 exit( 1 );
	 case 0: // fork thành công, chúng ta đang ở trong tiến trình con
		 if (execvp(parsed[0], parsed) < 0) { 
		    printf("Could not execute command..\n"); exit(0);
		}
		/*if(background==1){
			char c;
			scanf("%c",&c);
			printf("[%d]: is finished with status 0 ", getpid());
		}*/
		exit(0);
	 default: // fork thành công, chúng ta đang ở trong tiến trình cha
		if(background==0){
			waitpid(pid, NULL,0); 
		}else{
			printf("[1]: %d\n", getpid());
		}
		
	 }
 return ;
} 
void execArgsPiped(char** argv_in, char** argv_out, int background) {
    int fd[2]; //file description of in: fd[0] and out: fd[1]

    if (pipe(fd) == -1) //fd[0] is set up for reading, fd[1] is set up for writing
    {
        perror("Failed to pipe cmd");
        exit(EXIT_FAILURE);
    }
    //first argv's fork
    if (fork() == 0) {
        dup2(fd[1], STDOUT_FILENO);

        if (close(fd[0]) == -1 || close(fd[1]) == -1) {
            perror("Unexpected error when make a pipe I/O");
        }
       execArgs(argv_in,0);
        exit(EXIT_FAILURE); //prevent access to second argv's fork
    }
    //second argv's fork
    if (fork() == 0) {
        dup2(fd[0], STDIN_FILENO);
        if (close(fd[1]) == -1 || close(fd[0]) == -1) {
            perror("Unexpected error when make a pipe I/O");
        }
        execArgs(argv_out,0);
        exit(EXIT_FAILURE);
    }

    close(fd[0]);
    close(fd[1]);
    wait(0);
    wait(0);
} 


 void execArgsRedirector(char** argv,int cr, int background){
	int len = Len(argv);
	char* str;
	str = argv[len-1];
	if(strstr(str,".txt")==NULL){
		strcat(str,".txt");
	}
	argv[len-1] = NULL;
	if(cr==1){//dau >
		int file = open(str,O_WRONLY | O_TRUNC | O_CREAT,0777);
		int saved_stdout;  saved_stdout = dup (1);
		dup2(file, 1);
		execArgs(argv,0);
		dup2(saved_stdout, 1);  // returns back to normal stdout
		close(saved_stdout);
		close(file);
	}
	if (cr==2){// dau <	
	int file = open(str, O_RDONLY);
	if (file == -1){  // if open call fails
			printf(ANSI_COLOR_BRIGHT_RED "File not found: " ANSI_COLOR_RESET);  
			printf("type "); 
			printf(ANSI_COLOR_BIRGHT_BLUE "help" ANSI_COLOR_RESET);
			printf(" to view manual\n");
		}
	else {
		int saved_stdin;
		saved_stdin = dup (0);
		dup2(file,0);
		execArgs(argv,0);
		dup2(saved_stdin, 0);  // returns back to normal stdout
		close(saved_stdin);
		close(file);
	     }
	
	}
	if(cr==3){//dau >>
		//printf("%s",str);
		int file = open(str,O_WRONLY | O_APPEND  | O_CREAT,0777);
		int saved_stdout;  saved_stdout = dup (1);

		dup2(file, 1);  // output redirection
		//printf("%s%", argv[0]);
	
		execArgs(argv,0);
		dup2(saved_stdout, 1);  // returns back to normal stdout
		close(saved_stdout);
		close(file);
	}
	
}
	





