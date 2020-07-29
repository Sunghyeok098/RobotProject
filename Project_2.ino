const int Depth_Sensor = A0; //Depth Sensor Pin 정의
const int Sound_Pin = 10; //speaker pin 정의
const int ledPin_R = 11; //Red Pin 정의
const int ledPin_G = 12; //Green Pin 정의
const int ledPin_B = 13; //Blue Pin 정의

void setup() {
  Serial.begin(9600);
  pinMode(Sound_Pin, OUTPUT); //Speaker Pin을 OUTPUT으로 사용한다.
  pinMode(Depth_Sensor, INPUT); //Depth Sensor을 INPUT으로 사용한다.
}
 
//Detect_Depth function : detect depth of water, make a sound, turn on/off LED
void Detect_Depth(){
  
  int depth = analogRead(Depth_Sensor); //Depth_Sensor의 값을 읽고 depth 저장한다.
                                        //Depth_Sensor의 return 값은 0 ~ 1023 1023에 가까울수록 물이 부족함.
  Serial.println(depth); //Serial monitor 에서 확인

  
  if(depth > 900){ //depth 이 600 보다 크다면
    colorRGB(255,0,0); //LED 빨간불 ON
    colorRGB(0, 0, 0); 
    digitalWrite(Sound_Pin, HIGH); //Speaker 0.8초동안 ON
    delay(800);
    digitalWrite(Sound_Pin, LOW);  //Speaker 0.8초동안 OFF
    delay(800);
  }
  
  else{ //depth 이 600보다 작으면
    colorRGB(0, 0, 0); //LED OFF 
    digitalWrite(Sound_Pin, LOW); //Speaker OFF 
    delay(800);
  }
}

//colorRGB function : parameter value 만큼 LED를 밝힌다.
void colorRGB(int Red_value, int Green_value, int Blue_value){
  analogWrite(ledPin_R, Red_value); //Red value에 따라 LED를 킨다.
  analogWrite(ledPin_G, Green_value); //Green value에 따라 LED를 킨다.
  analogWrite(ledPin_B, Blue_value); //Blue value에 따라 LED를 킨다.
  delay(1000);
}
  


void loop() {
  Detect_Depth();  //Depth sensor 실행 function.
}
