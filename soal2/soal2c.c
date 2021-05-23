#include<stdio.h> 
#include<stdlib.h> 
#include<unistd.h> 
#include<sys/wait.h> 
#include<string.h> 
#include<sys/types.h> 
//read = 0; write = 1
int main() 
{ 
	int pipe1[2], pipe2[2]; 
	pid_t p1,p2;
	if (pipe(pipe1)==-1)
	{ 
		fprintf(stderr, "Pipe Gagal" ); 
		return 1; 
	} 
	if (pipe(pipe2)==-1)
	{ 
		fprintf(stderr, "Pipe Gagal" ); 
		return 1; 
	} 
	p1=fork();
	if (p1< 0) 
	{ 
		fprintf(stderr, "Fork Gagal" );
		return 1; 
	}  

    //parent p1
    else if (p1 > 0) {
        p2 = fork();
        if (p2< 0) 
	    { 
		    fprintf(stderr, "Fork Gagal" );
		    return 1; 
	    }  

        else if(p2 > 0){//parent p2
            close(pipe1[0]);
            close(pipe1[1]);

            dup2(pipe2[0], 0);
        
            close(pipe2[0]);
            close(pipe2[1]);

            char *arg1[] = {"head", "-5", NULL};
            execvp("/usr/bin/head", arg1);
            exit(0);
        }

        else{ //child p2
            //Read isi dari Pipe1
            dup2(pipe1[0], 0); 
            //Write ke Pipe 2
            dup2(pipe2[1], 1);

            close(pipe1[0]);
            close(pipe1[1]);
            close(pipe2[0]);
            close(pipe2[1]);

            char *arg2[] = {"sort", "-nrk", "3.3",NULL};
            execvp("/bin/sort", arg2);
        }
    }
    //child p1
	else{ //p1 == 0
    	dup2(pipe1[1], 1);

		close(pipe1[0]);
        close(pipe1[1]);

        char *arg2[] = {"ps", "aux",NULL};
        execvp("/bin/ps", arg2);
	} 
} 
