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

struct Array { /* TestArray */
#define CLASS_Array CLASS_HEAD; int nsize; int (*size)();
	CLASS_Array
};

struct Array* Array(int);

int
A_size( )
{
	struct Array *this = (struct Array*)THIS;

	return this->nsize;
}

PROTOTYPE_FUNC( A_size );

struct Array* Array( int test )
{
	struct classMethod list[] = {
		METHOD( size, struct Array ),	
	};
	struct Array* a = malloc( sizeof(struct Array) );

	*a = (struct Array){ genInfo( ID(Array), MLIST(list) ), test,
		FCOPY( A_size ),
	};

	superclass( a, 1, sizeof(struct Array), 1 );

	return a;
}


int
returnSize( void* class )
{
	int (*f)() = (int (*)())getMethod( class, "size" );

	return f ? f() : -1;
}

int
main()
{
	struct TestArray* ta = TestArray( 3 );
	struct Array* a = Array( 4 );

	printf("%d : %d\n", ta->size(), a->size() );

	printf("returnSize: %d\n", returnSize( a ) );

	return 0;
}
