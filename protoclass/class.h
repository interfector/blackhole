#ifndef _CLASS_
#define _CLASS_

#include "argmacro.h"

#define SECURE_CALL( func, ... ) if( func ) func( __VA_ARGS__ ); 

#define CLASS_SHARED ( PROT_READ | PROT_WRITE | PROT_EXEC )
#define CLASS_PRIVATE PROT_NONE

#define set_access( addr, mode ) mprotect( (char*)((unsigned)addr & ~(getpagesize() - 1)), 4, mode );

#define PROTOTYPE_FUNC(x) asm("_" # x ": .int .- " # x); \
					 extern int _##x

#define THIS 0x00000001

#define new(class, ... ) class( __VA_ARGS__ )

#define $(class, ... ) class( __VA_ARGS__ );

#define DUMPCLASS(class) { int index_dump = 0; for(; index_dump < sizeof( *class )/4;index_dump++) { printf("[%d]: 0x%x\n", index_dump, *((int*)class + index_dump)); } }
#define CLASS_ELEMENT(el, body) (nclass->el = function_copy( (fClass)body, _ ## body ))
#define COPYCLASS(class1, class2, gap) { int index_class = 0; for(;index_class < sizeof( *class2 )/4;index_class++) *((int*)class1 + index_class + gap) = *((int*)class2 + index_class); }
#define EXTENDS(class, var, ...) { int nvar = var; struct class* mcl = class(__VA_ARGS__);
#define ENDEX }

#define CLASS_HEAD struct classInfo* cinfo

#define FCOPY(f) function_copy( (fClass)f, _##f )
#define METHOD(x, class) { # x, offsetof( class, x) } 
#define ID(x) # x, (fClass)x
#define MLIST(l) l, sizeof(l)/sizeof(l[0])

typedef void (*fClass)();

struct classMethod {
	char* methodname;
	unsigned int offset;
};

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

struct Skeleton {
	CLASS_HEAD;
};

#endif 
