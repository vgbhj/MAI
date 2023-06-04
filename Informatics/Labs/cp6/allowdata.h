#ifndef allowdata_h
#define allowdata_h

#define ALLOWED_SURNAME_COUT 10

char* allowedSurnamesMale[ALLOWED_SURNAME_COUT] = {
    "Zaitsev",
    "Alekseev",
    "Nikitin",
    "Vasilyev",
    "Gagarin",
    "Maksimov",
    "Baykasenov",
    "Pozdniy",
    "Temniy",
    "Nadtochiy",
};

char* allowedSurnamesFemale[ALLOWED_SURNAME_COUT] = {
    "Zaitseva",
    "Alekseeva",
    "Nikitina",
    "Vasilyeva",
    "Gagarina",
    "Maksimova",
    "Baykasenova",
    "Pozdniya",
    "Temniya",
    "Nadtochiy",
};

char* allowedInitials[] = {
    "IA",
    "DM",
    "MS",
    "MC",
    "VV",
    "AZ",
    "OB",
    "NN",
    "PN",
    "KB",
    "PB",
    "OO",
    "AA",
    "NL",
    "LA",
    "US",
    "IS",
    "TO",
    "RB",
    "FM",
};
char* allowedSex[] = {
    "M",
    "F",
};
int allowedGroups[] = {
    // 101, 102, 103, 104, 105, 106, 107, 108, 109, 110
    101, 102, 103, 104,
};
int allowedMarks[] = {
    2,3,4,5
};
#endif