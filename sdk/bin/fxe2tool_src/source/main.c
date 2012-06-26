#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#ifdef WIN32
#include <fcntl.h>
#endif

#include "types.h"
#include "elf.h"
#include "fxe2.h"
#include "impcopy.h"

#define die(msg) do { fputs(msg "\n\n", stderr); return 1; } while(0)
#define safe_call(a) do { int rc = a; if(rc != 0) return rc; } while(0)

char* argv_1 = NULL;
char* argv_name = NULL;
char elf_name[FILENAME_MAX+1];
char fx2_name[FILENAME_MAX+1];
char imp_name[FILENAME_MAX+1];

void buildname(char* buffer, const char* prefix, const char* suffix)
{
	strncpy(buffer, prefix, FILENAME_MAX);
	strncat(buffer, suffix, FILENAME_MAX);
	buffer[FILENAME_MAX] = 0;
}

typedef struct tag_rlentry_t
{
	word_t addr;
	struct tag_rlentry_t* next;
} rlentry_t;

typedef struct
{
	FILE* outf;
	byte_t* img;
	Elf32_Sym* syms;
	Elf32_Shdr* sects;
	const char* sectnames;
	const char* symnames;
	int nsects;
	int nsyms;

	fxe2_header_t fxe2hdr;
	byte_t* loaddata;
	word_t vmetaaddr;
	byte_t* metadata;
	word_t metadatasize;
	rlentry_t* relocs;
} elf2fx2_cnvstruct_t;

int GetElfInfo(elf2fx2_cnvstruct_t* cs)
{
	int i;
	int status = 0;
	word_t lastOff = 0;
	for(i = 0; i < cs->nsects; i ++)
	{
		Elf32_Shdr* shdr = cs->sects + i;
		const char* sname = cs->sectnames + eswap_word(shdr->sh_name);
		switch(eswap_word(shdr->sh_type))
		{
			case SHT_PROGBITS:
			case SHT_ARM_EXIDX:
				if(strcmp(sname, ".meta") != 0)
				{
					if(status > 0) die("Unexpected loadable section!");

					word_t ssize = eswap_word(shdr->sh_size);
					byte_t* sdata = cs->img + eswap_word(shdr->sh_offset);

					if(lastOff == 0)
						cs->loaddata = sdata;
					else if(lastOff != eswap_word(shdr->sh_addr) || (cs->loaddata + lastOff) != sdata)
					{
						printf("{DEBUG0} %s\n", sname);
						printf("{DEBUG1} %08X -- %08X\n", lastOff, eswap_word(shdr->sh_addr));
						printf("{DEBUG2} %p -- %p\n", cs->loaddata + lastOff, sdata);
						die("Loadable sections are not contiguous!");
					}

					cs->fxe2hdr.loadsize += ssize;
					lastOff += ssize;
				}else
				{
					if(status != 1) die("Unexpected .meta section!");
					// This is the last section we want to process
					cs->vmetaaddr = eswap_word(shdr->sh_addr);
					cs->metadata = cs->img + eswap_word(shdr->sh_offset);
					cs->metadatasize = (int)eswap_word(shdr->sh_size);
					status ++;
				}
				break;
			case SHT_NOBITS:
				if(status != 0) die("Unexpected BSS section!");
				cs->fxe2hdr.bsssize += eswap_word(shdr->sh_size);
				status ++;
				break;
			case SHT_SYMTAB:
				cs->syms = (Elf32_Sym*) (cs->img + eswap_word(shdr->sh_offset));
				cs->symnames = (const char*)(cs->img + eswap_word(cs->sects[eswap_word(shdr->sh_link)].sh_offset));
				cs->nsyms = eswap_word(shdr->sh_size) / sizeof(Elf32_Sym);
				break;
			default:
				break;
		}
	}
	return 0;
}

int AddRelocation(elf2fx2_cnvstruct_t* cs, word_t addr)
{
	rlentry_t* ent = (rlentry_t*) malloc(sizeof(rlentry_t));
	if(!ent)
		die("Out of memory!");

	ent->addr = addr;
	ent->next = cs->relocs;
	cs->relocs = ent;
	return 0;
}

int PrerelocateSection(elf2fx2_cnvstruct_t* cs, word_t vsect, byte_t* sect, Elf32_Sym* symtab, Elf32_Rel* rels, int nrels)
{
	int i;

	for(i = 0; i < nrels; i ++)
	{
		Elf32_Rel* rel = rels + i;
		word_t rinfo = eswap_word(rel->r_info);
		int rtype = ELF32_R_TYPE(rinfo);
		Elf32_Sym* rsym = symtab + ELF32_R_SYM(rinfo);
		hword_t rsymsect = eswap_hword(rsym->st_shndx);
		Elf32_Shdr* symsect = NULL;
		if ( !(rsymsect >= SHN_LORESERVE && rsymsect <= SHN_HIRESERVE) )
			symsect = cs->sects + rsymsect;
		word_t rsymv = eswap_word(rsym->st_value);
		// The following is only valid for ET_REL ELF files
		//if(rsymsect < SHN_LORESERVE)
		//	rsymv += eswap_word(symsect->sh_addr);
		
		word_t vtarget = /*vsect +*/ eswap_word(rel->r_offset);
		word_t* rtarget = (word_t*)(cs->loaddata + vtarget);
		word_t* rsymp = (word_t*)(cs->loaddata + rsymv);

		const char* symname = cs->symnames + rsym->st_name;

		if (symsect && strncmp(cs->sectnames + symsect->sh_name, ".imp.", 5) == 0 // It's in an import section
		  && *symname && strncmp(symname, "__imp_", 6) != 0 // ...and it's *not* a direct import
		  && *rsymp == 0) // ...and it's a dummy import pointer
		{
			// Add an import copy entry
			word_t v_rtarget = *rtarget;
			word_t offFromTrg = 0;

			if (rtype == R_ARM_ABS32 || rtype == R_ARM_TARGET1)
			{
				*rtarget = 0;
				offFromTrg = (word_t)((int)v_rtarget - (int)rsymv);
			}else if (rtype == R_ARM_TARGET2 || rtype == R_ARM_REL32)
			{
				*rtarget = 1;
				offFromTrg = (word_t)((int)v_rtarget - ((int)rsymv - (int)vtarget));
			}else
			{
				printf("%d %s\n", rtype, symname);
				die("Non-supported type non-function imports are not supported!");
			}

			word_t offTopBits = offFromTrg >> 28;
			if (offTopBits != 0x0 && offTopBits != 0xF)
				die("Offset from non-function import target too big!");

			*rtarget |= offFromTrg << 4;

			AddImpCopy(rsymv, vtarget);
			continue;
		}

		/*
		printf("....REL type=%d, target=%08X, syma=%08X [%d]", rtype, vtarget, rsymv, rsym->st_other);
		if(*symname)
			printf(", symn=%s\n", symname);
		else
			printf("\n");
		*/

		switch(rtype)
		{
			// Notes:
			// R_ARM_TARGET2 is equivalent to R_ARM_REL32
			// R_ARM_PREL32 is an address-relative signed 31-bit offset

			case R_ARM_ABS32:
			case R_ARM_TARGET1:
			{
				// This relocation is the only one FXE2 supports
				if(vtarget & 3)
					die("Unaligned relocation!");

				// Ignore undefined weak target symbols (keep them 0)
				if (ELF32_ST_BIND(rsym->st_info) == STB_WEAK && rsymv == 0) break;

				if(sect != cs->metadata)
					safe_call(AddRelocation(cs, vtarget));
				break;
			}
		}
	}

	return 0;
}

int PrerelocateBinary(elf2fx2_cnvstruct_t* cs)
{
	int i;

	for(i = 0; i < cs->nsects; i ++)
	{
		Elf32_Shdr* shdr = cs->sects + i;
		word_t stype = eswap_word(shdr->sh_type);
		if(stype == SHT_REL)
		{
			Elf32_Shdr* targetsect = cs->sects + eswap_word(shdr->sh_info);
			word_t vsect = eswap_word(targetsect->sh_addr);
			byte_t* sect = cs->img + eswap_word(targetsect->sh_offset);

			Elf32_Sym* symtab = (Elf32_Sym*)(cs->img + eswap_word(cs->sects[eswap_word(shdr->sh_link)].sh_offset));
			Elf32_Rel* rels = (Elf32_Rel*)(cs->img + eswap_word(shdr->sh_offset));
			int nrels = (int)(eswap_word(shdr->sh_size) / eswap_word(shdr->sh_entsize));

			safe_call(PrerelocateSection(cs, vsect, sect, symtab, rels, nrels));
		}else if(stype == SHT_RELA)
			die("RELA relocations are not supported");
	}
	return 0;
}

int WriteData(FILE* f, void* p, size_t s)
{
	if(fwrite(p, 1, s, f) != s)
		die("File write error!");
	return 0;
}

int WriteBinary(elf2fx2_cnvstruct_t* cs)
{
	safe_call(WriteData(cs->outf, &cs->fxe2hdr, sizeof(fxe2_header_t))); // used mainly as a filler
	safe_call(WriteData(cs->outf, cs->loaddata, cs->fxe2hdr.loadsize));
	return 0;
}

int WriteRelocations(elf2fx2_cnvstruct_t* cs)
{
	// Clear out the data
	memset(cs->loaddata, 0, cs->fxe2hdr.loadsize);

	// Walk the relocation info linked list
	word_t* loaddata = (word_t*) cs->loaddata;
	rlentry_t* reloc = cs->relocs;
	while(reloc != NULL)
	{
		rlentry_t* next = reloc->next;
		loaddata[reloc->addr >> 2] = 1;
		free(reloc);
		reloc = next;
	}

	// Write the relocation bitmap
	word_t nwords = cs->fxe2hdr.loadsize >> 2;
	word_t i, rs, rp;
	for(i = 0; i < nwords; cs->fxe2hdr.nrelocs ++)
	{
		fxe2_reloc_t reloc;
		rs = 0;
		rp = 0;
		while((i < nwords) && !loaddata[i]) i ++, rs ++;
		while((i < nwords) && loaddata[i]) i ++, rp ++;

		// Remove empty trailing relocations
		if (i == nwords && rs && !rp)
			break;

		// Write excess skip relocations
		for(reloc.skip = 0xFFFF, reloc.patch = 0; rs > 0xFFFF; rs -= 0xFFFF)
		{
			safe_call(WriteData(cs->outf, &reloc, sizeof(fxe2_reloc_t)));
			cs->fxe2hdr.nrelocs ++;
		}

		// Write excess patch relocations
		for(reloc.skip = eswap_hword(rs), reloc.patch = 0xFFFF; rp > 0xFFFF; rp -= 0xFFFF)
		{
			safe_call(WriteData(cs->outf, &reloc, sizeof(fxe2_reloc_t)));
			cs->fxe2hdr.nrelocs ++;
			rs = reloc.skip = 0;
		}

		// Write remaining relocations
		if (rs || rp)
		{
			reloc.skip = eswap_hword(rs);
			reloc.patch = eswap_hword(rp);
			safe_call(WriteData(cs->outf, &reloc, sizeof(fxe2_reloc_t)));
		}
	}

	return 0;
}

int WriteHeader(elf2fx2_cnvstruct_t* cs)
{
	if(FX2_SUBSYSTEM(cs->fxe2hdr.flags) == 0)
		die("Unspecified subsystem!");

	// Endian swap
#define E_SWAP(t,d) cs->fxe2hdr.d = eswap_##t(cs->fxe2hdr.d)
	E_SWAP(word, loadsize);
	E_SWAP(word, bsssize);
	E_SWAP(word, nrelocs);
	E_SWAP(hword, flags);
	// Entrypoint already handled by ReadAndConvertElf()
	E_SWAP(hword, nexports);
	E_SWAP(hword, sexports);
	E_SWAP(hword, nimports);
	E_SWAP(hword, simports);
#undef E_SWAP

	fseek(cs->outf, 0, SEEK_SET);
	safe_call(WriteData(cs->outf, &cs->fxe2hdr, sizeof(fxe2_header_t)));

	return 0;
}

typedef struct tag_elf2fx2_expimp_t
{
	const char* name;
	word_t addr;
	struct tag_elf2fx2_expimp_t* next;
	struct tag_elf2fx2_expimp_t* prev;
} elf2fx2_expimp_t;

enum { AEI_EXPORT, AEI_IMPORT };

int AddExpImp(elf2fx2_cnvstruct_t* cs, elf2fx2_expimp_t** pexp, const char* name, word_t addr, int which)
{
	elf2fx2_expimp_t* exp = (elf2fx2_expimp_t*) malloc(sizeof(elf2fx2_expimp_t));
	if(!exp)
		die("Out of memory!");

	exp->name = name;
	exp->addr = addr;
	exp->next = NULL;
	if (!*pexp)
	{
		*pexp = exp;
		exp->prev = exp;
	}else
	{
		elf2fx2_expimp_t* lexp = (*pexp)->prev;
		lexp->next = exp;
		(*pexp)->prev = exp;
	}

	if (which == AEI_EXPORT)
	{
		cs->fxe2hdr.nexports ++;
		cs->fxe2hdr.sexports += sizeof(fxe2_export_t) + strlen(name) + 1;
	}else
	{
		cs->fxe2hdr.nimports ++;
		cs->fxe2hdr.simports += sizeof(fxe2_import_t) + strlen(name) + 1;
	}

	return 0;
}

int CheckForImport(elf2fx2_cnvstruct_t* cs, Elf32_Sym* sym, elf2fx2_expimp_t** pimp /* pun not intended */, const char** lastmodule)
{
	const char* symname = (const char*)(cs->symnames + eswap_word(sym->st_name));
	if(!*symname) return 0;
	if (strncmp(symname, "__imp_", 6) != 0) return 0;

	const char* impsymname = symname + 6;
	if(!*impsymname) return 0;

	const char* sectname = cs->sectnames + eswap_word(cs->sects[eswap_hword(sym->st_shndx)].sh_name);
	if(!*sectname) return 0;
	if (strncmp(sectname, ".imp.", 5) != 0) return 0;

	word_t symaddr = eswap_word(sym->st_value);

	const char* modulename = sectname + 5;
	if(!*modulename) return 0;

	//printf("{IMPORT %s FROM %s} %08X\n", impsymname, modulename, symaddr);
	if (strcmp(*lastmodule, modulename) != 0)
	{
		*lastmodule = modulename;
		safe_call(AddExpImp(cs, pimp, modulename, FX2_IMP_SELECT_MODULE, AEI_IMPORT));
	}

	safe_call(AddExpImp(cs, pimp, impsymname, symaddr, AEI_IMPORT));

	return 0;
}

int cmp_sym(const void* _a, const void* _b)
{
	const Elf32_Sym* a = (const Elf32_Sym*) _a;
	const Elf32_Sym* b = (const Elf32_Sym*) _b;
	return eswap_word(a->st_value) - eswap_word(b->st_value);
}

int SortSymbols(elf2fx2_cnvstruct_t* cs)
{
	qsort(cs->syms, cs->nsyms, sizeof(Elf32_Sym), cmp_sym);
	return 0;
}

int ProcessSymbols(elf2fx2_cnvstruct_t* cs)
{
	char cmd[1024];
	elf2fx2_expimp_t* exp_list = NULL;
	elf2fx2_expimp_t* imp_list = NULL;
	const char* imp_lastmodule = "";

	// symbol listing
	int i;
	for(i = 0; i < cs->nsyms; i ++)
	{
		Elf32_Sym* sym = cs->syms + i;
		if (ELF32_ST_BIND(sym->st_info) == STB_LOCAL) continue; // Ignore local symbols
		if (ELF32_ST_BIND(sym->st_info) == STB_WEAK && !sym->st_shndx) continue; // Ignore undefined weak symbols
		if (sym->st_other != STV_DEFAULT) { safe_call(CheckForImport(cs, sym, &imp_list, &imp_lastmodule)); continue; }
		const char* symname = (const char*)(cs->symnames + eswap_word(sym->st_name));
		if(!*symname) continue;
		if(
			(strncmp(symname, "__feos_", 7) == 0) ||
			(strncmp(symname, "__text_", 7) == 0) ||
			(strncmp(symname, "__load_", 7) == 0) ||
			(strncmp(symname, "__exidx_", 8) == 0) ||
			(strncmp(symname, "__bss_", 6) == 0)  ||
			(strcmp(symname, "__hinstance") == 0) ||
			(strcmp(symname, "__start__") == 0)   ||
			(strcmp(symname, "__end__") == 0)
		) continue;
		word_t symaddr = eswap_word(sym->st_value);
		//printf("{EXPORT} %s: %08X\n", symname, symaddr);

		char buf[FILENAME_MAX+1];
		sprintf(buf, "%s.imp.s", symname);
		FILE* tempfile = fopen(buf, "w");
		if (!tempfile)
			die("Can't create export file!");

		fprintf(tempfile, ".section .imp.%s, \"ax\", %%progbits\n.global __imp_%s\n.hidden __imp_%s\n", argv_name, symname, symname);
		if (ELF32_ST_TYPE(sym->st_info) == STT_FUNC)
			fprintf(tempfile, ".global %s\n.hidden %s\n.type %s STT_FUNC\n", symname, symname, symname),
			fprintf(tempfile, "%s:\n\tldr pc, [pc, #-4]\n", symname);
		else
			fprintf(tempfile, ".global %s\n.hidden %s\n%s:", symname, symname, symname);
		fprintf(tempfile, "__imp_%s:\n\t.word 0", symname);
		fclose(tempfile);
		sprintf(cmd, "arm-none-eabi-gcc -x assembler-with-cpp -g0 -c %s.imp.s -o %s.imp.o", symname, symname);
		if (system(cmd) != 0)
			die("arm-none-eabi-gcc returned error");

		safe_call(AddExpImp(cs, &exp_list, symname, symaddr, AEI_EXPORT));
	}

	if(exp_list)
	{
		sprintf(cmd, "rm -f %s", imp_name);
		if (system(cmd) != 0)
			die("rm returned error");
		FILE* tempfile = fopen("__mkimp.mk", "w");
		fprintf(tempfile, "all:\n\t@arm-none-eabi-ar -rc %s $(wildcard *.imp.o)\n", imp_name);
		fclose(tempfile);
		//sprintf(cmd, "arm-none-eabi-ar -rc %s *.imp.o", imp_name);
		//if (system(cmd) != 0)
		if (system("make -f __mkimp.mk"))
			die("arm-none-eabi-ar returned error");
		sprintf(cmd, "rm -f *.imp.o *.imp.s __mkimp.mk");
		if (system(cmd) != 0)
			die("rm returned error");

		fxe2_export_t* expdata = (fxe2_export_t*) malloc(cs->fxe2hdr.sexports);
		if (!expdata)
			die("Out of memory");
		word_t expsymoff = sizeof(fxe2_export_t) * cs->fxe2hdr.nexports;
		fxe2_export_t* expdata_curexp = expdata;
		char* expdata_cursym = (char*) expdata + expsymoff;

		// Walk the exports linked list
		elf2fx2_expimp_t* exp = exp_list;
		while(exp != NULL)
		{
			elf2fx2_expimp_t* next = exp->next;

			expdata_curexp->address = eswap_word(exp->addr);
			expdata_curexp->nameoffset = eswap_word(expsymoff);
			expdata_curexp++;
			
			word_t symlen = strlen(exp->name) + 1;
			strcpy(expdata_cursym, exp->name);
			expdata_cursym += symlen;
			expsymoff += symlen;

			free(exp);
			exp = next;
		}

		safe_call(WriteData(cs->outf, expdata, cs->fxe2hdr.sexports));
		free(expdata);
	}

	if (imp_list)
	{
		// Oh boy, we gotta do imports... { it's basically the code for exports with s/ex/im }
		fxe2_import_t* impdata = (fxe2_import_t*) malloc(cs->fxe2hdr.simports);
		if (!impdata)
			die("Out of memory");
		word_t impsymoff = sizeof(fxe2_import_t) * cs->fxe2hdr.nimports;
		fxe2_import_t* impdata_curimp = impdata;
		char* impdata_cursym = (char*) impdata + impsymoff;

		// Walk the imports linked list
		elf2fx2_expimp_t* imp = imp_list;
		while(imp != NULL)
		{
			elf2fx2_expimp_t* next = imp->next;

			impdata_curimp->address = eswap_word(imp->addr);
			impdata_curimp->nameoffset = eswap_word(impsymoff);
			impdata_curimp++;
			
			word_t symlen = strlen(imp->name) + 1;
			strcpy(impdata_cursym, imp->name);
			impdata_cursym += symlen;
			impsymoff += symlen;

			free(imp);
			imp = next;
		}

		safe_call(WriteData(cs->outf, impdata, cs->fxe2hdr.simports));
		free(impdata);
	}

	return 0;
}

int ReadAndConvertElf(FILE* outf, byte_t* img)
{
	elf2fx2_cnvstruct_t cs;

	Elf32_Ehdr* ehdr = (Elf32_Ehdr*) img;
	if(memcmp(ehdr->e_ident, ELF_MAGIC, 4) != 0)
		die("Invalid ELF file!");
	if(eswap_hword(ehdr->e_type) != ET_EXEC)
		die("ELF file must be executable! (hdr->e_type should be ET_EXEC)");

	Elf32_Phdr* phdr = (Elf32_Phdr*)(img + eswap_word(ehdr->e_phoff));
	if(phdr->p_paddr != 0)
		die("ELF base is not zero!");

	// Fill the conversion struct
	memset(&cs, 0, sizeof(elf2fx2_cnvstruct_t));
	memcpy(cs.fxe2hdr.magic, FXE2_MAGIC, 4);
	cs.outf = outf;
	cs.img = img;
	cs.sects = (Elf32_Shdr*)(img + eswap_word(ehdr->e_shoff));
	cs.nsects = (int)eswap_hword(ehdr->e_shnum);
	cs.sectnames = (const char*)(img + eswap_word(cs.sects[eswap_hword(ehdr->e_shstrndx)].sh_offset));

	// Get information about the ELF file including relocations
	safe_call(GetElfInfo(&cs));

	// Set entrypoint
	if(eswap_word(ehdr->e_entry) > cs.fxe2hdr.loadsize)
		die("Entrypoint out of bounds!");
	cs.fxe2hdr.entrypoint = ehdr->e_entry;
	cs.fxe2hdr.flags = FX2_SUBSYSTEM_EXEC; // TEMPTEMP

	// Prerelocate the binary
	safe_call(PrerelocateBinary(&cs));

	if (HasImpCopy())
		cs.fxe2hdr.flags |= FX2_LDRFLAGS_HASIMPCOPY;

	// Write the binary to the file
	safe_call(WriteBinary(&cs));

	// Write the relocations to the file
	safe_call(WriteRelocations(&cs));

	// Sort the ELF symbols for the following stage
	safe_call(SortSymbols(&cs));

	// Process imports and exports
	safe_call(ProcessSymbols(&cs));

	// Write the import copy table
	if (HasImpCopy()) WriteImpCopy(outf);

	// Write the final header
	safe_call(WriteHeader(&cs));

	return 0;
}

int main(int argc, char* argv[])
{
	int i;

	if(argc != 2 && (argc != 3 || strcmp(argv[2], "-extra") != 0))
	{
		printf("Usage:\n%s filePrefix (-extra)\n", argv[0]);
		return 0;
	}
	
#ifdef WIN32
	if(*argv[1] == '/')
	{
		// Fix MinGW paths.
		argv[1][0] = argv[1][1];
		argv[1][1] = ':';
	}
#endif

	argv_1 = argv[1];
	buildname(elf_name, argv_1, ".elf");
	buildname(fx2_name, argv_1, ".fx2");
	buildname(imp_name, argv_1, ".imp.a");

	argv_name = argv_1;
	for(i = strlen(argv_1)-1; i >= 0; i --)
	{
		if (argv_1[i] == '/')
		{
			argv_name = argv_1 + i + 1;
			break;
		}
	}

	FILE* elf_file = fopen(elf_name, "rb");
	if(!elf_file)
		die("Can't open ELF file!");

	FILE* fx2_file = fopen(fx2_name, "wb");
	if(!fx2_file)
	{
		fclose(elf_file);
		die("Can't open FX2 file!");
	}

	fseek(elf_file, 0, SEEK_END);
	size_t elf_size = ftell(elf_file);
	rewind(elf_file);

	byte_t* img = (byte_t*) malloc(elf_size);
	if(!img)
	{
		fclose(elf_file);
		fclose(fx2_file);
		die("Out of memory!");
	}
	
	if(fread(img, 1, elf_size, elf_file) != elf_size)
	{
		free(img);
		fclose(elf_file);
		fclose(fx2_file);
		die("Can't read ELF file!");
	}

	fclose(elf_file);
	int rc = ReadAndConvertElf(fx2_file, img);

	if (argc == 3)
	{
		// Copy extra data from stdin
		int x;
#ifdef WIN32
		setmode(fileno(stdin), O_BINARY);
#endif
		fseek(fx2_file, 0, SEEK_END);
		byte_t buf[1024];
		do
		{
			x = fread(buf, 1, sizeof(buf), stdin);
			fwrite(buf, 1, x, fx2_file);
		} while(x);
	}

	free(img);
	fclose(fx2_file);
	if(rc != 0)
		remove(fx2_name);
	return rc;
}
