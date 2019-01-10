
#include <stdio.h>
#include <stdlib.h>
#include "symtable.h"

int main(int argc, char **argv) {

    int num;
    void *ptrElem;
    const char *criteria;
    FILE *file = fopen("../_data.txt", "r");
    size_t arrayLength, maxWordLength;
    fscanf(file, "%i, %i\n", &arrayLength, &maxWordLength);
    SymTable smtb = smtb_new();
    if (smtb == NULL) {
        fprintf(stderr, "Memory issues [array]...\n");
        exit(EXIT_FAILURE);
    }
    while (1) {
        char *ptrKey = (char *) malloc((maxWordLength + 1) * sizeof(char));
        double *ptrValue = (double *) malloc(sizeof(double));
        if (ptrKey == NULL) {
            fprintf(stderr, "Memory issues [ptrKey]...\n");
            exit(EXIT_FAILURE);
        }
        if (ptrValue == NULL) {
            fprintf(stderr, "Memory issues [ptrValue]...\n");
            exit(EXIT_FAILURE);
        }
        num = fscanf(file, " %s\t%lf\n", ptrKey, ptrValue);
        if (num == 0) {
            break;
        }
        if (num == EOF) {
            break;
        }
        smtb_put(smtb, ptrKey, (void *) ptrValue);
    }
    fclose(file);

    criteria = "wprvxmbftj";
    ptrElem = smtb_remove(smtb, criteria);
    if (ptrElem == NULL) {
        printf("Element not found!!!\n");
    } else {
        printf("    Passed key ---> %s\nReturned value ---> %.8f\n\n", criteria, *(double *) ptrElem);
        smtb_print(smtb);
    }

    criteria = "chctpccwhq";
    ptrElem = smtb_remove(smtb, criteria);
    if (ptrElem == NULL) {
        printf("Element not found!!!\n");
    } else {
        printf("    Passed key ---> %s\nReturned value ---> %.8f\n\n", criteria, *(double *) ptrElem);
        smtb_print(smtb);
    }

    criteria = "tgmhopsexx";
    ptrElem = smtb_remove(smtb, criteria);
    if (ptrElem == NULL) {
        printf("Element not found!!!\n");
    } else {
        printf("    Passed key ---> %s\nReturned value ---> %.8f\n\n", criteria, *(double *) ptrElem);
        smtb_print(smtb);
    }

    criteria = "yuaznzqsbr";
    ptrElem = smtb_remove(smtb, criteria);
    if (ptrElem == NULL) {
        printf("Element not found!!!\n");
    } else {
        printf("    Passed key ---> %s\nReturned value ---> %.8f\n\n", criteria, *(double *) ptrElem);
        smtb_print(smtb);
    }

    criteria = "jvwqwxvfdq";
    ptrElem = smtb_remove(smtb, criteria);
    if (ptrElem == NULL) {
        printf("Element not found!!!\n");
    } else {
        printf("    Passed key ---> %s\nReturned value ---> %.8f\n\n", criteria, *(double *) ptrElem);
        smtb_print(smtb);
    }

    criteria = "fduqzqecmb";
    ptrElem = smtb_remove(smtb, criteria);
    if (ptrElem == NULL) {
        printf("Element not found!!!\n");
    } else {
        printf("    Passed key ---> %s\nReturned value ---> %.8f\n\n", criteria, *(double *) ptrElem);
        smtb_print(smtb);
    }

    printf("\n\n\n<<<----------------------------------------------->>>\n\n\n");

    criteria = "asjdgas";
    ptrElem = smtb_remove(smtb, criteria);
    if (ptrElem == NULL) {
        printf("Element not found!!!\n");
    } else {
        printf("    Passed key ---> %s\nReturned value ---> %.8f\n\n", criteria, *(double *) ptrElem);
        smtb_print(smtb);
    }

    criteria = "zwvbanrerr";
    ptrElem = smtb_remove(smtb, criteria);
    if (ptrElem == NULL) {
        printf("Element not found!!!\n");
    } else {
        printf("    Passed key ---> %s\nReturned value ---> %.8f\n\n", criteria, *(double *) ptrElem);
        smtb_print(smtb);
    }

    criteria = "hgmtkexgbi";
    ptrElem = smtb_remove(smtb, criteria);
    if (ptrElem == NULL) {
        printf("Element not found!!!\n");
    } else {
        printf("    Passed key ---> %s\nReturned value ---> %.8f\n\n", criteria, *(double *) ptrElem);
        smtb_print(smtb);
    }

    criteria = "nydfhnulfa";
    ptrElem = smtb_remove(smtb, criteria);
    if (ptrElem == NULL) {
        printf("Element not found!!!\n");
    } else {
        printf("    Passed key ---> %s\nReturned value ---> %.8f\n\n", criteria, *(double *) ptrElem);
        smtb_print(smtb);
    }

    criteria = "asrtdu";
    ptrElem = smtb_remove(smtb, criteria);
    if (ptrElem == NULL) {
        printf("Element not found!!!\n");
    } else {
        printf("    Passed key ---> %s\nReturned value ---> %.8f\n\n", criteria, *(double *) ptrElem);
        smtb_print(smtb);
    }

    smtb_free(smtb);

    return(EXIT_SUCCESS);

}
