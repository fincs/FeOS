#include "common.h"
#include "tinfl.h"

// "The output buffer is at least the size of the dictionary (typically 32KB)."
#define BUFSIZE TINFL_LZ_DICT_SIZE

static byte_t buf_in[BUFSIZE];
static byte_t buf_out[BUFSIZE];

static inline size_t sz_min(size_t a, size_t b)
{
	return (a < b) ? a : b;
}

static inline size_t sz_max(size_t a, size_t b)
{
	return (a > b) ? a : b;
}

// Based on miniz's example5.c
static int deflate_dec(FILE* fout, FILE* fin, size_t compSize)
{
	size_t remaining = compSize;
	size_t avail_in = 0;
	size_t avail_out = BUFSIZE;
	const void* next_in = buf_in;
	void* next_out = buf_out;
	size_t total_in = 0, total_out = 0;

	tinfl_decompressor inflator;
	tinfl_init(&inflator);

	for (;;)
	{
		size_t in_bytes, out_bytes;
		tinfl_status status;

		if (!avail_in)
		{
			// Read more bytes
			size_t n = sz_min(BUFSIZE, remaining);

			progressbar(compSize - remaining, compSize);
			if (fread(buf_in, 1, n, fin) != n)
				return 1; // Read failure

			next_in = buf_in;
			avail_in = n;

			remaining -= n;
		}

		in_bytes = avail_in;
		out_bytes = avail_out;
		status = tinfl_decompress(&inflator, next_in, &in_bytes, buf_out, next_out, &out_bytes, (remaining ? TINFL_FLAG_HAS_MORE_INPUT : 0));

		avail_in -= in_bytes;
		next_in += in_bytes;
		total_in += in_bytes;

		avail_out -= out_bytes;
		next_out += out_bytes;
		total_out += out_bytes;

		bool bIsLessOrEqualDone = status <= TINFL_STATUS_DONE;

		if (bIsLessOrEqualDone || !avail_out)
		{
			// Output buffer is full, write it
			size_t n = BUFSIZE - avail_out;
			if (fwrite(buf_out, 1, n, fout) != n)
				return 1; // write failure
			next_out = buf_out;
			avail_out = BUFSIZE;
		}

		if (bIsLessOrEqualDone)
		{
			if (status == TINFL_STATUS_DONE)
			{
				progressbar(compSize, compSize);
				printf("\n\n");
				break;
			}

			return 1; // Decoding failure
		}
	}
	return 0;
}

int decompress_fpkg(const char* fileName, const char* outFile)
{
	FILE* fin = fopen(fileName, "rb");
	if (!fin)
	{
		fprintf(stderr, "Unable to open src: %s\n", fileName);
		return 1;
	}

	fseek(fin, sizeof(fpkg_header_t) + 4, SEEK_SET);
	word_t mSize;
	fread(&mSize, sizeof(mSize), 1, fin);
	fseek(fin, mSize+4, SEEK_CUR);
	size_t compSize;
	fread(&compSize, sizeof(size_t), 1, fin);
	fseek(fin, 4, SEEK_CUR); // skip uncompressed size

	FILE* fout = fopen(outFile, "wb");
	if (!fout)
	{
		fprintf(stderr, "Unable to open dest: %s\n", outFile);
		fclose(fin);
		return 1;
	}

	int rc = deflate_dec(fout, fin, compSize);
	fclose(fin);
	fclose(fout);
	return rc;
}
