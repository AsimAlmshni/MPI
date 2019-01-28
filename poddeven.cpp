#include<iostream>
#include<stdio.h>
#include<stdlib.h>
#include"func.h"
#include<mpi.h>
#include <algorithm>
#include <fstream>
#include <cstring>
#include <iostream>
#include <fstream>

using namespace std;

int compare (const void * a, const void * b)
{
  return ( *(long int*)a > *(long int*)b );
}



int main(int argc, char* argv[]) {

	int rank, p;
	long int *local_A;
	int global_n;
	int local_n;
	int arrSize; 
	long int *fldArray;
	int ptn, flag;
	long int *buf;
   	long int *data, recdata[1000], recdata2[1000];
	long int *temp;
	int ierr,i;
	int root_process;
	int sendcounts;
	int n;
	int avg;
	ofstream myfile;
 	myfile.open ("sorted_keys.txt");


	MPI_Status status;

	MPI_Comm comm;		

	ierr = MPI_Init(&argc, &argv);
	
	comm = MPI_COMM_WORLD;
	ierr = MPI_Comm_size(comm, &p);
	ierr = MPI_Comm_rank(comm, &rank);
  
  	 if (rank == 0){
		cout<<"enter size of array :";
		cin>>n ; 
		flag = n % p;
		data = genkeys(n, flag);
   		ptn = n / p;
		local_n = ptn;
		//recdata = new long int[n];
		//recdata2 = new long int[n];
	//	buf = new long int(ptn);
	}else{
		data = NULL;
	} 
	ierr = MPI_Barrier(comm); 
	ierr = MPI_Bcast(&local_n, 1, MPI_LONG, 0, comm);
	ierr = MPI_Scatter(data, local_n, MPI_LONG, &recdata, n, MPI_LONG, 100, MPI_COMM_WORLD);

	 printf("%d:received data:",rank);
         for(i=0;i<local_n;i++){
               cout<< recdata[i];
         }
	 printf("\n");

	
	sort(recdata,recdata+local_n);
	
	int oddrank,evenrank;

	if(rank%2==0){
    		oddrank=rank-1;
    		evenrank = rank+1;
	}
 	else {
 		oddrank = rank+1;
 		evenrank = rank-1;
	}
	
	if (oddrank == -1 || oddrank == p)
 		oddrank = MPI_PROC_NULL;
	if (evenrank == -1 || evenrank == p)
 		evenrank = MPI_PROC_NULL;

	int np;
	for (np=0; np<p-1; np++) {
 	if (np%2 == 1) /* Odd phase */
	MPI_Sendrecv(recdata, local_n, MPI_INT, oddrank, 1, recdata2,
 	local_n, MPI_INT, oddrank, 1, MPI_COMM_WORLD, &status);
 	else /* Even phase */
 	MPI_Sendrecv(recdata, local_n, MPI_INT, evenrank, 1, recdata2,
	local_n, MPI_INT, evenrank, 1, MPI_COMM_WORLD, &status);
	
	 printf("%d:received data:",rank);
         for(i=0;i<local_n;i++){
               cout<< recdata2[i]<<" ";
         }
         printf("\n");




	//if (oddrank == -1 || oddrank ==local_n*sizeof(int));
	temp = new long int[local_n];
	for(i=0;i<local_n;i++){
 		temp[i]=recdata[i];
	}


	if(status.MPI_SOURCE==MPI_PROC_NULL) continue;
 	
	else if(rank<status.MPI_SOURCE){
 	//store the smaller of the two
 	int x,j,k;
 	for(x=j=k=0;k<local_n;k++){
 		if(j==local_n||(x<local_n && temp[x]<recdata2[j]))
 			recdata[k]=temp[x++];
 		else
 			recdata[k]=recdata2[j++];
 		}
 	}else{
 	//store the larger of the two
 	int x,j,k;
 	for(x=j=k=local_n-1;k>=0;k--){
 		if(j==-1||(x>=0 && temp[x]>=recdata2[j]))
 			recdata[k]=temp[x--];
 		else
 			recdata[k]=recdata2[j--];
 		}
 	}//else

	 printf("%d:received data:",rank);
         for(i=0;i<local_n;i++){
               cout<< recdata[i]<< "  ";
         }
         printf("\n");


 	}//for
	
	ierr = MPI_Gather(recdata,local_n,MPI_INT,data,local_n,MPI_INT,0,MPI_COMM_WORLD);
	if(rank == 0){
	printf("final sorted data:");
        for(i=0;i<n;i++){
         	cout<< data[i]<<"  ";
		myfile << data[i]<<" ";
  		}
		myfile.close();
   	 printf("\n");
	}

	ierr = MPI_Finalize();
   
   return 0;
} // end of main
