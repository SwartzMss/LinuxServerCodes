#ifndef __SWARTZ_MUTEX_H__
#define __SWARTZ_MUTEX_H__

#ifdef __cplusplus
extern "C" {
#endif /* __cplusplus */

typedef struct swartz_mutex_t swartz_mutex_t;

#define SWARTZ_MUTEX_ATTR_RECURSIVE	0
#define SWARTZ_MUTEX_ATTR_FAST		1
#define SWARTZ_MUTEX_ATTR_ERRORCHECK	2

int swartz_mutex_create(swartz_mutex_t** handle, int attr);

int swartz_mutex_lock(swartz_mutex_t* handle);

int swartz_mutex_trylock(swartz_mutex_t* handle);

int swartz_mutex_unlock(swartz_mutex_t* handle);

int swartz_mutex_destroy(swartz_mutex_t* handle);

#ifdef __cplusplus
}
#endif /* __cplusplus */

#endif

