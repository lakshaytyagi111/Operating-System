// this is a banker's algorithm
// this is incomplete
#include <stdio.h>
#include <stdlib.h>

int main(){
	
	int n; // number of processes
	printf("Enter the number of processes in total: ");
	scanf("%d", &n);
	// number of resource types in a system;
	int m;
	printf("Enter the number of resource types in total: ");
	scanf("%d", &m);

	int available[m]; 

	for (int i = 0; i < m ; i++){
		printf("Enter the number of resources of type %d: ", i);
		scanf("%d", &available[i]);
	}
	int max[n][m]; // max means nth process can have at max k resources of type m;
	for (int i = 0; i < n ; i++){
		for (int j = 0; j < m; j++){
			printf("Enter the max number of resources of type %d required by process %d: ", j, i);

			scanf("%d", &max[i][j]);
		}
	}
	int allocation[n][m]; // this means that currently k resources are allocated to process n with  resource type m;
	
	for (int i = 0; i < n ; i++){
		for (int j = 0; j < m; j++){
			printf("Enter the number of resources of type %d allocated to processes %d: ", j, i);
			scanf("%d", &allocation[i][j]);
		}
	}
	int need[n][m];
	int finish[n];
	for (int i = 0; i < n; i++){
		for (int j = 0; j < m; j++){
			need[i][j] = max[i][j] - allocation[i][j];
		}
		finish[i] = 0;
	}
	int work[m];
	for (int j = 0; j < m; j++){
		work[j] = available[j];
	}
        
	int safeSeq[n];
	int k = 1, p=1;
	for (int i = 0; i < n; i++){
		if (finish[i] == 0){
			for (int j = 0; j < m; j++){
				if (need[i][j] > work[j]){
					k = 0;
					break;
				}
			}	
			if (k == 1){
				safeSeq[p] = i;
				finish[i] = 1;
				for (int j = 0; j < m; j++){
					work[j] += allocation[i][j];
				}
				k = 0;
				p+=1;
				i = 0;
			}
		} 
		
	}
	p+=1;
	printf("p is : %d", p);
	if (p == n){
		for (int i = 0; i < n ; i++){
			printf("%d , ", safeSeq[i]);
		}
		return 0;
	} else {
		printf("Hi this is not possible");

	}

	return 0;
}
