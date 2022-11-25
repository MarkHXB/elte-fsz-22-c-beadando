typedef struct person
{
    char name[50];
    char job[50];
    char age[50];
    char phone[50];
} person;

typedef struct vehicle
{
    int elerheto;
    person *passengers;
} vehicle;

