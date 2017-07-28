// On commence par inclure les bibliothèques indispensables
#include <ESP8266WiFi.h> // pour le module
#include <MQ135afinal.h> // pour les MQ135

// Pin sur lequel est branché le MQ135
const int mq135Pin_a = A0; // pour le CO2
const int mq135Pin_b = D3; // pour le CO
const int mq135Pin_c = D5; // pour le NH3

// Initialise l'objet MQ135 sur le Pin spécifié 
MQ135_a gasSensora = MQ135_a(mq135Pin_a); // CO2 
MQ135_b gasSensorb = MQ135_b(mq135Pin_b); // CO
MQ135_c gasSensorc = MQ135_c(mq135Pin_c); // NH3

const char* ssid     = "nom du wifi";      // NOM du WIF
const char* password = "mot de passe du wifi";   // Son Mot de passe 

WiFiClient client;     // L'objet qui nous servira à la communication

char servername[]="mems-27-1.appspot.com";  // Le serveur à interroger // byte servername[] = {192, 168, 177, 77};

String result; // pour ce qui sera la réponse de notre serveur 

float CO2;
float CO;
float NH3;

void setup() 
{
  Serial.begin(1000000); // On demarre la voie série pour déboguer : à 1000000 bit/s
  
  Serial.print("On tente de connecter au "); // On essaie de connecter 
  Serial.println(ssid);                       // au wifi qu'on a spécifier plus haut
  WiFi.begin(ssid, password);                 // A l'aide de cette fonction begin()

  while (WiFi.status() != WL_CONNECTED)       // Tant qu'on est pas connecté
  {
    Serial.print(".");                      // On affiche juste "."
    delay(1000);                              // A chaque seconde
  }
  Serial.println("Ca y est, on est connecté au WIFI "); // Ici, on est connecté
  
  /*les lignes suivantes n'ont rien avoir a la connection, 
   * il se contente juste d'afficher dans le moniteur série le R0, R1 et R2
   * Pour le déboguer
  */
     /// R0 pour CO2
   float rzeroO = gasSensora.getRZero();
   Serial.print("R0: ");
   Serial.print(rzeroO);// Valeur à reporter ligne 27 du fichier mq135.h après 48h de préchauffage
   Serial.print("\t\t\t\t");
   
   /// R1 pour CO
   float rzero1 = gasSensorb.getRZero();
   Serial.print("R1: ");
   Serial.print(rzero1);  // Valeur à reporter ligne 27 du fichier mq135.h après 48h de préchauffage
   Serial.print("\t\t\t\t");
   
   /// R2 pour NH3
   float rzero2 = gasSensorc.getRZero();
   Serial.print("R2: ");
   Serial.println(rzero2);   // Valeur à reporter ligne 27 du fichier mq135.h après 48h de préchauffage 

   delay(1000);        // Donne une seconde au module pour s'initialiser 
}

void loop() 
{
 float CO2 = recupererCO2(); // on récupère la valeur retourner par le capteur de C02
 float CO = recupererCO();   //  on récupère la valeur retourner par le capteur de C0
 float NH3 = recupererNH3(); // on récupère la valeur retourner par le capteur de NH3

 /* On envoi les 3 valeurs 
  * qu'on vien de récupérer 
  */
 String CO2String = String(CO2, 1);
 String COString = String(CO, 1);
 String NH3String = String(NH3, 1);
 EnvoyerLesTroisValeursAuServeur(CO2String, COString, NH3String);
 
 delay(4000); // on donne 4 sec au serveur pour inserer ces valeurs
}
/*Définition des fonction de
 * récupération des 3 gazs
 */
float  recupererCO2()
{
    // CO2
    float ppm_a = gasSensora.getPPM();
    Serial.print("A0: ");
    Serial.print(analogRead(mq135Pin_a));
    Serial.print(" CO2: ");
    Serial.print(ppm_a);
    Serial.print(" ppm");
    Serial.print("\t\t\t");

    return ppm_a;
}
float recupererCO()
{
    float ppm_b = gasSensorb.getPPM();
    Serial.print("D3: "); 
    Serial.print(analogRead(mq135Pin_b));
    Serial.print(" CO: ");
    Serial.print(ppm_b);
    Serial.print(" ppm");
    Serial.print("\t\t\t"); 

    return ppm_b;
}
float recupererNH3()
{
    float ppm_c = gasSensorc.getPPM();
    Serial.print("D5: "); 
    Serial.print(analogRead(mq135Pin_c));
    Serial.print(" NH3: ");
    Serial.print(ppm_c);
    Serial.println(" ppm");

    return ppm_c;
}

/*Défintion de la fonction 
 * d'envoie des valeurs des gazs au serveur
 */
void EnvoyerLesTroisValeursAuServeur(String CO2, String CO, String NH3)
{
    if (client.connect(servername, 80)) //On connecte notre Arduino sur "leserveur.com" et le port 80 (defaut pour l'http)
    {  
    Serial.println("OK ! On est Connecté au Serveur ");        // Si c'est OK, on affiche cela
    client.print("GET /post.php?");
    client.print("&ValeurCO2=");
    client.print(CO2);
    client.print("&ValeurCO=");
    client.print(CO);
    client.print("&ValeurNH3=");
    client.print(NH3);
    client.println(" HTTP/1.1"); //puis on envoie la requete
    client.println("Host: mems-27-1.appspot.com");      // On précise le serveur, le site qui devra faire (héberger, "host" en anglais) la réponse
    client.println("Connection: close");  // on signale au serveur que la connexion sera fermée lorsque les données sont transférées.
    client.println(); //fin de la requete GET, pour prévenir que l’on vient de finir d’écrire notre en-tête (header), on envoie une ligne blanche.
    } 
    else // La connexion a échouée :(
    {
    Serial.println("Connection au serveur Echoué !"); //message d'eurreur si le clien n'est pas connécter
    Serial.println();
    }
    /*lecture et affichage de
     * la réponse du seveur
     */
    while(client.connected() && !client.available()) // Tant que client connécté ET données pas dispo
      delay(1); // On les attends
    
    while (client.connected() || client.available()) // Soit le client est connécté , soit des données sont  dispo
    { 
      char c = client.read(); // On les lis une par une 
      result = result+c;
    }
    // Enfin, quand tout a été lu, on va vérifier l’état de notre connexion, bien sur si on est sortie du While precedent c'est qu'on est déconnecté
    client.stop(); // On ferme le client
    Serial.println(result); // ET on affiche les caractères lues dans le moniteur série
}

