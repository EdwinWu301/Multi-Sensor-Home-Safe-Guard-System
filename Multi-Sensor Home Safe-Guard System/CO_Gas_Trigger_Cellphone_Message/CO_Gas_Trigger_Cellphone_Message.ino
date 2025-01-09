
// Purpose: 
// When CO_Gas sensor successfully detectes CO gas leakage, it will send out alarm message to a predefined cellphone number
// that has already registered with a Pushsaver account ID which will be used by calling Pushsafer API to push a
// text message to that cellphone #.

#include <ESP8266WiFi.h>
#include <WiFiClient.h>
#include <Pushsafer.h>

const int CO_Gas_Sensor_Pin = 2;  // using Digital Reading pin of 2 on Arduino board 
                                // Output Value: HIGH -- within normal CO_Gas intensity threshold
                                //               LOW  -- exceeding the CO_Gas intensity threshold
                              
unsigned long start_Time, current_Time, CO_Gas_Length, CO_Gas_Number;

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
  pinMode(CO_Gas_Sensor_Pin, INPUT);
  Serial.begin(9600); //using serial baud rate of 9600

  odor_Number = 1;       
  odor_Length = 150; // a CO_Gas intensity should exceed the threshold value defineded by manually rotating the nobe on CO_Gas sensor
                     // and last minimun 150 millisecond; Otherwise, it wiil be considered as noise data

  pushsafer(Pushsafer_Key, client); // register the cellphone number's Pushasfer account holder's ID and the WiFi client connection

  WiFi.mode(WIFI_STA);  // Set WiFi to station mode and disconnect from an AP if it was Previously connected

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
  Serial.println(start_Time);
}


void loop() 
{
  int sensor_Data = digitalRead(CO_Gas_Sensor_Pin);
  
  // Debugging process, monitor the odor sensor output
  Serial.print("In CO_Gas sensor debugging mode:  ");
  Serial.println(CO_Gas_Number++);  //for debugging monitoring
  delay (100); // every 100 milisecond read the CO_Gas sensor data
  
  if (sensor_Data == LOW) // detected an intensive CO_Gas leakage exceeding the threshold 
                          // when CO_Gas sensor output value changed from HIGH to LOW
  {
    currentTime = millis();   // get the current time;
    
    if (current_Time - start_Time > CO_Gas_Length) // it is not background noise
    {
      Serial.print("Excessive CO_Gas leakage detected ");
      Serial.println(CO_Gas_Number++);

      co_Gas_Triger_Send_Cellphone_Message(); // call to send alarm message to predefined cellphone number
      
      start_Time = millis();
    }
  } //end of detected an excessive odor smell
} // end of loop


void co_Gas_Triger_Send_Cellphone_Message()
{
  struct PushSaferInput input;
  input.message = "Grandma needs help! Excessive CO_Gas leakage detected from grandma's home: please turn on grandma's in room security camera remotely from your cell phone to check and talk with gramma  ";
  input.title = "Excessive smell Emergency!";
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
