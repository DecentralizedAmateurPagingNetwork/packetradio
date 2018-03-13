Support for Packet-Radio Mailbox Gateway for notification on DAPNET
Source of files: http://download.prgm.org/boxfiles/funkruf/

Infos about the files and their function from Marc-André:
````
> #define Mailinfo   "/usr/local/funkruf/lib/mailinfo"
> auf sich hat. Braucht man das? Schreibt da die Box was rein? Wenn ja, muss man das irgendwo anders hinlegen.

So sieht die aus:

root@db0uhi:/usr/local/funkruf/lib# cat mailinfo
DG8OP
DO1OFA
DO1OFM
DO1OFR
DO1OIM
DG9OBU
DJ2BF
...

Da sind alle Rufzeichen drin, die in der Funkruf-DB sind UND deren
MyBBS, also die Heimatbox, sich hier befindet. Er entscheidet mit
Hilfe dieser Liste ob er einen Funkruf erzeugt oder nicht, denn das
macht er nur für lokale User. Grund ist, weil "m_filter.prg" von der
BCM für jede eintreffende Mail, also auch die im Forward (!) aufgerufen
wird, und wir wollen keine Funkrufe für User erzeugen, die nicht hier
ihre MyBBS haben.

Ich lasse die BCM einmal am Tag per BCM-Cron ihre User herausschreiben:

root@db0uhi:/bcm# cat userlst.imp
export -u /bcm/userlist.lst u @=db0uhi

Das lasse ich auch beim Funkrufmaster machen, der kann seine DB in
Textform dumpen. Rufzeichen, die in beiden Listen vorkommen, wandern
in die mailinfo-Datei.

Ich habe das Zeug auf UHI gefunden, muss es nur mal herunterladen, ich
schicke das dann noch nach. Es ist ein bash-Script das den Abgleich
vornimmt und mailinfo generiert, nichts wildes, nur ein paar verschachtelte greps.
````
