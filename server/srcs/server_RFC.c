#include    "../includes/server.h"

zframe_t *identify(char *name)
{
	return (zframe_from("Identify OK"));
}

zframe_t *forward(char */*name*/)
{
	return (zframe_from("Forward OK"));	
}

zframe_t *backward(char */*name*/)
{
	return (zframe_from("Backward OK"));	
}

zframe_t *leftfwd(char */*name*/)
{
	return (zframe_from("Leftfwd OK"));	
}

zframe_t *rightfwd(char */*name*/)
{
	return (zframe_from("Rightfwd OK"));	
}

zframe_t *right(char */*name*/)
{
	return (zframe_from("Right OK"));	
}

zframe_t *left(char */*name*/)
{
	return (zframe_from("Left OK"));
}

zframe_t *looking(char */*name*/)
{
	return (zframe_from("Looking OK"));
}

zframe_t *gather(char */*name*/)
{
	return (zframe_from("Gather OK"));
}

zframe_t *watch(char */*name*/)
{
	return (zframe_from("Watch OK"));
}

zframe_t *attack(char */*name*/)
{
	return (zframe_from("Attack OK"));
}

zframe_t *selfid(char */*name*/)
{
	return (zframe_from("Selfid OK"));
}

zframe_t *selfstats(char */*name*/)
{
	return (zframe_from("Selfstats OK"));
}

zframe_t *inspect(char *name)
{
	return (zframe_from("Inspect OK"));
}

zframe_t *next(char */*name*/)
{
	return (zframe_from("Next OK"));
}

zframe_t *jump(char */*name*/)
{
	return (zframe_from("Jump OK"));
}
