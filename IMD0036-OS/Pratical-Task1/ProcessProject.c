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

int prod_matrix(int*** matrix, int* rows, int* columns, char* fileName, int index, int num_elements){
    
    int elements = num_elements * index;
    int end_elements = elements + num_elements;
    int max = rows[0]* columns[1];
    
    if (end_elements > max)
    {
        end_elements -= (end_elements - max);
    }
    
    printf("%s %i %i\n", fileName, elements, end_elements);

    clock_t t;
    t = clock();
    for (size_t i = elements; i < end_elements; i++)
    {
        int x = i/rows[0];
        int y = i%columns[1];
        int soma = 0;
        for (size_t j = 0; j < rows[0]; j++)
        {
            soma+= matrix[0][x][j] * matrix[1][j][y];
        }
        matrix[2][x][y] = soma;
    }
    t = clock() - t;

    double time_taken = ((double)t)/CLOCKS_PER_SEC;
    printf("tempo [%f]\n", time_taken);

    FILE *fp = fopen(fileName, "wt");
    fprintf(fp,"%i %i\n", rows[0], columns[1]);

    for (int i = elements; i < end_elements; i++)
        {
            int x = i/rows[0];
            int y = i%columns[1];
            fprintf(fp, "C%i %i %i \n", x, y, matrix[2][x][y]);
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

    int num_elements = atoi(argv[3]);

    int process = (rows[0] * columns[1])/num_elements;
    if((rows[0] * columns[1])%num_elements != 0){
        process++;
    }

    printf("Number of process  : %i\n", process);

    fileName = (char**) malloc (process * sizeof(char*));
    for (size_t i = 0; i < process; i++)
    {
        fileName[i] = (char*) malloc (100 * sizeof(char*));
        sprintf(fileName[i], "Teste-%02ld.txt", i);
        //printf("%s\n",fileName[i]);
    }
    
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
            prod_matrix(matrix, rows, columns, fileName[i], i, num_elements);         
            exit(0);
        }
        else{
           wait(NULL);
        }
    }
    printf("Processo Pai finalizou\n");
    return 0;
}