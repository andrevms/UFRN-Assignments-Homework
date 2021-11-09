#include <stdlib.h>
#include <stdio.h>
#include <time.h>
#include <unistd.h>
#include <sys/types.h>
#include <sys/shm.h>
#include <sys/ipc.h>
#include <sys/wait.h>


int** matrix1, **matrix2;
int* rows, *columns;


int** load_txt_Matrix(char* fileName, int index){
    FILE *fp = fopen(fileName, "r");
    fscanf(fp,"%i", &rows[index]);
    fscanf(fp,"%i",&columns[index]);
    
    //printf("\nrow %i\n",rows[index]);
    //printf("column %i\n",columns[index]);
        
    int** matrix = (int**)malloc(rows[index] * sizeof(int*));
    for (size_t i = 0; i < rows[index]; i++){
        matrix[i] = (int*) malloc (columns[index] * sizeof(int));
        for (size_t y = 0; y < columns[index]; y++)
        {
            fscanf(fp,"%i", &matrix[i][y]);
            //printf("c%li%li = %i\n",i,y, matrix[i][y]);
        }
    }
    fclose(fp);

    return matrix;
}

int rowXcolumn(int row, int column){
    int soma = 0;
    for(size_t i = 0; i < rows[0]; i++)
    {
        soma += matrix1[row][i] * matrix2[i][column];

    }
    printf("C%i%i = %i\n",row, column, soma);
    return soma;
}

void save_txt_arrayFile(int* matrix, char* fileName, int begin_row, int end_row, double time){
    
    FILE *fp = fopen(fileName, "wt");
    fprintf(fp,"%i %i\n",rows[0], columns[1]);
    int count = 0;

    for (size_t i = begin_row; i < end_row; i++){
        for (size_t y = 0; y < columns[1]; y++)
        {
            fprintf(fp, "C%li %li = %i\n", i, y , matrix[count]);
            count+= 1;
        }
    }    
    fprintf(fp, "%f", time);
    fclose(fp);
}

void prod_matriz(int n_processo, int x, int index){
    printf("Entrou aqui"); 
    int* array = (int*) malloc( n_processo * sizeof(int));
    int count = 0;
    clock_t t;
    t = clock();
    for (size_t i = x; i < x * index; i++)
    {
        for (size_t j = 0; j < columns[1]; j++)
        {
            array[count] = rowXcolumn(i, j);
            count++;
        }
        
    }
    t = clock() - t;
    double time_taken = ((double)t)/CLOCKS_PER_SEC;
    char num = '0' + index;
    char* fileName = {{"p1.txt"},{"p2.txt"},{"p3.txt"},{"p4.txt"},{"p5.txt"},{"p6.txt"},{"p7.txt"},{"p8.txt"}};
    save_txt_arrayFile(array, fileName[index], x, (x*index), time_taken);
    printf("tempo [%f]\n", time_taken);
}

int main(int argc, char* argv[]){
 
    rows = (int*)malloc((2) * sizeof(int));
    columns = (int*)malloc((2) * sizeof(int));

    char* fileName = argv[1];
    matrix1 = load_txt_Matrix(fileName, 0);

    fileName = argv[2];
    matrix2 = load_txt_Matrix(fileName, 1);

    int process = atoi(argv[3]);
    printf("Number of process  : %i\n", process);


    int num_Process = (rows[0]*columns[1])/process;
    pid_t pid;
    int count = 0;

    for (size_t i = 0; i < num_Process; i++)
    {
       pid = fork();

        if(pid < 0) { 
            fprintf(stderr, "Criação Falhou\n");
            exit(-1);
        }
        else if (pid == 0) { 
            printf("processo numero %ld ", i);
            printf("Executando o filho (PID=%d), cujo pai tem PID=%d\n", getpid(), getppid());
            prod_matriz( process ,(process / num_Process) * i , i + 1);
            exit(0);
        }
    }
    
     for (size_t i = 0; i < num_Process; i++)
    {
        wait(NULL);
    }
    

    printf("Processo finalizou\n");
    return 0;
}