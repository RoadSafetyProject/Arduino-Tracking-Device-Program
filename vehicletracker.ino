#include "SIM900.h"
 
#include <SoftwareSerial.h>

#include "gps.h"

#include "inetGSM.h"
 
InetGSM inet;
 
char msg[50];

GPSGSM gps;   //object for GPS
char lon[10];
char lat[10];
char alt[10];
char gtime[15];
char vel[10];
char stat;



void setup()
{
  
  //Serial connection.
  Serial.begin(9600);
  if (gsm.begin(2400)){
    Serial.println("\nstatus=READY");
    //Attaxh the gprs device
    if (inet.attachGPRS("internet.wind", "", ""))
      Serial.println("status=ATTACHED");
    else Serial.println("status=ERROR");
    delay(1000);   
  }
  else Serial.println("\nstatus=IDLE"); 
};
void loop()
{
  if (gps.attachGPS())
  {
    Serial.println("status=GPSON");
    delay(20000); //Time for fixing
    stat=gps.getStat();
    if(stat==1)
      Serial.println("NOT FIXED");
    else if(stat==0)
      Serial.println("GPS OFF");
    else if(stat==2)
      Serial.println("2D FIXED");
    else if(stat==3)
      Serial.println("3D FIXED");
    else{
      Serial.print("GPS Status Unknown:");
      Serial.println(stat);
    }
    delay(5000);
    //Get the gps data
    gps.getPar(lon,lat,alt,gtime,vel);
    Serial.println(lon);
    Serial.println(lat);
    Serial.println(alt);
    Serial.println(gtime);
    Serial.println(vel);
    if(lon != "") //if location is captured
    {
      //prepare http request
      char url[60] = "/tracker/tracker.php?latitude=-6.772221&longitude=39.240237";
      strcpy (url,"/tracker/tracker.php?latitude=");
      strcat (url,lat);
      strcat (url,"&longitude=");
      strcat (url,lon);
      strcat (url,"&velocity=");
      strcat (url,vel);
      strcat (url,"&altitude=");
      strcat (url,alt);
      //send an http request to the server
      inet.httpGET("roadsafety.go.tz", 8080, url, msg, 50);
    }
 }
    else Serial.println("status=GPSERROR");
  delay(1000);
}
