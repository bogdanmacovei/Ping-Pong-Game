Joc Ping-Pong Arduino
Bogdan Macovei

Demo: https://drive.google.com/open?id=1sxdMK1WEqhOf5vkKn3pnC7O5RyKOWWhR
Elemente principale utilizate
Pentru realizarea jocului am folosit urmatoarele elemente principale, conectate la placuta Arduino UNO:
-	matrice LED-uri 8x8;
-	driver Max7219;
-	2 x joyStick;
-	afisaj LCD;
-	buton.
Descrierea jocului
1.	Jocul porneste in urma apasarii unui buton, iar pana in acel moment, matricea de LED-uri va fi stinsa, activ fiind doar afisajul LCD care contine scorul 0 pentru ambii jucatori.
2.	In momentul apasarii butonului, fiecare isi poate controla paleta prin intermediul joyStick-ului. Acesta lucreaza doar pe axa Y, o paleta avand 3 LED-uri, iar axele X sunt constante (0 pentru prima paleta, 7 pentru cea de-a doua).
3.	Daca un jucator rateaza mingea, atunci celalalt jucator primeste 1p in tura respectiva.
4.	In primele 10 astfel de ture viteza creste treptat, iar apoi se stabilizeaza pe o valoare constanta.
5.	Daca un jucator ajunge la 10 puncte, atunci el este desemnat castigator, iar jocul se reseteaza (punctajele redevin 0, continutul matricii este sters, iar programul asteapta reapasarea butonului pentru configurarea paletelor). Pana la resetarea propriu-zisa, pe LCD apare un mesaj de forma “Palayer1 a castigat” sau “Player2 a castigat”, in functie de caz.
Implementarea soft
Jocul Ping-Pong este controlat in totalitate de o clasa numita pingPongGame, care admite urmatoarele metode:
pingPongGame(paleta1, paleta2, minge, sensDeplasare, scor, viteza): 
-	este un constructor cu valori implicite;
-	seteaza configuratiile initiale pentru pornirea corecta a jocului.
void setPaddle (paleta1, paleta2, mod):
-	seteaza ambele palete in functie de argumentul mod, care poate fi true (ambele palete au LED-urile aprinse) sau false.
void modifyPaddles (paleta1, paleta2):
-	in urma executarii codului, modifica in clasa noile pozitii ale paletelor la un moment de timp.
void writeScore (scor):
-	actualizeaza scorul pe ecranul LCD-ului.
void reset ():
-	reseteaza joculul intre ture (dupa ce un jucator pierde, dar niciunul nu are inca 10p).
void generalReset ():
-	reseteaza jocul in momentul in care unul dintre jucatori a acumulat 10p.
int xyDirection (arg) si int xyTransform (arg):
-	sunt doua functii care interpreteaza modul in care se deplaseaza o paleta in functie de semnalul transmis de joyStick;
-	stiuta o pozitie (x, y) anterioara, noile valori sunt calculate astfel:
a.	xyDirection (arg), unde arg este valoarea indicata de joyStick, returneaza -1, 0, 1 dupa cum paleta trebuie sa se deplaseze sus/jos sau sa ramana pe loc.
b.	xyTransform (arg) se asigura de faptul ca paleta nu poate “sa iasa” din matrice, ea ramane mereu cu cele 3LED-uri in interiorul spatiului de joc.
void currentAction ():
-	are rolul de a actualiza vizual pozitiile paletelor si ale mingii, cat si de a modifica pozitia mingii in functie de doi versori de deplsare.
void currentLogic ():
-	are rolul de a actualiza in memorie pozitiile paletelor si ale mingii si de a lua deciziile din program, precum:
a.	mingea atinge suprafata unei palete;
b.	mingea rateaza o paleta;
c.	mingea are o coliziune cu un perete pe care trebuie sa ricoseze;
d.	un jucator a acumulat 10p.
