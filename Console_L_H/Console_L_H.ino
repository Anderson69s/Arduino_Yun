/*
  Console Pixel

 An example of using the Arduino board to receive data from the
 Console on the Arduino Yún.  In this case, the Arduino boards turns on an LED when
 it receives the character 'H', and turns off the LED when it
 receives the character 'L'.

 To see the Console, pick your Yún's name and IP address in the Port menu
 then open the Port Monitor. You can also see it by opening a terminal window
 and typing
 ssh root@ yourYunsName.local 'telnet localhost 6571'
 then pressing enter. When prompted for the password, enter it.


 The circuit:
 * LED connected from digital pin 13 to ground

 created 2006
 by David A. Mellis
 modified 25 Jun 2013
 by Tom Igoe

 This example code is in the public domain.

 http://arduino.cc/en/Tutorial/ConsolePixel

Modified and translated by Anderson69s 12/2014
 */


#include <Console.h> //On inclut la librarie Console qui est l'équivalent de Serial mais en Wifi...

const int led = 13; //On définit une constance entière pour nommer le pin de la led
char valeur_entre;// On crée un variable pour stocker les valeurs que l'on enverra via la console

void setup() {
  //Initialisation de la console et attente de l'ouverture du port par défaut : 5555
  Bridge.begin();//On démarre Bridge
  Console.begin();//On démarre la console
  pinMode(led, OUTPUT);//On définit le pin 13 en sortie

  while (!Console) {
    ; // On attend que la console se connecte
  }   
}

void loop() {
    Console.println("Hello World!!! Tapez H ou L pour jouer avec la led 13 :-)"); //On imprime un petit message de bienvenue
    valeur_entre = Console.read();//On lit le buffer de la console pour vérifier le dernier byte écrit
    Console.println(valeur_entre);//On affiche la valeur si on en envoie une
    if (valeur_entre == 'H') {//Si un H (ASCII 72) est reçut
      digitalWrite(led, HIGH);//On allume la led
    }
    
    if (valeur_entre == 'L') {//Si un L (ASCII 76) est reçut
      digitalWrite(led, LOW);//On éteint la led
    }
 }
