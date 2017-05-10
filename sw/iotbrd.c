#include <Wire.h>
#include <RTClib.h>
#include <U8x8lib.h>
#include <SimpleDHT.h>
#include <Servo.h>


//Definiciones de pines

//Pines de salida
#define _OUT_LED_RGB_R_         5
#define _OUT_LED_RGB_G_         3
#define _OUT_LED_RGB_B_         6
#define _OUT_MOSFET_            2
#define _OUT_RELE_              4
#define _OUT_BUZZ_              7
#define _OUT_SERVO_             9

// Pines de Entrada
#define _IN_A0_POT_             A0
#define _IN_A2_POT_             A2
#define _IN_A0_LM35_            A0
#define _IN_A1_DHT11_           15



//Pines de Control
#define _NFR_CS_                8
#define _NFR_CE_                10

//Contantes de control
#define _LED_RGB_ON_            0
#define _LED_RGB_OFF_           1
#define _MOSFET_ON_             1
#define _MOSFET_OFF_            0
#define _BUZZ_ON_               1
#define _BUZZ_OFF_              0
#define _RELE_ON_               1
#define _RELE_OFF_              0



//MACROS
#define ledRGB_ON_R()   digitalWrite(_OUT_LED_RGB_R_, _LED_RGB_ON_)
#define ledRGB_ON_G()   digitalWrite(_OUT_LED_RGB_G_, _LED_RGB_ON_)
#define ledRGB_ON_B()   digitalWrite(_OUT_LED_RGB_B_, _LED_RGB_ON_)
#define ledRGB_OFF_R()  digitalWrite(_OUT_LED_RGB_R_, _LED_RGB_OFF_)
#define ledRGB_OFF_G()  digitalWrite(_OUT_LED_RGB_G_, _LED_RGB_OFF_)
#define ledRGB_OFF_B()  digitalWrite(_OUT_LED_RGB_B_, _LED_RGB_OFF_)

#define mosfetON()      digitalWrite(_OUT_MOSFET_, _MOSFET_ON_)
#define mosfetOFF()     digitalWrite(_OUT_MOSFET_, _MOSFET_OFF_)

#define buzzON()      digitalWrite(_OUT_BUZZ_, _BUZZ_ON_)
#define buzzOFF()     digitalWrite(_OUT_BUZZ_, _BUZZ_OFF_)

#define releON()      digitalWrite(_OUT_RELE_, _RELE_ON_)
#define releOFF()     digitalWrite(_OUT_RELE_, _RELE_OFF_)


// Define variables Globales
U8X8_SSD1306_128X64_NONAME_HW_I2C lcdi2c(U8X8_PIN_NONE);
RTC_DS1307      rtc;
SimpleDHT11     dht11;
Servo           myServo;  // create a servo object

// char daysOfTheWeek[7][12] = {"Domingo", "Lunes", "Martes", "Miercoles", "Jueves", "Viernes", "Sabado"};

void iniIOTbrdUD(){
    analogReference(INTERNAL);
    Serial.begin(9600);             // Para el debug via USB o Bluetooth

    // Configura LED
    pinMode(_OUT_LED_RGB_R_, OUTPUT);
    pinMode(_OUT_LED_RGB_G_, OUTPUT);
    pinMode(_OUT_LED_RGB_B_, OUTPUT);
    ledRGB_OFF_R();
    ledRGB_OFF_G();
    ledRGB_OFF_B();

    // Configura MOSFET
    pinMode(_OUT_MOSFET_, OUTPUT);
    mosfetOFF();

    // Configura Buzzer
    pinMode(_OUT_BUZZ_, OUTPUT);
    buzzOFF();

    // Configura Rele
    pinMode(_OUT_RELE_, OUTPUT);
    releOFF();

    // Configura Servo
    pinMode(_OUT_SERVO_, OUTPUT);
    myServo.attach(9); // attaches the servo on pin 9 to the servo object

    // Configura RTC
    rtc.begin();
    if (! rtc.isrunning()) rtc.adjust(DateTime(F(__DATE__), F(__TIME__)));

    // Configura LCD
    lcdi2c.begin();
    lcdi2c.setFont(u8x8_font_amstrad_cpc_extended_r);
    // lcdi2c.setCursor(0, 0); // x,y || [0-15 , 0-7] 16X8
    // lcdi2c.print("IoT UD");
    lcdi2c.clear();
    lcdi2c.draw2x2String(4, 1, "BOARD");
    lcdi2c.draw2x2String(3, 4, "IoT UD");
    delay(1000);
}

void demoRGB_1(){
    lcdi2c.clear();
    lcdi2c.draw2x2String(1, 1, "DEMO #1");
    lcdi2c.draw2x2String(1, 4, "LED RGB");

    ledRGB_ON_R();
    delay(2000);
    ledRGB_OFF_R();
    ledRGB_ON_G();
    delay(2000);
    ledRGB_OFF_G();
    ledRGB_ON_B();
    delay(2000);
    ledRGB_ON_R();
    delay(2000);
    ledRGB_OFF_B();
    ledRGB_ON_G();
    delay(2000);
    ledRGB_OFF_R();
    ledRGB_ON_B();
    delay(2000);
    ledRGB_OFF_B();
    ledRGB_OFF_G();
}

void demoRGB_2(){
    lcdi2c.clear();
    lcdi2c.draw2x2String(1, 1, "DEMO #2");
    lcdi2c.draw2x2String(1, 4, "LED RGB");
    int brillo = 255;
    while(brillo >= 0){
        analogWrite(_OUT_LED_RGB_R_, brillo);
        analogWrite(_OUT_LED_RGB_G_, brillo);
        analogWrite(_OUT_LED_RGB_B_, brillo);
        delay(20);
        brillo--;
    }
    brillo = 0;
    while(brillo <= 255){
        analogWrite(_OUT_LED_RGB_R_, brillo);
        analogWrite(_OUT_LED_RGB_G_, brillo);
        analogWrite(_OUT_LED_RGB_B_, brillo);
        delay(20);
        brillo++;
    }
}

void demoBuzzer(){
    lcdi2c.clear();
    lcdi2c.draw2x2String(5, 1, "DEMO");
    lcdi2c.draw2x2String(3, 4, "BUZZER");
    buzzON();
    delay(1000);
    buzzOFF();
    delay(1000);
    buzzON();
    delay(100);
    buzzOFF();
    delay(100);
    buzzON();
    delay(100);
    buzzOFF();
}

void demoRele(){
    lcdi2c.clear();
    lcdi2c.draw2x2String(5, 1, "DEMO");
    lcdi2c.draw2x2String(5, 4, "RELE");

    releON();
    delay(5000);
    releOFF();
}

void demoMosfet(){
    lcdi2c.clear();
    lcdi2c.draw2x2String(5, 1, "DEMO");
    lcdi2c.draw2x2String(4, 4, "MOSFET");

    mosfetON();
    delay(5000);
    mosfetOFF();
}

void demoSensores(){
    lcdi2c.clear();
    lcdi2c.draw2x2String(5, 1, "DEMO");
    lcdi2c.draw2x2String(0, 4, "SENSORES");
    delay(2000);
    lcdi2c.clear();

    for(uint16_t i=0; i<1000; i++){
        // lcdi2c.clear();
        lcdi2c.setCursor(0, 0);
        DateTime now = rtc.now();
        lcdi2c.print(now.year(), DEC);
        lcdi2c.print('/');
        lcdi2c.print(now.month(), DEC);
        lcdi2c.print('/');
        lcdi2c.print(now.day(), DEC);
        lcdi2c.setCursor(0, 1);
        lcdi2c.print(now.hour(), DEC);
        lcdi2c.print(':');
        lcdi2c.print(now.minute(), DEC);
        lcdi2c.print(':');
        lcdi2c.print(now.second(), DEC);

        Serial.print(now.year(), DEC);
        Serial.print('/');
        Serial.print(now.month(), DEC);
        Serial.print('/');
        Serial.println(now.day(), DEC);
        Serial.print(now.hour(), DEC);
        Serial.print(':');
        Serial.print(now.minute(), DEC);
        Serial.print(':');
        Serial.println(now.second(), DEC);

        lcdi2c.setCursor(0, 3);
        lcdi2c.print("*C: ");
        Serial.print("*C: ");
        uint16_t tempC = analogRead(_IN_A0_LM35_) / 9.31;
        lcdi2c.print(tempC);
        Serial.println(tempC);

        lcdi2c.setCursor(0, 4);
        lcdi2c.print("%H: ");
        Serial.print("%H: ");
        byte humidity = 0;
        byte temperature = 0;
        dht11.read(_IN_A1_DHT11_, &temperature, &humidity, NULL);
        lcdi2c.print((int)humidity);
        Serial.println((int)humidity);


        lcdi2c.setCursor(0, 5);
        uint16_t potVal = analogRead(_IN_A2_POT_); // read the value of the potentiometer
        // print out the value to the serial monitor
        lcdi2c.print("potVal: ");
        lcdi2c.print(potVal);
        // scale the numbers from the pot
        uint16_t angle = map(potVal, 0, 1023, 0, 179);
        // print out the angle for the servo motor
        myServo.write(angle);



        delay(500);
    }
}

void setup() {
    iniIOTbrdUD();
}



void loop() {

    demoRGB_1();
    demoRGB_2();
    demoBuzzer();
    demoRele();
    demoMosfet()
;    demoSensores();

}



