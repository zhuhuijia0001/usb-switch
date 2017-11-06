#ifndef _TRACE_H_
#define _TRACE_H_

#include <rtconfig.h>

extern void rt_kprintf(const char *fmt, ...);

#ifdef RT_DEBUG
#define TRACE(...)  rt_kprintf(__VA_ARGS__)
#else
#define TRACE(...) 
#endif

#endif

