/*gsm test */

#define RXD2 16
#define TXD2 17
#define Debug  1 
const char *APN = "Internet.vodafone.net";

void setup()
{
  //Begin serial communication with Arduino and Arduino IDE (Serial Monitor)
  Serial.begin(19200);
  
  //Begin serial communication with Arduino and SIM900A
  Serial2.begin(19200, SERIAL_8N1, RXD2, TXD2);


  Serial.println("Initializing...");
  delay(3000);

  sendCommand("AT"); //Once the handshake test is successful, it will back to OK
  updateSerial();
  sendCommand("AT+CSQ"); //Signal quality test, value range is 0-31 , 31 is the best
  updateSerial();
  sendCommand("AT+CCID"); //Read SIM information to confirm whether the SIM is plugged
  updateSerial();
  sendCommand("AT+CREG?"); //Check whether it has registered in the network
  updateSerial();
  sendCommand("AT"); //Once the handshake test is successful, it will back to OK
  updateSerial();
    sendCommand("AT"); //Handshaking with SIM900
  // updateSerial();
sendGetRequest() ; 
//   sendCommand("AT+CMGF=1"); // Configuring TEXT mode
//   updateSerial();
//  sendCommand("AT+CMGS=\"+201551008479\"");//change ZZ with country code and xxxxxxxxxxx with phone number to sms
//   updateSerial();
//   sendCommand("ESlam"); //text content
//   updateSerial();
//   // sendCommand(26);


}

void loop()
{
  updateSerial();
}

void updateSerial()
{
  delay(500);
  while (Serial.available()) 
  {
    Serial2.write(Serial.read());//Forward what Serial received to Software Serial Port
  }
  while(Serial2.available()) 
  {
    Serial.write(Serial2.read());//Forward what Software Serial received to Serial Port
  }
}

/******************************************************************/
/*function to Send command */
/*@parm  : const char string */
void sendCommand(const char* command) {
  Serial2.println(command);
  #if Debug == 1 
  ShowSerialData();
  #endif 
}
/******************************************************************/

void ShowSerialData() {
  Serial.println("Show serial data:");
  while (Serial2.available()) {
    char c = Serial2.read();
    Serial.write(c);
  }
  Serial.println("");
  delay(1000);
}




void sendGetRequest(){

  //Check if the module is responsive, expected value OK
  sendCommand("AT"); 
  //close or turn off network connection in case it was left open, expected value OK
  sendCommand("AT+CIPSHUT"); 
  // close GPRS context bearer in case it was left open, expected value OK
  sendCommand("AT+SAPBR=0,1"); 
  // open GPRS context establish GPRS connection
  sendCommand("AT+SAPBR=3,1,\"Contype\",\"GPRS\"");
  //Set the Access Point Name (APN) for the network provider
  //change this apn value for your SIM card
  sendCommand("AT+SAPBR=3,1,\"Internet.vodafone.net\",\"internet.mtn\"");
  //open GPRS context bearer
  sendCommand("AT+SAPBR=1,1");
  //initiate HTTP request
  sendCommand("AT+HTTPINIT");
  //set parameters for http session, HTTP context identifier
  sendCommand("AT+HTTPPARA=\"CID\",1");
   //Change the URL from google.com to the server you want to reach
  sendCommand("AT+HTTPPARA=\"URL\",\"https://google.com/\"");
  //Initiate the HTTP GET request, send http request to specified URL
  sendCommand("AT+HTTPACTION=0");
  // Wait for the response (adjust the delay as needed)
  delay(9000); 
  // Read the HTTP response, normally contains status code 200 if successful
  sendCommand("AT+HTTPREAD");
  //Terminate the HTTP service
  sendCommand("AT+HTTPTERM");
  //close or turn off network connection
  sendCommand("AT+CIPSHUT");
  // close GPRS context bearer
  sendCommand("AT+SAPBR=0,1");
  

}


void testPostRequest(String jsonToSend){
  //Example format of JSON:
  // String jsonToSend="{\"uploadedAt\":\"2023-06-26T20:18:22.826Z\",\"data\":[{\"unit\":\"C\",\"reading\":31}]}";
  

  sendCommand("AT");
  ShowSerialData();
  sendCommand("AT+CIPSHUT");
  ShowSerialData();
  delay(500);
  sendCommand("AT+SAPBR=0,1");
  delay(2000);
  
  ShowSerialData();
  sendCommand("AT+SAPBR=3,1,\"Contype\",\"GPRS\"");
  ShowSerialData();
  sendCommand("AT+SAPBR=3,1,\"APN\",\"internet.apn\"");
  ShowSerialData();
  sendCommand("AT+SAPBR=1,1");
  delay(2000);
  ShowSerialData();
  sendCommand("AT+HTTPINIT");
  delay(1000);
  ShowSerialData();
  sendCommand("AT+HTTPPARA=\"CID\",1");
  ShowSerialData();
  sendCommand("AT+HTTPPARA=\"URL\",\"http://example.com/data\"");
  ShowSerialData();
  sendCommand("AT+HTTPPARA=\"CONTENT\",\"application/json\"");
  ShowSerialData();
  sendCommand(("AT+HTTPDATA=" + String(jsonToSend.length()) + ",20000").c_str());
  delay(6000);
  Serial2.println(jsonToSend);
  delay(16000);
  ShowSerialData();
  sendCommand("AT+HTTPACTION=1");
  delay(20000);
  ShowSerialData();
  sendCommand("AT+HTTPREAD");
  ShowSerialData();
  sendCommand("AT+HTTPTERM");
  ShowSerialData();
  sendCommand("AT+CIPSHUT");
  ShowSerialData();
}