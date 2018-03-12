/*********************************************************************/
/* Dieses Programm durchsucht bein empfangen einer Mail ob der User  */
/* im mailinfo-File steht und informiert ihn dann ueber das          */
/* Eintreffen einer neuen PR-Mail                                    */
/* Ausserdem wird die Rubrik P gesondert behandelt                   */
/* => DF3VI POCSACLISTSERVER in der BCM                              */
/* unbedingt m_filter.h anpassen ! ! !                               */
/*                                                                   */
/* Stand 20.11.2002   DH6BB                                          */
/* Bei Fragen oder Problemen:                                        */
/* PR    : dh6bb@db0whv.#nds.deu.eu                                  */
/* e-Mail: dh6bb@darc.de                                             */
/*                                                                   */
/* Dez. 2001  DH6BB  Neucodierung in C (aus Perl).                   */
/* 24.12.2001 DH6BB  Anpassung an DP-Box.                            */
/* 02.01.2001 DH6BB  Rubrik-P-Mails die nicht fuer Boxcall sind	     */
/*		     werden jetzt ignoriert.			     */
/* 18.03.2002 DH6BB  Beseitigung einiger Compiler-Warnings.          */
/* 18.04.2002 DH6BB  Problem mit der DP-Box beseitigt.               */
/*                   Empfaenger-Feld lief ueber.                     */
/* 18.11.2002 DH6BB  Es wird jetzt nur noch eine Import-Datei fuer   */
/*                   das neue Funkruf-User-Interface generiert.      */
/*                   Es erfolgt kein Datenbank-Zugriff mehr.         */
/* 18.11.2002 DH6BB  Es koennen Kepler-Files automatisch kopiert     */
/*                   werden, wenn diese im Funkruf-User-Interface    */
/*                   benoetigt werden.                               */
/* 20.11.2002 DH6BB  Es wird jetzt beim Kepler-Filer auch die Rubrik */
/*                   mit ausgewertet.                                */
/* 15.10.2006 DH6BB  OpenBCM 1.07b1 hat das Format der Mail ein      */
/*                   klein wenig geaendert. Tnx Info DG8NGN/DL9SAU   */
/* 12.03.2018 DH3WR  Started with DAPNET support implementation      */
/*********************************************************************/

#include "m_filter.h"

FILE *InFile;	// global, die "Mail"
 char TITEL[80]="\0";
 char AT[80]="\0";
 char FROM[80]="\0";
 char TO[80]="\0";
 char TEXT[80]="\0";
 char *Dateiname;


//********************************************************************
// Mailerkennung
//********************************************************************
void Mailerkennung()
{
 int i=0, a=0;             // Zaehler
 int gefunden=0;           // Schluesselwoerter gefunden?
 int Ende=50;              // Max. durchlaeufe
 char puffer[4096];         // puffer fuer Zeilen einlesen
 char Empfaenger[10]="\0"; // Empfaenger
 char ZielBox[10]="\0";	   // Ziel-Mailbox
 char Absender[10]="\0";   // Absender
 char Titel[300]="\0";     // Titel
#ifdef KEPLER
 char KepText[300]="\0";
#endif 
 
#ifdef DPBOX
//#########################################################################
//# Hier die Mailerkennung fuer die DP-Box                                #
//######################################################################### 
 fgets(puffer,300,InFile);                  // letzte Box

 fgets(puffer,300,InFile);                  // Absender
 puffer[strlen(puffer)-1]='\0';
 puffer[strcspn(puffer, ".")]='\0';	    // bei DH6BB.#NDS.DEU.EU trennen
 strncpy(Absender, puffer, strlen(puffer));

 fgets(puffer,300,InFile);                  // Empfaenger
 puffer[strlen(puffer)-1]='\0';
 puffer[strcspn(puffer, ".")]='\0';	    // bei DH6BB.#NDS.DEU.EU trennen
 strncpy(Empfaenger, puffer, strlen(puffer));

 fgets(puffer,300,InFile);                  // Ziel-Mailbox
 puffer[strlen(puffer)-1]='\0';
 puffer[strcspn(puffer, ".")]='\0';	    // bei DH6BB.#NDS.DEU.EU trennen
 strncpy(ZielBox, puffer, strlen(puffer));
 
 fgets(puffer,300,InFile);                  // LifeTime
 fgets(puffer,300,InFile);                  // BID

 fgets(puffer,300,InFile);                  // Titel
 puffer[strlen(puffer)-1]='\0';
 strncpy(Titel, puffer, strlen(puffer));
#endif  //DP-Box

#ifdef BCM
//#########################################################################
//# Die BayCom Mailbox hat einen ganz anderen Header-Aufbau.              #
//# Folgendes Beispiel erklaert aber vieles:                              #
//# 1. Zeile:  DH6BB < DH9BHD ........      bedeutet von DG9BHD an DH6BB  #
//# 4. Zeile:  Hallo Jens                   Titel der Nachricht           #
//#########################################################################

 fgets(puffer,300,InFile);                  // 1. Zeile
 strncpy(Empfaenger, puffer, strcspn(puffer, " "));
 Empfaenger[strcspn(puffer, " ")]='\0';    // 1. Zeile Empfaenger
 do { i++; }while (puffer[i] != ' ');
 do { i++; }while (puffer[i] != ' ');
 i++;

 do
 {
  Absender[a]=puffer[i];
  i++; a++;
 } while (puffer[i] != ' ');            // Absender
 

 fgets(puffer,4096,InFile);       // 2. Zeile
 fgets(puffer,300,InFile);       // 3. Zeile
 fgets(puffer,300,InFile);       // 4. Zeile
 strcpy(Titel, puffer);
 strcpy(ZielBox, Boxcall);       // DUMMY !!!
#endif   //BCM

 // Ist die Mail in der Rubrik P?
 if ((strncmp(Titel, "Message for",11)==0 || strncmp(Titel, "Mail for",8)==0) && (strcmp(Empfaenger,"P")==0) && (strcmp(ZielBox, Boxcall)==0))
 {
  do
  {
   fgets(puffer,300,InFile);       // Zeile der Mail einlesen
   Ende--;

   if (strncmp(puffer, "From: ",6)==0)
   {
    gefunden++;
   }

   if (strncmp(puffer, "To: ",4)==0)
   {
    gefunden++;
   }

   if (strncmp(puffer, "TO   ",5)==0)   // Schluesselwort TO
   {
    gefunden++;
    i=6; a=0;
    do
    {
       TO[a]=puffer[i];
       i++; a++;
    } while (puffer[i] != ' ' && puffer[i] != '\0' && puffer[i] != '\n');
   }

   if (strncmp(puffer, "FROM ",5)==0)   // Schluesselwort FROM
   {
    gefunden++;
    i=6; a=0;
    do
    {
       FROM[a]=puffer[i];
       i++; a++;
    } while (puffer[i] != ' ' && puffer[i] != '\0' && puffer[i] != '\n');
   }

   if (strncmp(puffer, "AT   ",5)==0)   // Schluesselwort AT
   {
    i=6; a=0;
    do
    {
       AT[a]=puffer[i];
       i++; a++;
    } while (puffer[i] != '.' && puffer[i] != ' ' && puffer[i] != '\0' && puffer[i] != '\n');
   }

   if (strncmp(puffer, "TITEL ",5)==0)  // Schluesselwort TITEL
   {
    i=6; a=0;
    do
    {
       TITEL[a]=puffer[i];
       i++; a++;
    } while (puffer[i] != '\0' && puffer[i] != '\n');
   }

   if (strncmp(puffer, "TEXT ",5)==0)   // Schluesselwort TEXT
   {
    gefunden++;
    i=0; a=0;
    i +=6;
    do
    {
       TEXT[a]=puffer[i];
       i++; a++;
    } while (puffer[i] != '\0' && puffer[i] != '\n');
   }

  } while (Ende!=0 && !feof(InFile) && gefunden < 5);
 }

#ifdef KEPLER 
 if ((strncmp(Titel, KeplerTitel,strlen(KeplerTitel))==0)
   &&(strncmp(Absender, KeplerCall,strlen(KeplerCall))==0)
   &&(strncmp(Empfaenger, KeplerRubrik,strlen(KeplerRubrik))==0))
 {
    sprintf(KepText, "cp %s %s", Dateiname, Keplerfile);
    if(system(KepText)!=0)
    {
#ifdef DEBUG
	printf("Fehler beim Uebertragen des Kepler-Files\n");
#endif	
    }
 }
#endif

 if (strncmp(Titel, "Mail for",8)==0 && strcmp(Empfaenger, "P")==0) Funkruf(1);
 else if (strncmp(Titel, "Message for",11)==0 && strcmp(Empfaenger, "P")==0) Funkruf(2);
 else
 {
   strcpy(TO, Empfaenger);
   strcpy(FROM, Absender);
   strcpy(TITEL, Titel);
   Funkruf(0);
 }
}


//********************************************************************
// Mail Aufbereiten
//********************************************************************
void Funkruf(int MailorMess)
{
 FILE *MailDatei;
 int gefunden=0;
 char puffer[80]="\0";
 char NeuTEXT[100]="\0";

  if (MailorMess==0)     // Mail direkt
  {
   if ((MailDatei=fopen(Mailinfo,"r")) != NULL)
   {
     do
     {
        fgets(puffer,80,MailDatei);
        puffer[strlen(puffer)-1]='\0';
	if (strcmp(puffer, TO)==0)
	    gefunden=1;
     }while (!feof(MailDatei) && gefunden!=1);
     fclose(MailDatei);
     if (gefunden==0) return;
   }
   else
   {
#ifdef DEBUG 
    printf("Kann Mailinfo-Datei nicht oeffnen: %s\n", Mailinfo);
#endif
    return;
   }
  }
  
    if (MailorMess==0)     // Mail direkt
    {
       strcpy(NeuTEXT,"Neue Mail in ");
       strcat(NeuTEXT,Boxcall);
       if (strlen(NeuTEXT)<20) strcat(NeuTEXT," ");
       strcat(NeuTEXT, FROM);
       strcat(NeuTEXT, ":");
       strcat(NeuTEXT, TITEL);
       Sende(NeuTEXT);
    }

    if (MailorMess==1)     // Mail aus P
    {
       strcpy(NeuTEXT,"Neue Mail in ");
       strcat(NeuTEXT,AT);
       if (strlen(NeuTEXT)<20) strcat(NeuTEXT," ");
       strcat(NeuTEXT, FROM);
       strcat(NeuTEXT, ":");
       strcat(NeuTEXT, TITEL);
       Sende(NeuTEXT);
    }

    if (MailorMess==2)     // Message aus P
    {
       strcpy(NeuTEXT, FROM);
       strcat(NeuTEXT,":");
       strcat(NeuTEXT,TEXT);
       Sende(NeuTEXT);
    }
}


//********************************************************************
// Generiere die Funkruf-Datei
//********************************************************************
void Sende(char FunkrufText[])
{
  # Example:
  # curl -H "Content-Type: application/json" -X POST -u dl1acb:sehrgeheimespasswort -d '{ "text": "DL1ABC: Das ist eine Sendung über die REST-API", "callSignNames": ["dh1xyz"], "transmitterGroupNames": ["dl-bw"], "emergency": false }' http://dapnet.afu.rwth-aachen.de/api/calls
  # Maybe nicer implementation with https://curl.haxx.se/libcurl/c/example.html
  
  # Buffer for command line command
  char curl_command[2000]="\0";
  
  # Build up the curl command and add dynamic content
  strcpy(curl_command, "curl -H \"Content-Type: application/json\" -X POST -u ");
  strcat(curl_command, DAPNET_NAME);
  strcat(curl_command, ":");
  strcat(curl_command, DAPNET_PASSWORD);
  strcat(curl_command, " -d '{ \"text": \"");
  strcat(curl_command, TO);
  strcat(curl_command, ": ");
  strcat(curl_command, FunkrufText);
  strcat(curl_command, "\", \"callSignNames\": [\"");
  strcat(curl_command, tolower(TO);
  strcat(curl_command, "\"], \"transmitterGroupNames\": [\"all\"], \"emergency\": false }' ");
  strcat(curl_command, DAPNET_URL);

  # Execute the command
  system(curl_command);
}


//********************************************************************
// Das Hauptprogramm
//********************************************************************
int main(int argc[], char *argv[])
{
 if ((InFile=fopen(argv[1],"r")) == NULL) 
 {
#ifdef DEBUG 
   printf("Datei %s konnte nicht geoeffnet werden\n", argv[1]);
#endif   
   return(0);
 }
 else
 {
    Dateiname=argv[1];
    Mailerkennung();
    fclose(InFile);
    return(0);
 }
}
// Ende von m_filter.cpp
