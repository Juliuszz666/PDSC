#include "primlib.h"
#include <stdlib.h>
#include <unistd.h>
#include "pieces.h"

int main(int argc, char* argv[])
{
	if (gfx_init()) {
		exit(3);
	}
	
	return 0;
}
