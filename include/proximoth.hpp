#ifndef _PROXIMOTH_HPP
#define _PROXIMOTH_HPP

#define PROXIMOTH_VERSION "0.0.1-beta"

#include <cstdio>
#include <cstdlib>
#include <cstring>
#include <cerrno>
#include <climits>
#include <ctime>
#include <cmath>
#include <cstdint>

#include <sys/time.h>
#include <sys/wait.h>
#include <getopt.h>
#include <pcap.h>
#include <pthread.h>
#include <unistd.h>
#include <signal.h>
#include <fcntl.h>
#include <linux/wireless.h>
#include <sys/ioctl.h>
#include <ifaddrs.h>

#include <string>
#include <iostream>

#define COLOR "\033["
#define BOLD "1;"
#define CLEAR "\033[0m"
#define RGB(r,g,b) "38;2;"#r";"#g";"#b"m"

typedef unsigned char byte_t;

#endif