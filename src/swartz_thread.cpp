#include "swartz_thread.h"
#include <pthread.h>
#include <stdlib.h>

#define SWARTZ_THREAD_DEFAULT_STACKSIZE 1048576 //1MB

typedef struct swartz_thread_t
{
	pthread_t handle;
	int priority;
	void* fn;
	void* arg;
}swartz_thread_t;

int swartz_thread_create(swartz_thread_t** handle, void*fn, void* arg, int stack_size, int priority)
{
	(void)(priority);

	if (!handle || !fn)
	{
		return -1;
	}

	*handle = (swartz_thread_t*)malloc(sizeof(swartz_thread_t));
	if (*handle == NULL)
	{
		return -1;
	}

	if (stack_size == 0)
	{
		stack_size = SWARTZ_THREAD_DEFAULT_STACKSIZE;
	}

	pthread_attr_t attr;
	int ret = pthread_attr_init(&attr);
	if (ret)
	{
		free(*handle);
		*handle = NULL;
		return -1;
	}

	if (stack_size != 0)
	{
		ret = pthread_attr_setstacksize(&attr, stack_size);
		if (ret != 0)
		{
			free(*handle);
			*handle = NULL;
			pthread_attr_destroy(&attr);
			return -1;
		}
	}

	ret = pthread_create(&(*handle)->handle, &attr, (void *(*)(void*))fn, arg);
	if (ret)
	{
		free(*handle);
		*handle = NULL;
		pthread_attr_destroy(&attr);
		return -1;
	}
	else
	{
		(*handle)->arg = arg;
		(*handle)->fn = fn;
		(*handle)->priority = priority;

		pthread_attr_destroy(&attr);
		return 0;
	}
}

int swartz_thread_wait(swartz_thread_t* handle)
{
	if (!handle)
	{
		return -1;
	}

	int ret = 0;

	ret = pthread_join(handle->handle, NULL); 
	free(handle);
	handle = NULL;

	return (ret == 0) ? 0 : -1;
}

int swartz_thread_detached_create(void*fn, void* arg, int stack_size, int priority)
{
	(void)(priority);

	if (!fn)
	{
		return -1;
	}

	if (stack_size == 0)
	{
		stack_size = SWARTZ_THREAD_DEFAULT_STACKSIZE;
	}

	pthread_attr_t attr;
	int ret = pthread_attr_init(&attr);
	if (ret)
	{
		return -1;
	}

	ret = pthread_attr_setdetachstate(&attr, PTHREAD_CREATE_DETACHED);
	if (ret)
	{
		pthread_attr_destroy(&attr);
		return -1;
	}

	if (stack_size != 0)
	{
		ret = pthread_attr_setstacksize(&attr, stack_size);
		if (ret != 0)
		{
			pthread_attr_destroy(&attr);
			return -1;
		}
	}

	pthread_t id;
	ret = pthread_create(&id, &attr, (void *(*)(void*))fn, arg);
	if (ret)
	{
		pthread_attr_destroy(&attr);
		return -1;
	}
	else
	{
		pthread_attr_destroy(&attr);
		return 0;
	}
}

