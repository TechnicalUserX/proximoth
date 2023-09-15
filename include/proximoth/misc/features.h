#ifndef PROXIMOTH_FEATURES_H
#define PROXIMOTH_FEATURES_H

#ifndef _GNU_SOURCE
    #define _GNU_SOURCE
#endif

#if !defined(__GNUC__) && !defined(__clang__)
    #error "This program is only intended for gcc and clang compilers"
#endif

#if __STDC_VERSION__ < 201112L
    #error "Accepted minimum C standard is C11"
#endif

#endif