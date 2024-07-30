# Tideman Voting System

This project is an implementation of the Tideman voting system in C. The Tideman voting system, also known as the ranked pairs method, selects a single winner using votes that express preferences.  
You can read more about it here - [Tideman Wiki](https://en.wikipedia.org/wiki/Tideman_alternative_method)

## About The Project

The Tideman voting system uses a directed graph to record preferences and determines the winner by identifying the source of the graph. 
## Key Features

- **Recording Preferences**: Records each voter's preferences and creates a directed graph where each node represents a candidate.
- **Adding Pairs**: Adds pairs of candidates based on the preferences, indicating which candidate is preferred over the other.
- **Sorting Pairs**: Sorts the pairs in decreasing order of the strength of the victory.
- **Locking Pairs**: Locks pairs into the graph without creating cycles, ensuring the integrity of the results.
- **Determining the Winner**: Identifies the source of the graph (a node with no incoming edges) as the winner.

## How it works

1. **Vote Recording**: Votes are recorded and preferences are updated.
2. **Preference Recording**: Preferences for each voter are stored in a matrix.
3. **Pair Addition**: Pairs of candidates are added where one candidate is preferred over another.
4. **Pair Sorting**: Pairs are sorted in decreasing order of victory strength.
5. **Pair Locking**: Pairs are locked into the graph while ensuring no cycles are created.
6. **Winner Declaration**: The candidate with no incoming edges in the locked graph is declared the winner.

## Code Overview

### Main Function

The main function initializes the candidate list, records votes, updates preferences, adds pairs, sorts pairs, locks pairs, and finally prints the winner.

### Vote Function

The `vote` function updates the ranks array based on the voter's input and checks if the vote is valid.

### Record Preferences

The `record_preferences` function updates the global preferences matrix based on the ranks array.

### Add Pairs

The `add_pairs` function adds all pairs of candidates where one is preferred over the other to the pairs array.

### Sort Pairs

The `sort_pairs` function sorts the pairs array in decreasing order of the strength of victory using a custom bubble sort algorithm.

### Lock Pairs

The `lock_pairs` function locks pairs into the graph without creating cycles.

### Print Winner

The `print_winner` function identifies and prints the winner of the election.

## Usage
Compile:  
gcc -o tideman tideman.c

Run:  
./tideman Alice Bob Charlie
