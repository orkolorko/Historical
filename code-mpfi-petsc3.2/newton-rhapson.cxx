#include "mpfi.h"


int main(int argc, char *argv[])
{
	double temp(17);
	{
	mpfi_t x, y;
	mpfi_init (x); /* use default precision */
	mpfi_init2 (y, 256); /* precision exactly 256 bits */
	mpfi_clear (x);
	mpfi_clear (y);
	}

	return 0;
}
