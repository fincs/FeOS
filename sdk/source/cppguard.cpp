// Dummy C++ ABI guard functions

extern "C" int __cxa_guard_acquire(int* gv)
{
	return 1;
}

extern "C" void __cxa_guard_release(int* gv)
{
}

extern "C" void __cxa_guard_abort(int* gv)
{
}
