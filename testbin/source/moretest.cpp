#include <feos.h>
#include <stdio.h>
#include "testclasses.h"

static void Something(IBase* pObj)
{
	ICompletelyUseless* pObj2;
	if (!pObj->Coerce("ICompletelyUseless", (void**) &pObj2))
		printf("Can't coerce object!\n");
	else
	{
		pObj2->HelloWorld();
		pObj2->Release();
	}
}

extern "C" void TestCpp()
{
	CTest testObj;
	testObj.SayHello();

	ICompletelyUseless* pObj;

	if (!CreateObject("ICompletelyUseless", (void**) &pObj))
		printf("Can't create object!\n");
	else
	{
		pObj->HelloWorld();
		Something(pObj);
		pObj->Release();
	}
}
