
#include <stdlib.h>
#include "symtable.h"

struct SymbolTable {
    const void **firstElement;
};

SymTable smtb_new(void) {}
void smtb_free(SymTable symTable) {}
size_t smtb_getLength(SymTable symTable) {}
int smtb_put(SymTable symTable, const char *key, const char *value) {}
void *smtb_replace(SymTable symTable, const char *key, const char *value) {}
int smtb_contains(SymTable symTable, const char *key) {}
void *smtb_get(SymTable symTable, const char *key) {}
void *smtb_remove(SymTable symTable, const char *key) {}
void smtb_map(SymTable symTable, void (*func)(const char *key, void *value, void *extra), const void *extra) {}

int main(int argc, char **argv) {

    //

    return(EXIT_SUCCESS);

}