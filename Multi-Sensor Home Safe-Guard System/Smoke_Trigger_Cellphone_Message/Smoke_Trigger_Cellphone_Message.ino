
// Purpose: 
// When smoke sensor successfully detectes excessive smoke, it will send out alarm message to a predefined cellphone number
// that has already registered with a Pushsaver account ID which will be used by calling Pushsafer API to push a
// text message to that cellphone #.


#include <ESP8266WiFi.h>
#include <WiFiClient.h>
#include <Pushsafer.h>

const int smoke_Sensor_Pin = 4; // using Digital Reading pin of 4 on Arduino board 
                                // Output Value: HIGH -- within normal smoke intensity threshold
                                //               LOW  -- exceeding the smoke intensity threshold
                              
unsigned long start_Time, current_Time, smoke_Length, smoke_Number;

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
  pinMode(smoke_Sensor_Pin, INPUT);
  Serial.begin(9600); //using serial baud rate of 9600

  odor_Number = 1;       
  odor_Length = 250; // a strong smoke should exceed the threshold value defineded by manually rotating the nobe on smoke sensor
                     // and last minimun 250 millisecond; Otherwise, it wiil be considered as noise data

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
    delay(100);  
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
  int sensor_Data = digitalRead(smoke_Sensor_Pin);
  
  // Debugging process, monitor the odor sensor output
  Serial.print("In odor sensor debugging mode:  ");
  Serial.println(smoke_Number++);  //for debugging monitoring
  delay (100);  // every 100 milisecond read the smoke sensor data
  
  if (sensor_Data == LOW) // detected an intensive smoke exceeding the threshold when smoke sensor output value changed from HIGH to LOW
  {
    currentTime = millis();   // get the current time;
    
    if (current_Time - start_Time > smoke_Length) // it is not background noise
    {
      Serial.print("Excessive odor detected ");
      Serial.println( smoke_Number++);

      smoke_Triger_Send_Cellphone_Message(); // call to send alarm message to predefined cellphone number
      
      start_Time = millis();
    }
  } //end of detected an excessive smoke
} // end of loop


void smoke_Triger_Send_Cellphone_Message()
{
  struct PushSaferInput input;
  input.message = "Grandma needs help! Excessive smoke detected from grandma's home: please turn on grandma's in room security camera remotely from your cell phone to check and talk with gramma  ";
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