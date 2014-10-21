#include <stdlib.h>
#include <memory.h>
#include <unistd.h>
#include <string.h>
#include <errno.h>
#include <sys/mman.h>
#include <class.h>

unsigned char* memmem( unsigned char*, unsigned char*, int, int);

void*
function_copy( fClass addr, int size )
{
	int i, j;
	unsigned char* func = (unsigned char*)malloc( size );
	unsigned char* rfunc, *faddr;
	char data[3] = { 0 };

	memcpy( (unsigned char*)func, (unsigned char*)addr, size );

	for(i = 0;i < size;i++) {
		if(  func[i] == 0xe8 &&
			(i > 1 && 
			 func[i - 2] != 0x8b && 
			 func[i - 2] != 0xc7 && 
			 func[i - 2] != 0x89 && 
			 func[i - 2] != 0x3b && 
			 func[i - 2] != 0x03 && 
			 func[i - 2] != 0x01 && 
			 func[i - 2] != 0xc7 && 
			 func[i - 2] != 0x83) &&
			(i > 0 && func[i - 1] != 0x83)) {

			memcpy( &rfunc, &func[i+1], sizeof(int));
			rfunc += (int)&((char*)addr)[i + 5] - (int)&func[i + 5];
			memcpy( &func[i + 1], &rfunc, sizeof(int));
		}
	}

	return func;
}

void
superclass( void* cl, int var, int size, int extends )
{
	int i;
	int len = (size / 4) - var - extends;

	unsigned int method[256] = { 0 };
	unsigned page = 0;

	struct Skeleton* tmp = (struct Skeleton*)cl;

	tmp->cinfo->csize = size;

	for(i = 0;i < len; i++)
		memcpy( &method[i], ((char*)cl) + ((i + var + extends)  * sizeof(int)), sizeof(int));
		
	for(i = 0;i < len;i++)
	{
		if(method[i])
		{
			page = ((unsigned int)method[i]) & ~(getpagesize() - 1);

			mprotect((char*)page, sizeof(int) * 4, PROT_READ | PROT_WRITE | PROT_EXEC );
		}
	}

	for(i = 0;i < len;i++)
	{
		if( method[i] )
		{
			unsigned char* addr = memmem(((unsigned char*)method[i]), "\xc7\x45", 2, 20 );

			if( addr )
				memcpy( addr , &cl, sizeof(int) );
		}
	}
}

unsigned char*
memmem( unsigned char* addr, unsigned char* find, int size, int len )
{
	int i;

	for(i = 0;i < len;i++)
		if(!memcmp(addr+i, find, size))
			return addr + i + 3;

	return NULL;
}

struct classInfo*
genInfo( char* name, fClass f, struct classMethod list[], int size)
{
	struct classInfo* c = malloc( sizeof(struct classInfo) );

	c->classname = strdup(name);
	c->new = f;
	c->methodList = malloc( sizeof(struct classMethod) * size );
	c->methodCount = size;

	memcpy( c->methodList, list, sizeof(struct classMethod) * size );

	return c;
}

fClass
getMethod( void* class, char* name )
{
	int i;
	struct Skeleton* tmp = (struct Skeleton*)class;

	for(i = 0;i < tmp->cinfo->methodCount;i++)
		if(!strcmp(tmp->cinfo->methodList[i].methodname, name))
			return (fClass)(*(int*)((char*)class + (int)tmp->cinfo->methodList[i].offset));

	return NULL;
}
