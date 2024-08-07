#include <cs50.h>
#include <stdbool.h>
#include <stdio.h>
#include <string.h>

// Max number of candidates
#define MAX 9

// preferences[i][j] is number of voters who prefer i over j
int preferences[MAX][MAX];

// locked[i][j] means i is locked in over j
bool locked[MAX][MAX];

// Each pair has a winner, loser
typedef struct
{
    int winner;
    int loser;
} pair;

// Array of candidates
string candidates[MAX];
pair pairs[MAX * (MAX - 1) / 2];

int pair_count;
int candidate_count;
int votes = 0;

// Function prototypes
bool vote(int rank, string name, int ranks[]);
void record_preferences(int ranks[]);
void add_pairs(void);
void sort_pairs(void);
void lock_pairs(void);
void print_winner(void);
int get_strength(int index);
bool is_cycle(int x, int start);

int main(int argc, string argv[])
{
    // Check for invalid usage
    if (argc < 2)
    {
        printf("Usage: tideman [candidate ...]\n");
        return 1;
    }

    // Populate array of candidates
    candidate_count = argc - 1;
    if (candidate_count > MAX)
    {
        printf("Maximum number of candidates is %i\n", MAX);
        return 2;
    }
    for (int i = 0; i < candidate_count; i++)
    {
        candidates[i] = argv[i + 1];
    }

    // Clear graph of locked in pairs
    for (int i = 0; i < candidate_count; i++)
    {
        for (int j = 0; j < candidate_count; j++)
        {
            locked[i][j] = false;
        }
    }

    pair_count = 0;
    int voter_count = get_int("Number of voters: ");

    // Query for votes
    for (int i = 0; i < voter_count; i++)
    {
        // ranks[i] is voter's ith preference
        int ranks[candidate_count];

        // Query for each rank
        for (int j = 0; j < candidate_count; j++)
        {
            string name = get_string("Rank %i: ", j + 1);

            if (!vote(j, name, ranks))
            {
                printf("Invalid vote.\n");
                return 3;
            }
        }

        record_preferences(ranks);

        printf("\n");
    }

    add_pairs();
    sort_pairs();
    lock_pairs();
    print_winner();
    return 0;
}

// Update ranks given a new vote
bool vote(int rank, string name, int ranks[])
{            // total votes / total candidates = total voters
    votes++; // we will be using this information later in other functions

    for (int i = 0; i < candidate_count; i++)
    {
        if (strcmp(candidates[i], name) == 0)
        {
            ranks[rank] = i;
            return true;
        }
    }
    return false;
}

// Update preferences given one voter's ranks
void record_preferences(int ranks[])
{
    int bitshift = 0; // seen count that uses binary bitshifting
    // based on its rank context, its important to check if candidate has been seen
    for (int i = 0; i < candidate_count; i++)
    {
        int candidate = ranks[i];

        for (int j = 0; j < candidate_count; j++)
        {
            if ((j == candidate) || (bitshift & (1 << j)))
            { // this means its ranking himself or has been seen
                continue;
            }
            preferences[candidate][j]++;
        }
        bitshift |= 1 << candidate;
    }
}

// Record pairs of candidates where one is preferred over the other
void add_pairs(void)
{
    for (int i = 0; i < candidate_count; i++)
    {
        for (int j = 0; j < candidate_count; j++)
        {
            int x = preferences[i][j];
            int y = preferences[j][i];

            if (x > y)
            { // checks if votes is higher than 50%, this ignores ties
                pairs[pair_count].winner = i;
                pairs[pair_count].loser = j;
                pair_count++;
            }
        }
    }
}

// Sort pairs in decreasing order by strength of victory
void sort_pairs(void)
{ // FUCK IMPLEMENTING MERGE SORT HERE JUST LOST HOURS TRYING
    bool has_swaped = true;

    while (has_swaped)
    { // some wacky bubble sort algo i made
        has_swaped = false;
        for (int i = 0; i < pair_count - 1; i++)
        {
            pair temp = pairs[i];
            int x = get_strength(i);
            int y = get_strength(i + 1);

            if (y > x) // if right one is stronger than left one
            {
                pairs[i] = pairs[i + 1];
                pairs[i + 1] = temp;
                has_swaped = true;
            }
        }
    }
}

// Lock pairs into the candidate graph in order, without creating cycles
void lock_pairs(void)
{
    for (int i = 0; i < pair_count; i++)
    {
        int winner = pairs[i].winner;
        int loser = pairs[i].loser;
        if (!is_cycle(loser, winner))
        {
            locked[winner][loser] = true;
        }
    }
}

bool is_cycle(int x, int start)
{ // check if loser of the above function meets the winner
    if (x == start)
    {                // is it returning from the current function aswell?
        return true; // base case
    }
    for (int i = 0; i < candidate_count; i++)
    {
        if (locked[x][i])
        { // might need an if here to return the current function
            if (is_cycle(i, start))
            {
                return true;
            }
        }
    }
    return false;
}

// Print the winner of the election
void print_winner(void)
{
    bool has_been_locked[candidate_count];
    for (int i = 0; i < candidate_count; i++)
    {
        for (int j = 0; j < candidate_count; j++)
        {
            if (locked[i][j])
            {
                has_been_locked[j] = true;
            }
        }
    }
    for (int i = 0; i < candidate_count; i++)
    {
        if (!has_been_locked[i])
        {
            printf("%s\n", candidates[i]);
        }
    }
}

int get_strength(int index)
{ // we can get loser vote count by having voter_count - win_votes
    // and with that we can get strength which is win_votes - voter_count
    // the formula below is a bit more simplified but same idea
    int voters = votes / candidate_count; // gets voters size
    int win_votes = preferences[pairs[index].winner][pairs[index].loser];
    return (2 * win_votes) - voters;
}
