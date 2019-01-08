
#include <stdio.h>
#include <stdlib.h>
#include <assert.h>
#include "symtable.h"

struct SymbolTable {
    const void *first;
    size_t length;
};

struct Node {
    const char *key;
    const void *value;
    struct Node *next;
};

SymTable smtb_new(void) {
    SymTable symTable = (struct SymbolTable *) malloc(sizeof(struct SymbolTable));
    if (symTable == NULL) {
        return NULL;
    }
    return symTable;
}

void smtb_free(SymTable symTable) {
    assert(symTable != NULL);
    struct Node *current;
    struct Node *temp;
    for (current = symTable->first; current->next != NULL;) {
        temp = current->next;
        free(current);
        current = temp;
    }
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
    struct Node *current;
    struct Node *prev;
    for (current = symTable->first; current->next != NULL; current = current->next) {
        if (*current->key == *key) {
            return 0;
        }
        prev = current;
    }
    current = (struct Node *) malloc(sizeof(struct Node));
    if (current == NULL) {
        return 0;
    }
    prev->next = current;
    current->key = key;
    current->value = value;
    current->next = NULL;
    return 1;
}

void *smtb_replace(SymTable symTable, const char *key, const void *value) {
    assert(symTable != NULL);
    assert(key != NULL);
    assert(value != NULL);
    struct Node *current;
    for (current = symTable->first; current->next != NULL; current = current->next) {
        if (*current->key == *key) {
            const void *oldValue = current->value;
            current->value = value;
            return oldValue;
        }
    }
    return NULL;
}

int smtb_contains(SymTable symTable, const char *key) {
    assert(symTable != NULL);
    assert(key != NULL);
    struct Node *current;
    for (current = symTable->first; current->next != NULL; current = current->next) {
        if (*current->key == *key) {
            return 1;
        }
    }
    return 0;
}

void *smtb_get(SymTable symTable, const char *key) {}

void *smtb_remove(SymTable symTable, const char *key) {}

void smtb_map(SymTable symTable, void (*func)(const char *key, void *value, void *extra), const void *extra) {}

int main(int argc, char **argv) {

    //

    return(EXIT_SUCCESS);

}
