
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <assert.h>
#include "symtable.h"

static size_t BUCKETCOUNT_EXPANSION[] = {509, 1021, 2039, 4093, 8191, 16381, 32749, 65521};

struct SymbolTable {
    const void **buckets;
    size_t length;
    size_t expansionState;
};

struct Binding {
    const char *key;
    const void *value;
    struct Binding *next;
};

static size_t smtb_hash(const char *key, size_t bucketCount) {
    const size_t h = 65599;
    size_t hash = 0;
    for (size_t i = 0; key[i] != 0; i++) {
        hash = hash * h + (size_t) key[i];
    }
    return (hash % bucketCount);
}

static size_t smtb_bucket_count(SymTable symTable) {
    assert(symTable != NULL);
    return BUCKETCOUNT_EXPANSION[symTable->expansionState];
}

static size_t smtb_count_empty_buckets(SymTable symTable) {
    assert(symTable != NULL);
    size_t empty = 0;
    size_t nonEmpty = 0;
    struct Binding *current;
    for (size_t i = 0; i < smtb_bucket_count(symTable); i++) {
        if (symTable->buckets[i] != NULL) {
            nonEmpty++;
        } else {
            empty++;
        }
    }
    printf("    EMPTY ---> %i\nNON-EMPTY ---> %i\n", (int) empty, (int) nonEmpty);
    return nonEmpty;
}

static void smtb_expand(SymTable symTable) {
    assert(symTable != NULL);
    if (symTable->expansionState == (sizeof(BUCKETCOUNT_EXPANSION) / sizeof(BUCKETCOUNT_EXPANSION[0]) - 1)) {
        return;
    }
    const void **tempBuckets = symTable->buckets;
    const void **temp = (const void **) calloc(BUCKETCOUNT_EXPANSION[symTable->expansionState + 1], sizeof(void *));
    if (temp == NULL) {
        return;
    }
    symTable->buckets = temp;
    symTable->expansionState++;
    struct Binding *current;
    for (size_t i = 0; i < BUCKETCOUNT_EXPANSION[symTable->expansionState - 1]; i++) {
        current = (struct Binding *) tempBuckets[i];
        for (; current != NULL; current = current->next) {
            smtb_put(symTable, current->key, current->value);
            symTable->length--;
            free((void *) current->key);
        }
    }
    free(tempBuckets);
}

SymTable smtb_new(void) {
    SymTable symTable = (struct SymbolTable *) malloc(sizeof(struct SymbolTable));
    if (symTable == NULL) {
        return NULL;
    }
    symTable->buckets = (const void **) calloc(BUCKETCOUNT_EXPANSION[0], sizeof(void *));
    if (symTable->buckets == NULL) {
        return NULL;
    }
    symTable->length = 0;
    symTable->expansionState = 0;
    return symTable;
}

void smtb_free(SymTable symTable) {
    assert(symTable != NULL);
    struct Binding *current, *prev;
    for (int i = 0; i < smtb_bucket_count(symTable); i++) {
        current = (struct Binding *) symTable->buckets[i];
        prev = current;
        for (; current != NULL; current = prev) {
            prev = current->next;
            free((void *) current->key);
            free((void *) current);
        }
    }
    free(symTable->buckets);
    free(symTable);
}

size_t smtb_getLength(SymTable symTable) {
    assert(symTable != NULL);
    return symTable->length;
}

int smtb_put(SymTable symTable, const char *key, const void *value) {
    assert(symTable != NULL);
    assert(key != NULL);
    assert(value != NULL);
    if (symTable->length == BUCKETCOUNT_EXPANSION[symTable->expansionState]) {
        smtb_expand(symTable);
    }
    char *ptrKey;
    size_t hash = smtb_hash(key, smtb_bucket_count(symTable));
    struct Binding *current = (struct Binding *) symTable->buckets[hash];
    if (current == NULL) {
        current = (struct Binding *) malloc(sizeof(struct Binding));
        if (current == NULL) {
            return 0;
        }
        ptrKey = (char *) malloc((strlen(key) + 1) * sizeof(char));
        if (ptrKey == NULL) {
            return 0;
        }
        symTable->buckets[hash] = current;
        strcpy(ptrKey, key);
        current->key = ptrKey;
        current->value = value;
        current->next = NULL;
        symTable->length++;
        return 1;
    }
    struct Binding *prev = current;
    for (; current != NULL; current = current->next) {
        if (strcmp(current->key, key) == 0) {
            return 0;
        }
        prev = current;
    }
    ptrKey = (char *) malloc((strlen(key) + 1) * sizeof(char));
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
    size_t hash = smtb_hash(key, smtb_bucket_count(symTable));
    struct Binding *current = (struct Binding *) symTable->buckets[hash];
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
    size_t hash = smtb_hash(key, smtb_bucket_count(symTable));
    struct Binding *current = (struct Binding *) symTable->buckets[hash];
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
    size_t hash = smtb_hash(key, smtb_bucket_count(symTable));
    struct Binding *current = (struct Binding *) symTable->buckets[hash];
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
    size_t hash = smtb_hash(key, smtb_bucket_count(symTable));
    void *oldValue;
    struct Binding *current = (struct Binding *) symTable->buckets[hash];
    if ((strcmp(current->key, key) == 0)) {
        oldValue = (void *) current->value;
        symTable->buckets[hash] = current->next;
        symTable->length--;
        free((void *) current->key);
        free((void *) current);
        return oldValue;
    }
    struct Binding *prev = current;
    current = current->next;
    for (; current != NULL; prev = current, current = current->next) {
        if (strcmp(current->key, key) == 0) {
            oldValue = (void *) current->value;
            prev->next = current->next;
            free((void *) current->key);
            free((void *) current);
            symTable->length--;
            return oldValue;
        }
    }
    return NULL;
}

void smtb_map(SymTable symTable, void (*func)(const char *key, void *value, void *extra), const void *extra) {
    assert(symTable != NULL);
    assert(func != NULL);
    assert(extra != NULL);
    for (size_t i = 0; i < smtb_bucket_count(symTable); i++) {
        struct Binding *current = (struct Binding *) symTable->buckets[i];
        for (; current != NULL; current = current->next) {
            func(current->key, (void *) current->value, (void *) extra);
        }
    }
}

void smtb_print(SymTable symTable) {
    assert(symTable != NULL);
    /* FILE *output = fopen("../output.txt", "w");
    if (output == NULL) {
        output = stdout;
    } */
    if (smtb_getLength(symTable) == 0) {
        printf("!!! Symbol table currently has no elements !!!\n");
        return;
    }
    FILE *output = stdout;
    // printf("\n<<<--------- { SYMBOL TABLE --------->>>\n\n");
    fprintf(output, "\n<<<--------- { SYMBOL TABLE --------->>>\n\n");
    for (size_t i = 0; i < smtb_bucket_count(symTable); i++) {
        // printf("<- BUCKET[%i] ->\n", (int) (i + 1));
        fprintf(output, "<- BUCKET[%i] ->\n", (int) (i + 1));
        int j = 1;
        struct Binding *current = (struct Binding *) symTable->buckets[i];
        for (; current != NULL; current = current->next, j++) {
            // printf("\t Item #%i ---> (%s -> %i)\n", j, current->key, *(int *) current->value);
            fprintf(output, "\t Item #%i ---> (%s -> %i)\n", j, current->key, *(int *) current->value);
        }
        // printf("\n\n");
        fprintf(output, "\n");
    }
    // printf("\n<<<--------- SYMBOL TABLE } --------->>>\n\n\n");
    fprintf(output, "Total number of buckets: %i\n", (int) smtb_bucket_count(symTable));
    fprintf(output, "Total number of items: %i\n\n", smtb_getLength(symTable));
    fprintf(output, "\n<<<--------- SYMBOL TABLE } --------->>>\n\n\n");
}

void smtb_print_detail(SymTable symTable) {
    assert(symTable != NULL);
    if (smtb_getLength(symTable) == 0) {
        printf("!!! Symbol table currently has no elements !!!\n");
        return;
    }
    printf("\n<<<--------- { SYMBOL TABLE(detail) --------->>>\n\n");
    for (size_t i = 0; i < smtb_bucket_count(symTable); i++) {
        printf("<- BUCKET[%i] ->\n\tAddress ---> %u (%u)\n", (int) (i + 1), (unsigned int) (symTable->buckets + i),
               (unsigned int) symTable->buckets[i]);
        int j = 1;
        struct Binding *current = (struct Binding *) symTable->buckets[i];
        for (; current != NULL; current = current->next, j++) {
            printf("\tItem #%i (%u) ---> (%s -> %.8f) (%u -> %u, HASH ---> %i) ---> %u\n", j,
                   (unsigned int) current,
                   current->key,
                   *(double *) current->value, (unsigned int) current->key, (unsigned int) current->value,
                   (int) smtb_hash(current->key, smtb_bucket_count(symTable)),
                   (unsigned int) current->next);
        }
        printf("\n");
    }
    printf("Total number of buckets: %i\n", (int) smtb_bucket_count(symTable));
    printf("Total number of items: %i\n\n", smtb_getLength(symTable));
    printf("\n<<<--------- SYMBOL TABLE(detail) } --------->>>\n\n\n");
}

int smtb_compare(SymTable symTable1, SymTable symTable2) {
    assert(symTable1 != NULL);
    assert(symTable2 != NULL);
    FILE *report = fopen("../cmpreport.txt", "w");
    if (report == NULL) {
        perror("Report file error\n");
        return -1;
    }
    // printf("<<<--- COMPARING SYMBOL TABLES --->>>\n\n\n");
    fprintf(report, "<<<--- COMPARING SYMBOL TABLES --->>>\n\n\n");
    if (smtb_bucket_count(symTable1) != smtb_bucket_count(symTable2)) {
        // printf("BUCKET_COUNT(smtb_1) ---> %i\nBUCKET_COUNT(smtb_2) ---> %i\n\n", smtb_bucket_count(symTable1), smtb_bucket_count(symTable2));
        fprintf(report, "BUCKET_COUNT(smtb_1) ---> %i\nBUCKET_COUNT(smtb_2) ---> %i\n\n", smtb_bucket_count(symTable1), smtb_bucket_count(symTable2));
        // return -1;
    }
    if (smtb_getLength(symTable1) != smtb_getLength(symTable2)) {
        // printf("SMTB_LENGTH(smtb_1) --->%i\nSMTB_LENGTH(smtb_2) ---> %i\n\n", smtb_getLength(symTable1), smtb_getLength(symTable2));
        fprintf(report, "SMTB_LENGTH(smtb_1) --->%i\nSMTB_LENGTH(smtb_2) ---> %i\n\n", smtb_getLength(symTable1), smtb_getLength(symTable2));
        // return -1;
    }
    struct Binding *iterator1, *iterator2;
    size_t i = 0;
    for (; i < smtb_bucket_count(symTable1); i++) {
        iterator1 = (struct Binding *) symTable1->buckets[i];
        iterator2 = (struct Binding *) symTable2->buckets[i];
        for (; (iterator1 != NULL && iterator2 != NULL); iterator1 = iterator1->next, iterator2 = iterator2->next) {
            // printf("%s ?? %s\n", iterator1->key, iterator2->key);
            // printf("%i ?? %i\n", *(int *) iterator1->value, *(int *) iterator2->value);
            fprintf(report, "%s ?? %s\n", iterator1->key, iterator2->key);
            fprintf(report, "%i ?? %i\n", *(int *) iterator1->value, *(int *) iterator2->value);
            if (strcmp(iterator1->key, iterator2->key) != 0) {
                // printf("KEYS ARE DIFFERENT!!! [ABORT]\n");
                fprintf(report, "KEYS ARE DIFFERENT!!! [ABORT]\n");
                // return -1;
            }
            if ((*(int *) iterator1->value) != (*(int *) iterator2->value)) {
                // printf("VALUES ARE DIFFERENT!!! [ABORT]\n");
                fprintf(report, "VALUES ARE DIFFERENT!!! [ABORT]\n");
                // return -1;
            }
        }
    }
    fclose(report);
    // printf("SYMBOL TABLES ARE IDENTICAL!!!\n");
    return 0;
}
