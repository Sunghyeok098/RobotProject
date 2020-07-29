#include <DHT11.h> //온습도센서 library function 포함 
#include<LiquidCrystal.h> //LiquidCrysal library function 포함

const int value1 = 40; //적정 습도의 최저습도
const int value2 = 60; //적정 습도의 최대습도

const int ledPin_R = 11; //Red Pin 정의
const int ledPin_G = 12; //Green Pin 정의
const int ledPin_B = 13; //Blue Pin 정의
const int atom_pin = 10; //UltraSonic transducer Pin 정의
const int Sensor_Pin = 9; //온습도센서 Pin 정의
const int buttonPin_1 = A2; //ON/OFF Pin 정의
const int buttonPin_2 = A3; //timer Pin 정의
const int buttonPin_3 = A4; //count Pin 정의

int count=0;                 // count 만큼 time 늘어남
int ledState = HIGH;         // the current state of the output pin
int buttonState;             // the current reading from the input pin
int lastButtonState = LOW;   // the previous reading from the input pin
long lastDebounceTime = 0;   // the last time the output pin was toggled
long debounceDelay = 50;     // the debounce time; increase if the output flickers


DHT11 dht11(Sensor_Pin); //온습도센서 object 생성 
LiquidCrystal lcd(2,3,4,5,6,7); //LCD object 생성

//colorRGB function parameter value 만큼 LED를 밝힌다.
void colorRGB(int Red_value, int Green_value, int Blue_value){
  analogWrite(ledPin_R, Red_value); //Red value에 따라 LED를 킨다.
  analogWrite(ledPin_G, Green_value); //Green value에 따라 LED를 킨다.
  analogWrite(ledPin_B, Blue_value); //Blue value에 따라 LED를 킨다.
 }

//A3 button Pin을 누른 수 만큼 가습기 작동
 void timer(){
   int reading_2 = digitalRead(buttonPin_2); //A3 Pin의 값을 읽어옴
   int reading_3 = digitalRead(buttonPin_3); //A4 Pin의 값을 읽어옴
   
   if(reading_3 == HIGH){ //A3 Pin value 가 HIGH 라면
      count++; //초 늘어남
      Serial.println(count); //Serial moniter에 표시
    }
    
   if(reading_2 == HIGH){ //A4 Pin value 가 HIGH 라면
      digitalWrite(atom_pin, HIGH); //가습기 ON
      delay(count*1000); //A3 누른 수만큼 
      count=0; //초 초기화
    }
    
}

void setup(){
  
   Serial.begin(9600); 
   lcd.begin(16,2); //LCD 정의
   pinMode(ledPin_R ,OUTPUT); //Red Pin을 OUTPUT으로 사용
   pinMode(ledPin_G ,OUTPUT); //Green Pin을 OUTPUT으로 사용
   pinMode(ledPin_B ,OUTPUT); //Blue Pin을 OUTPUT으로 사용
   pinMode(atom_pin, OUTPUT); //UltraSonic transducer Pin을 OUPUT으로 사용
   pinMode(Sensor_Pin, INPUT); //Sensor Pin 을 INPUT으로 사용
   pinMode(buttonPin_1, INPUT); //ON/OFF Pin 을 INPUT으로 사용
   pinMode(buttonPin_2, INPUT); //timer Pin 을 INPUT으로 사용
   pinMode(buttonPin_3, INPUT); //count Pin 을 INPUT으로 사용
}

void loop(){
  
  int error;
  float temp, humi; //온도 값과 습도 값의 변수 생성.

  //온도 값과 습도 값이 정확히 읽혀진다면
  if((error = dht11.read(humi, temp))==0){
    
    Serial.print("temperature:"); 
    Serial.print(temp);
    Serial.print(" humidity:");
    Serial.print(humi);
    Serial.println(); 
    //Serial Moniter 에 출력
  }

  timer(); //timer function 실행
  lcd.setCursor(0,0);
  lcd.print("temperature : "); //LCD에 temperature 표시
  lcd.print(temp); //LCD에 온도 값 표시
  
  lcd.setCursor(0,1); //cursor을 0,1로 옮긴다
  lcd.print("humidity : "); //LCD에 Humidity 표시
  lcd.print(humi); //LCD에 습도 값 표시

  if(humi < value1){
    colorRGB(255, 0, 0); //습도가 적정습도 최저보다 낮으면 빨간색 불을 ON 한다.
  } 
  else if(humi >= value1 &&  humi < value2){
    colorRGB(0, 255, 0); //습도가 적정습도 사이에 있으면 초록색 불을 ON 한다.
  }
  else{
    colorRGB(0, 0, 255); //습도가 적정습도 최대보다 높으면 파란색 불을 ON 한다.
  }
  
  int reading_1 = digitalRead(buttonPin_1); //A2 Pin value을 읽어옴
  
  if (reading_1 != lastButtonState) { 
    
    lastDebounceTime = millis(); //switch 값이 변한다면, dedouncing timer을 초기화함
  }
  
  if ((millis() - lastDebounceTime) > debounceDelay) {
    // whatever the reading is at, it's been there for longer
    // than the debounce delay, so take it as the actual current state:

      // if the button state has changed:
      if (reading_1 != buttonState) {
      buttonState = reading_1;

       // only toggle the LED if the new button state is HIGH
      if (buttonState == HIGH) {
        ledState = !ledState;
      }
    }
  }

  digitalWrite(atom_pin, ledState); //button 눌렀을 때 가습기 ON/OFF
                                    //적정습도범위의 습도일 경우에만 ON/OFF 하도록함
                                    
  if(humi > value2){
    ledState = LOW; //상태를 LOW로 함
    digitalWrite(atom_pin, ledState); //적정습도 보다 높으면 가습기 OFF
  }
  
  if(humi < value1){
    ledState = HIGH; //상태를 HIGH로 함
    digitalWrite(atom_pin, ledState); //적정습도 보다 낮으면 가습기 ON
  }
  
  lastButtonState = reading_1; //마지막 버튼상태를 A2읽은 값으로 설정함

  delay(1000); 
  lcd.clear(); //LCD clear

}
