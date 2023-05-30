#include <stdio.h>
#include <string.h>
#include <limits.h>
#include "person.h"

int max(int a, int b){
    if(a >= b){
        return a;
    }
    return b;
}

int min(int a, int b){
    if(a > b){
        return b;
    }
    return a;
}

void usage() {
    printf("Usage: program [-key] filename\nKeys: -f -p\n");
}

int main(int argc, char *argv[]) {
    FILE *in;
    int f = 0, p = 0;
    if (argc != 3) {
        usage();
        return 1;
    }
    if (strcmp(argv[1], "-f") == 0) {
        f = 1;
        in = fopen(argv[2], "r");
    } else if (strcmp(argv[1], "-p") == 0) {
        p = 1;
        in = fopen(argv[2], "r");
    } else {
        usage();
        return 2;
    }
    stud s;
    if (!in) {
        perror("Can not open file\n");
        return 3;
    }
    if (f) {
        printf("______________________________________________________________________\n");
        printf("|          NAME           | I | S | GROUP | MATH | PHYS | INF | HIST |\n");
        printf("______________________________________________________________________\n");

        while (fread(&s, sizeof(s), 1, in) == 1) {
            if (f) {
                printf("|%-25s|%-3s|%-3c|%-7d|%-6d|%-6d|%-5d|%-6d|\n", s.surname, s.initials, s.sx, s.gr, s.mathematics, s.physics, s.informatics, s.history);
                printf("______________________________________________________________________\n");
            }
        }
    }
    fseek(in, 0, SEEK_SET);
    if(p){
        double v[4][2];
        while (fread(&s, sizeof(s), 1, in) == 1) {
            double sr = (s.mathematics+s.physics+s.physics+s.history)*1.0/4;
            int gr = s.gr%10-1;
            if(v[gr][0] == 0){
                v[gr][0] = sr;
                v[gr][1] = sr;
            }
            v[gr][0] = min(v[gr][0], sr);
            v[gr][1] = max(v[gr][1], sr);
        }
        int ans = 101;
        int mx = 0;
        for(int i = 0; i < 4; ++i){
            if(v[i][1] - v[i][0] > mx){
                ans = 101+i;
                mx = v[i][1] - v[i][0];
            }
            // Debug
            printf("%d, min: %f, max: %f \n", 100+i+1, v[i][0], v[i][1]);
        }
        printf("%d\n", ans);
    }
    
    return 0;
}