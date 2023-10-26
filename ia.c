#include<stdlib.h>
#include<stdio.h>
#include<string.h>


#define N 100 /* Defining the variables that will be used in the program and the maximum number of people*/
#define min(i, j) (((i) < (j)) ? (i) : (j))

#define HASH_SIZE 101 /* Hash table size */

/* Hash function */
unsigned int hash(char* str)
{
    unsigned int hashval = 0;
    for(int i = 0; i < strlen(str); i++)
    {
        hashval = hashval * 31 + str[i];
    }
    return hashval % HASH_SIZE;
}

/* Hash table entry */
typedef struct hash_entry
{
    char name[N];
    int index;
    struct hash_entry* next;
} hash_entry;

/* Hash table */
hash_entry* hash_table[HASH_SIZE];

/* Insert a person into the hash table */
void insert_person(char* name, int index)
{
    unsigned int hashval = hash(name);
    hash_entry* entry = (hash_entry*) malloc(sizeof(hash_entry));
    strcpy(entry->name, name);
    entry->index = index;
    entry->next = hash_table[hashval];
    hash_table[hashval] = entry;
}

/* Find the index of a person in the people array */
int find_person(char* name)
{
    unsigned int hashval = hash(name);
    hash_entry* entry = hash_table[hashval];
    while(entry != NULL)
    {
        if(strcmp(entry->name, name) == 0)
        {
            return entry->index;
        }
        entry = entry->next;
    }
    return -1;
}

/* Graph of debts */
int** graph;

/* Names of people */
char people[N][N];

/* Number of people */
int n;

/* Returns index of minimum value in a[] */
int get_min(int a[])
{
    int min_ind = 0;
    for(int i=1; i<n; i++)
    {
        if (a[i] < a[min_ind])
        {
            min_ind = i;
        }
    }
    return min_ind;
}

/* Returns index of maximum value in a[] */
int get_max(int a[])
{
    int max_ind = 0;
    for(int i=1; i<n; i++)
    {
        if (a[i] > a[max_ind])
        {
            max_ind = i;
        }
    }
    return max_ind;
}

/* amt[p] indicates the net amount to be credited/debited to from person 'p'
  If amt[p] is positive, then i'th person will get amt[i]
  If amt[p] is negative, then i'th person will give  -amt[i]*/
void min_cashflow_rec(int amt[])
{
    int mcr = get_max(amt), mdb = get_min(amt);
    if(amt[mcr] == 0 && amt[mdb] == 0)
    {
        return;
    }
    int minval = min(-amt[mdb], amt[mcr]);
    amt[mcr] -= minval;
    amt[mdb] += minval;
    printf("\n%s will pay Rs.%d to %s",people[mdb],minval,people[mcr]);
    min_cashflow_rec(amt);
}

/* Given a set of persons as graph[] where graph[i][j] indicates the amount that person i needs
  to pay person j, this function finds and prints the minimum cash flow to settle all debts. */
void min_cashflow(int** graph)
{
    int amt[10];
    for(int i=0; i<n; i++)
    {
        amt[i] = 0;
    }
    for(int p=0; p<n; p++)
    {
      for(int i=0; i<n; i++)
      {
        amt[p] += (graph[i][p] -  graph[p][i]);
      }
    }
    min_cashflow_rec(amt);
}

int main()
{
    printf("      ** Welcome to our Data Structures Project on Cash Flow Minimiser. **\n\n");
    printf("                                                                          -made by Soumya Bokare & Kaveri Chawhan ");
    printf("\nEnter no. of people: ");
    scanf("%d",&n);

    /* Initialize the hash table to NULL */
    for(int i = 0; i < HASH_SIZE; i++)
    {
        hash_table[i] = NULL;
    }

    /* Allocate memory for the graph array */
    graph = (int**) malloc(n * sizeof(int*));
    for(int i = 0; i < n; i++)
    {
        graph[i] = (int*) malloc(n * sizeof(int));
    }

    /* Accept names of people who owe each other */
    for(int i=0; i<n; i++)
    {
        printf("Enter name %d : ",i+1);
        scanf("%s",&people[i]);
        insert_person(people[i], i);
    }

    /* Accept debts */
    for(int i=0; i<n; i++)
    {
        for(int j=0; j<n; j++)
        {
            if(i == j)
            {
                continue;
            }
            printf("How much %s has to pay %s?   Rs.",people[i],people[j]);
            scanf("%d",&graph[i][j]);
        }
    }

    /* Find the minimum cash flow */
    min_cashflow(graph);

    /* Free memory allocated for the graph array */
    for(int i = 0; i < n; i++)
    {
        free(graph[i]);
    }
    free(graph);

    return 0;
}