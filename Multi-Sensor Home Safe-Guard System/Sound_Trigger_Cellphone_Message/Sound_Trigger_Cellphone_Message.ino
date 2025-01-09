
// Purpose: using Pushsaver framework API to send alarm message to cellphone
// When sound sensor successfully detectes loud sound, it will send out alarm message to a predefined cellphone number
// that has already registered with a Pushsaver account ID which will be used by calling Pushsafer API to push a
// text message to that cellphone #.

#include <ESP8266WiFi.h>
#include <WiFiClient.h>
#include <Pushsafer.h>

const int sound_Sensor_Pin = 8;  // using Digital Reading pin of 8 on Arduino board 
                                 // Output Value: HIGH -- within normal sound volumn threshold
                                 //               LOW  -- exceeding the predefined volumn threshold
                              
unsigned long start_Time, current_Time, sound_Length, sound_Number;

// Initialize Wifi connection to the router
char ssid[] = "AUM-W94M26192W";     // your network SSID (name)
char password[] = "Woodfish3";      // your network key

// Pushsafer private or alias key
#define Pushsafer_Key "dIaFiLtC5b905NoBEhRH"  // Pushsafer key for cellphone #: 1-832-580-6930, the Pushsafer account holder's ID

//WiFi Client Secure connection, using WiFiClient and Pushsafer API
WiFiClient client;
Pushsafer pushsafer;

void setup() 
{
  pinMode(sound_Sensor_Pin, INPUT);
  Serial.begin(9600); //using serial baud rate of 9600

  sound_Number = 1;       
  sound_Length = 125; // a big sound should exceed the threshold volumn value defineded by manually rotating the nobe on sound sensor
                    // and last minimun 125 millisecond; Otherwise, it wiil be considered as noise

  pushsafer(Pushsafer_Key, client); // register the cellphone number's Pushasfer account holder's ID and the WiFi client connection

  // Set WiFi to station mode and disconnect from an AP if it was Previously connected
  WiFi.mode(WIFI_STA);
  WiFi.disconnect();
  delay(100);

  // Attempt to connect to Wifi network:
  Serial.print("Connecting Wifi: ");
  Serial.println(ssid);
  WiFi.begin(ssid, password);

  while (WiFi.status() != WL_CONNECTED) 
  {
    Serial.print("."); // while in process try to connect to WiFi
    delay(500);
  }

  Serial.println("");
  Serial.println("WiFi connected");
  Serial.print("IP address: ");
  Serial.println(WiFi.localIP());

  pushsafer.debug = true;
  
  startTime = millis();  // use the current time as starting time
  Serial.println(startTime);
}


void loop() 
{
  int sensor_Data = digitalRead(sound_Sensor_Pin);
  
  // Debugging process, monitor the sound sensor output
  Serial.print("In sound sensor debugging mode:  ");
  Serial.println( sound_Number++);  //for debugging monitoring
  delay (500);
  
   if (sensor_Data == LOW) // detected a loud sound exceeding the volumn threshold when sound sensor output value changed from HIGH to LOW
   {
      current_Time = millis();   // get the current time;
      
      if (current_Time - start_Time > sound_Length) // it is not background noise
      {
        Serial.print("Loud sound detected ");
        Serial.println( sound_Number++);
  
        sound_Triger_Send_Cellphone_Message(); // call to send alarm message to predefined cellphone number
        
        startTime = millis();
      }
   } //end of detected a loud sound
} // end of loop


void sound_Triger_Send_Cellphone_Message()
{
  struct PushSaferInput input;
  input.message = "Grandma needs help! Loud sound detected from grandma's home: please turn on grandma's in room video camera remotely from your cell phone to check and talk with gramma  ";
  input.title = "Loud Sound Emergency!";
  input.sound = "8";
  input.vibration = "1";
  input.icon = "1";
  input.iconcolor = "#FFCCCC";
  input.priority = "2";
  input.device = "a";
  input.url = "https://www.pushsafer.com";
  input.urlTitle = "Open Pushsafer.com";
  input.picture = "";
  input.picture2 = "";
  input.picture3 = "";
  input.time2live = "";
  input.retry = "";
  input.expire = "";
  input.confirm = "";
  input.answer = "";
  input.answeroptions = "";
  input.answerforce = "";

  Serial.println(pushsafer.sendEvent(input));
  Serial.println("Alarm Message Sent to cellphone");
}
