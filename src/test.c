
#include <stdio.h>
#include <stdlib.h>
#include "symtable.h"

static void smtb_test_replace(SymTable symTable, const char *key, double newValue) {
    double *ptrNewValue = &newValue;
    void *ptrElem = smtb_replace(symTable, key, ptrNewValue);
    printf("\nTESTING smtb_replace():\n");
    if (ptrElem == NULL) {
        printf("\tElement not found!!!\n\n");
    } else {
        printf("\t         Passed key ---> %s\n\tReturned (old)value ---> %.8f\n\n", key, *(double *) ptrElem);
        smtb_print(symTable);
    }
    printf("\n\n\n");
}

static void smtb_test_contains(SymTable symTable, const char *key) {
    int num = smtb_contains(symTable, key);
    printf("\nTESTING smtb_contains():\n");
    if (num == 0) {
        printf("\tElement not found!!!\n\n\n");
    } else {
        printf("\tElement under provided key exists in symbol table.\n\n\n");
    }
}

static void smtb_test_get(SymTable symTable, const char *key) {
    void *ptrElem = smtb_get(symTable, key);
    printf("\nTESTING smtb_get():\n");
    if (ptrElem == NULL) {
        printf("Element not found!!!\n\n\n");
    } else {
        printf("    Passed key ---> %s\n\tReturned value ---> %.8f\n\n\n", key, *(double *) ptrElem);
    }
}

static void smtb_test_remove(SymTable symTable, const char *key) {
    void *ptrElem = smtb_remove(symTable, key);
    printf("\nTESTING smtb_remove():\n");
    if (ptrElem == NULL) {
        printf("\tElement not found!!!\n\n\n");
    } else {
        printf("\t         Passed key ---> %s\n\tReturned (old)value ---> %.8f\n\n", key, *(double *) ptrElem);
        smtb_print(symTable);
    }
    printf("\n\n");
}

int main(int argc, char **argv) {

    int errorCheck, num;
    void *ptrElem;
    const char *criteria;
    FILE *file = fopen("../_data.txt", "r");
    size_t arrayLength, maxWordLength;
    fscanf(file, " %i, %i\n", &arrayLength, &maxWordLength);
    SymTable smtb = smtb_new();
    if (smtb == NULL) {
        fprintf(stderr, "Memory issues [array]...\n");
        exit(EXIT_FAILURE);
    }
    char *ptrKey;
    while (1) {
        ptrKey = (char *) malloc((maxWordLength + 1) * sizeof (char));
        if (ptrKey == NULL) {
            fprintf(stderr, "Memory issues [ptrKey]...\n");
            exit(EXIT_FAILURE);
        }
        double *ptrValue = (double *) malloc(sizeof (double));
        if (ptrValue == NULL) {
            fprintf(stderr, "Memory issues [ptrValue]...\n");
            exit(EXIT_FAILURE);
        }
        errorCheck = fscanf(file, " %s\t%lf\n", ptrKey, ptrValue);
        if (errorCheck == 0) {
            fprintf(stderr, "Wrong input!...\n");
            exit(EXIT_FAILURE);
        }
        if (errorCheck == EOF) {
            break;
        }
        errorCheck = smtb_put(smtb, ptrKey, ptrValue);
        if (errorCheck == 0) {
            printf("\n\n<--------- smtb_put() returned 0! --------->\n\n");
            break;
        }
        free(ptrKey);
    }
    fclose(file);

    smtb_print_detail(smtb);

    smtb_free(smtb);

    return (EXIT_SUCCESS);

}
