#pragma once

#ifdef __cplusplus
extern "C" {
#endif

void AddImpCopy(word_t from, word_t to);
int HasImpCopy();
void WriteImpCopy(FILE* f);

#ifdef __cplusplus
}
#endif
