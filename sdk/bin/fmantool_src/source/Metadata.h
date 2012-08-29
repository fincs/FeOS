#pragma once
#include "common.h"

class MetadataWriter
{
	struct Entry
	{
		std::string key, value;
		int keyPos, valuePos;

		Entry() : keyPos(0), valuePos(0) { }

		bool operator < (const Entry& rhs) const
		{
			return strcmp(key.c_str(), rhs.key.c_str()) < 0;
		}

		void Fixup(int& off)
		{
			keyPos = off;
			off += key.length() + 1;
			valuePos = off;
			off += value.length() + 1;
		}
	};

	int bufSize;
	std::vector<Entry> v;

public:
	MetadataWriter() : bufSize(0) { }

	void AddEntry(const char* key, const char* value)
	{
		Entry e;
		e.key = key;
		std::transform(e.key.begin(), e.key.end(), e.key.begin(), ::tolower);
		e.value = value;
		v.push_back(e);
	}

	void Preprocess()
	{
		std::sort(v.begin(), v.end());

		for (auto e = v.begin(); e != v.end(); e ++)
			(*e).Fixup(bufSize);
	}

	int Size()
	{
		return 4 + 8*v.size() + bufSize;
	}

	void WriteToFile(FileClass& f)
	{
		f.WriteWord(v.size());
		int q = 8*v.size();
		for (auto _e = v.begin(); _e != v.end(); _e ++)
		{
			auto e = *_e;
			f.WriteWord(q + e.keyPos);
			f.WriteWord(q + e.valuePos);
		}
		for (auto _e = v.begin(); _e != v.end(); _e ++)
		{
			auto e = *_e;
			f.WriteRaw(e.key.c_str(), e.key.length() + 1);
			f.WriteRaw(e.value.c_str(), e.value.length() + 1);
		}
	}
};

class MetadataParser
{
	MetadataWriter mw;

public:
	void ParseBlob(char* editableBlob);
	void Add(const char* key, const char* value) { mw.AddEntry(key, value); }
	int Size() { return mw.Size(); }
	void Write(FileClass& f) { mw.WriteToFile(f); }
};
