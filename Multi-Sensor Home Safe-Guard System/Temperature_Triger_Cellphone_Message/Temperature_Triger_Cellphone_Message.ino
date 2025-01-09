// Purpose: 
// when temperature sensor detectes temperature outof allowed range it will use Pushsaver framework API 
// to send out alarm message to a predefined cellphone number that has already registered with a Pushsaver 
// account ID which will be used by calling Pushsafer API to push a text message to that cellphone #.


const int temp_Sensor_Pin = A0;  // select the temperature sensor analog reading pin
const float high_Temp = 39.0;
const float low_Temp = 15.0;

void setup()
{
    pinMode(temp_Sensor_Pin, INPUT);
    Serial.begin(9600);  //using serial baud rate of 9600
}

void loop() 
{
   int temp_Value;
   temp_Value = analogRead(temp_Pin); // read analog value from temperature sensor pin and save it to variable tempValue
                                      // the temperature sensor analog reading value range is: (0 to 1023), represents voltage value range of: (0 - 5V)

   float voltage;
   voltage = (temp_Value * 5.0) / 1024.0; // convert the analog reading value: (0 to 1023) to voltage value: (0 - 5V)
                                          // Formula: V = reading from temperature sensor * (5.0 / 1024.0)

   float temp_Celsius = (voltage - 0.5) * 100 ; // convert to its temperature equivalent in Celsius degree
                                                // Formula: Temperature (°C) = (Voltage – 0.5) * 100
 
   if (temp_Celsius > high_Temp) // detected an excessive high room temperature
   {
        Serial.print("Excessive High room temperature in celsius degree of: ");    
   } 
   else if (temp_Celsius < low_Temp) // detected an excessive low room temperature
   {
        Serial.print("Excessive low room temperature in celsius degree of: ");
   }
   Serial.println(temp_Celsius); 
   temperature_Triger_Send_Cellphone_Message(temp_Celsius); // call to send alarm message to predefined cellphone number
        
   delay(1000); // update sensor reading each one second
}

void temperature_Triger_Send_Cellphone_Message(float temp_Value)
{
  struct PushSaferInput input;
  
  if (temp_Value > high_Temp)
         input.message = "Grandma needs help! High temperature detected from grandma's home: please turn on grandma's in room security camera remotely from your cell phone to check and talk with gramma  ";
  else 
         input.message = "Grandma needs help! Low temperature detected from grandma's home: please turn on grandma's in room security camera remotely from your cell phone to check and talk with gramma  ";

  input.title = "Excessive Room Temperature Emergency!";
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
