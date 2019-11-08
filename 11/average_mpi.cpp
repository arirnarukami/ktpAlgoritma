#include <mpi.h>
#include <stdio.h>
#include <stdlib.h>
#include <time.h>

int main(int argc, char *argv[]){
	//Variables
	int i;
	int master = 0;
	int my_id;
	int numprocs;
	int ele_per_procs = 1000000;
	int input;
	int sum = 0;
	int sum_all;
	float average;
	int array[ele_per_procs];

        for(i = 0;i < ele_per_procs;i++){
	        array[i] = rand() % 100;
        }

		
	//Inisialisasi MPI
	MPI_Init(&argc, &argv);

	//Memasukkan jumlah proses ke variabel numprocs
	MPI_Comm_size(MPI_COMM_WORLD, &numprocs);

	//Memasukan rank tiap prises ke variabel my_id
	MPI_Comm_rank(MPI_COMM_WORLD, &my_id);

	//Header - Hanya dilakukan oleh proses master -> id = 0
	if(my_id == master){
		printf("--------------------------------------------\n");
		printf("AVERAGE - WITH MPI:\n");
		printf("Mencari rata-rata dari elemen di dalam array, menggunakan MPI Reduce dan MPI Send & Receive\n");
		printf("by :\n");
		printf("\tFaris Ramadhan\n");
		printf("\tHaekal Rahimmas Fariis\n");
		printf("\tM. Ridho Hanif Y.\n");
		printf("\tM. Fikri Haikal\n");
		printf("\tRaihan Praditya A.\n");
		printf("---------------------------------------------\n\n");
		printf("Jumlah proses yang digunakan = %d\n", numprocs);
		printf("Elemen per proses =  %d\n", ele_per_procs);
		//printf("Isi Array =\n[");
		//for(i = 0; i < ele_per_procs; i++){
		//	printf("%d -", array[i]);
		//}
		//printf("]\n");

	}

	//Mendapatkan Nilai user Input per proses untuk di cari rata-ratanya menggunakan proses 0 dan 1
	if(my_id < 2){
		for(i = 0; i < ele_per_procs; i++){
			sum += array[i];
		}

		//Setiap proses mengeprint hasil sum lokal
		printf("Proses %d - hasil penjumlahan setiap elemen = %d\n, rata-rata = %4.2f\n", my_id, sum, (double) sum / ele_per_procs);
		
	}

	//Menjumlahkan semua sum lokal menjadi sum global menggunakan MPI Reduce
	MPI_Reduce(&sum, &sum_all, 1, MPI_INTEGER, MPI_SUM, master, MPI_COMM_WORLD);

	//Hasil di print oleh proses master
	if(my_id == master){
		printf("\nproses master -> Total penjumlahan dari semua proses adalah = %d\n", sum_all);
		printf("proses master -> total elemen = %d\n\n", numprocs * ele_per_procs);
		average = (double) sum_all / ((numprocs - 1) * ele_per_procs);
		//Mengirim average ke master menggunakan MPI send
		MPI_Send(&average, 1, MPI_FLOAT, 2, 0, MPI_COMM_WORLD);
	}

	if(my_id == 2){
		MPI_Recv(&average, 1, MPI_FLOAT, master, 0, MPI_COMM_WORLD, MPI_STATUS_IGNORE);
                printf("proses %d -> Rata - ratanya = %4.2f\n", my_id , average);
                printf("\n\n----- Program Selesai -----\n\n");

	}

	//Exit MPI
	MPI_Finalize();
	return 0;

}
