/*
  Arduino Yun Bridge example

 This example for the Arduino Yun shows how to use the
 Bridge library to access the digital and analog pins
 on the board through REST calls. It demonstrates how
 you can create your own API when using REST style
 calls through the browser.

 Commande possible grace à ce sketch:

 * "/arduino/digital/13"     -> digitalRead(13)
 * "/arduino/digital/13/1"   -> digitalWrite(13, HIGH)
 * "/arduino/analog/2/123"   -> analogWrite(2, 123)
 * "/arduino/analog/2"       -> analogRead(2)
 * "/arduino/mode/13/input"  -> pinMode(13, INPUT)
 * "/arduino/mode/13/output" -> pinMode(13, OUTPUT)

Cet exemple de code est open-source.

http://arduino.cc/en/Tutorial/Bridge

Modified and translate by Anderson69s
 */

#include <Bridge.h>//On démarre la librarie Bridge pour communiquer entre Linino et atmega32u4
#include <YunServer.h>//On démarre la librarie YunServer qui démarre le serveur coté linino
#include <YunClient.h>//On démarre la librarie YunClient qui démarre le serveur web coté linino

YunServer server;// On écoute sur le port 5555 (par défaut) pour le webserver

void setup() {
  pinMode(13, OUTPUT);//on définit le port 13 en sortie
  digitalWrite(13, LOW);//on éteint la led 13
  Bridge.begin();// On démarre Bridge
  digitalWrite(13, HIGH);//On éclaire la led 13 quand bridge est lancé

  server.listenOnLocalhost(); //On n'accepte que les connexions venant du meme réseau
  server.begin();//On lance le seveur
}

void loop() {
  YunClient client = server.accept();//On accepte les connexions
  if (client) {//Si il y a un nouveau client
    process(client);//On lance la fonction process qui permet de gérer les requetes du client

    client.stop();//Sinon on ferme la connexion et on libère des ressources
  }

  delay(50); // On fait cela toutes les 50 millisecondes
}

void process(YunClient client) {//Fonction process
  String command = client.readStringUntil('/');//On lit la commande que le client envoie en la découpant à chaque "/"

  if (command == "digital") {//Si c'est une commande digital
    digitalCommand(client);//On lance la fonction digital
  }

  if (command == "analog") {//Si c'est une commande analog
    analogCommand(client);//On lance la fonction analog
  }

  if (command == "mode") {//Si c'est une commande mode pour régler un port en entrée ou sortie
    modeCommand(client);//On lance la fonction mode
  }
}

void digitalCommand(YunClient client) {//Fonction digital
  int pin, value;//on crée deux variables pin et value

  pin = client.parseInt();//On lit la valeur envoyé par le client et on la décompose

  // Si le caractère suivant est un "/", cela veut dire que nous avons une url
  // du type : "/digital/13/1" c'est dans le cas ou le port est définit en sortie
  if (client.read() == '/') {
    value = client.parseInt();
    digitalWrite(pin, value);
  }
  else {//Sinon on lit la valeur digital
    value = digitalRead(pin);
  }

  //On envoie des indications au client pour dire que tout va bien
  client.print(F("Pin D"));
  client.print(pin);
  client.print(F(" regle a "));
  client.println(value);

  // On met à jour la valeur entrée dans la mémoire du linino
  String key = "D";
  key += pin;
  Bridge.put(key, String(value));
}

void analogCommand(YunClient client) {//Fonction analog
  int pin, value;//on crée deux variables pin et value

  pin = client.parseInt();//On lit la valeur envoyé par le client et on la décompose

  // Si le caractère suivant est un "/", cela veut dire que nous avons une url
  // du type : "/digital/9/150" c'est dans le cas ou le port est définit en sortie
  if (client.read() == '/') {
    value = client.parseInt();//On lit et on applique la commande
    analogWrite(pin, value);

    //On envoie des indications au client pour dire que tout va bien
    client.print(F("Pin D"));
    client.print(pin);
    client.print(F(" regle a "));
    client.println(value);

    // On met à jour la valeur entrée dans la mémoire du linino
    String key = "D";
    key += pin;
    Bridge.put(key, String(value));
  }
  else {//Sinon on lit la valeur analog
    value = analogRead(pin);

    //On envoie des indications au client pour dire que tout va bien
    client.print(F("Pin A"));
    client.print(pin);
    client.print(F(" lit un analog de "));
    client.println(value);

    // On met à jour la valeur entrée dans la mémoire du linino
    String key = "A";
    key += pin;
    Bridge.put(key, String(value));
  }
}

void modeCommand(YunClient client) {//Fonction mode
  int pin;//on crée deux variables pin et value
  pin = client.parseInt();//On lit le numéro du pin et on le décompose

  // Si le caractère suinant n'est pas un "/" nous avons une url mal entrée
  if (client.read() != '/') {
    client.println(F("erreur : mauvaise url"));
    return;
  }

  String mode = client.readStringUntil('\r');//On définit une string mode qui lit les valeurs du client

  if (mode == "input") {//Si le mode est input
    pinMode(pin, INPUT);
    //On envoie des indications au client pour dire que tout va bien
    client.print(F("Pin D"));
    client.print(pin);
    client.print(F(" configure en INPUT !"));
    return;
  }

  if (mode == "output") {//Si le mode est output
    pinMode(pin, OUTPUT);
    //On envoie des indications au client pour dire que tout va bien
    client.print(F("Pin D"));
    client.print(pin);
    client.print(F(" configure en OUTPUT !"));
    return;
  }
 //Sinon on envoie un message d'erreur :
  client.print(F("erreur: mode invalide "));
  client.print(mode);
}

