#include <string.h>
#include <stdio.h>
#include "startswith.h"

bool startswith(char strStartingWith[], char code[]) {
    int similarity = 0;
    int length = strlen(strStartingWith);
    for (int i = 0; i < length; i++){
        if (strStartingWith[i] == code[i]){
            similarity++;
        }
    }
    return similarity >= length;
}
