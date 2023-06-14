#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stdio.h>
#include <stdlib.h>
#include <fcntl.h>     //open,creat
#include <sys/types.h> //open
#include <sys/stat.h>
#include <errno.h> //perror, errno
#include <stdlib.h>
#include <stdio.h>
#include <signal.h>
#include <sys/types.h>
#include <sys/wait.h>
#include <unistd.h>
#include <sys/ipc.h> 
#include <sys/msg.h> 
#include <sys/types.h> 
#include <stdio.h> 
#include <stdlib.h> 
#include <string.h> 
#include <unistd.h> 
#include <wait.h> 
char* file_to_string(int* num_of_applicants, char* day, char** second_turn);
void handler(int signumber);

void handler(int signumber){
  printf("Az első busz indulásra kész!\n");
}

void handler2(int signumber){
    printf("A második busz indulásra kész!\n");
}

int capacity_of_days[] = {1, 7, 3, 5, 8, 5, 1};
const char *days[] = {"hétfő", "kedd", "szerda", "csütörtök", "péntek", "szombat", "vasárnap"};


int wc(char *file_path, char *word);
typedef struct Applicant
{
    int nameLength;
    char *name;
    int days[7];
} Applicant;
char *readLine(int *lineLength)
{
    int allocated = 20;
    char *line = malloc(allocated);
    for (int i = 0; i < allocated; i++)
    {
        line[i] = 0;
    }
    int alreadyRead = 0;
    char c = getchar();
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
                    c = getchar();
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
        c = getc(stdin);
        alreadyRead++;
    }
    if (lineLength != NULL)
    {
        *lineLength = alreadyRead;
    }
    return line;
}
/*char *readLine(int *lineLength, const char* fname, int offset)
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
}*/
int application(Applicant *applicant, char* name, int nameLength)
{
    int length;
    char *line;
    if(name){
        line=name;
        length = nameLength;
    }
    else{
        
        printf("Kérem a neved:");
        line = readLine(&length);
    }
    if (line == NULL)
    {
        printf("read fail\n");
        return 1;
    }
    printf("Mely napok: ");
    applicant->name = line;
    applicant->nameLength = length;

    for (int i = 0; i < sizeof(applicant->days) / sizeof(applicant->days[0]); i++)
        applicant->days[i] = 0;
    line = readLine(&length);
    if (line == NULL)
    {
        return 0;
    }

    char *space = strtok(line, " ");
    while (space)
    {
        if (!strcmp(space, "hétfő"))
        {
            applicant->days[0] = 1;
        }
        else if (!strcmp(space, "kedd"))
        {
            applicant->days[1] = 1;
        }
        else if (!strcmp(space, "szerda"))
        {
            applicant->days[2] = 1;
        }
        else if (!strcmp(space, "csütörtök"))
        {
            applicant->days[3] = 1;
        }
        else if (!strcmp(space, "péntek"))
        {
            applicant->days[4] = 1;
        }
        else if (!strcmp(space, "szombat"))
        {
            applicant->days[5] = 1;
        }
        else if (!strcmp(space, "vasárnap"))
        {
            applicant->days[6] = 1;
        }
        space = strtok(NULL, " ");
    }
    char c;
    int g = open("applicants.txt", O_WRONLY | O_CREAT | O_APPEND, S_IRUSR | S_IWUSR);
    int full_days[7];
    int successfullApps = 0;
    for (size_t i = 0; i < 7; i++)
    {
        if (applicant->days[i] && wc("applicants.txt", days[i]) == capacity_of_days[i])
        {
            printf("Erre a napra a jelentkezés lezárult:%s\n", days[i]);
            full_days[i] = 1;
        }
        else
        {
            full_days[i] = 0;
            if (applicant->days[i])
                successfullApps++;
        }
    }
    if (successfullApps > 0)
    {
        write(g, "\n", sizeof(char));
        for (int i = 0; i < applicant->nameLength; i++)
        {
            write(g, applicant->name + i, sizeof(char));
        }
    }
    for (size_t i = 0; i < 7; i++)
    {
        if (applicant->days[i] && !full_days[i])
        {
            write(g, " ", sizeof(char));
            for (int j = 0; j < strlen(days[i]); j++)
            {
                write(g, days[i] + j, sizeof(char));
            }
        }
    }
    free(line);
    close(g);
    if (successfullApps > 0){
        if(name) 
            printf("SIKERES MÓDOSÍTÁS\n");
        else
            printf("SIKERES JELENTKEZÉS\n");
    }
    else{
        if(name) 
            printf("SIKERTELEN MÓDOSÍTÁS\n");
        else
            printf("SIKERTELEN JELENTKEZÉS\n");
        return 0;
    }
    return 1;
}
void print_summary()
{
    int g = open("applicants.txt", O_RDONLY, S_IRUSR | S_IWUSR);
    // the three parameter long version of open - it can create the file if it doesnt exist
    // there is a creat function as well - creat(filename,permissions);
    // O_TRUNC = if it existed, clear the content,
    // O_CREAT=create if it doesn't exist
    // 3. parameter the permission, if it has to be created
    // S_IRUSR=permission for reading by the owner e.g.
    // S_IRUSW=permission for writing by the owner e.g.
    if (g < 0)
    {
        perror("Error at opening the file\n");
        exit(1);
    }

    char c;
    while (read(g, &c, sizeof(c)))
    {
        // read gives back the number of bytes
        // 1. parameter the file descriptor
        // 2. parameter the address of variable, we read into
        // 3. parameter the number of bytes we want to read in
        printf("%c", c); // we prints out the content of the file on the screen
                         // write gives back the number of written bytes
                         // 1. parameter the file descriptor
                         // 2. parameter the address of variable we want to write out
                         // 3. parameter the number of bytes we want to write out
    }
    close(g);
}
int find_string_in_file(const char *search_string)
{
    FILE *fp;

    char filename[] = "applicants.txt", line[200];

    fp = fopen(filename, "r");
    if (!fp)
    {
        perror("could not find the file");
        return 0;
    }
    int cnt = 0;
    int match = 0;
    while (fgets(line, 200, fp) != NULL) /* read a line */
    {
        cnt++;
        if (strstr(line, search_string))
        {
            // fputs ( line, stdout ); /* write the line */
            match = 1;
            break;
        }
    }
    fclose(fp);
    if (match)
    {
        printf("sor száma:%i \n", cnt);
        return cnt;
    }
    else
        return 0;
}
int delete_line(int delete_line)
{
    FILE *fileptr1, *fileptr2;
    char filename[] = "applicants.txt";
    char ch;
    int temp = 1;

    // printf("Enter file name: ");
    // scanf("%s", filename);
    // open file in read mode
    fileptr1 = fopen(filename, "r");
    ch = getc(fileptr1);
    /*printf("Az aktuális jelentkezők:\n");
    while (ch != EOF)
    {
        printf("%c", ch);
        ch = getc(fileptr1);
    }*/
    // rewind
    rewind(fileptr1);
    //printf(" \n Enter line number of the line to be deleted:");
    // scanf("%d", &delete_line);
    // open new file in write mode
    fileptr2 = fopen("replica.c", "w");
    int count = 0;
    char c;
    for (c = getc(fileptr1); c != EOF; c = getc(fileptr1))
        if (c == '\n') // Increment count if this character is newline
            count = count + 1;
    // printf("sorok száma: %d\n",count+1);
    rewind(fileptr1);
    ch = getc(fileptr1);
    if (delete_line == count + 1)
    {
        while (temp < delete_line)
        {
            if (ch == '\n')
                temp++;
            if (temp == delete_line)
                break;
            putc(ch, fileptr2);
            ch = getc(fileptr1);
        }
    }
    else
    {

        // ch = getc(fileptr1);
        while (ch != EOF)
        {
            // except the line to be deleted
            if (temp != delete_line)
            {
                // copy all lines in file replica.c
                putc(ch, fileptr2);
            }
            if (ch == '\n')
            {
                temp++;
            }
            ch = getc(fileptr1);
        }
    }
    fclose(fileptr1);
    fclose(fileptr2);
    remove(filename);
    // rename the file replica.c to original name
    rename("replica.c", filename);
    /*printf("\n The contents of file after being modified are as follows:\n");
    fileptr1 = fopen(filename, "r");
    ch = getc(fileptr1);
    while (ch != EOF)
    {
        printf("%c", ch);
        ch = getc(fileptr1);
    }
    fclose(fileptr1);*/
}
int print_daily_summary();
void clearInputBuffer() {
    int c;
    while ((c = getchar()) != '\n' && c != EOF) {
        // Discard characters until newline or EOF is encountered
    }
}
struct uzenet { 
    long mtype;
    int num;
};
int main(int argc, char** argv)
{
    printf("\n**SZŐLŐGAZDASÁG**\nA szükséges munkások száma naponként:\n");
    printf("HÉTFŐ: %d\n",capacity_of_days[0]);
    printf("KEDD: %d\n",capacity_of_days[1]);
    printf("SZERDA: %d\n",capacity_of_days[2]);
    printf("CSÜTÖRTÖK: %d\n",capacity_of_days[3]);
    printf("PÉNTEK: %d\n",capacity_of_days[4]);
    printf("SZOMBAT: %d\n",capacity_of_days[5]);
    printf("VASÁRNAP: %d\n\n",capacity_of_days[6]);
    printf("A kívánt műveletek végrehajtásához nyomja meg az alábbi billentyűket:\n");
    printf(" JELENTKEZÉS---1\n TÖRLÉS---2\n MÓDOSÍTÁS---3\n JELENTKEZŐK LISTÁZÁSA---4\n NAPI JELENTKEZŐK LISTÁZÁSA---5\n BUSZOK INDÍTÁSA---6\n KILÉPÉS---0\n");
   
    int menu=-1;
    while (menu!=0)
    {   
        printf("\nVégrehajtandó művelet:");
        scanf("%d", &menu);
        //getchar(); // clear linefeed from buffer
        clearInputBuffer();
        //printf("menu: %d",menu);
        printf("\n");
        if(menu==0){
            exit(0);
        }else
        switch (menu)
        {
        case 1:
        printf("JELENTKEZÉS\n");
            Applicant x;
            application(&x,NULL,0);
            //printf("%s\n",x.name);
            break;
        case 2:
            printf("TÖRLÉS\n");
            printf("Alkalmazott neve:\n");
            int readchar_cnt;
            char *name = readLine(&readchar_cnt);
            //printf("x:%d\n", readchar_cnt);
            printf("Beolvasott szöveg: %s\n", name);
            if (readchar_cnt)
            {
                int valid_id = find_string_in_file(name);
                if (valid_id)
                {
                    delete_line(valid_id);
                    //return 0;
                }
                else
                    printf("HIBA: Nincs ilyen nevű alkalmazott\n");
            }
            else printf("HIBA: Nincs ilyen nevű alkalmazott\n");
            //return 0;
            break;
        case 3:{
               //RÉGI VERZIÓ
                printf("MÓDOSÍTÁS\n");
                printf("Alkalmazott neve:\n");
                int readchar_cnt;
                char *name = readLine(&readchar_cnt);
                //printf("x:%d\n", readchar_cnt);
                printf("Beolvasott szöveg: %s\n", name);
                if (readchar_cnt)
                {
                    int valid_id = find_string_in_file(name);
                    if (valid_id)
                    {
                        delete_line(valid_id);
                    Applicant x;
                    application(&x,name,readchar_cnt);
                        //return 0;
                    }else printf("HIBA: Nincs ilyen nevű alkalmazott\n");
                }
                else printf("HIBA: Nincs ilyen nevű alkalmazott\n");
                /*//ÚJ VERZIÓ
                printf("MÓDOSÍTÁS\n");
                Applicant x;
                if(application(&x,NULL,0)){
                    int valid_id = find_string_in_file(x.name);
                    if (valid_id)
                    {
                        delete_line(valid_id);
                        //return 0;
                    }else printf("HIBA: Nincs ilyen nevű alkalmazott\n");
                }
                else printf("HIBA: Nincs ilyen nevű alkalmazott\n");
            //return 0;*/
        }
            break;
        case 4:
            printf("ÖSSZESÍTŐ LISTA\n");
            print_summary();

            printf("\n");
            break;
        case 5:
            printf("NAPI LISTA\n");
            print_daily_summary();
            break;
        case 6:
            signal(SIGTERM,handler);
            signal(SIGUSR1,handler2);
            int key = ftok(argv[0], 1);
            int messageQueue = msgget(key, 0600 | IPC_CREAT);
            int num_of_applicants;
            int length;
            printf("\nMai nap:");
            char* day = readLine(&length);
            //printf("%s\n",day);
            //scanf("%d", &day);
            //getchar(); // clear linefeed from buffer
            //clearInputBuffer();
            char* message2;
            int second_turn_num=0;
            char* message=file_to_string(&num_of_applicants,day,&message2);
            //printf("\n---DEBUG---\nnapi dolgozok:\n%sdolgozok szama:%d\n",message,num_of_applicants);fflush(NULL);
            if(message2){
                second_turn_num=num_of_applicants-5;
                //printf("\nSDEBUG\ndolgozok a masodik turnusban:\n%s\ndolgozok szama a masodik turnusban:%d\n\n",message2,second_turn_num);fflush(NULL);
            }
            int pipefd[2]; // unnamed pipe file descriptor array
            int pipefd2[2];
            char sz[1000];  // char array for reading from pipe
            char sz2[1000];
            if (pipe(pipefd) == -1) 
	        {
               perror("Hiba a pipe nyitaskor!");
               exit(EXIT_FAILURE);
            }
            if (pipe(pipefd2) == -1) 
	        {
               perror("Hiba a pipe nyitaskor!");
               exit(EXIT_FAILURE);
            }
            int status;
            pid_t  child1=0;
            pid_t child2=0;
            child1=fork(); //forks make a copy of variables
            if(child1!=0){
                child2=fork();
            }
            //if (child1<0 || child2){perror("The fork calling was not succesful\n"); exit(1);} 
            if (child1>0 && child2>0) //the parent process, it can see the returning value of fork - the child variable!
            {
                
                pause(); //waits till a signal arrive 
                pause();
                close(pipefd2[0]); //Usually we close unused read end
                close(pipefd[0]); //Usually we close unused read end

                //char message[]="Hajrá fradi!";
                write(pipefd[1],message,strlen(message)+1);
                if(message2)
                    write(pipefd2[1],message2,strlen(message2)+1);
                //printf("aaaaa:%s\n",message2);
                close(pipefd[1]); // Closing write descriptor 
                close(pipefd2[1]); // Closing write descriptor
                printf("Szulo beirta az adatokat a csobe!\n");
                sleep(4);
                struct uzenet uz;
                msgrcv(messageQueue, &uz, 1024, 5, 0);
                printf("Szulonek uzenete jott: beszallitott munkasok szama: %i\n", uz.num);
                msgrcv(messageQueue, &uz, 1024, 5, 0);
                printf("Szulonek uzenete jott: beszallitott munkasok szama: %i\n", uz.num);
	            printf("Szulo befejezte!");	
                fflush(stdout);
                waitpid(child1,&status,0);
                waitpid(child2,&status,0);
                msgctl(messageQueue, IPC_RMID, NULL); // messageQueue delete
                
                //waits the end of child process PID number=child, the returning value will be in status
                //0 means, it really waits for the end of child process - the same as wait(&status)

            }
            else if(child1==0 )//child1 process
            {
                close(pipefd2[0]);
                close(pipefd2[1]);
                printf("Első busz felkészítése az indulásra..\n");
                fflush(stdout);
                sleep(1);
                kill(getppid(),SIGTERM);
                close(pipefd[1]);  //Usually we close the unused write end
                sleep(3);
	            printf("Gyerek1 elkezdi olvasni a csobol az adatokat!\n");
                read(pipefd[0],sz,sizeof(sz)); // reading max 100 chars
                printf("Gyerek1 által olvasott uzenet:\n%s \n",sz);
                fflush(stdout);
                close(pipefd[0]); // finally we close the used read end
                if(num_of_applicants<=5){
                    const struct uzenet uz = { 5, num_of_applicants }; 
                    msgsnd(messageQueue, &uz, sizeof(uz), 0);
                }
                else
                {
                    const struct uzenet uz = { 5, 5 }; 
                    msgsnd(messageQueue, &uz, sizeof(uz), 0);
                }
                      
                exit(0);         
            }
            else if(child2==0 && child1>0){
                close(pipefd[0]);
                close(pipefd[1]);
                sleep(2);
                printf("Második busz felkészítése az indulásra..\n");
                fflush(stdout);
                kill(getppid(),SIGUSR1);
                fflush(stdout);
                close(pipefd2[1]);
                sleep(5);
                printf("Gyerek2 elkezdi olvasni a csobol az adatokat!\n");
                read(pipefd2[0],sz2,sizeof(sz2)); // reading max 100 chars
                printf("Gyerek2 által olvasott uzenet:\n%s \n",sz2);
                fflush(stdout);
                const struct uzenet uz = { 5, second_turn_num }; 
                msgsnd(messageQueue, &uz, sizeof(uz), 0);
                close(pipefd2[0]);
                exit(0);
            }
            fflush(stdout);
            sleep(3);
            break;
        }
    }
    
    return 0;
}

