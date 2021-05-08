#include <stdio.h>
#include <sys/shm.h>
#include <sys/stat.h>
#include <unistd.h>
#include <pthread.h>
#include <sys/ipc.h>
 
#define B1 4
#define K1B2 3
#define K2 6

int M1[B1][K1B2]; 
int M2[K1B2][K2];
int (*value)[10];
int res = 0;



void *inputM1(void *arg)
{
    printf("Matriks 1:\n");
    int i,j,k;
    for (i=0; i<B1; i++){
    	for(j=0; j<K1B2; j++){
    		scanf("%d", &M1[i][j]);
    	}printf("\n");
    }
	for (i=0; i<B1; i++){
		for(j=0; j<K1B2; j++){
			printf("%d ",M1[i][j] );
		}
		printf("\n");
	}
	return NULL;
}

void *inputM2(void *arg)
{
    printf("Matriks 2:\n");
    int i,j,k;
    for (i=0; i<K1B2; i++){
    	for(j=0; j<K2; j++){
    		scanf("%d", &M2[i][j]);
    	}printf("\n");
    }
	for (i=0; i<K1B2; i++){
		for(j=0; j<K2; j++){
			printf("%d ",M2[i][j] );
		}
		printf("\n");
	}
	return NULL;
}

void *perkalian(void *arg)
{
	for(int i=0;i<B1;i++)
	{
		for(int j=0;j<K2;j++)
		{
			for(int k=0;k<K1B2;k++)
			{
				res += M1[i][k] * M2[k][j];
			}
            		value[i][j] = res; 
            		res =0;
		}
	}
	return NULL;
}

pthread_t thread1, thread2, thread3;
int main()
{
    //Shared Memory
	key_t key = 1234;
    int shmid = shmget(key, sizeof(int[10][10]), IPC_CREAT | 0666);
    value = shmat(shmid, 0, 0);

	pthread_create(&thread1, NULL, inputM1, NULL);
	pthread_join(thread1,NULL);

	pthread_create(&thread2, NULL, inputM2, NULL);
	pthread_join(thread2,NULL);

    printf("\n");

	for(int i=0;i<B1;i++){
		for(int j=0;j<K2;j++){
			value[i][j] = 0;
		}
		pthread_create(&thread3, NULL, perkalian, NULL);
		pthread_join(thread3,NULL);
	}

	printf("Hasil Perkalian: \n");
	for(int i=0; i<B1; i++){
		for(int j=0;j<K2;j++){
			printf("%d ", value[i][j]);
		}
		printf("\n");
	}
}
