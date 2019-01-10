
#include <stdio.h>
#include <stdlib.h>
#include "symtable.h"

static void smtb_test_replace(SymTable symTable, const char *key, double newValue) {
    double *ptrNewValue = &newValue;
    void *ptrElem = smtb_replace(symTable, key, ptrNewValue);
    if (ptrElem == NULL) {
        printf("Element not found!!!\n\n\n");
    } else {
        printf("         Passed key ---> %s\nReturned (old)value ---> %.8f\n\n", key, *(double *) ptrElem);
        smtb_print(symTable);
    }
}

static void smtb_test_contains(SymTable symTable, const char *key) {
    int num = smtb_contains(symTable, key);
    if (num == 0) {
        printf("Element not found!!!\n\n\n");
    } else {
        printf("Element under provided key exists in symbol table.\n\n\n");
    }
}

static void smtb_test_get(SymTable symTable, const char *key) {
    void *ptrElem = smtb_get(symTable, key);
    if (ptrElem == NULL) {
        printf("Element not found!!!\n\n\n");
    } else {
        printf("    Passed key ---> %s\nReturned value ---> %.8f\n\n", key, *(double *) ptrElem);
    }
}

static void smtb_test_remove(SymTable symTable, const char *key) {
    void *ptrElem = smtb_remove(symTable, key);
    if (ptrElem == NULL) {
        printf("Element not found!!!\n\n\n");
    } else {
        printf("         Passed key ---> %s\nReturned (old)value ---> %.8f\n\n", key, *(double *) ptrElem);
        smtb_print(symTable);
    }
}

int main(int argc, char **argv) {

    int num;
    void *ptrElem;
    const char *criteria;
    FILE *file = fopen("./_data.txt", "r");
    size_t arrayLength, maxWordLength;
    fscanf(file, "%i, %i\n", &arrayLength, &maxWordLength);
    SymTable smtb = smtb_new();
    if (smtb == NULL) {
        fprintf(stderr, "Memory issues [array]...\n");
        exit(EXIT_FAILURE);
    }
    char *ptrKey;
    while (1) {
        ptrKey = (char *) malloc((maxWordLength + 1) * sizeof (char));
        double *ptrValue = (double *) malloc(sizeof (double));
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
    free(ptrKey);

    smtb_print(smtb);

    smtb_test_remove(smtb, "aiojhyuemo");

    smtb_free(smtb);

    return (EXIT_SUCCESS);

}
