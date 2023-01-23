#ifndef PROXIMOTH_H
#define PROXIMOTH_H

#define PROXIMOTH_VERSION "0.0.3-beta"

#ifndef _GNU_SOURCE
    #define _GNU_SOURCE
#endif



#include <time.h>
#include <math.h>
#include <stdint.h>
#include <limits.h>
#include <errno.h>
#include <string.h>
#include <stdlib.h>
#include <stdio.h>
#include <stdbool.h>
#include <ctype.h>
#include <sys/time.h>
#include <sys/wait.h>
#include <getopt.h>
#include <pcap.h>
#include <pthread.h>
#include <semaphore.h>
#include <unistd.h>
#include <signal.h>
#include <fcntl.h>
#include <linux/wireless.h>
#include <sys/ioctl.h>
#include <ifaddrs.h>
#include <iso646.h>


#define PROXIMOTH_MAC_STRING_SIZE 18 // 17+1

#define COLOR "\033["
#define BOLD "1;"
#define CLEAR "\033[0m"
#define RGB(r,g,b) "38;2;"#r";"#g";"#b"m"

typedef unsigned char byte_t;


#endif
