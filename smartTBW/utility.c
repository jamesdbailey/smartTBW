//
//  utility.c
//  smartTBW
//
//  Created by James Bailey on 4/5/21.
//

#include <inttypes.h>
#include <string.h>
#ifdef HAVE_LOCALE_H
#include <locale.h>
#endif
#include "utility.h"

size_t snprintf_u128_u(char *buf, size_t buflen, uint128_t u128, char *separator)
{
    size_t rc;

    if (u128 > UINT64_MAX)
    {
        char num[64];
        size_t rc_num;
        uint128_t hi = u128 / P10_UINT64;
        uint64_t lo = u128 % P10_UINT64;
        rc = snprintf_u128_u(buf, buflen, hi, NULL);
        
        rc_num = snprintf(num, sizeof(num), "%." TO_STRING(E10_UINT64) PRIu64, lo);
        
        strncpy(buf+rc, num, rc_num + 1);
        rc += rc_num;
    }
    else
    {
        uint64_t u64 = u128;
        rc = snprintf(buf, buflen, "%" PRIu64, u64);
    }
                 
    if (separator)
    {
        char num[64];
        size_t numlen = rc;
        int i = 0, j = 0;
        do
            num[j++] = buf[i++];
        while (i < numlen && (numlen - i) % 3 != 0 && j < buflen-1);
        
        num[j] = 0;
        
        while (i < numlen && j < buflen - 1)
        {
            j += snprintf(num+j, buflen-j, "%s%.3s", separator, buf+i);
            i += 3;
        }
        
        strncpy(buf, num, strlen(num)+1);
    }
    
    return rc;
}

// Format capacity with SI prefixes
const char * format_capacity(char * str, size_t strsize, uint128_t val)
{
    char *decimal_point = ".";
    const unsigned factor = 1000; // 1024 for KiB,MiB,...
    static const char prefixes[] = " KMGTP";
    
    // Find d with val in [d, d*factor)
    unsigned i = 0;
    uint64_t d = 1;
    for (uint64_t d2 = d * factor; val >= d2; d2 *= factor) {
        d = d2;
        if (++i >= sizeof(prefixes)-2)
            break;
    }
    
    // Print 3 digits
    uint64_t n = val / d;
    if (i == 0)
        snprintf(str, strsize, "%u B", (unsigned)n);
    else if (n >= 100) // "123 xB"
        snprintf(str, strsize, "%" PRIu64 " %cB", n, prefixes[i]);
    else if (n >= 10)  // "12.3 xB"
        snprintf(str, strsize, "%" PRIu64 "%s%u %cB", n, decimal_point,
                 (unsigned)(((val % d) * 10) / d), prefixes[i]);
    else               // "1.23 xB"
        snprintf(str, strsize, "%" PRIu64 "%s%02u %cB", n, decimal_point,
                 (unsigned)(((val % d) * 100) / d), prefixes[i]);
    
    return str;
}

// Format a Kelvin temperature value in Celsius.
const char * kelvin_to_str(char *str, size_t len, uint16_t k)
{
    if (!k) // unsupported?
    {
        str[0] = '-';
        str[1] = 0;
    }
    else
    {
        float c = (k - 273.15);
        float f = ((1.8*c)+32);
        snprintf(str, len, "%.2f °C (%.2f °F)", c, f);
    }
    
    return str;
}

uint128_t lowhigh_to_uint128(uint64_t low, uint64_t high)
{
    uint128_t total = 0;
    if (high != 0ULL)
    {
        total = ((uint128_t)UINT64_MAX + 1) * high + low;
    }
    else
    {
        total = low;
    }
    
    return total;
}

void format128(char *num, size_t len, uint64_t low, uint64_t high) {
    uint128_t total = lowhigh_to_uint128(low, high);
    snprintf_u128_u(num, len, total, ",");
}

