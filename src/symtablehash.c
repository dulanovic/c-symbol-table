
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <assert.h>
#include "symtable.h"

static size_t BUCKETCOUNT_EXPANSION[] = {59, 1021, 2039, 4093, 8191, 16381, 32749, 65521};

struct SymbolTable {
    const void **bindings;
    size_t length;
    size_t bucketCount;
};

struct Binding {
    const char *key;
    const void *value;
    struct Binding *next;
};

static size_t smtb_hash(const char *key, size_t bucketCount) {
    const size_t h = 65599;
    size_t hash = 0;
    for (size_t i = 0; key[i] != '\0'; i++) {
        hash = hash * h + (size_t) key[i];
    }
    return (hash % bucketCount) - 1;
}

void smtb_print_addresses(SymTable symTable) {
    printf("<<<--------- { SYMBOL TABLE ADDRESSES --------->>>\n\n");
    for (size_t i = 0; i < symTable->bucketCount; i++) {
        printf("BUCKET_ADDRESS[%i] ---> %u (== NULL ---> %s)\n", (int) i, (unsigned int) (symTable->bindings + i), (((struct Binding *) symTable->bindings[i]) == NULL) ? "NULL" : "NOT NULL");
    }
    printf("\n\n<<<--------- SYMBOL TABLE ADDRESSES } --------->>>\n\n");
}

SymTable smtb_new(void) {
    SymTable symTable = (struct SymbolTable *) malloc(sizeof(struct SymbolTable));
    if (symTable == NULL) {
        return NULL;
    }
    symTable->bindings = (const void **) calloc(BUCKETCOUNT_EXPANSION[0], sizeof(void *));
    if (symTable->bindings == NULL) {
        return NULL;
    }
    symTable->length = 0;
    symTable->bucketCount = BUCKETCOUNT_EXPANSION[0];
    return symTable;
}

void smtb_free(SymTable symTable) {
    assert(symTable != NULL);
    struct Binding *current, *prev;
    for (int i = 0; i < symTable->bucketCount; i++) {
        current = (struct Binding *) symTable->bindings[i];
        prev = current;
        for (; current != NULL; current = prev) {
            prev = current->next;
            free((void *) current->key);
            free((void *) current);
        }
        free((void *) symTable->bindings[i]);
    }
    free((void *) symTable);
}

size_t smtb_getLength(SymTable symTable) {
    assert(symTable != NULL);
    return symTable->length;
}

int smtb_put(SymTable symTable, const char *key, const void *value) {
    assert(symTable != NULL);
    assert(key != NULL);
    assert(value != NULL);
    char *ptrKey;
    printf("1\n");
    size_t hash = smtb_hash(key, symTable->bucketCount);
    printf("2 ---> %i\n", hash);
    printf("2.5 ---> %u\n", (unsigned int) (symTable->bindings[hash]));
    struct Binding *current = (struct Binding *) symTable->bindings[hash];
    printf("CURRENT ---> %s\n", (current == NULL) ? "NULL" : "NOT NULL");
    printf("3\n");
    if (current == NULL) {
        printf("4\n");
        current = (struct Binding *) malloc(sizeof(struct Binding));
        printf("5\n");
        if (current == NULL) {
            return 0;
        }
        printf("6\n");
        ptrKey = (char *) malloc(sizeof(strlen(key) + 1));
        printf("7\n");
        if (ptrKey == NULL) {
            return 0;
        }
        printf("8\n");
        symTable->bindings[hash] = current;
        printf("9\n");
        strcpy(ptrKey, key);
        printf("10\n");
        current->key = ptrKey;
        printf("11\n");
        current->value = value;
        printf("12\n");
        current->next = NULL;
        printf("13\n");
        symTable->length++;
        printf("14\n");
        return 1;
    }
    struct Binding *prev = current;
    for (; current != NULL; current = current->next) {
        if (strcmp(current->key, key) == 0) {
            return 0;
        }
        prev = current;
    }
    ptrKey = (char *) malloc(sizeof(strlen(key) + 1));
    if (ptrKey == NULL) {
        return 0;
    }
    strcpy(ptrKey, key);
    current = (struct Binding *) malloc(sizeof(struct Binding));
    if (current == NULL) {
        return 0;
    }
    current->key = ptrKey;
    current->value = value;
    current->next = NULL;
    prev->next = current;
    symTable->length++;
    return 1;
}

void *smtb_replace(SymTable symTable, const char *key, const void *value) {
    assert(symTable != NULL);
    assert(key != NULL);
    assert(value != NULL);
    size_t hash = smtb_hash(key, symTable->bucketCount);
    struct Binding *current = (struct Binding *) symTable->bindings[hash];
    /* if (current == NULL) {
        return NULL;
    } */
    for (; current != NULL; current = current->next) {
        if (strcmp(current->key, key) == 0) {
            void *oldValue = (void *) current->value;
            current->value = value;
            return oldValue;
        }
    }
    return NULL;
}

int smtb_contains(SymTable symTable, const char *key) {
    assert(symTable != NULL);
    assert(key != NULL);
    size_t hash = smtb_hash(key, symTable->bucketCount);
    struct Binding *current = (struct Binding *) symTable->bindings[hash];
    for (; current != NULL; current = current->next) {
        if (strcmp(current->key, key) == 0) {
            return 1;
        }
    }
    return 0;
}

void *smtb_get(SymTable symTable, const char *key) {
    assert(symTable != NULL);
    assert(key != NULL);
    size_t hash = smtb_hash(key, symTable->bucketCount);
    struct Binding *current = (struct Binding *) symTable->bindings[hash];
    for (; current != NULL; current = current->next) {
        if (strcmp(current->key, key) == 0) {
            return (void *) current->value;
        }
    }
    return NULL;
}

void *smtb_remove(SymTable symTable, const char *key) {
    assert(symTable != NULL);
    assert(key != NULL);
    size_t hash = smtb_hash(key, symTable->bucketCount);
    struct Binding *current = (struct Binding *) symTable->bindings[hash];
    struct Binding *prev = current;
    for (; current != NULL; current = current->next) {
        if (strcmp(current->key, key) == 0) {
            void *oldValue = (void *) current->value;
            prev->next = current->next;
            free((void *) current->key);
            free((void *) current);
            symTable->length--;
            return oldValue;
        }
        prev = current;
    }
    return NULL;
}

void smtb_map(SymTable symTable, void (*func)(const char *key, void *value, void *extra), const void *extra) {
    assert(symTable != NULL);
    assert(func != NULL);
    assert(extra != NULL);
    for (size_t i = 0; i < symTable->bucketCount; i++) {
        struct Binding *current = (struct Binding *) symTable->bindings[i];
        for (; current != NULL; current = current->next) {
            func(current->key, (void *) current->value, (void *) extra);
        }
    }
}

void smtb_print(SymTable symTable) {
    assert(symTable != NULL);
    printf("\n<<<--------- { SYMBOL TABLE --------->>>\n\n");
    for (size_t i = 0; i < symTable->bucketCount; i++) {
        printf("<- BUCKET[%i] ->\n", (int) i);
        int j = 1;
        struct Binding *current = (struct Binding *) symTable->bindings[i];
        for (; current != NULL; current = current->next, j++) {
            printf("\t Item #%i ---> (%s -> %.8f)\n", j, current->key, *(double *) current->value);
        }
        printf("\n\n");
    }
    printf("\n<<<--------- SYMBOL TABLE } --------->>>\n\n\n");
}

void smtb_print_detail(SymTable symTable) {
    assert(symTable != NULL);
    printf("\n<<<--------- { SYMBOL TABLE(detail) --------->>>\n\n");
    for (size_t i = 0; i < symTable->bucketCount; i++) {
        printf("<- BUCKET[%i] ->\n\tAddress ---> %u (%u)\n", (int) i, (unsigned int) (symTable->bindings + i), (unsigned int) symTable->bindings[i]);
        int j = 1;
        struct Binding *current = (struct Binding *) symTable->bindings[i];
        for (; current != NULL; current = current->next) {
            printf("\tItem #%i (%u) ---> (%s -> %.8f) (%u -> %u) ---> %u\n", j, (unsigned int) current, current->key, *(double *) current->value, (unsigned int) current->key, (unsigned int) current->value, (unsigned int) current->next);
        }
        printf("\n");
    }
    printf("\n<<<--------- SYMBOL TABLE(detail) } --------->>>\n\n\n");
}

int main(int argc, char **argv) {

    printf("HASH ---> %i\n", (int) smtb_hash("tgmhopsexx", 59));

    int errorCheck;
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
    /* printf("BUCKETCOUNT ---> %i\n", (int) smtb->bucketCount);
    printf("     LENGTH ---> %i\n", (int) smtb->length);
    for (size_t i = 0; i < smtb->bucketCount; i++) {
        printf("\tBINDINGS[%i] ---> %u (%u)\n", i, (unsigned int) (smtb->bindings + i), (unsigned int) smtb->bindings[i]);
    } */
    // smtb_print(smtb);
    char *ptrKey;
    while (1) {
        printf("main - 1\n");
        ptrKey = (char *) malloc((maxWordLength + 1) * sizeof (char));
        printf("main - 2\n");
        if (ptrKey == NULL) {
            fprintf(stderr, "Memory issues [ptrKey]...\n");
            exit(EXIT_FAILURE);
        }
        printf("main - 3\n");
        // double *ptrValue = (double *) malloc(sizeof (double));
        double doubleVal;
        printf("main - 4\n");
        /* if (ptrValue == NULL) {
            fprintf(stderr, "Memory issues [ptrValue]...\n");
            exit(EXIT_FAILURE);
        } */
        printf("main - 5\n");
        errorCheck = fscanf(file, " %s\t%lf\n", ptrKey, &doubleVal);
        printf("main - 6\n");
        if (errorCheck == 0) {
            fprintf(stderr, "Wrong ipnut!...\n");
            exit(EXIT_FAILURE);
        }
        printf("main - 7\n");
        if (errorCheck == EOF) {
            break;
        }
        printf("main - 8\n");
        errorCheck = smtb_put(smtb, ptrKey, &doubleVal);
        printf("main - 9\n");
        if (errorCheck == 0) {
            printf("\n\n<--------- smtb_put() returned 0! --------->\n\n");
            break;
        }
        printf("main - 10\n");
        smtb_print_detail(smtb);
        free(ptrKey);
    }
    fclose(file);
    smtb_print(smtb);

    smtb_free(smtb);

    return(EXIT_SUCCESS);

}