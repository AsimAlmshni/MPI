//#include<iostream>
//#include<stdio.h>
#include<limits>
#include <cstdlib>
#include"func.h"

using namespace std; 

long int * genkeys(int size, int temp){
	long int *arr = NULL;
	arr = new long int[size];
	if (temp == 0){
		for(int i = 0; i < size; i++){
			arr[i] = rand() % 1000000;
		}
	}else{
		for(int j = 0; j < size; j++)
			arr[j] = rand() % numeric_limits<long int>::max();
	}
	return arr;
}
