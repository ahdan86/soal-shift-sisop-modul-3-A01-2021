#include<stdio.h>
#include<string.h>
#include<pthread.h>
#include<stdlib.h>
#include<unistd.h>
#include<sys/types.h>
#include<sys/wait.h>
#include<dirent.h>
#include <sys/stat.h>
#include <stdbool.h>
#include <ctype.h>

const char *get_filename(const char *filename){
    const char *dot = strrchr(filename, '/');
    if(!dot || dot == filename) return "";
    return dot + 1;
}

const char *get_filename_ext(const char *filename)
{
    const char *dot2 = strchr(filename, '.');
    if(!dot2 || dot2 == filename) return "";
    return dot2 + 1;
}

bool trig = false , trig2=false;
char folder[100];
void *makeMoveDir (void *arg)
{
    char root[100] = "/home/ahdan/Modul3";
    
    char folderPath[100]; 
    strcat(folderPath,root);
    strcat(folderPath,"/");

    char filePathOld[100]; 
    if(!trig)
        strcpy(filePathOld,arg);
    else
    {
        strcpy(filePathOld,folder);
        strcat(filePathOld,arg);
    }
    
    char ext[10];
    strcpy(ext,get_filename_ext((char *)arg));
    if(ext[0]=='\0'){
        if(strchr(arg,'.')!=NULL){
            strcat(folderPath,"Hidden");
        }
        else strcat(folderPath,"Unknown");
    }
    else{
        for(int i=0;i<strlen(ext);i++){
//             printf("masuk");
            ext[i] = tolower(ext[i]);
        }
        printf("%s",ext);
        strcat(folderPath, ext);
    }   

    char filePathNew[100]; 
    strcpy(filePathNew,folderPath);
    strcat(filePathNew,"/");
    if(!trig)
        strcat(filePathNew,get_filename((char *)arg));
    else
        strcat(filePathNew,arg);
   
    DIR* dir = opendir(folderPath);
    if(!dir){
        mkdir(folderPath,0777);
    }
    int stat = rename(filePathOld,filePathNew);
    if(stat==0&&trig==false) printf("%s : Berhasil Dikategorikan\n",get_filename((char *)arg));
    else if(stat==-1&&trig==false) printf("%s : Sad, gagal :(\n",get_filename((char *)arg));
    if(stat==-1&&trig==true) trig2=true;
    // printf("%s %s %s %d",filePathOld,filePathNew,arg,stat);
}

// void fungsiDBintang(char )
// {

// }

int main(int argc, char *argv[])
{
    if(!strcmp(argv[1],"-f")){
        pthread_t tid[1000];
        for(int i = 2; i<argc ;i++){
            int error = pthread_create(&tid[i], NULL, makeMoveDir, (void *)argv[i]);
            if(error != 0){
                printf("\nCant Create Thread! : [%s]\n",strerror(error));
            }
        }

        for(int j=2; j<argc; j++){
			pthread_join(tid[j], NULL);
        }
    }
    else
    {
        if(argc > 3){
            printf("Hanya Boleh <= 3 Argumen\n");
            return 0;
        }
        else{
            trig = true;
            pthread_t tid2[1000];
            
            DIR *d;
            struct dirent *dir;
            if(!strcmp(argv[1],"-d"))
            {
                char directory[100];
                strcpy(directory, argv[2]);

                strcpy(folder,directory);
                strcat(folder,"/");
                d = opendir(argv[2]);
            }
            else if(argv[1][0]=='*')
            {
                d = opendir("/home/ahdan/Modul3");
            }
            
            int i=0;
            while ((dir = readdir(d)) && d!= NULL)
            {
                if(!strcmp(dir->d_name,"soal3.o")||!strcmp(dir->d_name,"soal3.c")||!strcmp(dir->d_name,".")||!strcmp(dir->d_name,"..")||dir->d_type==DT_DIR)
                {
                    continue;
                }

                int error = pthread_create(&tid2[i], NULL, makeMoveDir, dir->d_name);
                if(error != 0){
                    printf("\nCant Create Thread! : [%s]\n",strerror(error));
                }

                i++;
            }

            for(int j=0;j<i;j++){
                pthread_join(tid2[j], NULL);
            }
            closedir(d);

            if(trig2) printf("Yah, gagal disimpan :(\n");
            else printf("Direktori sukses disimpan!\n");
        }
    }
}
