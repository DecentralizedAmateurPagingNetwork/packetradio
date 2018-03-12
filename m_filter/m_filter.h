/***************************************************************/
/* m_filter.prg zum Funkruf-Server                             */
/* Stand 15.10.2006  DH6BB                                     */
/***************************************************************/

#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <libgen.h>

//#define DEBUG
//#define BCM                   // <== Fuer die BayBox die "//" entfernen!!
#define Boxcall    "DB0WHV"	// <== A E N D E R N !!!  (ohne SSID)
#define Mailinfo   "/usr/local/funkruf/lib/mailinfo"

#DAPNET
#define DAPNET_URL			http://dapnet.afu.rwth-aachen.de/api/calls
#define DAPNET_NAME			bcm
#define DAPNET_PASSWORD		

#define KEPLER
#ifdef KEPLER
 #define KeplerCall   "CX2SA"
 #define KeplerRubrik "KEPS"
 #define KeplerTitel  "2Line"
 #define Keplerfile   "/usr/local/funkruf/lib/kepler.txt"
#endif

//********* Ab hier nichts mehr aendern !!! *************
#define Version    0.26

#ifndef BCM
 #define DPBOX
#endif

void Mailerkennung(void);
void Funkruf(int);
void Sende(char [100]);
