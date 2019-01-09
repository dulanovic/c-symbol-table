
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
        free((void *) current->value);
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
    struct Node *current = (struct Node *) symTable->first;
    if (current == NULL) {
        current = (struct Node *) malloc(sizeof(struct Node));
        if (current == NULL) {
            return 0;
        }
        current->key = key;
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
    current->key = key;
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
            const void *oldValue = current->value;
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
    current = current->next;
    struct Node *prev = current;
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
        printf("Item #%i ---> (%s -> %.8f)\n", i, current->key, *(double *) current->value);
        i++;
    }
    printf("\n<<<--------- SYMBOL TABLE } --------->>>\n\n");
}

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
