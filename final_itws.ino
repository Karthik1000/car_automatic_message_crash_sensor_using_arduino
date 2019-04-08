
#include <SoftwareSerial.h>
SoftwareSerial mySerial(2, 3);
SoftwareSerial Serial1(10,11); //make RX arduino line is pin 2, make TX arduino line is pin 3.
SoftwareSerial gps(2,3);
char msg;
char call;

int i=0,k=0;
int  gps_status=0;
float latitude=0; 
float logitude=0;                       
String Speed="";
String gpsString="";
char *test="$GPRMC";


int motor1_forward=5;
int motor1_reverse=7;
int motor2_forward=9;
int motor2_reverse=11;
const int trigpin=6;
const int echopin=8;
long duration;
int distance;

int led=10;
int button_pin=8;
int val;


void setup() {
  // put your setup code here, to run once:
  mySerial.begin(9600);   // Setting the baud rate of GSM Module  
pinMode(motor1_forward,OUTPUT);
pinMode(motor1_reverse,OUTPUT);
pinMode(motor2_forward,OUTPUT);
pinMode(motor2_reverse,OUTPUT);
pinMode(trigpin,OUTPUT);
pinMode(echopin,INPUT);
  
Serial.begin(9600);
  Serial1.begin(9600);
    Serial.println("Initializing....");

Serial.println("Initialized Successfully");


  Serial.print("Waiting For GPS.....\n");
 // lcd.setCursor(0,1);
  //lcd.print("     Signal    ");
 // delay(2000);
  gps.begin(9600);
  get_gps();
  show_coordinate();
  delay(2000);
  Serial.println("System Ready..");
Serial.println("start1");
pinMode(led,OUTPUT);

pinMode(button_pin,INPUT);

}

  


void gpsEvent()
{
  gpsString="";
  while(1)
  {
   while (gps.available()>0)            //Serial incoming data from GPS
   {
    char inChar = (char)gps.read();
     gpsString+= inChar;                    //store incoming data from GPS to temparary string str[]
     i++;
    // Serial.print(inChar);
     if (i < 7)                      
     {
      if(gpsString[i-1] != test[i-1])         //check for right string
      {
        i=0;
        gpsString="";
      }
     }
    if(inChar=='\r')
    {
     if(i>60)
     {
       gps_status=1;
       break;
     }
     else
     {
       i=0;
     }
    }
  }
   if(gps_status)
    break;
  }
}

void get_gps()
{
  //lcd.clear();
  Serial.print("\nGetting GPS Data\n");
  //lcd.setCursor(0,1);
  Serial.print("Please Wait.....\n");
   gps_status=0;
   int x=0;
   while(gps_status==0)
   {
    gpsEvent();
    int str_lenth=i;
    coordinate2dec();
    i=0;x=0;
    str_lenth=0;
   }
}

void show_coordinate()
{
    /*lcd.clear();
    lcd.print("Lat:");
    lcd.print(latitude);
    lcd.setCursor(0,1);
    lcd.print("Log:");
    lcd.print(logitude);*/
    latitude=latitude-19.50049;
    logitude=logitude-0.000118;
    Serial.print("Latitude:");
    Serial.println(latitude);
    Serial.print("Longitude:");
    Serial.println(logitude);

   Serial.print("http://maps.google.com/maps?&z=15&mrt=yp&t=k&q=");
   Serial.print(latitude,6);
   Serial.print("+");              //28.612953, 77.231545   //28.612953,77.2293563
   Serial.print(logitude,6);
    /*Serial.print("Speed(in knots)=");
    Serial.println(Speed);
    
    lcd.clear();
    lcd.print("Speed(Knots):");
    lcd.setCursor(0,1);
    lcd.print(Speed);*/
    delay(2000);
}

void coordinate2dec()
{
  String lat_degree="";
    for(i=20;i<=21;i++)         
      lat_degree+=gpsString[i];
      
  String lat_minut="";
     for(i=22;i<=28;i++)         
      lat_minut+=gpsString[i];

  String log_degree="";
    for(i=32;i<=34;i++)
      log_degree+=gpsString[i];

  String log_minut="";
    for(i=35;i<=41;i++)
      log_minut+=gpsString[i];
    
    Speed="";
    for(i=45;i<48;i++)          //extract longitude from string
      Speed+=gpsString[i];
      
     float minut= lat_minut.toFloat();
     minut=minut/60;
     float degree=lat_degree.toFloat();
     latitude=degree+minut;
     
     minut= log_minut.toFloat();
     minut=minut/60;
     degree=log_degree.toFloat();
     logitude=degree+minut;
}






void SendMessage()
{
  mySerial.println("AT+CMGF=1");    //Sets the GSM Module in Text Mode
  delay(1000);  // Delay of 1000 milli seconds or 1 second
  mySerial.println("AT+CMGS=\"8639636689\"\r"); // Replace x with mobile number
  delay(1000);
  mySerial.println("Alert");// The SMS text you want to send
  delay(100);
  
   mySerial.println((char)26);// ASCII code of CTRL+Z
  delay(1000);
  Serial.print("message sent");
}





void loop() {
  // put your main code here, to run repeatedly:
int incomingByte='0';

    get_gps();
    show_coordinate();
      delay(2000);

if(Serial.available()>0)
{
  incomingByte=Serial.read();
}
/*switch(incomingByte)
{
  case '8':
  digitalWrite(motor1_forward,HIGH);
  digitalWrite(motor1_reverse,LOW);
  digitalWrite(motor2_forward,HIGH);
  digitalWrite(motor2_reverse,LOW);
  delay(10000);
  Serial.println("stop\n");
  incomingByte='0';
  break;
  case '2':
  digitalWrite(motor1_forward,LOW);
  digitalWrite(motor1_reverse,HIGH);
  digitalWrite(motor2_forward,LOW);
  digitalWrite(motor2_reverse,HIGH);
  delay(10000);
  Serial.println("stop\n");
  incomingByte='0';
  break;
  case '6':
  digitalWrite(motor1_forward,HIGH);
  digitalWrite(motor1_reverse,LOW);
  digitalWrite(motor2_forward,LOW);
  digitalWrite(motor2_reverse,LOW);
  delay(3000);
  Serial.println("stop\n");
  incomingByte='0';
  break;
  case '4':
  digitalWrite(motor1_forward,LOW);
  digitalWrite(motor1_reverse,LOW);
  digitalWrite(motor2_forward,HIGH);
  digitalWrite(motor2_reverse,LOW);
  delay(3000);
  Serial.println("stop\n");
  incomingByte='0';
  break;
  case '5':
  digitalWrite(motor1_forward,HIGH);
  digitalWrite(motor1_reverse,LOW);
  digitalWrite(motor2_forward,LOW);
  digitalWrite(motor2_reverse,HIGH);
  delay(3000);
  Serial.println("stop\n");
  incomingByte='0';
  break;
  case '0':
  digitalWrite(motor1_forward,HIGH);
  digitalWrite(motor1_reverse,HIGH);
  digitalWrite(motor2_forward,HIGH);
  digitalWrite(motor2_reverse,HIGH);
  delay(3000);
  Serial.println("stop\n");
  incomingByte='0';
  break;
  
  
  
  
}*/



digitalWrite(trigpin,LOW);
delayMicroseconds(20);
digitalWrite(trigpin,HIGH);
delayMicroseconds(10);
digitalWrite( trigpin, LOW);
duration=pulseIn(echopin,HIGH);
distance=duration*0.034/2;
Serial.println("Distance(cm):  ");
Serial.println(distance);
//collision sensor

/*
val=digitalRead(button_pin);
if(val==LOW)
{
  digitalWrite(led,HIGH);
  delay(100);  
  }
else
{
  digitalWrite(led,LOW);
  
  }

*/




  while(1)
 {
  digitalWrite(trigpin,LOW);
delayMicroseconds(20);
digitalWrite(trigpin,HIGH);
delayMicroseconds(10);
digitalWrite( trigpin, LOW);
duration=pulseIn(echopin,HIGH);
distance=duration*0.034/2;
Serial.println("Distance(cm):  ");
Serial.println(distance);

  if(distance<10 && distance>1)
{
  digitalWrite(led,HIGH);
}
else
{
  digitalWrite(led,LOW);
}
  if(distance>5)
{
 digitalWrite(motor1_forward,HIGH);
  digitalWrite(motor1_reverse,LOW);
  digitalWrite(motor2_forward,HIGH);
  digitalWrite(motor2_reverse,LOW);
}

  else
  {
          digitalWrite(motor1_forward,HIGH);
  digitalWrite(motor1_reverse,HIGH);
  digitalWrite(motor2_forward,HIGH);
  digitalWrite(motor2_reverse,HIGH);
  SendMessage();

  break;
    }
  }
/*  
if(distance<3)
{
  digitalWrite(motor1_forward,HIGH);
  digitalWrite(motor1_reverse,HIGH);
  digitalWrite(motor2_forward,HIGH);
  digitalWrite(motor2_reverse,HIGH);

  }*/
}
