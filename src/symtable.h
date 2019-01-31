
#ifndef SYMBOL_TABLE_SYMTABLE_H
#define SYMBOL_TABLE_SYMTABLE_H

typedef struct SymbolTable *SymTable;

SymTable smtb_new(void);
void smtb_free(SymTable symTable);
size_t smtb_getLength(SymTable symTable);
int smtb_put(SymTable symTable, const char *key, const void *value);
void *smtb_replace(SymTable symTable, const char *key, const void *value);
int smtb_contains(SymTable symTable, const char *key);
void *smtb_get(SymTable symTable, const char *key);
void *smtb_remove(SymTable symTable, const char *key);
void smtb_map(SymTable symTable, void (*func)(const char *key, void *value, void *extra), const void *extra);
void smtb_print(SymTable symTable);
void smtb_print_detail(SymTable symTable);
int smtb_compare(SymTable symTable1, SymTable symTable2);

#endif
