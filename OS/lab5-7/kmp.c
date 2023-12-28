#include "kmp.h"

// Function to calculate the prefix function for the pattern
void computePrefix(const char *pattern, int *prefix, int patternSize) {
    int length = 0;
    prefix[0] = 0;

    for (int i = 1; i < patternSize; i++) {
        while (length > 0 && pattern[i] != pattern[length]) {
            length = prefix[length - 1];
        }

        if (pattern[i] == pattern[length]) {
            length++;
        }

        prefix[i] = length;
    }
}

// Function to perform string matching using KMP algorithm and return positions
int* kmpSearch(const char *text, const char *pattern, int *resultSize) {
    int textSize = strlen(text);
    int patternSize = strlen(pattern);

    // Allocate memory for the prefix function
    int *prefix = (int *)malloc(patternSize * sizeof(int));
    computePrefix(pattern, prefix, patternSize);

    // Allocate memory to store positions
    int *positions = (int *)malloc(textSize * sizeof(int));
    int count = 0;

    int i = 0; // Index for text
    int j = 0; // Index for pattern

    while (i < textSize) {
        if (pattern[j] == text[i]) {
            j++;
            i++;
        }

        if (j == patternSize) {
            // Pattern found at index i - j
            positions[count++] = i - j;
            j = prefix[j - 1];
        } else if (i < textSize && pattern[j] != text[i]) {
            if (j != 0) {
                j = prefix[j - 1];
            } else {
                i++;
            }
        }
    }

    // Update the resultSize
    *resultSize = count;

    // Reallocate memory for the exact size
    positions = (int *)realloc(positions, count * sizeof(int));

    // Free memory
    free(prefix);

    return positions;
}

char* KMP(const char* str, const char* sub) {
    char ans[256];

    int resultSize;
    int *positions = kmpSearch(str, sub, &resultSize);

    if (resultSize > 0) {
        int offset = 0;
        for (int i = 0; i < resultSize; i++) {
            offset += sprintf(ans + offset, "%d", positions[i]);
            if (i < resultSize - 1) {
                offset += sprintf(ans + offset, ";");
            }
        }
    } else {
        sprintf(ans, "-1");
    }

    free(positions);

    return strdup(ans);  // strdup allocates memory for the string and returns a pointer
}