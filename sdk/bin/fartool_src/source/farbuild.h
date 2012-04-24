#pragma once
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <vector>
#include <string>
#include <algorithm>
#include "far.h"
#include "FileClass.h"

using std::vector;
using std::string;

struct FAR_File;
struct FAR_Dir;
struct FAR_Entry;
typedef vector<FAR_Entry*> entryvector;
typedef entryvector::iterator entryiter;

struct FAR_Entry
{
	word_t flags;

	word_t nameoff, entryoff;
	string name, fullname;
	entryvector* subentries;
	word_t size;
	int nTotalEntries;

	FAR_Entry() : flags(0), entryoff(~0), subentries(NULL), size(0), nTotalEntries(0) { }
	~FAR_Entry();
};

struct FAR_Builder
{
	FAR_Entry topdir;
	int nameSize, nEntries;

	FAR_Builder() : nameSize(0), nEntries(0) { }

	int ScanDir(const char* path);
	int OutputFile(const char* path);
	int OutputFile(FileClass& f);
};

#define die(msg) do { fputs(msg "\n\n", stderr); return 1; } while(0)
#define safe_call(a) do { int rc = a; if(rc != 0) return rc; } while(0)
