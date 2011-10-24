#include <string>

namespace std
{
	string::string() : mSize(16)
	{
		_alloc_mem();
		*mBuf = '\0';
	}

	string::string(size_t size) : mSize(size) // NON-STANDARD/internal use only
	{
		_alloc_mem();
	}

	string::string(const string& str) : mSize(str.mSize)
	{
		_alloc_mem();
		memcpy(mBuf, str.mBuf, mSize);
	}

	string::string(const string& str, size_t pos, size_t n)
	{
		if (n != npos)
			mSize = n + 1;
		else
			mSize = str.mSize - pos;

		_alloc_mem();
		strncpy(mBuf, str.mBuf + pos, mSize);
	}

	string::string(const char* str) : mSize(strlen(str) + 1)
	{
		_alloc_mem();
		memcpy(mBuf, str, mSize);
	}

	string::string(const char* str, size_t n) : mSize(n + 1)
	{
		_alloc_mem();
		strncpy(mBuf, str, mSize);
	}

	string::string(size_t n, char c) : mSize(n + 1)
	{
		_alloc_mem();
		memset(mBuf, c, n);
		mBuf[n] = '\0';
	}

	string::~string()
	{
		_free_mem();
	}

	string& string::operator =(char c)
	{
		reserve(1);
		mBuf[0] = c;
		mBuf[1] = '\0';
		return *this;
	}

	string& string::operator +=(char c)
	{
		size_t curSize = size();
		reserve(curSize + 1);
		mBuf[curSize] = c;
		mBuf[curSize+1] = '\0';
		return *this;
	}

	string& string::append(const char* str)
	{
		size_t strSize = strlen(str);
		size_t curSize = size();
		reserve(curSize + strSize);
		memcpy(mBuf + curSize, str, strSize + 1);
		return *this;
	}

	// TODO: throw out_of_range exception
	string& string::append(const string& str, size_t pos, size_t n)
	{
		size_t curSize = size();
		reserve(curSize + n);
		strncpy(mBuf + curSize, str.mBuf + pos, n + 1);
		return *this;
	}

	string& string::append(const char* s, size_t n)
	{
		size_t curSize = size();
		reserve(curSize + n);
		strncpy(mBuf + curSize, s, n + 1);
		return *this;
	}

	string& string::append(size_t n, char c)
	{
		size_t curSize = size();
		reserve(curSize + n);
		memset(mBuf + curSize, c, n);
		mBuf[curSize + n] = '\0';
		return *this;
	}

	string& string::assign(const string& str)
	{
		reserve(str.mSize - 1);
		memcpy(mBuf, str.mBuf, str.mSize + 1);
		return *this;
	}

	string& string::assign(const char* str)
	{
		size_t strSize = strlen(str);
		reserve(strSize);
		memcpy(mBuf, str, strSize + 1);
		return *this;
	}

	// TODO: throw out_of_range exception
	string& string::assign(const string& str, size_t pos, size_t n)
	{
		reserve(n);
		strncpy(mBuf, str.mBuf + pos, n + 1);
		return *this;
	}

	string& string::assign(const char* s, size_t n)
	{
		reserve(n);
		strncpy(mBuf, s, n + 1);
		return *this;
	}

	string& string::assign(size_t n, char c)
	{
		reserve(n);
		memset(mBuf, c, n);
		mBuf[n] = '\0';
		return *this;
	}

	size_t string::copy(char* buf, size_t n, size_t pos) const
	{
		size_t copySize = size() - pos;
		if (copySize > n) copySize = n;
		memcpy(buf, mBuf + pos, copySize);
		return copySize;
	}

	size_t string::find(const char* str, size_t pos, size_t n) const
	{
		size_t strSize = size();
		for(; pos < strSize; pos ++)
			if (strncmp(mBuf + pos, str, n) == 0) return pos;
		return npos;
	}

	size_t string::find(char c, size_t pos) const
	{
		size_t strSize = size();
		for(; pos < strSize; pos ++)
			if (mBuf[pos] == c) return pos;
		return npos;
	}

	void string::reserve(size_t size)
	{
		size ++;
		if (mSize >= size) return;

		size_t oldSize = mSize;
		char* oldBuf = mBuf;

		_alloc_mem(size);
		memcpy(mBuf, oldBuf, oldSize);
		delete [] oldBuf;
	}

	void string::resize(size_t n, char c)
	{
		size_t strSize = size();
		if (n > strSize)
		{
			reserve(n + 1);
			memset(mBuf + strSize, c, n - strSize);
		}

		mBuf[n] = '\0';
	}

	void string::swap(string& str)
	{
		char* this_buf = mBuf;
		size_t this_size = mSize;

		mBuf = str.mBuf;
		mSize = str.mSize;
		str.mBuf = this_buf;
		str.mSize = this_size;
	}
}
