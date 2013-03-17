#pragma once

/** @file md5.h
 *  \brief MD5 hash library.
 *
 *  The code is based off of "RFC 1321 - The MD5 Message Digest Algorithm"
 */

typedef void* MD5_CTX; //!< MD5 hashing context.

int MD5_Init(MD5_CTX *c); //!< Initializes a MD5 hashing context.
int MD5_Update(MD5_CTX *c, const void *data, unsigned long len); //!< Hashes the specified data. Data can be incrementally hashed.
int MD5_Final(unsigned char *md, MD5_CTX *c); //!< Frees a MD5 hashing context and generates the final digest (16 bytes).
