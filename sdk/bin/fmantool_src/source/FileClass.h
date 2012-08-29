#pragma once
#include "common.h"

class FileClass
{
	FILE* f;
	bool LittleEndian, own, canSeek;
	int filePos;

public:

	size_t _RawRead(void* buffer, size_t size)
	{
		size_t x = fread(buffer, 1, size, f);
		if (!canSeek) filePos += x;
		return x;
	}

	size_t _RawWrite(const void* buffer, size_t size)
	{
		size_t x = fwrite(buffer, 1, size, f);
		if (!canSeek) filePos += x;
		return x;
	}

	FileClass(const char* file, const char* mode) : LittleEndian(true), own(true), canSeek(true), filePos(0)
	{
		f = fopen(file, mode);
	}
	FileClass(FILE* inf) : f(inf), LittleEndian(true), own(false), canSeek(false), filePos(0) { }
	~FileClass()
	{
		if (f && own) fclose(f);
	}

	void SetLittleEndian() { LittleEndian = true; }
	void SetBigEndian() { LittleEndian = false; }
	void SetCanSeek(bool bNewVal) { canSeek = bNewVal; }
	bool CanSeek() { return canSeek; }

	FILE* get_ptr() { return f; }
	bool openerror() { return f == NULL; }

	word_t ReadWord()
	{
		word_t value;
		_RawRead(&value, sizeof(word_t));
		return LittleEndian ? le_word(value) : be_word(value);
	}

	void WriteWord(word_t value)
	{
		value = LittleEndian ? le_word(value) : be_word(value);
		_RawWrite(&value, sizeof(word_t));
	}

	hword_t ReadHword()
	{
		hword_t value;
		_RawRead(&value, sizeof(hword_t));
		return LittleEndian ? le_hword(value) : be_hword(value);
	}

	void WriteHword(hword_t value)
	{
		value = LittleEndian ? le_hword(value) : be_hword(value);
		_RawWrite(&value, sizeof(hword_t));
	}

	byte_t ReadByte()
	{
		byte_t value;
		_RawRead(&value, sizeof(byte_t));
		return value;
	}

	void WriteByte(byte_t value)
	{
		_RawWrite(&value, sizeof(byte_t));
	}
	
	bool ReadRaw(void* buffer, size_t size) { return _RawRead(buffer, size) == size; }
	bool WriteRaw(const void* buffer, size_t size) { return _RawWrite(buffer, size) == size; }

	void Seek(int pos, int mode = SEEK_SET) { if (canSeek) fseek(f, pos, mode); }
	void Skip(int bytes) { if (canSeek) Seek(bytes, SEEK_CUR); else while(bytes--) ReadByte(); }
	int Tell() { return canSeek ? ftell(f) : filePos; }
	void Flush() { fflush(f); }
};

static inline char* StringFromFile(const char* filename)
{
	FILE* f = fopen(filename, "rb");
	if (!f) return NULL;
	fseek(f, 0, SEEK_END);
	int size = ftell(f);
	rewind(f);
	char* buf = (char*)malloc(size+1);
	if (!buf)
	{
		fclose(f);
		return NULL;
	}
	fread(buf, 1, size, f);
	buf[size] = 0;
	fclose(f);
	return buf;
}
