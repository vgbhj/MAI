#include <stdio.h>
#include <string.h>
#include <errno.h>

#include "person.h"

void usage(){
    printf("Usage: program input_filename output_filename\n");
}

int readpassenger(FILE *in, stud *s){
    int n = fscanf(in, "%[^\t]\t%[^\t]\t%c\t%d\t%d\t%d\t%d\t%d\n", s->surname, s->initials, &s->sx, &s->gr, &s->mathematics, &s->physics, &s->informatics, &s->history);
    // Debug
    // printf("%d\n", n);
    return n == 8;
}

int main(int argc, char* argv[]){
    if (argc != 3){
        usage();
        return 1;  
    }
    stud s;
    FILE *in = fopen(argv[1], "r");
    FILE *out = fopen(argv[2], "w");
    if (!(out&&in)){
        perror("Can't open file");
        return 2;
    }
    while (readpassenger(in, &s)){
        fwrite(&s, sizeof (s), 1, out);
    }
    // Debug
    // fclose(out);
    // out = fopen(argv[2], "r");
    // while (fread(&s, sizeof(s), 1, out) == 1) {
    //         printf("|%-25s|%-3s|%-3c|%-7d|%-6d|%-6d|%-6d|%-6d|\n", s.surname, s.initials, s.sx, s.gr, s.mathematics, s.physics, s.informatics, s.history);
    //         printf("_____________________________________________________________________\n");
    // }
    return 0;
}