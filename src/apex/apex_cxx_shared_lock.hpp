/*
 * Copyright (c) 2014-2021 Kevin Huck
 * Copyright (c) 2014-2021 University of Oregon
 *
 * Distributed under the Boost Software License, Version 1.0. (See accompanying
 * file LICENSE_1_0.txt or copy at http://www.boost.org/LICENSE_1_0.txt)
 */

#pragma once

// It's gotten fun.  Apparently gcc 7 supports shared_mutex, but even though
// clang 5/6 *claims* to support c++17, it doesn't have shared_mutex.h
#if (defined(__GNUC__) && !defined(__clang__))
#define GCC_COMPILER_7
#else
#undef GCC_COMPILER_7
#endif

#if __cplusplus > 201701L && defined(GCC_COMPILER_7)
    #include <shared_mutex>
#elif __cplusplus >= 201500L && defined(GCC_COMPILER_7)
    // if we've got gcc 6.1+ and -std=c++17
    #include <shared_mutex>
#elif __cplusplus > 201402L
    #include <mutex>
#else
    #if defined(_POSIX_VERSION) && !defined(_MSC_VER) && !defined(__APPLE__)
        #include <pthread.h>
    #else
        #include <mutex>
    #endif
#endif

namespace apex
{

#if __cplusplus > 201701L && defined(GCC_COMPILER_7)
    // if we've got gcc 6.1 and -std=c++17
    typedef std::shared_mutex shared_mutex_type;
    typedef std::shared_lock<shared_mutex_type> read_lock_type;
    typedef std::unique_lock<shared_mutex_type> write_lock_type;
#elif __cplusplus > 201402L && defined(GCC_COMPILER_7)
    // if we've got gcc 4.9+ and -std=c++14
    typedef std::shared_timed_mutex shared_mutex_type;
    typedef std::shared_lock<shared_mutex_type> read_lock_type;
    typedef std::unique_lock<shared_mutex_type> write_lock_type;
#else // older compiler, without shared locks. Try posix support...
#if defined(_POSIX_VERSION) && !defined (_MSC_VER) && !defined(__APPLE__)
    class posix_shared_mutex {
    public:
        posix_shared_mutex() {
            pthread_rwlock_init(&rwlock, nullptr);
        }
        pthread_rwlock_t* get_mutex() { return &rwlock; }
    private:
        pthread_rwlock_t rwlock;
    };

    class posix_shared_lock {
    public:
        posix_shared_lock(posix_shared_mutex &mutex) : _mutex(mutex) {
            pthread_rwlock_rdlock(_mutex.get_mutex());
        }
        ~posix_shared_lock() {
            pthread_rwlock_unlock(_mutex.get_mutex());
        }
    private:
        posix_shared_mutex &_mutex;
    };

    class posix_unique_lock {
    public:
        posix_unique_lock(posix_shared_mutex &mutex) : _mutex(mutex) {
            pthread_rwlock_wrlock(_mutex.get_mutex());
        }
        ~posix_unique_lock() {
            pthread_rwlock_unlock(_mutex.get_mutex());
        }
    private:
        posix_shared_mutex &_mutex;
    };
    typedef posix_shared_mutex shared_mutex_type;
    typedef posix_shared_lock read_lock_type;
    typedef posix_unique_lock write_lock_type;
#else // no posix
    // no modern compiler, no posix - go with slow instead.
    typedef std::mutex shared_mutex_type;
    typedef std::unique_lock<shared_mutex_type> read_lock_type;
    typedef std::unique_lock<shared_mutex_type> write_lock_type;
#endif // no posix

#endif // compiler language version

}

