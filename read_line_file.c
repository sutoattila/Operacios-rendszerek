#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stdio.h>
#include <stdlib.h>
#include <fcntl.h>     //open,creat
#include <sys/types.h> //open
#include <sys/stat.h>
#include <errno.h> //perror, errno

typedef struct Applicant
{
    int nameLength;
    char *name;
    int days[7];
} Applicant;

int number_of_lines(const char* fn){
    FILE* fp = fopen(fn,"r");
    int count=0;
        for (char c = getc(fp); c != EOF; c = getc(fp))
        if (c == '\n') // Increment count if this character is newline
            count = count + 1;
        if(!count)
            return count;
        return count+1;
}
char *readLine_file(int *lineLength, const char* fname, int offset)
{
    FILE *fp = fopen(fname,"r");
    int allocated = 20;
    char *line = malloc(allocated);
    for (int i = 0; i < allocated; i++)
    {
        line[i] = 0;
    }
    int alreadyRead = 0;
    fseek( fp, offset, SEEK_SET );
    char c = getc(fp);
    while (c != '\r' && c != '\n')
    {
        if (alreadyRead + 1 >= allocated)
        {
            allocated = allocated * 2;
            char *reallocate = realloc(line, allocated);
            if (!reallocate)
            {
                free(line);
                while (c != '\r' && c != '\n')
                {
                    c = getc(fp);
                }
                return NULL;
            }
            else
            {
                for (int i = allocated / 2; i < allocated; i++)
                {
                    reallocate[i] = 0;
                }
                line = reallocate;
            }
        }
        line[alreadyRead] = c;
        c = getc(fp);
        if(c==EOF)
            break;
        alreadyRead++;
    }
    if (lineLength != NULL)
    {
        *lineLength = alreadyRead;
    }
    return line;
}
int count_bytes(const char* filename){
    FILE* fp = fopen(filename,"r");
    int bytes;
    for(bytes = 0; getc(fp) != EOF; ++bytes);
    return bytes;
}
int splitLine(char* line,int* nameLength, char** name, char** days){
    int i=0;
    while(line[i]!=' ')
        i++;
    *nameLength = i-1;
    line[i]='\0';
    *name=line;
    *days=line+i+1;
    return 1;
}
int print_daily_summary()
//int main()
{
    int bytes = count_bytes("applicants.txt");
    //TODO check bytes = 0
        char hello[]="hello";
    int nol = number_of_lines("applicants.txt");
    Applicant applicants[nol];
    int x=0;
    int offset=0;
    int cnt=0;
    while (offset<bytes)
    {
        char* line = readLine_file(&x,"applicants.txt",offset);
        offset=offset+x+1;
        Applicant applicant;
        char* name;
        char* days;
        int nameLength;
        splitLine(line,&nameLength,&name,&days);
        applicant.name = name;
        //strcpy(applicant.name,name);
        applicant.nameLength=nameLength;
        for (int i = 0; i < 7; i++)
            applicant.days[i] = 0;
        if (days == NULL)
        {
            return 1;
        }
        char *space = strtok(days, " ");
        while (space)
        {
            if (!strcmp(space, "hétfő"))
            {
                applicant.days[0] = 1;
            }
            else if (!strcmp(space, "kedd"))
            {
                applicant.days[1] = 1;
            }
            else if (!strcmp(space, "szerda"))
            {
                applicant.days[2] = 1;
            }
            else if (!strcmp(space, "csütörtök"))
            {
                applicant.days[3] = 1;
            }
            else if (!strcmp(space, "péntek"))
            {
                applicant.days[4] = 1;
            }
            else if (!strcmp(space, "szombat"))
            {
                applicant.days[5] = 1;
            }
            else if (!strcmp(space, "vasárnap"))
            {
                applicant.days[6] = 1;
            }
            space = strtok(NULL, " ");
        }
            applicants[cnt]=applicant;
            cnt++;
    }
    
    for (size_t i = 0; i < nol; i++)
    {
        printf("név:%s napok:",applicants[i].name);
        for (size_t j = 0; j < 7; j++)
        {
            printf("%d",applicants[i].days[j]);
        }
        printf("\n");
    }
    char day[20];
    printf("Listázandó nap: ");
    scanf("%s",day);
    int index=0;
    const char *days[] = {"hétfő", "kedd", "szerda", "csütörtök", "péntek", "szombat", "vasárnap"};
    int i=0;
    while (strcmp(day,days[i])) //ez a függvény maga a sátán
    {
        i++;
    }
    index=i;  
    //printf("index:%d",index);
    printf("Jelentkezők ezen a napon: %s\n",day);
    for (size_t i = 0; i < nol; i++)
    {
        if(applicants[i].days[index])
            printf("%s\n",applicants[i].name);
    }
    return 0;
}

char* file_to_string(int* num_of_applicants, char* day, char** second_turn)
//int main()
{
    int bytes = count_bytes("applicants.txt");
    //TODO check bytes = 0
        char hello[]="hello";
    int nol = number_of_lines("applicants.txt");
    Applicant applicants[nol];
    int x=0;
    int offset=0;
    int cnt=0;
    while (offset<bytes)
    {
        char* line = readLine_file(&x,"applicants.txt",offset);
        offset=offset+x+1;
        Applicant applicant;
        char* name;
        char* days;
        int nameLength;
        splitLine(line,&nameLength,&name,&days);
        applicant.name = name;
        //strcpy(applicant.name,name);
        applicant.nameLength=nameLength;
        for (int i = 0; i < 7; i++)
            applicant.days[i] = 0;
        if (days == NULL)
        {
            return NULL;
        }
        char *space = strtok(days, " ");
        while (space)
        {
            if (!strcmp(space, "hétfő"))
            {
                applicant.days[0] = 1;
            }
            else if (!strcmp(space, "kedd"))
            {
                applicant.days[1] = 1;
            }
            else if (!strcmp(space, "szerda"))
            {
                applicant.days[2] = 1;
            }
            else if (!strcmp(space, "csütörtök"))
            {
                applicant.days[3] = 1;
            }
            else if (!strcmp(space, "péntek"))
            {
                applicant.days[4] = 1;
            }
            else if (!strcmp(space, "szombat"))
            {
                applicant.days[5] = 1;
            }
            else if (!strcmp(space, "vasárnap"))
            {
                applicant.days[6] = 1;
            }
            space = strtok(NULL, " ");
        }
            applicants[cnt]=applicant;
            cnt++;
    }
    
    /*for (size_t i = 0; i < nol; i++)
    {
        printf("név:%s napok:",applicants[i].name);
        for (size_t j = 0; j < 7; j++)
        {
            printf("%d",applicants[i].days[j]);
        }
        printf("\n");
    }*/

    int index=0;
    const char *days[] = {"hétfő", "kedd", "szerda", "csütörtök", "péntek", "szombat", "vasárnap"};
    int i=0;
    while (strcmp(day,days[i])) //ez a függvény maga a sátán
    {
        i++;
    }
    index=i;  
    //printf("index:%d",index);
    *num_of_applicants=0;
    *second_turn=malloc(1024*sizeof(char));
    char* str=malloc(1024*sizeof(char));
    for (size_t i = 0; i < nol; i++)
    {
        if(*num_of_applicants>=5){
            if(applicants[i].days[index]){
                (*num_of_applicants)++;
                strcat(*second_turn,applicants[i].name);
                strcat(*second_turn,"\n");
            }
        }
        else
        if(applicants[i].days[index]){
            (*num_of_applicants)++;
            strcat(str,applicants[i].name);
            strcat(str,"\n");
        }
    }
    if(*num_of_applicants<=5){
        free(*second_turn);
        *second_turn=NULL;
    }
    //printf("%s",str);
    return str;
}
/*int main(){
    int strlength;
    char day[20];
    scanf("%s",day);
    char* str=file_to_string(&strlength,day);
    printf("%s\n",str);
    free(str);
    return 0;
}*/
