/*
 *    UNIVERSIDAD TECNICA DEL NORTE
 *    SISTEMAS EMBEBIDOS
 *    STEVEN FLORES 
 *    DEBER 9
 */
 
 #include<avr/power.h>  //Librerias
 #include<avr/sleep.h>
 #include<avr/wdt.h>
 #include<EEPROM.h>
 int on;          //Variable para menu
 int i ;          //Variable para tiempo rep[eticion
 int dato;        //Variable para valor obntenido por adc
 int act ;        //Variable activacion lectura adc
 void (* resetFunc)(void)=0;
 void setup(){
  Serial.begin(9600);     //Comunicacion cx
  
//  EEPROM.write(0,0);   //Iniciovalores EEPROM
//  EEPROM.write(1,0);
//  EEPROM.write(2,0);
//  EEPROM.write(3,0);
  on=EEPROM.read(0);    //Lectura valores EEPROM
  i=EEPROM.read(1);
  act= EEPROM.read(2);
  dato=EEPROM.read(3);
  if (i<dato){          //Condicion para tiempos
    i++;
  }else{
    i=dato;
  }
  EEPROM.update(1,i);   //Incremento de tiempo
  
  if(on==2){
    sleep();          //Condicion perro guardian
  }
  attachInterrupt(0, activacion, LOW);    //Sleccion de menus
  attachInterrupt(1, sleep, LOW);         //Activacion perro guardian
 }
 
void perroguardian(){               //Metodo perro guardian
    wdt_enable(WDTO_1S);
    set_sleep_mode(SLEEP_MODE_PWR_SAVE);
    sleep_enable();
    sleep_mode();
    sleep_disable();
    wdt_disable();
}
 void loop(){
  if(act==1){               //Condicion lectura dato adc
  dato = analogRead(0); 
  dato = map(dato,0,1024,0,60);   //Reasignacion rango de valores
  EEPROM.update(3,dato);           //Actualizacion en memoria EEPROM
  Serial.println(String(dato)+String(" Segundos "));  //Mensaje de confirmacion
  delay(250);               
  }
 }

 void activacion(){         //Metodo de menu
  switch(on){
    case 0:                 //Caso 1
      act = 1;              
      Serial.println("Inicio del sistema");     //Mensaje de informacion 
      Serial.println("Seleccione el tiempo de perro guardian mediante pot"); //Mensaje de informacion 
      on++;  //Incremento para el menu 2
      EEPROM.update(0,on);
      EEPROM.update(2,act);
      EEPROM.update(1,0); 
    break;
    case 1:         //Caso para seleccion de tiempo
      act = 0;     
     Serial.println(String("Perro guardian en :")+String(dato)+String(" segundos "));  //Informacion de tiempo elegido
     on++;            //Aumento para menu 3
     EEPROM.update(0,on);   //Actualizacion dato para activacion
     EEPROM.update(2,act);    //Actualizacion dato para la activacion de puerto adc
    break;
    case 2:               //caso para apagado
      Serial.println("Sistema apagado");    //Mensaje de confirmacion
      on=0;         
      act = 0;
     EEPROM.update(0,on);         //Encerar las variables
     EEPROM.update(2,act);
     EEPROM.update(4,0);
    break;
  }
 }
void sleep (){                   //Metodo para activacion de perro guardian.
  if(on==2){                    //Condicion de activacion
    if(i<dato){                 //Condicion de tiempo para perro guardian
    delay(100);       
    perroguardian();            //Ejecucion metodo
    }else{
      EEPROM.update(0,0);       //Reinicio de variables
      wdt_enable(WDTO_15MS);
    }
  }
}
void encender(){              //Metodo de reset
  resetFunc();
}
