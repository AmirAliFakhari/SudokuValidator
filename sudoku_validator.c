#include <pthread.h>
#include <stdio.h>

// Sample Sudoku board (0 represents empty cells)
int board[9][9] = {
    {5, 3, 0, 0, 7, 0, 0, 0, 0},
    {6, 0, 0, 1, 9, 5, 0, 0, 0},
    {0, 9, 8, 0, 0, 0, 0, 6, 0},
    {8, 0, 0, 0, 6, 0, 0, 0, 3},
    {4, 0, 0, 8, 0, 3, 0, 0, 1},
    {7, 0, 0, 0, 2, 0, 0, 0, 6},
    {0, 6, 0, 0, 0, 0, 2, 8, 0},
    {0, 0, 0, 4, 1, 9, 0, 0, 5},
    {0, 0, 0, 0, 8, 0, 0, 7, 9}
};

// int board[9][9] = {
//     {5, 3, 5, 6, 7, 8, 9, 1, 2}, // Duplicate 5 in row 0
//     {6, 7, 2, 1, 9, 5, 3, 4, 8},
//     {1, 9, 8, 3, 4, 2, 5, 6, 7},
//     {8, 5, 9, 7, 6, 1, 4, 2, 3},
//     {4, 2, 6, 8, 5, 3, 7, 9, 1},
//     {7, 1, 3, 9, 2, 4, 8, 5, 6},
//     {9, 6, 1, 5, 3, 7, 2, 8, 4},
//     {2, 8, 7, 4, 1, 9, 6, 3, 5},
//     {3, 4, 5, 2, 8, 6, 1, 7, 9}
// };

// Array to store validation results (0: invalid, 1: valid)
int valid[11] = {0};

// Function to check all rows
void* check_rows(void* arg) {
    for (int i = 0; i < 9; i++) {
        int seen[10] = {0};
        for (int j = 0; j < 9; j++) {
            int num = board[i][j];
            if (num == 0) continue; // Skip empty cells
            if (num < 1 || num > 9 || seen[num]) return NULL;
            seen[num] = 1;
        }
    }
    valid[0] = 1; // Rows are valid
    return NULL;
}

// Function to check all columns
void* check_columns(void* arg) {
    for (int j = 0; j < 9; j++) {
        int seen[10] = {0};
        for (int i = 0; i < 9; i++) {
            int num = board[i][j];
            if (num == 0) continue; // Skip empty cells
            if (num < 1 || num > 9 || seen[num]) return NULL;
            seen[num] = 1;
        }
    }
    valid[1] = 1; // Columns are valid
    return NULL;
}

// Function to check a 3x3 subgrid
void* check_subgrid(void* arg) {
    int idx = *(int*)arg;
    int row = (idx / 3) * 3;
    int col = (idx % 3) * 3;
    int seen[10] = {0};
    for (int i = row; i < row + 3; i++) {
        for (int j = col; j < col + 3; j++) {
            int num = board[i][j];
            if (num == 0) continue; // Skip empty cells
            if (num < 1 || num > 9 || seen[num]) return NULL;
            seen[num] = 1;
        }
    }
    valid[2 + idx] = 1; // Subgrid is valid
    return NULL;
}

int main() {
    pthread_t threads[11];
    int subgrid_ids[9] = {0, 1, 2, 3, 4, 5, 6, 7, 8};

    // Create thread for rows
    pthread_create(&threads[0], NULL, check_rows, NULL);

    // Create thread for columns
    pthread_create(&threads[1], NULL, check_columns, NULL);

    // Create 9 threads for subgrids
    for (int i = 0; i < 9; i++) {
        pthread_create(&threads[2 + i], NULL, check_subgrid, &subgrid_ids[i]);
    }

    // Wait for all threads to finish
    for (int i = 0; i < 11; i++) {
        pthread_join(threads[i], NULL);
    }

    // Check final result
    for (int i = 0; i < 11; i++) {
        if (!valid[i]) {
            printf("Sudoku is INVALID\n");
            return 0;
        }
    }
    printf("Sudoku is VALID\n");
    return 0;
}