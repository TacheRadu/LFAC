#include <string.h>

void logical_operations(float *$$, float *$1, float *$3, char *$2){
    if(strcmp($2, "==") == 0)
        *$$ = (float)(*$1 == *$3);
    else if(strcmp($2, "<") == 0)
        *$$ = (float)(*$1 < *$3);
    else if(strcmp($2, "<=") == 0)
        *$$ = (float)(*$1 <= *$3);
    else if(strcmp($2, ">") == 0)
        *$$ = (float)(*$1 > *$3);
    else if(strcmp($2, ">=") == 0)
        *$$ = (float)(*$1 >= *$3);
}