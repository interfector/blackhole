#ifndef _CLASS_
#define _CLASS_

#include "argmacro.h"

/* HINT: Per qualche motivo (che uno può immaginare) ogni tanto i programmi vanno in segfault */

#define CLASS_SHARED ( PROT_READ | PROT_WRITE | PROT_EXEC )
#define CLASS_PRIVATE PROT_NONE

#define set_access( addr, mode ) mprotect( (char*)((unsigned)addr & ~(getpagesize() - 1)), 4, mode ); /* Non funziona */

/* Serve per nascondere lo schifo, e avere la 'lunghezza' in byte di una funzione */
#define PROTOTYPE_FUNC(x) asm("_" # x ": .int .- " # x); \
					 extern int _##x

#define THIS 0x00000001

#define new(class, ... ) class( __VA_ARGS__ )
#define $(class, ... ) class( __VA_ARGS__ );

/* Suggerisco l'uso estensivo/unico uso di puntatori in modo da avere che 
 * la grandezza della classe sarà un multiplo di sizeof(int) */

/* Fa il dump della elementi della classe */
#define DUMPCLASS(class) { int index_dump = 0; for(; index_dump < sizeof( *class )/sizeof(int);index_dump++) { printf("[%d]: 0x%x\n", index_dump, *((int*)class + index_dump)); } }

/* Agevola l'uso delle classe nelle estensioni */
#define CLASS_ELEMENT(el, body) (nclass->el = function_copy( (fClass)body, _ ## body ))

/* Copia gli elementi di una classe (usato nelle estensioni) */
#define COPYCLASS(class1, class2, gap) { int index_class = 0; for(;index_class < sizeof( *class2 )/4;index_class++) *((int*)class1 + index_class + gap) = *((int*)class2 + index_class); }

/* Estensioni delle classi */
#define EXTENDS(class, var, ...) { int nvar = var; struct class* mcl = class(__VA_ARGS__);
#define ENDEX }

/* Informazioni della classe */
#define CLASS_HEAD struct classInfo* cinfo

/* Define per rendere il tutto più carino */
#define FCOPY(f) function_copy( (fClass)f, _##f )
#define METHOD(x, class) { # x, offsetof( class, x) } 
#define ID(x) # x, (fClass)x
#define MLIST(l) l, sizeof(l)/sizeof(l[0])

typedef void (*fClass)();

struct classMethod {
	char* methodname;
	unsigned int offset;
};

/* Informazioni della classe
 *
 * @ classname: Il nome della classe
 * @ new: Il puntatore al metodo 'new'
 * @ csize: La grandezza della classe in byte (può sempre servire)
 *
 * @ methodList: Lista dei metodi della classe
 * @ methodCount: Lunghezza della lista
 */
struct classInfo {
	char* classname;
	fClass new;
	int csize;

	struct classMethod* methodList;
	int methodCount;
};

void  superclass( void*, int, int, int );
void* function_copy( fClass, int );

struct classInfo* genInfo(char*, fClass, struct classMethod[], int);
fClass getMethod( void*, char* );


/* Scheletro della classe */
struct Skeleton {
	CLASS_HEAD;
};

#endif 
