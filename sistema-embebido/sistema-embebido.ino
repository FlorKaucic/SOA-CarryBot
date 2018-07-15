#include "src/ObstacleDetector/ObstacleDetector.h"
#include "src/CarControl/CarControl.h"
#include "src/LED/LED.h"
#include "src/Alarm/Alarm.h"
#include "src/Bluetooth/BTHandler/BTHandler.h"
#include "src/SignalComparator/SignalComparator.h"
#include <string.h>

/**************************
Declaracion de constantes
***************************/

//Constante para los pines del detector de obstaculos
const int TRIGGER_PIN = 50;
const int ECHO_PIN = 51;
//Constante para el pin del speaker
const int SPEAKER_PIN = 53;
//Constante para el pin de la placa de presion
const int PLACA_PIN = 3;     
//Constantes para los motores
const int LEFT_MOTOR_CLOCKWISE_PIN = 30;
const int LEFT_MOTOR_COUNTER_CLOCKWISE_PIN = 31;
const int RIGHT_MOTOR_CLOCKWISE_PIN = 32;
const int RIGHT_MOTOR_COUNTER_CLOCKWISE_PIN = 33;
//Constante para el pin del led
const int LED_PIN = 2;
//Constantes para el filtro de Kalman del control de deteccion de la señal
const double PROCESS_NOISE = 0.125;
const double SENSOR_NOISE = 32;
const double ESTIMATED_ERROR = 1023;
//Cantidad de veces que se pregunta por el obstaculo antes de forzar el modo manual
const int TIMEOUT_ERROR = 10;
//Default variables
const int DF_LOOPS_BETWEEN_INQUIRIES = 15; //Cantidad de veces que se avanza por default
const int DF_MILLISECONDS_FOR_TURNING = 1300; //Turning time o tiempo de giro por default
const int DF_THRESHOLD = -3;
const int DF_RSSI_LLEGADA = -50;
const char DF_SERVER_MAC[] = "4B1:67:40005D";
//Estados
const int ST_PREPARE_TRAVEL = 10;
const int ST_AUTOMATIC_MOVEMENT = 20;
const int ST_MANUAL_MOVEMENT = 30;
//Tipos de viajes
const int TT_SERVER_TO_CLIENT = 1;
const int TT_CLIENT_TO_SERVER = 2;
const int TT_CANCEL_TO_SERVER = 3;
const int TT_CANCELLING = 4;
const int TT_NONE = 5;
//Comandos BT
const String CBT_PING = "PING";
const String CBT_CONFIG = "SET+";
const String CBT_CONFIG_MAC = "SET+MAC=";
const String CBT_CONFIG_LOOPS = "SET+LOOPS=";
const String CBT_CONFIG_THRESHOLD = "SET+THRESH=";
const String CBT_CONFIG_T_GIRO = "SET+TGIRO=";
const String CBT_CONFIG_RSSI_LLEGADA = "SET+LLEGADA=";
const String CBT_CONFIG_READY = "SET+DONE";
const String CBT_TO_MANUAL = "MANUAL";
const String CBT_TO_AUTOMATIC = "AUTOMATIC";
const String CBT_CANCEL = "CANCEL";
const String CBT_MOVE_LEFT = "MOVE+LEFT";
const String CBT_MOVE_RIGHT = "MOVE+RIGHT";
const String CBT_MOVE_FORWARD = "MOVE+FORWARD";
const String CBT_MOVE_BACKWARD = "MOVE+BACKWARD";
const String CBT_MOVE_ARRIVED = "MOVE+ARRIVED";
const String CBT_MOVE = "MOVE+";
//Respuestas
const String CBT_PONG = "PONG";
const String CBT_CONFIG_OK = "SET+OK"; //Ok
const String CBT_CONFIG_ERROR = "SET+ERROR"; //Error
const String CBT_STATUS_ALOAD = "STATUS+ALOAD"; //Awaiting load
const String CBT_STATUS_AUNLOAD = "STATUS+AUNLOAD"; //Awaiting unload
const String CBT_STATUS_ONTRAVEL = "STATUS+ONTRAVEL"; //On travel
const String CBT_STATUS_RBLOCK = "STATUS+RBLOCK"; //Road blocked
const String CBT_STATUS_AMAC = "STATUS+AMAC"; //Awaiting MAC
const String CBT_STATUS_ONDEST = "STATUS+ONDEST"; //On destiny

//Tipo de mensaje debug
const int DBG_BT = 1; //Solo envio el mensaje por BT
const int DBG_PC = 2; //Solo envio el mensaje por PC (Serial)
const int DBG_BOTH = 3; //Envio el mensaje por BT y PC
const int COM_BT = 4; //Comandos BT para informar a la app
//Tipos de valores en los comandos
const int CV_MAC = 1;
const int CV_INT = 2;

/*************************************************
Lista de MAC Bluetooth de dispositivos de prueba

Sebas = "305A:3A:11CB29";
TV Flor = "F877:B8:C3662F";
Flor = "E458:B8:90E129";
Motolola Facu = "8410:0D:7E7237";
Xiaomi Facu = "04B1:67:40005D";
*************************************************/

/**************************
Declaracion de variables
***************************/

//Direccion MAC Bluetooth utilizada
char server_MAC[15];
char destiny_MAC[15];
//Flag para saber si se está en modo debug o no
bool f_dbg = false;
//Esta la ruta bloqueada o no
bool roadBlocked;
//Control y comunicacion del detector de obstaculos
ObstacleDetector obstacleDetector(TRIGGER_PIN, ECHO_PIN);
//Control del speaker
Alarm alarm(SPEAKER_PIN);
//Comunicacion con el led
LED led(LED_PIN);
//Control de los motores
CarControl carControl(LEFT_MOTOR_CLOCKWISE_PIN,
                      LEFT_MOTOR_COUNTER_CLOCKWISE_PIN,
                      RIGHT_MOTOR_CLOCKWISE_PIN,
                      RIGHT_MOTOR_COUNTER_CLOCKWISE_PIN);
//Control de detección de la señal
SignalComparator signalComparator(&Serial2, server_MAC);

//Variables de configuracion
int loopsBetweenInquiries;
//Estado del auto
int state;
//Tipo de viaje
int travelType;

/***********************************
Declaracion de funciones principales
************************************/

//Setup principal, donde inicializo variables, espero la conexion del BT y recibo los parametros a setear
void setup() {
  String msg ="";
  //Inicializo los tres serial que se utilizan:
  // - Serial: Para el debug con la PC
  // - Serial2: (HC-05) Para la detección de señales Bluetooth
  // - Serial3: (HC-05) Para la comunicacion con Android
  Serial.begin(9600);
  Serial3.begin(38400);
  Serial2.begin(38400);
  debugmsg("> CarryBot <",DBG_BOTH);
  //Inicializo los pines
  pinMode(PLACA_PIN, INPUT);
  
  //Espero el PING para confirmar la conexion
  debugmsg("Msg: PING command",DBG_PC);
  debugmsg("Waiting PING",DBG_BT);

  bool btConnected = false;
  while(!btConnected){
      if(Serial3.available()) {
        msg = Serial3.readString();
        if(msg.startsWith(CBT_PING)){
          btConnected = true;
          debugmsg("Msg: PING command received",DBG_PC);
        }
      }
  }
  alarm.connected();
  debugmsg(CBT_PONG,COM_BT);
  dbgmode();
  debugmsg("Msg: Waiting SET params",DBG_PC);
  debugmsg("Set config:\n",DBG_BT);

  setearConfig();  
  debugmsg("Msg: Setting finished, starting program",DBG_PC);
  debugmsg("Set config finished, starting program:\n",DBG_BT);

  //Inicializo el comparador de señal bluetooth
  debugmsg("Msg: >Signal comparator init",DBG_PC);
  signalComparator.init();

  debugmsg("Config finished:\n",DBG_BT);

  state = ST_PREPARE_TRAVEL;
  travelType = TT_CLIENT_TO_SERVER;
}

void loop() {
  //Si hay algun comando en el BT, lo proceso
  if(Serial3.available()) {
    processCommand(Serial3.readString());
  }
  //Sino, realizo acciones dependiendo del estado en el que se encuentra
  else
    stateAction();
}

/***********************************
Declaracion de funciones secundarias
************************************/

//Seteo inicial de la configuracion
void setearConfig(){
  bool setFinished = false;
  String msg ="";
  int n = 0;
  //Valores default
  loopsBetweenInquiries = DF_LOOPS_BETWEEN_INQUIRIES;
  strcpy(server_MAC,DF_SERVER_MAC);
  int turningTime = DF_MILLISECONDS_FOR_TURNING;
  int threshold = DF_THRESHOLD;
  int noiseOnTarget = DF_RSSI_LLEGADA;
  debugmsg("Esperando comando: ", DBG_BOTH);
  while(!setFinished){ 
     if(Serial3.available()) {
       msg = Serial3.readString();
       while (Serial3.available()>0)  Serial3.read();
       debugmsg("comando: ", DBG_PC);
       debugmsg(msg, DBG_PC);
       if(msg.startsWith(CBT_CONFIG)){
         if(msg.startsWith(CBT_CONFIG_READY)){
            setFinished = true;
            debugmsg("Msg: Config done", DBG_PC);
            debugmsg(CBT_CONFIG_OK, COM_BT);
         }
         else if(msg.startsWith(CBT_CONFIG_MAC)){
            String m = getStrValue(msg);
             if(m != ""){
              m.toCharArray(server_MAC,15);
              debugmsg("Msg: New Config -> Server MAC", DBG_PC);
              debugmsg(CBT_CONFIG_OK, COM_BT);
            } else {
              debugmsg(CBT_CONFIG_ERROR, COM_BT);
            }  
            
         }
         else if(msg.startsWith(CBT_CONFIG_LOOPS)){
            n = getIntValue(msg);
            if(n > 0){
              loopsBetweenInquiries = n;
              debugmsg("Msg: New Config -> Loops", DBG_PC);
              debugmsg(CBT_CONFIG_OK, COM_BT);
            } else {
              debugmsg(CBT_CONFIG_ERROR, COM_BT);
            }  
         }
         else if(msg.startsWith(CBT_CONFIG_THRESHOLD)){
            n = getIntValue(msg);
            if(n > 0){
              threshold = -n;
              debugmsg("Msg: New Config -> Threshold", DBG_PC);
              debugmsg(CBT_CONFIG_OK, COM_BT);
            } else {
              debugmsg(CBT_CONFIG_ERROR, COM_BT);
            }
         }
         else if(msg.startsWith(CBT_CONFIG_T_GIRO)){ 
            n = getIntValue(msg);
            if(n > 0){
              turningTime = n;
              debugmsg("Msg: New Config -> Tgiro", DBG_PC);
              debugmsg(CBT_CONFIG_OK, COM_BT);
            } else {
              debugmsg(CBT_CONFIG_ERROR, COM_BT);
            }       
         }
         else if(msg.startsWith(CBT_CONFIG_RSSI_LLEGADA)){  
            n = getIntValue(msg);
            if(n > 0){
              noiseOnTarget = -n;
              debugmsg("Msg: New Config -> NoiseOnTarget", DBG_PC);
              debugmsg(CBT_CONFIG_OK, COM_BT);
            } else {
              debugmsg(CBT_CONFIG_ERROR, COM_BT);
            }         
         } else {
            debugmsg("ERROR: COMANDO INCORRECTO", DBG_BOTH);
            debugmsg(CBT_CONFIG_ERROR , COM_BT);
         }
         debugmsg("Msg: finish config SET+DONE",DBG_PC);
         
       }
     }
  }
  //Seteo los valores
  carControl.setMillisecondsForTurning(turningTime);
  signalComparator.setThreshold(threshold);
  signalComparator.setNoiseOnTarget(noiseOnTarget);
  debugmsg(CBT_CONFIG_OK, COM_BT);
}  

//Recepcion de los comandos que llegan por BT
void processCommand(String str){
  debugmsg("Msg: Command received ",DBG_PC);
  debugmsg(str,DBG_PC);

  if(str.startsWith(CBT_TO_MANUAL)){
    if(state == ST_AUTOMATIC_MOVEMENT){
      debugmsg("Msg: Changing to MANUAL",DBG_PC);
      debugmsg("Changing to MANUAL\n",DBG_BT);
      state = ST_MANUAL_MOVEMENT;
      debugmsg(CBT_CONFIG_OK, COM_BT);
    }
  }
  else if(str.startsWith(CBT_TO_AUTOMATIC)){
    if(state == ST_MANUAL_MOVEMENT){
      debugmsg("Msg: Changing to AUTOMATIC",DBG_PC);
      debugmsg("Changing to AUTOMATIC\n",DBG_BT);
      state = ST_AUTOMATIC_MOVEMENT;
      debugmsg(CBT_CONFIG_OK, COM_BT);
    }
  }
  else if(str.startsWith(CBT_CANCEL)){
    if(travelType == TT_SERVER_TO_CLIENT){
      debugmsg("Msg: Canceling travel",DBG_PC);
      debugmsg("Canceling travel\n",DBG_BT);
      state = ST_PREPARE_TRAVEL;
      travelType = TT_CANCELLING;
      debugmsg(CBT_CONFIG_OK, COM_BT);
    }
  }
  else if(str.startsWith(CBT_MOVE)){
    if(state == ST_MANUAL_MOVEMENT){
        debugmsg("Moving\n",DBG_BT);
        manualMovement(str);
    }
  }
}

//Accion que se realiza dependiendo del estado del embebido
void stateAction(){
  switch(state){
    case ST_PREPARE_TRAVEL:
      prepareTravel();
      break;
    case ST_AUTOMATIC_MOVEMENT:
      automaticMovement();
      break;
  }
}

//Preparacion del proximo viaje
void prepareTravel(){
   //Cambio el tipo de viaje
   debugmsg("Msg: Setting travel type",DBG_PC);
   debugmsg("New Travel\n",DBG_BT);
   switch(travelType){
      case TT_SERVER_TO_CLIENT: 
        travelType = TT_CLIENT_TO_SERVER;
        break;
      case TT_CLIENT_TO_SERVER:
      case TT_CANCEL_TO_SERVER:
        travelType = TT_SERVER_TO_CLIENT;
        break;
      case TT_CANCELLING:
        travelType = TT_CANCEL_TO_SERVER;
        break;
   }
   //Seteo la MAC destino dependiendo del tipo de viaje
   debugmsg("Msg: Setting new destiny MAC",DBG_PC);
   char newMAC[15];
   if(travelType == TT_SERVER_TO_CLIENT){
      //Pedir la MAC del cliente
      debugmsg("Waiting client MAC",DBG_BOTH);
      debugmsg(CBT_STATUS_AMAC,COM_BT);

      bool f = false;
      String msg = "";
      while(!f){
        if(Serial3.available()) {
          msg = Serial3.readString();
          while (Serial3.available()>0)  Serial3.read();
          if(msg != ""){
              msg.toCharArray(destiny_MAC,15);
              f = true;
          }
        }
      }
      debugmsg("Msg: Destiny MAC: Client",DBG_PC);
      debugmsg("Client MAC setted\n",DBG_BT);
      strcpy(newMAC,destiny_MAC);
   }
   else
   {
      debugmsg("Msg: Destiny MAC: Server",DBG_PC);
      debugmsg("Server MAC setted\n",DBG_BT);

      strcpy(newMAC,server_MAC);
   }
   signalComparator.setAddress(newMAC);



   //Espero a que haya o no peso en la placa para inicializar viaje
   if(travelType == TT_SERVER_TO_CLIENT){
      //Si el viaje es del servidor al cliente, primero espero que se cargue un objeto
      while(!hayPeso()){
          debugmsg("Msg: Waiting object",DBG_PC);
          debugmsg("Waiting object",DBG_BT);
          alarm.alert();
          led.fadeIn();
          delay(500);
          led.fadeOut();
          debugmsg(CBT_STATUS_ALOAD , COM_BT);
      }
   }
   else if(travelType == TT_CLIENT_TO_SERVER)
   {
      //Si el viaje es del cliente al servidor, primero espero que el cliente haya sacado el objeto transportado
      while(hayPeso()){
          debugmsg("Msg: Please take object",DBG_PC);
          debugmsg("Please take object",DBG_BT);
          alarm.alert();
          led.fadeIn();
          delay(500);
          led.fadeOut();
          debugmsg(CBT_STATUS_AUNLOAD , COM_BT);
      }
   }

   
   //Paso los parametros al comparador de señal BT
   debugmsg("Msg: >Signal comparator set parameters",DBG_PC);
   signalComparator.setFilterParameters(PROCESS_NOISE, SENSOR_NOISE, ESTIMATED_ERROR);
   //Inicio la medicion de señal
   debugmsg("Msg: >Signal comparator initial reading",DBG_PC);
   signalComparator.setInitialReading();

   debugmsg("Msg: >Signal comparator initial reading finished",DBG_PC);
   
   state = ST_AUTOMATIC_MOVEMENT;
   debugmsg(CBT_STATUS_ONTRAVEL ,COM_BT);

}

//Movimiento manual del auto
void manualMovement(String str){
  
    if(str.startsWith(CBT_MOVE_LEFT)){
      debugmsg(CBT_CONFIG_OK, COM_BT);
      carControl.turnLeft();
    } 
    else if(str.startsWith(CBT_MOVE_RIGHT)){
      debugmsg(CBT_CONFIG_OK, COM_BT);
      carControl.turnRight();
    } 
    else if(str.startsWith(CBT_MOVE_FORWARD)){
      debugmsg(CBT_CONFIG_OK, COM_BT);
      carControl.goForward();
      delay(300);
      carControl.stop();
    } 
    else if(str.startsWith(CBT_MOVE_BACKWARD)){
      debugmsg(CBT_CONFIG_OK, COM_BT);
      carControl.goBackwards();
      delay(300);
      carControl.stop();
    } 
    else if(str.startsWith(CBT_MOVE_ARRIVED)){
      debugmsg(CBT_CONFIG_OK, COM_BT);
      onTarget();
    } 
}

//Movimiento del auto cuando busca la señal (automatico)
void automaticMovement(){
  //Variables locales
  int loopCount = 0;
  //Avanzo el auto
  debugmsg("Moving\n",DBG_BT);
  debugmsg("GO GO GO",DBG_PC);
  carControl.goForward();
  
  //Detecto si hay un ostaculo
  roadBlocked = obstacleDetector.isRoadBlocked();

  //De no haber obstaculo, avanzo cierta cantidad de pasos, siempre revisando que no haya obstaculos
  while(!roadBlocked && loopCount < loopsBetweenInquiries) {
    roadBlocked = obstacleDetector.isRoadBlocked();
    loopCount++;
    delay(300);
  }

  //Detengo el auto en cierta cantidad de pasos o si hay un obstaculo
  debugmsg("STOP", DBG_PC);
  debugmsg("Stopping\n",DBG_BT);

  carControl.stop();

  //Si encontré un obstaculo, aviso al usuario hasta que se quite el obstaculo
  int y = 0;

  while(roadBlocked && y < TIMEOUT_ERROR) {
    led.fadeIn();
    alarm.error();
    led.fadeOut();
    debugmsg("Msg: ERROR: Road blocked",DBG_PC);
    debugmsg("Road Blocked\n",DBG_BT);
    debugmsg(CBT_STATUS_RBLOCK ,COM_BT);
    delay(200);
    roadBlocked = obstacleDetector.isRoadBlocked();
    y++;
  }
  if(roadBlocked){
    alarm.alert();
    debugmsg("Msg: Error, switching to manual mode ", DBG_PC);
    debugmsg("ERROR\n",DBG_BT);

    state = ST_MANUAL_MOVEMENT;
    return;
  }

  //Mido la intensidad de la señal
  debugmsg("Relative distance measuring: ", DBG_PC);
  debugmsg("Measuring relative distance\n",DBG_BT);

  RelativeDistance distance = signalComparator.getRelativeDistance();
  debugmsg("Result: ", DBG_PC);
  debugmsg("Relative distance measured\n",DBG_BT);

  //Comparo con la intensidad anterior y decido si girar o avanzar
  if(distance == RelativeDistance::FURTHER_AWAY_FROM_TARGET) {
    //Si fue menor a la anterior, giro
    debugmsg("> TIME TO TURN LEFT",DBG_PC);
      
      
    debugmsg("Turning left\n",DBG_BT);

    alarm.alert();
    carControl.turnLeft();
  } else if (distance == RelativeDistance::ON_TARGET) {
    onTarget();
    delay(1000);
  } else if (distance == RelativeDistance::ERROR) {
    //Si hubo algun error (por ejemplo, no hubo medicion de la señal requerida), no hago nada y espero
    debugmsg("Msg: SOME ERROR",DBG_PC);
    debugmsg("Critical error(?\n",DBG_BT);

    alarm.disconnected();
    delay(2000);
  }
}

//Metodo cuando se llega a destino
void onTarget(){
    //Si ya llegue a destino, lo informo
    debugmsg("Im on target\n",DBG_BOTH);
    debugmsg(CBT_STATUS_ONDEST, COM_BT);
    led.fadeIn();
    alarm.arrival();
    led.fadeOut();
    //Como ya llegue a destino, cambio el estado del auto
    state = ST_PREPARE_TRAVEL;  
}

//Deteccion de peso en la placa de presion
bool hayPeso(){
  if (digitalRead(PLACA_PIN) == HIGH)
    return true;
  else
    return false;
}

//Metodo para obtener el valor entero de los comandos BT
int getIntValue(String str){
  char* pch;
  char aux[str.length()+1];
  str.toCharArray(aux,str.length());
  int num = -1;
  strtok(aux,"=");
  pch = strtok(NULL,"\n");
  num = atoi(pch);
  return num;
}

//Metodo para obtener el valor String de los comandos BT
String getStrValue(String str){
    int indexEqual = str.indexOf("=");
    int indexEndLine = str.indexOf("\n");
    String res = "";
    res = str.substring(indexEqual+1, indexEndLine );
    return res;
}

//Metodo para debug
void debugmsg(String str, int type){
  if((f_dbg) && ((type == DBG_PC) || (type == DBG_BOTH)))
    Serial.println(str);
  if((f_dbg) && ((type == DBG_BT) || (type == DBG_BOTH)))
    Serial3.println(str);
  if((type == COM_BT))
    Serial3.println(str);
}

void dbgmode(){
  f_dbg = hayPeso();
  if(f_dbg){
      led.fadeIn();
      led.fadeOut();
      led.fadeIn();
      led.fadeOut();
      led.fadeIn();
      led.fadeOut();
  }
}

