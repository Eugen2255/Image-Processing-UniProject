#include "functions.h"

int get_coords(int cord, int max_len)
{
    if(cord < 0)
        return 0;
    else if (cord >= max_len)
        return max_len - 1;
    else 
        return cord;
}