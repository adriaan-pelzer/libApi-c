#include "addr.h"
#include "api.h"

static int (*userCB)(void *uCtx, jsonStruct_p jS);
static char **whitelisted_paths;
static size_t whitelist_size;

void freeApiCtx(apiCtx_p aCtx) {
    if (aCtx) {
        F(aCtx->url);
        F(aCtx->postargs);
        F(aCtx);
    }
    return;
}

apiCtx_p createApiCtx() {
    apiCtx_p rc = NULL, _rc = NULL;

    DOA("allocate memory for api context", malloc, _rc, NULL, sizeof(struct apiCtx));
    memset(_rc, 0, sizeof(struct apiCtx));

    rc = _rc;
over:
    FFF(_rc, freeApiCtx, _rc && (rc == NULL));
    return rc;
}

static int setUrl(apiCtx_p aCtx, const char *url, const char *postargs, connectionType ctype) {
    int rc = -1;

    F(aCtx->url);
    if(url) { DOA("allocate memory for url", strdup, aCtx->url, NULL, url); }
    F(aCtx->postargs);
    if(postargs) { DOA("allocate memory for postargs", strdup, aCtx->postargs, NULL, postargs); }
    aCtx->ctype = ctype;

    rc = 0;
over:
    return rc;
}

static int streamCB(void *uCtx, char **memory, size_t *size) { return 0; }

static int returnCB(void *uCtx, const char *memory, const size_t size) {
    int rc = -1;
    jsonStruct_p jS = NULL;

    if (strlen(memory) != size) {
        syslog(P_ERR, "Size mismatch");
        goto over;
    }

    if (size > 2) {
        DOA("get tweet json struct", parseJson, jS, NULL, memory, (const char **) whitelisted_paths, whitelist_size);
        DONT("call user callback function on json struct", userCB, 0, uCtx, jS);
    }

    rc = 0;
over:
    freeJsonStruct(jS);
    return rc;
}

int callApi(cc_p url, cc_p postargs, connectionType ctype, const char **paths, size_t pathcount, void *uCtx, int (*uCB)(void *uCtx, jsonStruct_p jS)) {
    int rc = -1;
    apiCtx_p aCtx = NULL;

    userCB = uCB;
    whitelisted_paths = (char **) paths;
    whitelist_size = pathcount;
    DOA("allocate memory for api context", createApiCtx, aCtx, NULL);
    DONT("set api url", setUrl, 0, aCtx, url, postargs, ctype);

    rc = curl_connect(aCtx->url, aCtx->ctype, aCtx->postargs, uCtx, returnCB, streamCB);
over:
    FF(aCtx, freeApiCtx);
    return rc;
}

