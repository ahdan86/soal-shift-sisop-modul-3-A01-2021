#include <stdio.h>
#include <sys/shm.h>
#include <pthread.h>
#include <sys/ipc.h>
#include <unistd.h>
#include <stdlib.h>

#define B 4
#define K 6

typedef long long ll;
int newvalue[4][6],beda,zero;

ll factorial(int n) {
    if (n == 0) return 1;
    return n*factorial(n-1);
}

ll factorial2(int n){
	if (n == beda) return 1;
        return n*factorial2(n-1);
}

void *finalmatriks(void* argv){
	ll n = *(ll*)argv;
	if(zero){
		printf("0 ");
	}
	else if(beda<1){
		printf("%lld ", factorial(n));
	} 
	else printf("%lld ", factorial2(n));
}

void main()
{
    //Shared Memory
    key_t key = 1234;
    int (*value)[10];
    int shmid = shmget(key, sizeof(int), IPC_CREAT | 0666);
    value = shmat(shmid, 0, 0);

	printf("Hasil Perkalian antara matriks A dan B adalah: \n");
	for(int i=0;i<B;i++){
		for(int j=0;j<K;j++){
			printf("%d ", value[i][j]);
		}
	printf("\n");
	}

	printf("Input matriks baru: \n");
    for(int x=0;x<B;x++){
		for(int y=0;y<K;y++){
			scanf("%d", &newvalue[x][y]);
		}
	}

	printf("\n");
	pthread_t thread_id[B*K];
    int cnt=0;
    for(int i = 0; i < B; i++){
        for(int j = 0; j < K; j++){
        	zero=0;
            ll *val = malloc(sizeof(ll[4][6]));

            *val = value[i][j];
            beda = value[i][j] - newvalue[i][j];

            if(value[i][j]==0 || newvalue[i][j]==0) zero=1;

            pthread_create(&thread_id[cnt], NULL, &finalmatriks, val);
            sleep(1);
            cnt++;
        }
        printf("\n");
    }
    for (int i = 0; i<cnt; i++){
		pthread_join(thread_id[i], NULL);
	}
}

