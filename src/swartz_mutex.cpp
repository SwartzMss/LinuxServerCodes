#include "swartz_mutex.h"

#include <pthread.h>
#include <stdlib.h>


typedef struct swartz_mutex_t
{
	pthread_mutex_t mutex;
}swartz_mutex_t;

int swartz_mutex_create(swartz_mutex_t** handle, int flag)
{
	if (NULL == handle)
	{
		return -1;
	}

	*handle = (swartz_mutex_t*)malloc(sizeof(swartz_mutex_t));
	if (NULL == *handle)
	{
		return -1;
	}

	int ret = 0;
	pthread_mutexattr_t attr;
	pthread_mutexattr_init(&attr);

	pthread_mutexattr_settype(&attr, flag);

	ret = pthread_mutex_init(&((*handle)->mutex), &attr); 

	pthread_mutexattr_destroy(&attr);
	
	if (ret != 0)
	{
		free(*handle);
		*handle = NULL;
	}

	return (ret == 0) ? 0 : -1;
}

int swartz_mutex_lock(swartz_mutex_t* handle)
{
	if (!handle)
	{
		return -1;
	}
	return pthread_mutex_lock(&(handle->mutex)) == 0 ? 0 : -1;

}

int swartz_mutex_trylock(swartz_mutex_t* handle)
{
	if ( (!handle))
	{
		return -1;
	}

	return pthread_mutex_trylock(&(handle->mutex)) == 0 ? 0 : -1;
}

int swartz_mutex_unlock(swartz_mutex_t* handle)
{
	if ( (!handle))
	{
		return -1;
	}

	return pthread_mutex_unlock(&(handle->mutex)) == 0 ? 0 : -1;
}

int swartz_mutex_destroy(swartz_mutex_t* handle)
{
	if (!handle)
	{
		return -1;
	}

	 pthread_mutex_destroy(&(handle->mutex));
	 free(handle);
	 return 0;
}

