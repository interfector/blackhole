#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stdarg.h>
#include <assert.h>
#include <stddef.h>
#include <class.h>

/* Classe #1 */

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
	struct classMethod list[] = { /* Creo la lista dei metodi(che dopo servirà) */
		METHOD( size, struct TestArray ),
	};
	struct TestArray* ta = malloc( sizeof(struct TestArray) );

	*ta = (struct TestArray){ genInfo(ID(TestArray), MLIST(list) ), NULL, test, /* Genero le informazioni e copio i metodi */
		FCOPY( TA_size ),
	};

	superclass( ta, 2, sizeof(struct TestArray), 1 ); /* Inizializzo */

	return ta;
}

/* Classe #2 */

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
	struct classMethod list[] = { /* Creo la lista dei metodi */
		METHOD( size, struct Array ),	
	};
	struct Array* a = malloc( sizeof(struct Array) );

	*a = (struct Array){ genInfo( ID(Array), MLIST(list) ), test, /* Genero le info e copio i metodi */
		FCOPY( A_size ),
	};

	superclass( a, 1, sizeof(struct Array), 1 ); /* Inizializzo */

	return a;
}

/*
 * Questa funzione non fa altro che prendere una classe,
 * cercare nella lista dei metodi il metodo 'size' e poi
 * lo esegue
 * */
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

	printf("returnSize(a): %d\nreturnSize(ta): %d\n", returnSize( a ), returnSize( ta ) ); /* returnSize funziona sia su 'a' che su 'ta' perchè su entrambe è definito il metodo 'size' */

	return 0;
}
