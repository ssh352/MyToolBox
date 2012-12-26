// Copyright (c) 2011 The LevelDB Authors. All rights reserved.
// Use of this source code is governed by a BSD-style license that can be
// found in the LICENSE file. See the AUTHORS file for names of contributors.
//
// This file contains the specification, but not the implementations,
// of the types/operations/etc. that should be defined by a platform
// specific port_<platform>.h file.  Use this file as a reference for
// how to port this package to a new platform.

#pragma once

#include <string>
#include <stdint.h>
#define snprintf _snprintf
#define close _close
#define fread_unlocked _fread_nolock


namespace leveldb {
namespace port {

// TODO(jorlow): Many of these belong more in the environment class rather than
//               here. We should try moving them and see if it affects perf.

// The following boolean constant must be true on a little-endian machine
// and false otherwise.
static const bool kLittleEndian = true /* or some other expression */;

// ------------------ Threading -------------------
class CondVar;
class Mutex {
public:
	Mutex();
	~Mutex();

	void Lock();
	void Unlock();
	void AssertHeld();

private:
	friend class CondVar;
	// critical sections are more efficient than mutexes
	// but they are not recursive and can only be used to synchronize threads within the same process
	// we use opaque void * to avoid including windows.h in port_win.h
	void * cs_;

	// No copying
	Mutex(const Mutex&);
	void operator=(const Mutex&);
};

// the Win32 API offers a dependable condition variable mechanism, but only starting with
// Windows 2008 and Vista
// no matter what we will implement our own condition variable with a semaphore
// implementation as described in a paper written by Andrew D. Birrell in 2003
class CondVar {
public:
	explicit CondVar(Mutex* mu);
	~CondVar();
	void Wait();
	void Signal();
	void SignalAll();
private:
	Mutex* mu_;

	Mutex wait_mtx_;
	long waiting_;

	void * sem1_;
	void * sem2_;
};

// Thread-safe initialization.
// Used as follows:
//      static port::OnceType init_control = LEVELDB_ONCE_INIT;
//      static void Initializer() { ... do something ...; }
//      ...
//      port::InitOnce(&init_control, &Initializer);
typedef volatile int OnceType;
#define LEVELDB_ONCE_INIT 0
extern void InitOnce(port::OnceType*, void (*initializer)());

// Storage for a lock-free pointer
class AtomicPointer {
private:
	void * rep_;
public:
	AtomicPointer() : rep_(0) { }
	explicit AtomicPointer(void* v); 
	void* Acquire_Load() const;

	void Release_Store(void* v);

	void* NoBarrier_Load() const;

	void NoBarrier_Store(void* v);
};

// ------------------ Compression -------------------

// Store the snappy compression of "input[0,input_length-1]" in *output.
// Returns false if snappy is not supported by this port.
extern bool Snappy_Compress(const char* input , size_t input_length,
                            std::string* output);

// If input[0,input_length-1] looks like a valid snappy compressed
// buffer, store the size of the uncompressed data in *result and
// return true.  Else return false.
extern bool Snappy_GetUncompressedLength(const char* input, size_t length,
                                         size_t* result);

// Attempt to snappy uncompress input[0,input_length-1] into *output.
// Returns true if successful, false if the input is invalid lightweight
// compressed data.
//
// REQUIRES: at least the first "n" bytes of output[] must be writable
// where "n" is the result of a successful call to
// Snappy_GetUncompressedLength.
extern bool Snappy_Uncompress(const char* input_data, size_t input_length,
                              char* output);

// ------------------ Miscellaneous -------------------

// If heap profiling is not supported, returns false.
// Else repeatedly calls (*func)(arg, data, n) and then returns true.
// The concatenation of all "data[0,n-1]" fragments is the heap profile.
extern bool GetHeapProfile(void (*func)(void*, const char*, int), void* arg);

}  // namespace port
}  // namespace leveldb

