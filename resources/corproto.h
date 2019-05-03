/* $Id: $ */
/*

  Copyright (C) 2005 Rafael Gonzalez (astrorafael@yahoo.es)

  This library is free software; you can redistribute it and/or
  modify it under the terms of the GNU Lesser General Public
  License as published by the Free Software Foundation; either
  version 2.1 of the License, or (at your option) any later version.
  
  This library is distributed in the hope that it will be useful,
  but WITHOUT ANY WARRANTY; without even the implied warranty of
  MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the GNU
  Lesser General Public License for more details.
  
  You should have received a copy of the GNU Lesser General Public
  License along with this library; if not, write to the Free Software
  Foundation, Inc., 59 Temple Place, Suite 330, Boston, MA  02111-1307  USA

*/


#ifndef COR_PROTOCOL_H
#define COR_PROTOCOL_H

#include "utils.h"		/* basic typedefs */

#pragma pack(1)			/* structure byte alignment */


#define COR_PROTOCOL_VERSION 21 /* version 2.1 */
#define UDP_BUF_SIZE 5000	/* WARNING: match the same constant from COR */
#define UDP_PORT     1236	/* local AND remote port used */

/* peripheal/message enumeration */

#define PERI_COR        0	/* status and keep alive message*/
#define PERI_SER_A      1	/* Serial port A. RCM2110 Console */
#define PERI_SER_B      2	/* Serial Port B. Usually LX200 telescope */
#define PERI_SER_C      3	/* Serial Port C. Usually CFS */
#define PERI_SER_D      4	/* Serial port D. Weather Station */
#define PERI_POWER     10	/* Parallel port A. Power control */
#define PERI_CCD1      30	/* CCD 1 (ppal) */
#define PERI_CCD_END1  31       /* message, end of image data CCD 1 */
#define PERI_CCD2      32	/* CCD 2 (guide) */
#define PERI_CCD_END2  33       /* message, end of image data CCD 2 */
#define PERI_PRU       50	/* test message */
#define PERI_RESET     60	/* this message resets the COR  */

/* peripheals as strings */
/* These strings appear as additional parameters in the config.xml file */

BEGIN_C_DECLS

extern const char* PERI_SER_A_STR; /* exactly "PERI_SER_A" */
extern const char* PERI_SER_B_STR; /* exactly "PERI_SER_B" */
extern const char* PERI_SER_C_STR; /* exactly "PERI_SER_C" */
extern const char* PERI_SER_D_STR; /* exactly "PERI_SER_D" */
extern const char* PERI_CCD1_STR;  /* exactly "PERI_CCD1"  */
extern const char* PERI_CCD2_STR;  /* exactly "PERI_CCD2"  */

/* handles conversion of PERI_SER_x strings to numbers */
int cor_str2serial(const char* perif);

/* handles conversion of PERI_CCDn strings to numbers */
int cor_str2ccd(const char* perif);

END_C_DECLS


/* message origin */

#define ORIGIN_PC  0       /* PC -> COR */
#define ORIGIN_COR 1       /* COR -> PC */


/**************************************************************************/
/*                     OUTGOING MESAGES TO COR                            */
/**************************************************************************/

/*
 * message specific structures. 
 * peripheal and source fields will appear in an outer structure
 *
 * NOTA: ASCIIZ = null terminated ASCII string
 */

/* ------------------------------------------------ */
/* -- CONNECTION REQUEST MESSAGE - SPECIFIC PART -- */
/* ------------------------------------------------ */

#define MSG_REQUEST   "Solicitando conexion COR" /* used verbatim */

typedef struct {
  char ipCOR[16];		/* ASCIIZ, COR's new IP */
  char ipPC[16];		/* ASCIIZ, PC's IP (from) */
  char maskCOR[16];		/* ASCIIZ, COR mask */
  char request[25];		/* ASCIIZ, with strlen(MSG_REQUEST) */
} Connect_Req_Msg;


/* ---------------------------------------- */
/* -- KEEP ALIVE MESAGE - SPECIFIC PART --  */
/* ---------------------------------------- */


#define MSG_CONT "Continuar conexion COR" /* used verbatim */

/* Power control bit definitions */

#define PWR_CCD     1	/* CCD & CFS devices */
#define PWR_TELES   2	/* power Telescope */
#define PWR_SERVER  4	/* PC Servidor web */
#define PWR_HEATER  8	/* Anti-dew heaters */
#define PWR_DOME   16	/* dome or roll-off open/close */

typedef struct {
  char manten[23]; 	/* cadena ASCIIZ, con cadena MSG_CONTINUACION */
  u_char powerStatus;	/* power status bits */
} KeepAlive_Req_Msg;



/* ---------------------------------------------- */
/* -- POWER ACTIVATION MESSAGE - SPECIFIC PART -- */
/* ---------------------------------------------- */

typedef struct {
  u_char relays;	/* 8 bits, one for each relay */
} Power_Req_Msg;


/* ----------------------------------------------------- */
/* -- RELY MESSAGE TO COR SERIAL PORT - SPECIFIC PART -- */
/* ----------------------------------------------------- */

#define MAX_PERI_LEN  32

typedef struct {
  u_char data[MAX_PERI_LEN];  /* data array to relay to serial port */
} Peripheal_Req_Msg;


/* -----------------------------_----------- */
/* -- CCD IMAGING MESSAGE - SPECIFIC PART -- */
/* ----------------------------------------- */

typedef struct {
  int16 cols;              /* sensor physical columns */
  int16 rows;              /* sensor physical rows */
  u_char binning;          /* normal=1,2,3 o 4. Test image: 11, 12, 13, 14 */
  u_int16 tMsecExp; /* exposure time: milliseconds */
  u_int16 tSecExp;  /* exposure time: seconds */
  int16 x1;                /* frame origing coords */
  int16 y1;
  int16 x2;                /* frame ending coords, excluding */
  int16 y2;
  u_char cancel;           /* flag 1= cancel. Readout cannot be cancelled */
  u_char nClear;           /* number of cleanning passes before exposure */
  u_char shutterMode;      /* obturation mode
			    0=normal, 1=inverted, 2=inactive L, 3=inactive H */
  u_char delay;            /* shutter delay in 1/10 sec */
  u_char readSeqLen;       /* readout sequence effective length */
  u_char readSeq[64];      /* readoutsequence, max 64 bytes */
  u_char clearSeqLen;      /* clearing sequence effective length */
  u_char clearSeq[64];     /* clearing sequence */
  u_char VSeqLen;          /* vertical sequence effective length */
  u_char VSeq[16];         /* vertical sequence */
} Image_Req_Msg;

/* -----------------------------_----------- */
/* -- CCD IMAGING MESSAGE - SPECIFIC PART -- */
/* ----------------------------------------- */

typedef struct {
  u_char dummy;		/* not used. Header is enough to cause reset */
} Reset_Req_Msg;


/******************************/
/* OUTGOING MESSAGE STRUCTURE */
/******************************/

typedef struct {
  u_char peripheal;	/* peripheal defined in the enumertaion above */
  u_char origin;		/* message origin */
} Header;

typedef struct {
  Header header;
  union {
    Connect_Req_Msg   conReq;
    KeepAlive_Req_Msg keepReq;
    Peripheal_Req_Msg periReq;
    Image_Req_Msg     imageReq;
    Power_Req_Msg     powerReq;
    Reset_Req_Msg     resetReq;
  } body;
} Outgoing_Message;

/* handy macro to fix outgoing message length */
#define MSG_LEN(type) (sizeof(Header)+sizeof(type))


/**************************************************************************/
/*                     INCOMING MESAGES FROM COR                          */
/**************************************************************************/



/* --------------------------------------------------------- */
/* "I'm alive" MESSAGE BROADCASTED BY COR - SPECIFIC PART -- */
/* --------------------------------------------------------- */

typedef struct {
  u_char relays;   /* relay status byte, 1 bit per relay */
  u_char group[4];  /* four group of 6 input each */
} Pwr_Status;

typedef struct {
  int16 ccd;			/* CCD temperature (cold finger) */
  int16 box;			/* hot side peltier temperature */
  int16 VPelt;			/* voltage applied to peltier */
} CCD_Temp;

#define MENS_ALIVE "COR presente"

typedef struct {
  char response[24];  /* keeps MENS_ALIVE or MENS_CONN_RESP */
  u_char step;			/* = SEQ_KCOL.  */
  char padding1[5];		/* padding. */
  Pwr_Status power;		/* power control status */
  CCD_Temp temp;		/* CCD temperature status */
  u_char padding2[9];		/* padding */
  char compDate[12];	 /* ASCIIZ compilation date. Format "Mnn dd yyyy"  */
  char compTime[9];	 /* ASCIIZ compilation time. Format "HH:MM:SS"  */
  char firmware[9];	 /* ASCIIZ firmware name*/
} KeepAlive_Msg;


/* ---------------------------------------------- */
/* -- CONECT CONFIRM FROM COR  - SPECIFIC PART -- */
/* ---------------------------------------------- */

#define MENS_CONN_RESP "COR conectado"

typedef KeepAlive_Msg Connect_Conf_Msg;


/* --------------------------------------------------- */
/* -- KEEP ALIVE RESPONSE FROM COR  - SPECIFIC PART -- */
/* --------------------------------------------------- */

/* identical to connect confirm for the time being */
typedef Connect_Conf_Msg KeepAlive_Resp_Msg;


/* ------------------------------------------------------- */
/* -- RELAY MESAGE FROM COR SERIAL PORT - SPECIFIC PART -- */
/* ------------------------------------------------------- */


typedef struct {
  u_char data[MAX_PERI_LEN];  /* data array from COR to PC */
} Peripheal_Resp_Msg;


/* ---------------------------------------------------------- */
/* -- RESPONSE FROM COR WITH POWER STATUS -- SPECIFIC PART -- */
/* ---------------------------------------------------------- */


typedef Pwr_Status Power_Resp_Msg;


/* ------------------------------------------------ */
/* -- IMAGE RESPONSE WITH DATA -- SPECIFIC PARTS -- */
/* ------------------------------------------------ */

/* image header lenght in bytes */
#define IMG_HEAD (sizeof(Header)+3*sizeof(int16))

/* trimmed message length */
#define MAX_IMG_LEN  (UDP_BUF_SIZE-IMG_HEAD)   


typedef struct {
  int16 msgNum;			/* image message sequence number */
  int16 rowNum;			/* rows per UDP message */
  int16 colNum;			/* number of columns per row */
  u_char data[MAX_IMG_LEN];
} Image_Data_Msg;

/* --------------------------------------------------------- */
/* - IMAGE FINAL MESSAGE WITH TIMESTAMPS  - SPECIFIC PARTS - */
/* --------------------------------------------------------- */

typedef struct {
  u_int32 expTime;		/* start exposure timestamp (msec.) */
  u_int32 readTime;		/* end exposure timestamp (msec.) */
  u_int32 endTime;		/* end readout timestamp (msec.) */
} Image_End_Msg;

/******************************/
/* INCOMING MESSAGE STRUCTURE */
/******************************/

typedef struct {
  Header header;
  union {
    KeepAlive_Msg      keepAlive;
    Connect_Conf_Msg   connConf;
    KeepAlive_Resp_Msg keepResp;
    Peripheal_Resp_Msg periResp;
    Power_Resp_Msg     powerResp;
    Image_Data_Msg     imgData;
    Image_End_Msg      imgEnd;
  } body;
} Incoming_Message;


#pragma pack(0)			/*  alineamiento por defecto */

#endif


