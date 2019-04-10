#include <stdio.h>
#include <stdlib.h>
inline void Fun_inlin(void)
{
    {

       break ;
    }
}
int main()
{

    for(int i = 0 ; i < 10 ; i++)
    {
        Fun_inlin();
    }
    return 0;
}
