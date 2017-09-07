#include "apue.h"
#include <errno.h>
#include <stdarg.h>
#include <syslog.h>

static void log_doit(int, int, const char*, va_list);

void log_open(const char *indent, int option, int facility){
	if(!log_to_stderr)
		openlog(indent, option, facility);
}

void log_ret(const char *fmt, ...){
	va_list ap;
	
	va_start(ap, fmt);
	log_doit(1, LOG_ERR, fmt, ap);
	va_end(ap);
}

void log_sys(const char *fmt, ...){
	va_list ap;
	
	va_start(ap, fmt);
	log_doit(1, LOG_ERR, fmt, ap);
	va_end(ap);
	exit(1);
}

void log_msg(const char *fmt, ...){
	va_list ap;
	
	va_start(ap, fmt);
	log_doit(0, LOG_ERR, fmt, ap);
	va_end(ap);
}

void log_quit(const char *fmt, ...){
	va_list ap;
	
	va_start(ap, fmt);
	log_doit(0, LOG_ERR, fmt, ap);
	va_end(ap);
	exit(1);
}

static void log_doit(int errnoflag, int priority, const char *fmt, va_list ap){
	int errno_save = errno;
	char buf[MAXLINE];
	
	vsnprintf(buf, MAXLINE, fmt, ap);
	if(errnoflag)
		snprintf(buf+strlen(buf), MAXLINE-strlen(buf), ": %s", strerror(errno_save));
	strcat(buf, "\n");
	
	if(log_to_stderr){	
		fflush(stdout);
		fputs(buf, stderr);
		fflush(stderr);
	}else
		syslog(priority, buf);
}

	 
