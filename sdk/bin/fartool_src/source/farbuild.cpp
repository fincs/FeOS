#include "farbuild.h"
#include <dirent.h>
#include <sys/stat.h>
#include <sys/param.h>
#ifdef WIN32
#include <fcntl.h>
#endif

FAR_Entry::~FAR_Entry()
{
	if (subentries)
	{
		foreach (entryiter, ent, *subentries)
			delete (*ent);
		delete subentries;
	}
}

static bool entrySort(FAR_Entry* a, FAR_Entry* b)
{
	// return a < b
	bool bIsDirA = FAR_EFLAG_GETTYPE(a->flags) == FAR_ETYPE_DIR;
	bool bIsDirB = FAR_EFLAG_GETTYPE(b->flags) == FAR_ETYPE_DIR;
	if (bIsDirA && !bIsDirB) return true;
	if (!bIsDirA && bIsDirB) return false;
	return a->name < b->name;
}

static int doDir(FAR_Entry& oDirEntry, int& nameSize, int& nEntries)
{
	entryvector& pVec = *oDirEntry.subentries;
	DIR* dir = opendir(oDirEntry.fullname.c_str());
	if (!dir) die("Can't open dir!");
	struct dirent* de;
	while ((de = readdir(dir)))
	{
		if (de->d_name[0] == '.') continue;

		char strbuf[MAXPATHLEN];
		strcpy(strbuf, oDirEntry.fullname.c_str());
		strcat(strbuf, "/");
		strcat(strbuf, de->d_name);

		struct stat st;
		if (stat(strbuf, &st)) die("Can't get entry props!");

		FAR_Entry* pEntry = new FAR_Entry;
		pEntry->name = de->d_name;
		nameSize += strlen(de->d_name) + 1;
		pEntry->fullname = strbuf;

		if (S_ISDIR(st.st_mode))
		{
			pEntry->flags = FAR_ETYPE_DIR;
			pEntry->subentries = new entryvector;
			safe_call(doDir(*pEntry, nameSize, nEntries/*, strbuf, &pEntry->pDir->size*/));
			oDirEntry.nTotalEntries += pEntry->nTotalEntries;
		} else if (S_ISREG(st.st_mode))
		{
			pEntry->flags = FAR_ETYPE_FILE;
			pEntry->size = st.st_size;
		} else die("Unsupported entry type!");

		pVec.push_back(pEntry);
	}
	closedir(dir);
	std::sort(pVec.begin(), pVec.end(), entrySort);
	nEntries += pVec.size();
	oDirEntry.nTotalEntries += pVec.size();
#if 0
	printf("Contents of %s:\n", oDirEntry.fullname.c_str());
	foreach (entryiter, ent, pVec)
	{
		bool bIsDir = FAR_EFLAG_GETTYPE((*ent)->flags) == FAR_ETYPE_DIR;
		if (bIsDir)
			printf("  [%s]\n", (*ent)->name.c_str());
		else
			printf("  %s\n", (*ent)->name.c_str());
	}
#endif
	return 0;
}

int FAR_Builder::ScanDir(const char* path)
{
	topdir.flags = FAR_ETYPE_DIR;
	topdir.subentries = new entryvector;
	topdir.fullname = path;
	return doDir(topdir, nameSize, nEntries/*, builddata*/);
}

static int writeDir(FileClass& f, FAR_Entry& oDirEntry, int& namePos, int& dataPos)
{
	entryvector& pVec = *oDirEntry.subentries;

	int startPos = f.Tell();
	int endPos = startPos + sizeof(FARentry_t)*pVec.size();
	foreach (entryiter, ent, pVec)
	{
		FAR_Entry* pEnt = (*ent);
		f.WriteWord(pEnt->flags);
		f.WriteWord(namePos); namePos += pEnt->name.length() + 1;
		if (FAR_EFLAG_GETTYPE(pEnt->flags) == FAR_ETYPE_DIR)
		{
			// Child pos + # entries
			f.WriteWord(endPos); endPos += sizeof(FARentry_t)*pEnt->nTotalEntries;
			f.WriteWord(pEnt->subentries->size());
		}else
		{
			// Data pos + size
			f.WriteWord(dataPos); dataPos += pEnt->size;
			f.WriteWord(pEnt->size);
		}
	}

	// Write children
	foreach (entryiter, ent, pVec)
		if (FAR_EFLAG_GETTYPE((*ent)->flags) == FAR_ETYPE_DIR)
			safe_call(writeDir(f, **ent, namePos, dataPos));
	return 0;
}

static int writeNames(FileClass& f, FAR_Entry& oDirEntry)
{
	entryvector& pVec = *oDirEntry.subentries;
	foreach (entryiter, ent, pVec)
	{
		FAR_Entry* pEnt = (*ent);
		const char* str = pEnt->name.c_str();
		int len = pEnt->name.length();
		f.WriteRaw(str, len);
		f.WriteByte(0);
	}
	// Write children
	foreach (entryiter, ent, pVec)
		if (FAR_EFLAG_GETTYPE((*ent)->flags) == FAR_ETYPE_DIR)
			safe_call(writeNames(f, **ent));

	return 0;
}

void fcopy(FileClass& fout, FileClass& fin, word_t size)
{
	byte_t buf[1024]; FILE* rfin = fin.get_ptr();
	int x;
	do
	{
		x = fread(buf, 1, sizeof(buf), rfin);
		fout.WriteRaw(buf, x);
	} while(x);
}

static int writeData(FileClass& f, FAR_Entry& oDirEntry)
{
	entryvector& pVec = *oDirEntry.subentries;
	foreach (entryiter, ent, pVec)
	{
		FAR_Entry* pEnt = (*ent);
		if (FAR_EFLAG_GETTYPE((*ent)->flags) != FAR_ETYPE_FILE)
			continue;
		FileClass f2(pEnt->fullname.c_str(), "rb");
		if (f2.openerror()) die("Can't copy file!");
		fcopy(f, f2, pEnt->size);
	}
	// Write children
	foreach (entryiter, ent, pVec)
		if (FAR_EFLAG_GETTYPE((*ent)->flags) == FAR_ETYPE_DIR)
			safe_call(writeData(f, **ent));

	return 0;
}

int FAR_Builder::OutputFile(const char* path)
{
	if (strcmp(path, "?stdout") != 0)
	{
		FileClass f(path, "wb");
		if (f.openerror()) die("Can't open output file!");
		return OutputFile(f);
	}else
	{
#ifdef WIN32
		setmode(fileno(stdout), O_BINARY);
#endif
		FileClass f(stdout);
		return OutputFile(f);
	}
}

int FAR_Builder::OutputFile(FileClass& f)
{
	f.WriteWord(FAR_MAGIC);
	f.WriteWord(0); // version
	f.WriteWord(nEntries); // nentries
	f.WriteWord(nameSize); // namesize
	f.WriteWord(topdir.subentries->size()); // topentries

	int namePos = f.Tell() + sizeof(FARentry_t)*nEntries, dataPos = namePos + nameSize;
	safe_call(writeDir(f, topdir, namePos, dataPos));
	safe_call(writeNames(f, topdir));
	safe_call(writeData(f, topdir));
	return 0;
}
