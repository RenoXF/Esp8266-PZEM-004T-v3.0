#include <LiquidCrystal_I2C.h>
#include <PZEM004Tv30.h>
#include <SoftwareSerial.h>
#include <Wire.h>

#define BLYNK_TEMPLATE_ID           "xxxxx"
#define BLYNK_TEMPLATE_NAME         "xxxxxxxx"
#define BLYNK_AUTH_TOKEN            "xxxxxxxxxxxx"

#define BLYNK_PRINT Serial

#include <ESP8266WiFi.h>
#include <BlynkSimpleEsp8266.h>

// Your WiFi credentials.
// Set password to "" for open networks.
char ssid[] = "your wifi ssid";
char pass[] = "your wifi password";

LiquidCrystal_I2C lcd(0x27, 20, 4);

#if defined(ESP32)
    #error "Software Serial is not supported on the ESP32"
#endif

/* Use software serial for the PZEM
 * Pin 12 Rx (Connects to the Tx pin on the PZEM)
 * Pin 13 Tx (Connects to the Rx pin on the PZEM)
*/
#if !defined(PZEM_RX_PIN) && !defined(PZEM_TX_PIN)
#define PZEM_RX_PIN 12
#define PZEM_TX_PIN 13
#endif

SoftwareSerial pzemSWSerial(PZEM_RX_PIN, PZEM_TX_PIN);
PZEM004Tv30 pzem(pzemSWSerial);

void setup() {
    /* Debugging serial */
    Serial.begin(115200);
    Blynk.begin(BLYNK_AUTH_TOKEN, ssid, pass);
    lcd.init();
    lcd.clear();
    lcd.backlight();
    
    //pzem.resetEnergy();
}

void loop() {
         
    Serial.print("Custom Address:");
    Serial.println(pzem.readAddress(), HEX);

    // Read the data from the sensor
    float voltage = pzem.voltage();
    float current = pzem.current();
    float power = pzem.power();
    float energy = pzem.energy();
    float frequency = pzem.frequency();
    float pf = pzem.pf();
    float harga = 1.352;

    // Check if the data is valid
    if(isnan(voltage)){
        Serial.println("Error reading voltage");
    } else if (isnan(current)) {
        Serial.println("Error reading current");
    } else if (isnan(power)) {
        Serial.println("Error reading power");
    } else if (isnan(energy)) {
        Serial.println("Error reading energy");
    } else if (isnan(frequency)) {
        Serial.println("Error reading frequency");
    } else if (isnan(pf)) {
        Serial.println("Error reading power factor");
    } else {

        // Print the values to the Serial console
        Serial.print("Voltage: ");      Serial.print(voltage);      Serial.println("V");
        Serial.print("Current: ");      Serial.print(current);      Serial.println("A");
        Serial.print("Power: ");        Serial.print(power);        Serial.println("W");
        Serial.print("Energy: ");       Serial.print(energy,3);     Serial.println("kWh");
        Serial.print("Frequency: ");    Serial.print(frequency, 1); Serial.println("Hz");
        Serial.print("PF: ");           Serial.println(pf);
        Serial.print("Harga Kwh: Rp.");    Serial.print(energy,3 * harga);
    }

    Serial.println();
    delay(5000);
    lcd.clear();
    lcd.setCursor(0, 0);
    lcd.print("Voltase :");
    lcd.setCursor(9, 0);
    lcd.print(voltage);
    lcd.setCursor(0,1);
    lcd.print("Ampere  :");
    lcd.setCursor(9, 1);
    lcd.print(current);
    lcd.setCursor(0, 2);
    lcd.print("Watt    :");
    lcd.setCursor(9, 2);
    lcd.print(power);
    lcd.setCursor(0, 3);
    lcd.print("kWh     :");
    lcd.setCursor(9, 3);
    lcd.print(energy,3);
    delay(5000);
    lcd.clear(); 
    lcd.setCursor(1, 0);
    lcd.print("MONITORING LISTRIK");
    lcd.setCursor(0, 1);
    lcd.print("Artstika  Decoration");
    lcd.setCursor(1, 2);
    lcd.print("Listrik 900Va");
    lcd.setCursor(1, 3);
    lcd.print("Rp.");
    lcd.setCursor(4, 3);
    lcd.print(energy,3 * harga);
    delay(5000);

    Blynk.virtualWrite(V10, voltage);
    Blynk.virtualWrite(V11, current);
    Blynk.virtualWrite(V12, power);
    Blynk.virtualWrite(V13, energy,3);
    Blynk.virtualWrite(V14, energy,3 * harga);
    Blynk.virtualWrite(V15, frequency, 1);
    Blynk.virtualWrite(V16, pf);
    
    Blynk.run();
}