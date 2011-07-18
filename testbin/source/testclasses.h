
#pragma once

class FEOS_EXPORT CTest
{
public:
	CTest();
	~CTest();

	void SayHello();
};

class IRefCount
{
public:
	virtual word_t AddRef();
	virtual word_t Release();
};

class IBase : public IRefCount
{
public:
	virtual bool Coerce(const char* aIface, void** ppObj);
};

class ICompletelyUseless : public IBase
{
public:
	virtual void HelloWorld();
};

extern "C" FEOS_EXPORT bool CreateObject(const char* aIface, void** ppObj);
