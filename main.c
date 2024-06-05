#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#define N 15

typedef struct subject 
{
    char sub[15];
    float gr;
    int a;
} Subject;

typedef struct people 
{
    char name[15], surname[15];
    float media;
    int asTot;
    Subject subjects[15];
} Register;

void insertData(Register r[], int nP, int nS);
void insertDetails(Register r[], int nP, int nS);
void orderData(Register r[], int nP);
void wrData(Register r[], int nP, int nS, FILE *w);
void media(Register r[], int nP, int nS, FILE *R);
int readData(Register r[], int nP, int nS, FILE *R);

int main() 
{
    FILE *read, *write;
    Register reg[N] = {0};
    char menu[6][120] = {"1. Insert Student", "2. Insert details", "3. Write data", "4. Read data", "5. Media","6. Exit"};
    int choose, maxSt = 0, maxSub = 0;

    printf("Insert max student: ");
    scanf("%d", &maxSt);
    printf("Insert max subject: ");
    scanf("%d", &maxSub);

    while (1) 
    {
        for (int i = 0; i < 6; i++) 
        {
            printf("%s\n", menu[i]);
        }
        printf("Choose: ");
        scanf("%d", &choose);

        switch (choose) 
        {
            case 1:
                printf("Insert data\n");
                insertData(reg, maxSt, maxSub);
            break;
            case 2:
                printf("Insert other data\n");
                insertDetails(reg, maxSt, maxSub);
                orderData(reg, maxSt);
            break;
            case 3:
                wrData(reg, maxSt, maxSub, write);
            break;
            case 4:
                readData(reg, maxSt, maxSub, read);
            break;
            case 5:
                media(reg, maxSt, maxSub, read);
            break;
            case 6:
                exit(0);
            break;
            default:
                printf("Invalid choice\n");
            break;
        }
    }
}

void insertData(Register r[], int nP, int nS) 
{
    int count = 0;
    do 
    {
        printf("Insert name: ");
        scanf("%s", r[count].name);
        printf("Insert surname: ");
        scanf("%s", r[count].surname);
        for (int i = 0; i < nS; i++) 
        {
            printf("Insert subject: ");
            scanf("%s", r[count].subjects[i].sub);
        }
        count++;
    }while(count != nP);
}

void orderData(Register r[], int nP) 
{
    Register mem;
    for (int i = 0; i < nP; i++) 
    {
        for (int k = 0; k < nP - i - 1; k++) 
        {
            if (strcmp(r[k].surname, r[k + 1].surname) > 0) 
            {
                mem = r[k];
                r[k] = r[k + 1];
                r[k + 1] = mem;
            }
        }
    }
}

void insertDetails(Register r[], int nP, int nS) 
{
    char nSub[15] = {0}, nStd[15] = {0};
    printf("Insert the subject: ");
    scanf("%s", nSub);
    printf("Select a student's surname: ");
    scanf("%s", nStd);

    for(int i = 0; i < nP; i++) 
    {
        if(strcmp(r[i].surname, nStd) == 0) 
        {
            for(int j = 0; j < nS; j++) 
            {
                if(strcmp(r[i].subjects[j].sub, nSub) == 0)
                {
                    printf("Insert vote: ");
                    scanf("%f", &r[i].subjects[j].gr);
                    printf("Insert A: ");
                    scanf("%d", &r[i].subjects[j].a);
                }
            }
        }
    }
    printf("Student or subject not found.\n");
}

void wrData(Register r[], int nP, int nS, FILE *w)
{
    w = fopen("Register.csv", "w");

    for(int data = 0; data<nP; data++)
    {
        for(int s = 0; s<nS; s++)
        {
            fprintf(w, "%s;%s;%.2f;%d\n", r[data].surname, r[data].subjects[s].sub, r[data].subjects[s].gr, r[data].subjects[s].a);
        }
    }

    fclose(w);
}

int readData(Register r[], int nP, int nS, FILE *R)
{
    char line[10000];
    R = fopen("Register.csv", "r");
    if(R == NULL)
    {
        perror("file not found\n");
        return EXIT_FAILURE;
    }
    
    while(fgets(line, sizeof(line), R) != NULL)
    {
        char *token;
        token = strtok(line, ";");
        while(token != NULL)
        {
            //printf("\n\n", r[nP].surname, r[nP].subjects[nS].sub, r[nP].subjects[nS].gr, r[nP].subjects[nS].a);
            printf("%-7s", token);
            token = strtok(NULL, ";");
        }
        printf("\n");    
    }
    fclose(R);
}

void media(Register r[], int nP, int nS, FILE *R)
{
    char line[10000];

    R = fopen("Register.csv", "r");
    if (R == NULL)
    {
        perror("file not found\n");
        return;
    }
    
    int studentIndex = 0;
    while (studentIndex < nP && fgets(line, sizeof(line), R) != NULL)
    {
        char *token;
        int subjectIndex = 0;
        float sum = 0;
        int count = 0;
        
        token = strtok(line, ";");
        while (token != NULL)
        {
            switch (subjectIndex % 4)
            {
                case 0:
                    strncpy(r[studentIndex].surname, token, 15);
                break;
                case 1:
                    strncpy(r[studentIndex].subjects[count].sub, token, 15);
                break;
                case 2:
                    r[studentIndex].subjects[count].gr = atof(token);
                    sum += r[studentIndex].subjects[count].gr;
                    count++;
                break;
                case 3:
                    r[studentIndex].subjects[count - 1].a = atoi(token);
                break;
            }
            token = strtok(NULL, ";");
            subjectIndex++;
        }
        r[studentIndex].media = count > 0 ? sum / count : 0;
        printf("Student %s's Media: %.2f\n", r[studentIndex].surname, r[studentIndex].media);
        studentIndex++;
    }
    fclose(R);
}