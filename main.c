#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <unistd.h> // for pipe()
#include <signal.h>
#include <sys/types.h>
#include <sys/wait.h>
#include <stdbool.h>

/*
Feladat:
beadando_1:
Edit ✅
Delete ✅
Dep search ✅
Display all ✅

beadando_2:
fork gotowork
*/

// WORKING FILE 🐱‍👤
#define WORK_FILE "gowest3_test.txt"

// TEST CASES   🌌
#define TEST_FILE_V1 "gowest1_test.txt"
#define TEST_FILE_V2 "gowest2_test.txt"
#define TEST_FILE_V3 "gowest3_test.txt"
#define TEST_FILE_V4 "gowest4_test.txt"

// DEFINE HOW TO RUN THE APP 🐱‍🏍
#define RUN_MODE "TEST"

#include "types.h"

void kocsi_handler(int sig)
{
    printf("Kocsi gyerek kapott egy %i signalt.\n", sig);
}

void kisbusz_handler(int sig)
{
    printf("Kisbusz gyerek kapott egy %i signalt.\n", sig);
}

void print_record(person p)
{
    printf("[\n");
    printf("\tName: %s\n", p.name);
    printf("\tAge: %s\n", p.age);
    printf("\tJob: %s\n", p.job);
    printf("\tPhone Number: %s", p.phone);
    printf("\n]\n");
}

int checkJobInput(char *in)
{
    int status = 0;
    char *jobs[4];
    jobs[0] = "programmer";
    jobs[1] = "bricklayer";
    jobs[2] = "carpenter";
    jobs[3] = "paver";
    for (size_t i = 0; i < sizeof(jobs) / sizeof(jobs[0]); i++)
    {
        if (strcmp(jobs[i], in) == 0)
        {
            status = 1;
            break;
        }
    }

    return status;
}

void display()
{
    person p;
    FILE *fp;
    fp = fopen(WORK_FILE, "r");

    while (fread(&p, sizeof(person), 1, fp))
    {
        print_record(p);
    }

    fclose(fp);
}

void append()
{
    FILE *fp;
    person p;

    fp = fopen(WORK_FILE, "ab+");

    fflush(stdin);
    printf("Enter name: ");
    scanf("%s", p.name);

    do
    {
        printf("\nEnter Job Category | ( programmer, bricklayer, carpenter, paver ) : ");
        scanf("%s", p.job);
    } while (checkJobInput(p.job) != 1);

    fflush(stdin);
    printf("Enter age: ");
    scanf("%s", p.age);

    fflush(stdin);
    printf("Enter phone number: ");
    scanf("%s", p.phone);

    fflush(stdin);

    fwrite(&p, sizeof(person), 1, fp);

    printf("\n[ %s ] successfully added to the file.\n", p.name);

    fclose(fp);
}

void search()
{
    person p;
    FILE *fp;
    char c[50];
    int founded;
    fp = fopen(WORK_FILE, "r");

    do
    {
        fflush(stdin);
        printf("\nEnter Job Category to Search | ( programmer, bricklayer, carpenter, paver ) : ");
        scanf("%s", &c);
    } while (checkJobInput(c) != 1);

    fflush(stdin);

    while (fread(&p, sizeof(person), 1, fp))
    {
        if (strcmp(p.job, c) == 0)
        {
            founded = 1;
            printf("Founded record that searched by value: '%s'\n", c);
            print_record(p);
        }
    }

    if (!founded)
        printf("\nRecord Not Found\n");

    fclose(fp);
}

void update()
{
    person p;
    FILE *fp, *fp1;
    char c[20];
    int founded;
    fp = fopen(WORK_FILE, "r");
    fp1 = fopen("temp.txt", "w"); // temp file

    printf("\nEnter Name to Update: ");
    scanf("%s", &c);

    fflush(stdin);
    while (fread(&p, sizeof(person), 1, fp))
    {
        if (strcmp(p.name, c) == 0)
        {
            founded = 1;

            // Name
            fflush(stdin);
            char c;
            printf("You want to modify the name? (y/n)\nCurrent value is: %s\n", p.name);
            scanf("%c", &c);
            if (c == 'y')
            {
                printf("\nEnter New Name: ");
                scanf("%s", p.name);
            }

            // Job Category
            fflush(stdin);
            printf("You want to modify the job category? (y/n)\nCurrent value is: %s\n", p.job);
            scanf("%c", &c);
            if (c == 'y')
            {
                do
                {
                    fflush(stdin);
                    printf("\nEnter New Job Category | ( programmer, bricklayer, carpenter, paver ) : ");
                    scanf("%s", p.job);
                } while (checkJobInput(p.job) != 1);
            }

            // Age
            fflush(stdin);
            printf("You want to modify the age? (y/n)\nCurrent value is: %s\n", p.age);
            scanf("%c", &c);
            if (c == 'y')
            {
                printf("\nEnter New Age: ");
                scanf("%s", p.age);
            }

            // Phone number
            fflush(stdin);
            printf("You want to modify the phone number? (y/n)\nCurrent value is: %s\n", p.phone);
            scanf("%c", &c);
            if (c == 'y')
            {
                printf("\nEnter New Phone number: ");
                scanf("%s", p.phone);
            }
        }
        fwrite(&p, sizeof(person), 1, fp1);
    }

    fclose(fp);
    fclose(fp1);

    if (founded)
    {
        fp1 = fopen("temp.txt", "r");
        fp = fopen(WORK_FILE, "w");

        while (fread(&p, sizeof(person), 1, fp1))
            fwrite(&p, sizeof(person), 1, fp);

        fclose(fp);
        fclose(fp1);
    }
    else
        printf("\nRecord Not Found\n");
}

void delete ()
{
    person p;
    FILE *fp, *fp1;
    char c[20];
    int founded;
    fp = fopen(WORK_FILE, "r");
    fp1 = fopen("temp.txt", "w"); // temp file
    printf("\nEnter Name to Delete: ");
    scanf("%s", &c);
    fflush(stdin);
    while (fread(&p, sizeof(person), 1, fp))
    {
        if (strcmp(p.name, c) == 0)
            founded = 1;
        else
            fwrite(&p, sizeof(person), 1, fp1);
    }

    fclose(fp);
    fclose(fp1);

    if (founded)
    {
        fp1 = fopen("temp.txt", "r");
        fp = fopen(WORK_FILE, "w");

        while (fread(&p, sizeof(person), 1, fp1))
            fwrite(&p, sizeof(person), 1, fp);

        fclose(fp);
        fclose(fp1);
    }
    else
        printf("\nRecord Not Found\n");
}

person *get_people(char *filename)
{
    person *people = calloc(1024, sizeof(person));
    person p;
    FILE *fp;
    fp = fopen(filename, "r");
    int db = 0;

    while (fread(&p, sizeof(person), 1, fp))
    {
        people[db] = p;
        db++;
    }

    fclose(fp);

    return people;
}

char *get_test(int version)
{
    switch (version)
    {
    case 1:
        return TEST_FILE_V1;
    case 2:
        return TEST_FILE_V2;
    case 3:
        return TEST_FILE_V3;
    case 4:
        return TEST_FILE_V4;
    default:
        return TEST_FILE_V1;
    }
}

int get_size(person *people)
{
    int i = 0;

    while (checkJobInput(people[i].job) != 0)
    {
        i++;
    }

    return i;
}

person *get_candidates_count_by_profession(person *people, char *profession)
{
    int db = 0;
    person *candidates = calloc(get_size(people), sizeof(person));
    for (int i = 0; i < get_size(people); i++)
    {
        if (strcmp(people[i].job, profession) == 0)
        {
            candidates[db] = people[i];
            db++;
        }
    }

    return candidates;
}

// Compare function for qsort
int cmpfunc(const void *a, const void *b)
{
    return (*(int *)a - *(int *)b);
}

void go_to_work(person *people)
{
    // ToDo -> to parent

    // most jön a varázslat 🧙‍♂️
    pid_t kocsi, kisbusz;
    int szervezotokocsipipe[2];
    int kocsipipetoszervezo[2];

    // Pipes
    int szervezotokisbuszpipe[2];
    int kisbuszpipetoszervezo[2];
    if (pipe(szervezotokocsipipe) == -1)
    {
        perror("Hiba a pipe3 nyitaskor!");
        exit(EXIT_FAILURE);
    }
    if (pipe(kocsipipetoszervezo) == -1)
    {
        perror("Hiba a pipe3 nyitaskor!");
        exit(EXIT_FAILURE);
    }
    if (pipe(szervezotokisbuszpipe) == -1)
    {
        perror("Hiba a pipe3 nyitaskor!");
        exit(EXIT_FAILURE);
    }
    if (pipe(kisbuszpipetoszervezo) == -1)
    {
        perror("Hiba a pipe3 nyitaskor!");
        exit(EXIT_FAILURE);
    }

    kocsi = fork();

    signal(SIGUSR1, kocsi_handler);
    signal(SIGUSR2, kisbusz_handler);

    int kisbuszstatus;
    int kocsistatus;
    /* Child A = kocsi */
    if (kocsi == 0)
    {
        pause();

        printf("Kocsi beindítva. Fuvarra készen...\n");

        person utasok[4];
        close(szervezotokocsipipe[1]);
        read(szervezotokocsipipe[0], utasok, sizeof(utasok));
        close(szervezotokocsipipe[0]);

        printf("Kocsi utasai a következők:\n");
        for (int i = 0; i < get_size(utasok); i++)
        {
            printf("{%i} %s\n", i + 1, utasok[i].name);
        }

        char *message = "Kész! Megyek haza, kérem a következőt!";
        close(kocsipipetoszervezo[0]);
        write(kocsipipetoszervezo[1], message, strlen(message) + 1);
        close(kocsipipetoszervezo[1]);

        sleep(1);
        kill(getpid(), SIGTERM);
    }
    else
    {
        kisbusz = fork();

        /* Child B = kisbusz */
        if (kisbusz == 0)
        {
            /* code */
            pause();
            printf("Kisbusz beindítva.\nFuvarra készen...\n");

            person utasok[8];
            close(szervezotokisbuszpipe[1]);
            read(szervezotokisbuszpipe[0], utasok, sizeof(utasok));
            close(szervezotokisbuszpipe[0]);

            printf("Kisbusz utasai a következők:\n");
            for (int i = 0; i < get_size(utasok); i++)
            {
                printf("{%i} %s\n", i + 1, utasok[i].name);
            }

            char *message = "Kész! Megyek haza, kérem a következőt!";
            close(kisbuszpipetoszervezo[0]);
            write(kisbuszpipetoszervezo[1], message, strlen(message) + 1);
            close(kisbuszpipetoszervezo[1]);

            sleep(1);
            kill(getpid(), SIGTERM);
        }
        /* Parent = gowest */
        else
        {
            person *programmers = get_candidates_count_by_profession(people, "programmer");
            person *carpenters = get_candidates_count_by_profession(people, "carpenter");
            person *pavers = get_candidates_count_by_profession(people, "paver");
            person *bricklayers = get_candidates_count_by_profession(people, "bricklayer");

            int programmersCount = get_size(programmers);
            int carpentersCount = get_size(carpenters);
            int paversCount = get_size(pavers);
            int bricklayersCount = get_size(bricklayers);

            if (programmersCount > 3 ||
                carpentersCount > 3 ||
                paversCount > 3 ||
                bricklayersCount > 3)
            {
                printf("A szervező összeállítja a jelentkezők alapján, ki melyik járművel megy dolgozni.\n");

                // Kik mennek a kisbusszal:
                // Todo:
                int arrPCounts[] = {programmersCount, carpentersCount, paversCount, bricklayersCount};
                qsort(arrPCounts, 4, sizeof(int), cmpfunc);
                int firstMax = arrPCounts[3];
                int secondMax = arrPCounts[2];
                person kocsivalmennek[4];
                person busszalmennek[8];
                bool buszOk = false;
                bool kocsiOk = false;
                if (firstMax > 4)
                {
                    if (programmersCount == firstMax)
                    {
                        for (int i = 0; i < get_size(programmers) && i < 7; i++)
                            busszalmennek[i] = programmers[i];
                        buszOk = true;
                    }
                    else if (carpentersCount == firstMax)
                    {
                        for (int i = 0; i < get_size(carpenters) && i < 8; i++)
                            busszalmennek[i] = carpenters[i];
                        buszOk = true;
                    }
                    else if (paversCount == firstMax)
                    {
                        for (int i = 0; i < get_size(pavers) && i < 8; i++)
                            busszalmennek[i] = pavers[i];
                        buszOk = true;
                    }
                    else
                    {
                        for (int i = 0; i < get_size(bricklayers) && i < 8; i++)
                            busszalmennek[i] = bricklayers[i];
                        buszOk = true;
                    }
                }
                if (firstMax > 2 && firstMax < 5 || (buszOk == true && secondMax > 2 && secondMax < 5))
                {
                    int max = firstMax;
                    if (buszOk == true)
                    {
                        max = secondMax;
                    }
                    if (programmersCount == max)
                    {
                        for (int i = 0; i < get_size(programmers); i++)
                        {
                            kocsivalmennek[i] = programmers[i];
                        }

                        kocsiOk = true;
                    }
                    else if (carpentersCount == max)
                    {
                        for (int i = 0; i < get_size(carpenters); i++)
                            kocsivalmennek[i] = carpenters[i];
                        kocsiOk = true;
                    }
                    else if (paversCount == max)
                    {
                        for (int i = 0; i < get_size(pavers); i++)
                            kocsivalmennek[i] = programmers[i];
                        kocsiOk = true;
                    }
                    else
                    {
                        for (int i = 0; i < get_size(bricklayers); i++)
                            kocsivalmennek[i] = bricklayers[i];
                        kocsiOk = true;
                    }
                }

                // Kik mennek busszal:
                if (get_size(busszalmennek) > 4 && buszOk == true)
                {
                    for (int j = 0; j < get_size(busszalmennek); j++)
                    {
                        printf("Összeállított utasok akik kisbusszal mennek {%i}: %s\n", j + 1, busszalmennek[j].name);
                    }

                    close(szervezotokisbuszpipe[0]);
                    write(szervezotokisbuszpipe[1], busszalmennek, sizeof(busszalmennek));
                    close(szervezotokisbuszpipe[1]);

                    printf("A szervező 'Go West' elküldte az utas listát a buszsofőrnek.\n");
                    sleep(1);
                    kill(kisbusz, SIGUSR2);
                    waitpid(kisbusz, &kisbuszstatus, 0);

                    char messageBuffer[254];
                    close(kisbuszpipetoszervezo[1]);
                    read(kisbuszpipetoszervezo[0], messageBuffer, sizeof(char) * 254);
                    close(kisbuszpipetoszervezo[0]);
                    printf("Kapott üzenet a kisbusztól: %s\n", messageBuffer);
                }
                // Kik mennek kocsival:
                if (get_size(kocsivalmennek) > 2 && kocsiOk == true)
                {
                    for (int j = 0; j < get_size(kocsivalmennek); j++)
                    {
                        printf("Összeállított utasok akik kocsival mennek {%i}: %s\n", j + 1, kocsivalmennek[j].name);
                    }

                    close(szervezotokocsipipe[0]);
                    write(szervezotokocsipipe[1], kocsivalmennek, sizeof(kocsivalmennek));
                    close(szervezotokocsipipe[1]);

                    printf("A szervező 'Go West' elküldte az utas listát az autósofőrnek.\n");
                    sleep(1);
                    kill(kocsi, SIGUSR1);
                    waitpid(kocsi, &kocsistatus, 0);

                    char messageBuffer[254];
                    close(kocsipipetoszervezo[1]);
                    read(kocsipipetoszervezo[0], messageBuffer, sizeof(char) * 254);
                    close(kocsipipetoszervezo[0]);
                    printf("Kapott üzenet a kocsitól: %s\n", messageBuffer);
                }

                //exit(1);
                sleep(1);
            }
            else
            {
                printf("Nincs elég azonos szakmájú jelentkező, ezért semmilyen formában nem fog történni fuvar.\n");
                exit(1);
            }
        }
    }
}

void run_test(char *version)
{
    // printf("\nI've got the version: %s", version);
    person *people = get_people(version);
    go_to_work(people);
}

void run_debug()
{
    int input;

    do
    {
        printf("\n1.\tDisplay all people");
        printf("\n2.\tAdd a person");
        printf("\n3.\tSearch by job category");
        printf("\n4.\tUpdate a person");
        printf("\n5.\tDelete a person");
        printf("\n6.\tSend people to work");
        printf("\n0.\tExit");

        printf("\nEnter your choice: ");
        scanf("%i", &input);

        switch (input)
        {
        case 1:
            display();
            break;
        case 2:
            append();
            break;
        case 3:
            search();
            break;
        case 4:
            update();
            break;
        case 5:
            delete ();
            break;
        case 6:
            go_to_work(get_people(WORK_FILE));
            break;
        default:
            break;
        }

    } while (input != 0);
}

int main(int argc, char **argv)
{
    // RUN APP AS COMMAND LINE WANT
    if (argc > 1)
    {
        char *run_mode = argv[1];
        int test_version;

        // GET TEST VERSION
        if (argc > 2)
            test_version = atoi(argv[2]);

        if (strcmp(run_mode, "debug") == 0)
            run_debug();
        else if (argc > 2 && strcmp(run_mode, "test") == 0)
            run_test(get_test(test_version));
    }
    // RUN APP IF ANY ARGUMENTS WERE GIVEN
    else
    {
        if (strcmp(RUN_MODE, "DEBUG") == 0)
            run_debug();
        else if (strcmp(RUN_MODE, "TEST") == 0)
            run_test(get_test(2));
        else
        {
            printf("\nYou should define either DEBUG or TEST run mode for the program...");
            exit(1);
        }
    }
}