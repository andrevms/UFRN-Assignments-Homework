#include <stdlib.h>
#include <stdio.h>
#include <time.h>
#include <unistd.h>
#include <sys/types.h>
#include <sys/shm.h>
#include <sys/ipc.h>
#include <sys/wait.h>


int*** matrix;
int* rows, *columns;
char** fileName; 


int** load_txt_Matrix(char* fileName, int index){
    FILE *fp = fopen(fileName, "r");
    fscanf(fp,"%i %i\n", &rows[index], &columns[index]);
    //printf("%i %i\n", rows[index], columns[index]);

    int count = 0;
    int aux1 , aux2;
    int** m = (int**) malloc( rows[index] * sizeof(int*));    

    for (int i = 0; i < rows[index]; i++){
        m[i] = (int*) malloc( columns[index] * sizeof(int));  
        for (int y = 0; y < columns[index]; y++)
        {
            fscanf(fp,"C%i %i %i\n", &aux1, &aux2, &m[i][y]);
            //printf("C%i %i %i\n", aux1, aux2, m[i][y]);

            count++;
        }
    }

    fclose(fp);
    return m;
}

void save_txt_File( char* fileName, double time, int begin_row, int end_row, int begin_column, int end_column){
    
    FILE *fp = fopen(fileName, "wt");
    fprintf(fp,"%i %i\n", rows[2], columns[2]);

    for (int x = 0, w = begin_row; x < (end_row - begin_row), w < end_row; x++, w++){
        for (int y = 0, z = begin_column; y < columns[2], z = end_column; y++, z++)
        {
            fprintf(fp, "C%i %i %i \n", w, z, matrix[2][x][y]);
        }
    }  

    fprintf(fp, "%f", time);
    fclose(fp);
}

int prod_matrix(int*** matrix, int* rows, int* columns, char* fileName, int index, int total_rows){
    int begin_row = index * total_rows;
    int end_row = (index+1)* total_rows;
    int begin_column = 0;
    int end_column = columns[1];    
    int soma = 0;
    int count = 0;
    printf("%s\n",fileName);
    clock_t t;
    t = clock();
    for (size_t i = begin_row; i < end_row; i++)
    {
        for (size_t y = begin_column; y < end_column; y++)
        {
            soma = 0;
            for(size_t j = 0; j < rows[0]; j++)
            {
                soma += matrix[0][i][j] * matrix[1][j][y];
            }
            matrix[2][i][y] = soma;
        }
    }
    t = clock() - t;
    double time_taken = ((double)t)/CLOCKS_PER_SEC;
    printf("tempo [%f]\n", time_taken);
    
    FILE *fp = fopen(fileName, "wt");
    fprintf(fp,"%i %i\n", rows[0], columns[1]);

    for (int x = begin_row; x < end_row; x++){
        for (int y = begin_column; y < end_column; y++)
        {
            fprintf(fp, "C%i %i %i \n", x, y, matrix[2][x][y]);
        }
    }  

    fprintf(fp, "%f", time_taken);
    fclose(fp);

   return 1;
}

int main(int argc, char* argv[]){
    
    
    matrix = (int***) malloc ((argc) * sizeof(int**));
    rows = (int*) malloc ((argc) * sizeof(int));
    columns = (int*) malloc ((argc) * sizeof(int));

    for (size_t i = 1; i < argc-1; i++)
    {
        matrix[i-1] = load_txt_Matrix(argv[i], i-1);
    }

    matrix[2] = (int**) malloc (rows[0] * sizeof(int*));
    for (size_t i = 0; i < rows[0]; i++)
    {
        matrix[2][i] = (int*) malloc (columns[1]*sizeof(int));
    }


    int process = atoi(argv[3]);
    printf("Number of process  : %i\n", process);

    fileName = (char**) malloc (process * sizeof(char*));
    for (size_t i = 0; i < process; i++)
    {
        fileName[i] = (char*) malloc (100 * sizeof(char*));
        sprintf(fileName[i], "Teste-%02ld.txt", i);
        //printf("%s\n",fileName[i]);
    }
    

    int count = 0;
    int* pid;
    pid = (int*) malloc (process * sizeof(int));

    for (int i = 0; i < process; i++)
    {
       pid[i] = fork();

        if(pid[i] < 0) { 
            fprintf(stderr, "Criação Falhou\n");
            exit(-1);
        }
        else if (pid[i] == 0) { // ajeitar aqui prod_matrix
            printf("processo numero %d ", i);
            printf("Executando o filho (PID=%d), cujo pai tem PID=%d\n", getpid(), getppid());
            prod_matrix(matrix, rows, columns, fileName[i], i, (rows[0]/process));
            exit(0);
        }
        else{
           wait(NULL);
        }
    }
    
    
    printf("Processo Pai finalizou\n");
    return 0;
}