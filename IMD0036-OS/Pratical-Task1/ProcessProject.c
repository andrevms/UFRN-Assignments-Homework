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
char* fileName; 


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

int rowXcolumn(int row, int column){
    int soma = 0;
    for(size_t i = 0; i < rows[0]; i++)
    {
        soma += matrix[0][row][i] * matrix[1][i][column];
    }
    return soma;
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

void prod_matrix(int index, char* fileName){
    //int begin_row, int end_row, int begin_column, int end_column){
   

    //save_txt_file()
    /*
    int count = 0;
    for (size_t i = begin_row; i < end_row; i++)
    {
        for (size_t y = begin_column; y < end_column; y++)
        {
            matrix[2][i][y] = rowXcolumn(i, y);
        }
    }
    */


}

int main(int argc, char* argv[]){
    
    
    matrix = (int***) malloc ((argc) * sizeof(int**));
    rows = (int*) malloc ((argc) * sizeof(int));
    columns = (int*) malloc ((argc) * sizeof(int));

    for (size_t i = 1; i < argc-1; i++)
    {
        matrix[i-1] = load_txt_Matrix(argv[i], i-1);
    }

    int process = atoi(argv[3]);
    printf("Number of process  : %i\n", process);

   
    int count = 0;
    char a;
    fileName = (char*) malloc (5*sizeof(char));
    int* pid;
    pid = (int*) malloc (process * sizeof(int));

    for (size_t i = 0; i < process; i++)
    {
       pid[i] = fork();

        if(pid[i] < 0) { 
            fprintf(stderr, "Criação Falhou\n");
            exit(-1);
        }
        else if (pid[i] == 0) { // ajeitar aqui prod_matrix
            printf("processo numero %ld ", i);
            printf("Executando o filho (PID=%d), cujo pai tem PID=%d\n", getpid(), getppid());
            a  = '0'+ i;
            fileName = "r1.txt";
            fileName[1] = a;

            printf("ProdM Number of process  : %li\n", i);
            printf("%s\n",fileName);
            //prod_matrix(i, fileName);
                // process ,(process / num_Process) * i , i + 1);
            exit(0);
        }
        else{
           wait(NULL);
        }
    }
    
    printf("Processo Pai finalizou\n");
    return 0;
}