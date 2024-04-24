/*
* Q) Write a C program to count nnumber of characters, words and lines in a text file. Logic to count number of characters, words and lines in a text file in C programming.How to count number of characters, words and lines in a text file in C programming.
 */

/*
───────┬───────────────────────────────────────────────────────────────────────────────────────────────
       │ File: test.txt
───────┼───────────────────────────────────────────────────────────────────────────────────────────────
   1   │ Hello world 
   2   │ I dont know what more to say 
   3   │ so gn
───────┴───────────────────────────────────────────────────────────────────────────────────────────────
*/

#include <stdio.h>

int main() {
    FILE *file;
    char filename[100];
    char ch;
    int charCount = 0, wordCount = 0, lineCount = 0, spaceCount = 0, inWord = 0;

    // Get the filename from the user
    printf("Enter the filename: ");
    scanf("%s", filename);

    // Open the file in read mode
    file = fopen(filename, "r");

    // Check if the file exists
    if (file == NULL) {
        printf("File not found or unable to open.\n");
        return 1; // Exit the program with an error code
    }

    // Read the characters from the file and count characters, words, spaces, and lines
    while ((ch = fgetc(file)) != EOF) {
        charCount++;

        // Check for a new line
        if (ch == '\n') {
            lineCount++;
        }

        // Check for a space
        if (ch == ' ' || ch == '\t') {
            spaceCount++;
            inWord = 0; // Reset the flag indicating that we are in a word
        } else {
            // Check for the start of a word
            if (inWord == 0) {
                inWord = 1;
                wordCount++;
            }
        }
    }

    // Close the file
    fclose(file);

    // Display the results
    printf("Total characters: %d\n", charCount);
    printf("Total words: %d\n", wordCount);
    printf("Total spaces: %d\n", spaceCount);
    printf("Total lines: %d\n", lineCount);

    return 0;
}

/*
Output:

Enter the filename: test.txt
Total characters: 49
Total words: 11
Total spaces: 10
Total lines: 3
*/
