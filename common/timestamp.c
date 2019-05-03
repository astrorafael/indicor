#include <time.h>
#include "timestamp.h"


/*---------------------------------------------------------------------------*/

const char* 
timestamp()
{
  static char ts[20];
  struct tm *tp;
  time_t t;
  
  time (&t);
  tp = gmtime (&t);
  strftime (ts, sizeof(ts), "%Y-%m-%dT%H:%M:%S", tp);
  return (ts);
}

/*---------------------------------------------------------------------------*/

int 
extractISOTime(const char *timestr, struct tm *utm)
{
  
  if (strptime(timestr, "%Y-%m-%dT%H:%M:%S", utm))
    return (0);
  if (strptime(timestr, "%Y/%m/%dT%H:%M:%S", utm))
    return (0);
  if (strptime(timestr, "%Y:%m:%dT%H:%M:%S", utm))
    return (0);
  if (strptime(timestr, "%Y-%m-%dT%H-%M-%S", utm))
    return (0);
  
  return (-1);
  
}

/*---------------------------------------------------------------------------*/


