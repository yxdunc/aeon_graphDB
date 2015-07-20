#include "error_handling.h"

void            aeon_error(int error_id)
{
        if (error_id == 0)
        {
                printf("error_id - 0 - : Data base failled to attach");
		exit(0);
        }
	else if (error_id == 1)
        {
                printf("error_id - 1 - : Too many arguments");
		exit(0);
	}
}

