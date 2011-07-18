#include <feos.h>
#include "testclasses.h"

static void Something(IBase* pObj)
{
	ICompletelyUseless* pObj2;
	if (!pObj->Coerce("ICompletelyUseless", (void**) &pObj2))
		FeOS_DebugPrint("Can't coerce object!\n");
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
		FeOS_DebugPrint("Can't create object!\n");
	else
	{
		pObj->HelloWorld();
		Something(pObj);
		pObj->Release();
	}
}
