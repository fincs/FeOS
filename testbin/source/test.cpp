#include <feos.h>
#include <stdio.h>
#include <string.h>
#include "testclasses.h"

CTest::CTest()
{
	printf("{CTest::CTest}\n");
}

CTest::~CTest()
{
	printf("{CTest::~CTest}\n");
}

void CTest::SayHello()
{
	printf("Hello from CTest!\n");
}

//--------------------------------------------------

class CCompletelyUseless : public ICompletelyUseless
{
	word_t refcount;
public:
	CCompletelyUseless() : refcount(1)
	{
		printf("{CCompletelyUseless()}\n");
	}

	~CCompletelyUseless()
	{
		printf("{~CCompletelyUseless()}\n");
	}

	virtual word_t AddRef()
	{
		return ++refcount;
	}

	virtual word_t Release()
	{
		word_t newrefcount = --refcount;
		if(!newrefcount)
			delete this;
		return newrefcount;
	}

#define CASE(IFACE) (strcmp(aIface, #IFACE) == 0) do { this->AddRef(); *(IFACE**)ppObj = (IFACE*) this; } while(0)

	virtual bool Coerce(const char* aIface, void** ppObj)
	{
		if CASE(ICompletelyUseless);
		else if CASE(IBase);
		else if CASE(IRefCount);
		else { *ppObj = NULL; return false; }
		return true;
	}

#undef CASE

	virtual void HelloWorld();
};

void CCompletelyUseless::HelloWorld()
{
	printf("Hello World!\n");
}

#define CASE(NAME) (strcmp(aIface, "I" #NAME) == 0) *(I##NAME**)ppObj = new C##NAME()

bool CreateObject(const char* aIface, void** ppObj)
{
	if CASE(CompletelyUseless);
	else { *ppObj = NULL; return false; }
	return true;
}
