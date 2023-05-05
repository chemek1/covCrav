#include <sys/time.h>
#include "time_functions.h" 

long timeval_to_ms(struct timeval t)
{
    long ms;
    ms = t.tv_sec * 1000; // conversion de secondes en millisecondes
    ms += t.tv_usec / 1000; // ajout de microsecondes converties en millisecondes
    return ms;
}
