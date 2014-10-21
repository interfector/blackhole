#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stdarg.h>
#include <assert.h>
#include <stddef.h>
#include <class.h>

struct TestArray { /* TestArray */
#define CLASS_TestArray CLASS_HEAD; int* array; int nsize; int (*size)();
	CLASS_TestArray
};

struct TestArray* TestArray(int);

int
TA_size( )
{
	struct TestArray *this = (struct TestArray*)THIS;

	return this->nsize;
}

PROTOTYPE_FUNC( TA_size );

struct TestArray* TestArray( int test )
{
	struct classMethod list[] = { /* Creo la lista */
		METHOD( size, struct TestArray ),	
	};
	struct TestArray* ta = malloc( sizeof(struct TestArray) );

	*ta = (struct TestArray){ genInfo(ID(TestArray), MLIST(list) ), NULL, test, /* Genero e copio */
		FCOPY( TA_size ),
	};

	superclass( ta, 2, sizeof(struct TestArray), 1 ); /* Inizializzo */

	return ta;
}

int
main()
{
	struct TestArray* ta = TestArray( 3 );

	printf("%d\n", ta->size() );

	int (*f)() = (int (*)())getMethod( ta, "size" ); /* Testo il metodo 'size' */

	printf("%d\n", f() );

	printf("csize: %d\n", ta->cinfo->csize );

	return 0;
}
