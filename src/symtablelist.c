
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
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
    symTable->first = NULL;
    symTable->length = 0;
    return symTable;
}

void smtb_free(SymTable symTable) {
    assert(symTable != NULL);
    struct Node *current;
    struct Node *temp;
    for (current = (struct Node *) symTable->first; current != NULL;) {
        temp = current->next;
        free((void *) current->key);
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
    char *ptrKey;    
    struct Node *current = (struct Node *) symTable->first;
    if (current == NULL) {
        current = (struct Node *) malloc(sizeof(struct Node));
        if (current == NULL) {
            return 0;
        }
        ptrKey = (char *) malloc((strlen(key) + 1) * sizeof(char));
        if (ptrKey == NULL) {
            return 0;
        }
        strcpy(ptrKey, key);
        current->key = ptrKey;
        current->value = value;
        current->next = NULL;
        symTable->first = current;
        symTable->length++;
        return 1;
    }
    struct Node *prev = current;
    for (; current != NULL; current = current->next) {
        if (current->key == key) {
            return 0;
        }
        prev = current;
    }
    current = (struct Node *) malloc(sizeof(struct Node));
    if (current == NULL) {
        return 0;
    }
    prev->next = current;
    ptrKey = (char *) malloc((strlen(key) + 1) * sizeof(char));
    strcpy(ptrKey, key);
    current->key = ptrKey;
    current->value = value;
    current->next = NULL;
    symTable->length++;
    return 1;
}

void *smtb_replace(SymTable symTable, const char *key, const void *value) {
    assert(symTable != NULL);
    assert(key != NULL);
    assert(value != NULL);
    if (smtb_getLength(symTable) == 0) {
        return NULL;
    }
    struct Node *current;
    for (current = (struct Node *) symTable->first; current != NULL; current = current->next) {
        if (strcmp(current->key, key) == 0) {
            void *oldValue = (void *) current->value;
            current->value = value;
            return (void *) oldValue;
        }
    }
    return NULL;
}

int smtb_contains(SymTable symTable, const char *key) {
    assert(symTable != NULL);
    assert(key != NULL);
    if (smtb_getLength(symTable) == 0) {
        return 0;
    }
    struct Node *current;
    for (current = (struct Node *) symTable->first; current != NULL; current = current->next) {
        if (strcmp(current->key, key) == 0) {
            return 1;
        }
    }
    return 0;
}

void *smtb_get(SymTable symTable, const char *key) {
    assert(symTable != NULL);
    assert(key != NULL);
    if (smtb_getLength(symTable) == 0) {
        return NULL;
    }
    struct Node *current;
    for (current = (struct Node *) symTable->first; current != NULL; current = current->next) {
        if (strcmp(current->key, key) == 0) {
            return (void *) current->value;
        }
    }
    return NULL;
}

void *smtb_remove(SymTable symTable, const char *key) {
    assert(symTable != NULL);
    assert(key != NULL);
    if (smtb_getLength(symTable) == 0) {
        return NULL;
    }
    void *oldValue;
    struct Node *current = (struct Node *) symTable->first;
    if (strcmp(current->key, key) == 0) {
        oldValue = (void *) current->value;
        symTable->first = current->next;
        symTable->length--;
        free((void *) current->key);
        free(current);
        return oldValue;
    }
    struct Node *prev = current;
    current = current->next;
    for (; current != NULL; current = current->next) {
        if (strcmp(current->key, key) == 0) {
            oldValue = (void *) current->value;
            prev->next = current->next;
            free((void *) current->key);
            free(current);
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
    if (smtb_getLength(symTable) == 0) {
        return;
    }
    struct Node *current;
    for (current = (struct Node *) symTable->first; current != NULL; current = current->next) {
        func(current->key, (void *) current->value, (void *) extra);
    }
}

void smtb_print(SymTable symTable) {
    printf("\n<<<--------- { SYMBOL TABLE --------->>>\n\n");
    printf("No. of items: %i\n\n--- ITEMS ---\n\n", (int) smtb_getLength(symTable));
    struct Node *current;
    int i = 1;
    for (current = (struct Node *) symTable->first; current != NULL; current = current->next) {
        printf("Item #%i --->\t(%s -> %.8f)\n", i, current->key, *(double *) current->value);
        i++;
    }
    printf("\n<<<--------- SYMBOL TABLE } --------->>>\n\n");
}
