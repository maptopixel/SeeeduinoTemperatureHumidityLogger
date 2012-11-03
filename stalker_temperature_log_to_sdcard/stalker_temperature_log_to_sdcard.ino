

#include <DS1307RTC.h>

#include <Time.h>

#include <SD.h>

#include <Wire.h>




/*#include <dht.h>

#include <Wire.h>
#include <SD.h>

#include "Time.h" 
#include "DS1307RTC.h"

*/


#include <DHT22.h>
// Only used for sprintf
#include <stdio.h>

// Data wire is plugged into port 2 on the Arduino
// Connect a 4.7K resistor between VCC and the data pin (strong pullup)

//#define DHT11_PIN 4
#define DHT22_PIN 2


DHT22 myDHT22(DHT22_PIN);




//Delay between recording new sensor readings in milliseconds
//Remember to bare in mind that this won't include the time
//the device takes to execute the code (i.e. 1 or 2 secs) which
//will be added onto the sampling delay to give the sample rate.
//Range 0 -> 4 294 967 295
//600000 ms = 10 mins
//unsigned long sampleDelay = 600000;
unsigned long sampleDelay = 600000; //Time to wait between general temperature readings
//MAke sure that sampleDelay is longer than maxRetrys * readDelay!!!!!
unsigned long readDelay = 2000; //Time to wait between attempts to read DHT22
unsigned long numberRetrys = 3;




boolean DEBUG_MODE = true;


int count = 0;

void setup()
{
  //Wire.begin();
  Serial.begin(9600);
  pinMode(10, OUTPUT);
  SD.begin(10);
  Serial.println("starting");
  Serial.println("DHT TEST PROGRAM STALKER LOG5");
  Serial.print("LIBRARY VERSION: ");

  Serial.println();
  Serial.println("Type,\tstatus,\tHumidity (%),\tTemperature (C)");
  
  //  setTime(17,05,0,1,3,10); // set time to 17:05:00  1 Mar 2010 (see below)
  //  RTC.set(now());  // set the RTC to the current time (set in the previous line)
  // format for setting time - setTime(hr,min,sec,day,month,yr);


  setSyncProvider(RTC.get);   // the function to get the time from the RTC




}
void loop()
{
  
  if (DEBUG_MODE == true) {
    Serial.println("starting"); 
    
    Serial.print( "The time is ");
   
    Serial.print( year() );    
    Serial.write('/');
     
    Serial.print( month() );  
    Serial.write('/');
    
    
    Serial.print( day() );    
    Serial.write(' ');
     
    Serial.print( hour() );    
    Serial.write(':');
    
    
    Serial.print( minute() );    
    Serial.write(':');
    Serial.println( second() );
  }
  

  
DHT22_ERROR_t errorCode;
Serial.print("Requesting data...");
 
count = 0;
 while (errorCode != DHT_ERROR_NONE && count < numberRetrys) {
   
    // The sensor can only be read from every 1-2s, and requires a minimum
    // 2s warm-up after power-on.
    delay(readDelay);

    count++;
    Serial.println("# of trys to read data");
    Serial.println(count);
    errorCode = myDHT22.readData();
    switch(errorCode)
    {
      case DHT_ERROR_NONE:
        Serial.print("Got Data ");
        Serial.print(myDHT22.getTemperatureC());
        Serial.print("C ");
        Serial.print(myDHT22.getHumidity());
        Serial.println("%");
        // Alternately, with integer formatting which is clumsier but more compact to store and
  	  // can be compared reliably for equality:
  	  //	  
        char buf[128];
        sprintf(buf, "Integer-only reading: Temperature %hi.%01hi C, Humidity %i.%01i %% RH",
                     myDHT22.getTemperatureCInt()/10, abs(myDHT22.getTemperatureCInt()%10),
                     myDHT22.getHumidityInt()/10, myDHT22.getHumidityInt()%10);
        Serial.println(buf);
        break;
      case DHT_ERROR_CHECKSUM:
        Serial.print("check sum error ");
        Serial.print(myDHT22.getTemperatureC());
        Serial.print("C ");
        Serial.print(myDHT22.getHumidity());
        Serial.println("%");
        break;
      case DHT_BUS_HUNG:
        Serial.println("BUS Hung ");
        break;
      case DHT_ERROR_NOT_PRESENT:
        Serial.println("Not Present ");
        break;
      case DHT_ERROR_ACK_TOO_LONG:
        Serial.println("ACK time out ");
        break;
      case DHT_ERROR_SYNC_TIMEOUT:
        Serial.println("Sync Timeout ");
        break;
      case DHT_ERROR_DATA_TIMEOUT:
        Serial.println("Data Timeout ");
        break;
      case DHT_ERROR_TOOQUICK:
        Serial.println("Polled to quick ");
        break;
    }
    
    if (errorCode != DHT_ERROR_NONE) {
        //delay(2000);
        Serial.println("Failed to get data ");
    }

  }
    
  
  //Record the data in th SD card  
   File dataFile = SD.open("Tmp11.txt", FILE_WRITE); //Creat a file to record the temperature data 
   if (dataFile) {
 
    dataFile.print(year());     
    dataFile.print('/');
    dataFile.print( month() );  
    dataFile.print('/');
    dataFile.print( day() );    
    dataFile.print(' ');
    dataFile.print( hour() );    
    dataFile.print(':'); 
    dataFile.print( minute() );    
    dataFile.print(':'); 
    dataFile.print( second() );
    dataFile.print( ',' );
  
    dataFile.print(errorCode);
    dataFile.print( ',' );

    dataFile.print(myDHT22.getHumidity());
    dataFile.print( ',' );
        
    dataFile.print(myDHT22.getTemperatureC());
    dataFile.println( ',' );
    
    dataFile.close();
    
    // Print to the serial port 
    //Serial.print(DHT.humidity,1);
    //Serial.print(DHT.temperature,1);
  }  
  // If the file isn't open, pop up an error:
  else {
    Serial.println("error opening datalog.txt");
  }
  //}
  
  
  
  delay(sampleDelay-(count*readDelay)); //Delay until next sensor reading  
}


