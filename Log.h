#ifndef LOG_H
#define LOG_H

#include <time.h>
#include <stdio.h>
#include <stdarg.h>
#include <string.h>
#include <errno.h>

#include <QDebug>

#define DEBUG

/*信息调试宏*/
#ifdef DEBUG
static inline int debug_ret(const char *file,
    const char *function, int line, const char *fmt, ...)
{
    char buf[1024] = {0};
    va_list ap;

    va_start(ap, fmt);
    snprintf(buf, sizeof(buf), "[%s:%s:%d]", file, function, line);
    vsnprintf(buf+strlen(buf), sizeof(buf)-strlen(buf), fmt, ap);

    fflush(stdout);
    fputs(buf, stdout);
    fflush(NULL);

    return 0;
}
#define myDebug(fmt, ...)									\
    debug_ret(__FILE__,__FUNCTION__,__LINE__,fmt,##__VA_ARGS__);
#else
    #define myDebug(format, ...)
#endif

/*出错信息打印*/
static inline int err_ret(const char *file,
    const char *function, int line, const char *fmt, ...)
{
    char buf[1024] = {0};
    char tm_buf[128] = {0};
    va_list ap;

    snprintf(tm_buf, sizeof(tm_buf), "%s %s", __DATE__, __TIME__);

    va_start(ap, fmt);
    snprintf(buf, sizeof(buf), "[%s:%s:%d][%s]", file, function, line, tm_buf);
    vsnprintf(buf+strlen(buf), sizeof(buf)-strlen(buf), fmt, ap);
    if ( errno ) {
        snprintf(buf+strlen(buf), sizeof(buf)-strlen(buf),
            ":[errno:%d]%s.\n", errno, (char*)strerror(errno));
        errno = 0;
    } else {
        snprintf(buf+strlen(buf), sizeof(buf)-strlen(buf), ".\n");
    }
    fflush(stdout);
    fputs(buf, stdout);
    fflush(NULL);

    return 0;
}
#define printError(fmt, ...)									\
do{																\
    err_ret(__FILE__,__FUNCTION__,__LINE__,fmt,##__VA_ARGS__);	\
}while(0)


#endif
