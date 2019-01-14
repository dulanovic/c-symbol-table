
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
    for (size_t i = 0; key[i] != '\0'; i++) {
        hash = hash * h + (size_t) key[i];
    }
    return (hash % bucketCount);
}

static size_t smtb_buckets_count(SymTable symTable) {
    assert(symTable != NULL);
    return BUCKETCOUNT_EXPANSION[symTable->expansionState];
}

static void smtb_print_addresses(SymTable symTable) {
    printf("<<<--------- { SYMBOL TABLE ADDRESSES --------->>>\n\n");
    for (size_t i = 0; i < smtb_buckets_count(symTable); i++) {
        printf("BUCKET_ADDRESS[%i] ---> %u (== NULL ---> %s)\n", (int) i, (unsigned int) (symTable->buckets + i), (((struct Binding *) symTable->buckets[i]) == NULL) ? "NULL" : "NOT NULL");
    }
    printf("\n\n<<<--------- SYMBOL TABLE ADDRESSES } --------->>>\n\n");
}

static size_t smtb_count_empty_buckets(SymTable symTable) {
    assert(symTable != NULL);
    size_t empty = 0;
    size_t nonEmpty = 0;
    struct Binding *current;
    for (size_t i = 0; i < smtb_buckets_count(symTable); i++) {
        if (symTable->buckets[i] != NULL) {
            nonEmpty++;
        } else {
            empty++;
        }
    }
    printf("    EMPTY ---> %i\nNON-EMPTY ---> %i\n", empty, nonEmpty);
    return nonEmpty;
}

void smtb_expand(SymTable symTable) {
    assert(symTable != NULL);
    if (symTable->expansionState == (sizeof(BUCKETCOUNT_EXPANSION) / sizeof(BUCKETCOUNT_EXPANSION[0]) - 1)) {
        return;
    }
    const void **temp = (const void **) realloc(symTable->buckets, BUCKETCOUNT_EXPANSION[++symTable->expansionState] * sizeof(void *));
    if (temp == NULL) {
        return;
    }
    symTable->buckets = temp;
}

SymTable smtb_new(void) {
    SymTable symTable = (struct SymbolTable *) malloc(sizeof (struct SymbolTable));
    if (symTable == NULL) {
        return NULL;
    }
    symTable->buckets = (const void **) calloc(BUCKETCOUNT_EXPANSION[0], sizeof (void *));
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
    for (int i = 0; i < smtb_buckets_count(symTable); i++) {
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
    size_t hash = smtb_hash(key, smtb_buckets_count(symTable));
    struct Binding *current = (struct Binding *) symTable->buckets[hash];
    if (current == NULL) {
        current = (struct Binding *) malloc(sizeof (struct Binding));
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
    current = (struct Binding *) malloc(sizeof (struct Binding));
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
    size_t hash = smtb_hash(key, smtb_buckets_count(symTable));
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
    size_t hash = smtb_hash(key, smtb_buckets_count(symTable));
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
    size_t hash = smtb_hash(key, smtb_buckets_count(symTable));
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
    size_t hash = smtb_hash(key, smtb_buckets_count(symTable));
    void *oldValue;
    struct Binding *current = (struct Binding *) symTable->buckets[hash];
    if ((strcmp(current->key, key) == 0) && (current->next == NULL)) {
        oldValue = (void *) current->value;
        free((void *) current->key);
        free((void *) current);
        symTable->buckets[hash] = NULL;
        symTable->length--;
        return oldValue;
    }
    struct Binding *prev = current;
    for (; current != NULL; current = current->next) {
        if (strcmp(current->key, key) == 0) {
            oldValue = (void *) current->value;
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
    for (size_t i = 0; i < smtb_buckets_count(symTable); i++) {
        struct Binding *current = (struct Binding *) symTable->buckets[i];
        for (; current != NULL; current = current->next) {
            func(current->key, (void *) current->value, (void *) extra);
        }
    }
}

void smtb_print(SymTable symTable) {
    assert(symTable != NULL);
    printf("\n<<<--------- { SYMBOL TABLE --------->>>\n\n");
    for (size_t i = 0; i < smtb_buckets_count(symTable); i++) {
        printf("<- BUCKET[%i] ->\n", (int) i);
        int j = 1;
        struct Binding *current = (struct Binding *) symTable->buckets[i];
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
    for (size_t i = 0; i < smtb_buckets_count(symTable); i++) {
        printf("<- BUCKET[%i] ->\n\tAddress ---> %u (%u)\n", (int) i, (unsigned int) (symTable->buckets + i), (unsigned int) symTable->buckets[i]);
        int j = 1;
        struct Binding *current = (struct Binding *) symTable->buckets[i];
        for (; current != NULL; current = current->next, j++) {
            printf("\tItem #%i (%u) ---> (%s -> %.8f) (%u -> %u) ---> %u\n", j, (unsigned int) current, current->key, *(double *) current->value, (unsigned int) current->key, (unsigned int) current->value, (unsigned int) current->next);
        }
        printf("\n");
    }
    printf("\n<<<--------- SYMBOL TABLE(detail) } --------->>>\n\n\n");
}
