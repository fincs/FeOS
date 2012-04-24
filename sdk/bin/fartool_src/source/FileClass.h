#pragma once
#include <stdio.h>
#include "types.h"

class FileClass
{
	FILE* f;
	bool LittleEndian, own;

public:
	FileClass(const char* file, const char* mode) : LittleEndian(true), own(true)
	{
		f = fopen(file, mode);
	}
	FileClass(FILE* inf) : f(inf), LittleEndian(true), own(false) { }
	~FileClass()
	{
		if (f && own) fclose(f);
	}

	void SetLittleEndian() { LittleEndian = true; }
	void SetBigEndian() { LittleEndian = false; }

	FILE* get_ptr() { return f; }
	bool openerror() { return f == NULL; }

	word_t ReadWord()
	{
		word_t value;
		fread(&value, sizeof(word_t), 1, f);
		return LittleEndian ? le_word(value) : be_word(value);
	}

	void WriteWord(word_t value)
	{
		value = LittleEndian ? le_word(value) : be_word(value);
		fwrite(&value, sizeof(word_t), 1, f);
	}

	hword_t ReadHword()
	{
		hword_t value;
		fread(&value, sizeof(hword_t), 1, f);
		return LittleEndian ? le_hword(value) : be_hword(value);
	}

	void WriteHword(hword_t value)
	{
		value = LittleEndian ? le_hword(value) : be_hword(value);
		fwrite(&value, sizeof(hword_t), 1, f);
	}

	byte_t ReadByte()
	{
		byte_t value;
		fread(&value, sizeof(byte_t), 1, f);
		return value;
	}

	void WriteByte(byte_t value)
	{
		fwrite(&value, sizeof(byte_t), 1, f);
	}
	
	bool ReadRaw(void* buffer, size_t size) { return fread(buffer, 1, size, f) == size; }
	bool WriteRaw(const void* buffer, size_t size) { return fwrite(buffer, 1, size, f) == size; }

	void Seek(int pos, int mode) { fseek(f, pos, mode); }
	int Tell() { return ftell(f); }
	void Flush() { fflush(f); }
};
