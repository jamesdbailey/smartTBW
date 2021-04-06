//
//  utility.h
//  smartTBW
//
//  Created by James Bailey on 4/5/21.
//

#ifndef utility_h
#define utility_h

#include <stdio.h>
#include <inttypes.h>

typedef unsigned __int128 uint128_t;

/*      UINT64_MAX 18446744073709551615ULL */
#define P10_UINT64 10000000000000000000ULL   /* 19 zeroes */
#define E10_UINT64 19

#define STRINGIZER(x)   # x
#define TO_STRING(x)    STRINGIZER(x)

int print_u128_u(uint128_t u128);
size_t snprintf_u128_u(char *buf, size_t buflen, uint128_t u128, char *separator);
const char * format_capacity(char * str, size_t strsize, uint128_t val);
uint128_t lowhigh_to_uint128(uint64_t low, uint64_t high);
void format128(char *num, size_t len, uint64_t low, uint64_t high);

#endif /* utility_h */
