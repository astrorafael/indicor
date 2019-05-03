#include <string.h>
#include "utils.h"
#include "corproto.h"

 const char* PERI_SER_A_STR = "PERI_SER_A" ;
 const char* PERI_SER_B_STR = "PERI_SER_B" ;
 const char* PERI_SER_C_STR = "PERI_SER_C" ;
 const char* PERI_SER_D_STR = "PERI_SER_D" ;
 const char* PERI_CCD1_STR  = "PERI_CCD1"  ;
 const char* PERI_CCD2_STR  = "PERI_CCD2"  ;

int 
cor_str2serial(const char* port)
{
  int perif;

  if(!strcmp(port,PERI_SER_A_STR))
    perif = PERI_SER_A;
  else if(!strcmp(port,PERI_SER_B_STR))
    perif = PERI_SER_B;
  else if(!strcmp(port,PERI_SER_C_STR))
    perif = PERI_SER_C;
  else if(!strcmp(port,PERI_SER_D_STR))
    perif = PERI_SER_D;
  else
    perif = -1;

  return(perif);
}

int 
cor_str2ccd(const char* ccd)
{
  int perif;
 
  if(!strcmp(ccd,PERI_CCD1_STR))
    perif = PERI_CCD1;
  else if(!strcmp(ccd,PERI_CCD2_STR))
    perif = PERI_CCD2;
  else
    perif = -1;

 return(perif);
}
