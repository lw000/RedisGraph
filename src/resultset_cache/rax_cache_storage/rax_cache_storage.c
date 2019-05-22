/*
* Copyright 2018-2019 Redis Labs Ltd. and Contributors
*
* This file is available under the Redis Labs Source Available License Agreement
*/

#include "rax_cache_storage.h"
#include "../../util/rmalloc.h"

RaxCacheStorage *RaxCacheStorage_New()
{
    // memeory allocation
    RaxCacheStorage *raxCacheStorage = rm_malloc(sizeof(RaxCacheStorage));
    // create new rax
    raxCacheStorage->rt = raxNew();
    return raxCacheStorage;
}

void clearCacheStorage(RaxCacheStorage *raxCacheStorage)
{
    // free rax
    raxFree(raxCacheStorage->rt);
    // free memeory
    raxCacheStorage->rt = raxNew();
}

void insertToCache(RaxCacheStorage *raxCacheStorage, CacheData *cacheData)
{
    // insert to rax the cache entry key, and the cache data itself as value
    raxInsert(raxCacheStorage->rt, (unsigned char *)&cacheData->hashKey, HASH_KEY_LENGTH, cacheData, NULL);
}

void removeFromCache(RaxCacheStorage *raxCacheStorage, CacheData *cacheData)
{
    // remove from rax according to the key
    raxRemove(raxCacheStorage->rt, (unsigned char *)&cacheData->hashKey, HASH_KEY_LENGTH, NULL);
}

CacheData *getFromCache(RaxCacheStorage *raxCacheStorage, unsigned char *hashKey)
{
    // search in rax
    CacheData *data = raxFind(raxCacheStorage->rt, hashKey, HASH_KEY_LENGTH);
    if (data == raxNotFound)
    {
        // if not found return null
        return NULL;
    }
    return data;
}

void RaxCacheStorage_Free(RaxCacheStorage *raxCacheStorage)
{
    // free rax and create new one
    raxFree(raxCacheStorage->rt);
    rm_free(raxCacheStorage);
}
