#include    "../includes/server.h"

zframe_t *identify(char *name)
{
	return (zframe_from("Identify OK"));
}

zframe_t *forward(char * __attribute__((__unused__)) name)
{
	return (zframe_from("Forward OK"));	
}

zframe_t *backward(char * __attribute__((__unused__)) name)
{
	return (zframe_from("Backward OK"));	
}

zframe_t *leftfwd(char * __attribute__((__unused__)) name)
{
	return (zframe_from("Leftfwd OK"));	
}

zframe_t *rightfwd(char * __attribute__((__unused__)) name)
{
	return (zframe_from("Rightfwd OK"));	
}

zframe_t *right(char * __attribute__((__unused__)) name)
{
	return (zframe_from("Right OK"));	
}

zframe_t *left(char * __attribute__((__unused__)) name)
{
	return (zframe_from("Left OK"));
}

zframe_t *looking(char * __attribute__((__unused__)) name)
{
	return (zframe_from("Looking OK"));
}

zframe_t *gather(char * __attribute__((__unused__)) name)
{
	return (zframe_from("Gather OK"));
}

zframe_t *watch(char * __attribute__((__unused__)) name)
{
	return (zframe_from("Watch OK"));
}

zframe_t *attack(char * __attribute__((__unused__)) name)
{
	return (zframe_from("Attack OK"));
}

zframe_t *selfid(char * __attribute__((__unused__)) name)
{
	return (zframe_from("Selfid OK"));
}

zframe_t *selfstats(char * __attribute__((__unused__)) name)
{
	return (zframe_from("Selfstats OK"));
}

zframe_t *inspect(char *name)
{
	return (zframe_from("Inspect OK"));
}

zframe_t *next(char * __attribute__((__unused__)) name)
{
	return (zframe_from("Next OK"));
}

zframe_t *jump(char * __attribute__((__unused__)) name)
{
	return (zframe_from("Jump OK"));
}
