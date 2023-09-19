#ifndef PROXIMOTH_FEATURES_H
#define PROXIMOTH_FEATURES_H

#ifndef _GNU_SOURCE
    #define _GNU_SOURCE
#endif

#if !defined(__GNUC__) && !defined(__clang__)
    #error "This program is only intended for gcc and clang compilers"
#endif

#if !defined(__cplusplus)
    #if __STDC_VERSION__ < 2011L
        #error "Accepted minimum C standard is C11"
    #endif
#else
    #if __cplusplus < 2011L
        #error "Accepted minimum C++ standard is C++11"
    #endif
#endif

#endif