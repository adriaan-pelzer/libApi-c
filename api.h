#include <stdio.h>
#include <stdlib.h>
#include <oauth.h>
#include <syslog.h>
#include <Json.h>
#include <Curl.h>

#ifndef _API_H_
#define _API_H_ 

typedef const char *cc_p;

typedef struct apiCtx {
    char *url;
    connectionType ctype;
    char *postargs;
} *apiCtx_p;

int callApi(cc_p url, cc_p postargs, connectionType ctype, const char **paths, size_t pathcount, void *uCtx, int (*uCB)(void *uCtx, jsonStruct_p jS));

#endif
