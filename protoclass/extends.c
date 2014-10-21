#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stdarg.h>
#include <assert.h>
#include <stddef.h>
#include <class.h>


struct TestArray { /* TestArray */
#define CLASS_TestArray	CLASS_HEAD; int* array; int nsize; int (*size)();
	CLASS_TestArray
};

struct TestArray* TestArray(int);


struct NTest { /* extends TestArray */
	CLASS_TestArray

#define CLASS_NTest	int dummy; void (*push)(int);
	CLASS_NTest
};

struct NTest* NTest(int);

int
TA_size( )
{
	struct TestArray *this = (struct TestArray*)THIS;

	return this->nsize;
}

PROTOTYPE_FUNC( TA_size );

struct TestArray* TestArray( int test )
{
	struct classMethod list[] = {
		METHOD( size, struct TestArray ),
	};
	struct TestArray* ta = malloc( sizeof(struct TestArray) );

	*ta = (struct TestArray){ genInfo(ID(TestArray), MLIST(list) ), NULL, test,
		FCOPY( TA_size ),
	};

	superclass( ta, 2, sizeof(struct TestArray), 1 );

	return ta;
}

void
NT_push( int i )
{
	struct NTest *this = (struct NTest*)THIS;

	printf("Class this: %p\n", this );
	this->nsize = i;

	this->dummy = i * 2;
}

PROTOTYPE_FUNC( NT_push );

struct NTest* NTest( int arg ) EXTENDS(TestArray, 2, arg) {
	struct NTest* nclass = malloc(sizeof(struct NTest));

	COPYCLASS(nclass, mcl, 0);

	struct classMethod list[] = {
		METHOD( size, struct NTest ),
		METHOD( push, struct NTest ),
	};
	nclass->cinfo = genInfo( ID(NTest), MLIST(list) );

	CLASS_ELEMENT( push, NT_push );
	
	superclass( nclass, nvar, sizeof(struct NTest), 1 );
	return nclass;
} ENDEX

int
main()
{
	struct NTest* nt = NTest( 4 );

	struct NTest* b = ((struct NTest* (*)())nt->cinfo->new)( 3 );

	printf("b size: %d\n", b->size() );

	printf("nt: %d\n", nt->size() );

	nt->push( 6 );

	printf("nt: %d : %d\n", nt->size(), nt->dummy );

	return 0;
}
