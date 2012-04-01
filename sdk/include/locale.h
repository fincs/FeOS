//
// FeOS C Standard Library
// locale.h
//     Locale functions
//

#pragma once

#ifdef __cplusplus
extern "C"
{
#endif

#define LC_ALL 0
#define LC_COLLATE 1
#define LC_CTYPE 2
#define LC_MONETARY 3
#define LC_NUMERIC 4
#define LC_TIME 5

struct lconv
{
	char *decimal_point, *thousands_sep, *grouping, *int_curr_symbol, *currency_symbol,
		*mon_decimal_point, *mon_thousands_sep, *mon_grouping, *positive_sign, *negative_sign;
	char int_frac_digits, frac_digits, p_cs_precedes, n_cs_precedes, p_sep_by_space, n_sep_by_space,
		p_sign_posn, n_sign_posn;
};

struct lconv* localeconv();
char* setlocale(int, const char*);

#ifdef __cplusplus
}
#endif
