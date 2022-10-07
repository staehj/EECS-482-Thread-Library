#ifndef _MUTEX_IMPL_H
#define _MUTEX_IMPL_H
#define _XOPEN_SOURCE

#include "mutex.h"

class mutex::impl {
public:
	impl();

	~impl();

	void impl_lock();

	void impl_unlock();

private:

};

#endif /* _MUTEX_IMPL_H */
