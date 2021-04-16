// Nama Tim : sinar matahari
// Nama Anggota 1 : hannayara
// Nama Anggota 2 : nadia apdila

// Deklarasi Variabel Sensor
int sensor1 = A0;
int sensor2 = A1;
int sensor3 = A2;
int sensor4 = A3;
int sensor5 = A4;
int sensor6 = A5;
int button1 = 8;
int button2 = 9;
int baca_sensor[6];

//Variable motor
int pinEnable = 4; //Harus diset HIGH apabila akan diaktifkan
int pinEnable2 = 2; //Harus diset HIGH apabila akan diaktifkan

//Motor kiri
int motor_kiri1 = 5; //Input motor driver 1 L293D
int motor_kiri2 = 6; //Input motor driver 2 L293D

//Motor kanan
int motor_kanan1 = 3; //Input motor driver 3 L293D
int motor_kanan2 = 11; //Input motor driver 3 L293D

int maxpwm = 255; //Agar output motor tidak overflow

//Variable bantuan da buffer
int x;

//Kode Error
int LastError = 0; //Nilai 0 berarti tidak ada error
int error = 0;
int PID_value;
int kecepatanMotorKanan, kecepatanMotorKiri, kecepatanSetPoint = 150;

void setup()
{
  //pinmode input sensor
  pinMode(sensor1, INPUT); //Set pin A0 sebagai input
  pinMode(sensor2, INPUT); //Set pin A1 sebagai input
  pinMode(sensor3, INPUT); //Set pin A2 sebagai input
  pinMode(sensor4, INPUT); //Set pin A3 sebagai input
  pinMode(sensor5, INPUT); //Set pin A4 sebagai input
  pinMode(sensor6, INPUT); //Set pin A5 sebagai input
   
  //pinmode output sensor
  pinMode(pinEnable, OUTPUT);
  pinMode(pinEnable2, OUTPUT);
  pinMode(motor_kiri1, OUTPUT);
  pinMode(motor_kiri2, OUTPUT);
  pinMode(motor_kanan1, OUTPUT);
  pinMode(motor_kanan2, OUTPUT);
  
  //Inisialisasi komunikasi serial
  Serial.begin(9600); //Baud standar 9600
}

void readsensor(){ // untuk membaca sensor kemudian menyimpan Array
  baca_sensor[0] = analogRead(sensor1);
  baca_sensor[1] = analogRead(sensor2);
  baca_sensor[2] = analogRead(sensor3);
  baca_sensor[3] = analogRead(sensor4);
  baca_sensor[4] = analogRead(sensor5);
  baca_sensor[5] = analogRead(sensor6);
  
 /*
   for (x = 0; x <= 5; x++){
   Serial.println (baca_sensor[x]);
  }
  */
}

void If_Error(){
    
  //Sensor 1 saja yang mendeteksi gelap
  	 if (baca_sensor[0] < 34 && baca_sensor[1] > 34 && 
         baca_sensor[2] > 34 && baca_sensor[3] > 34 && 
         baca_sensor[4] > 34 && baca_sensor[5] > 34){
     
        LastError = -2;
        try_PID(LastError);
    }
  
  	//Sensor 2 saja yang mendeteksi gelap
  	 if (baca_sensor[0] > 34 && baca_sensor[1] < 34 && 
         baca_sensor[2] > 34 && baca_sensor[3] > 34 && 
         baca_sensor[4] > 34 && baca_sensor[5] > 34){
     
        LastError = -1;
        try_PID(LastError);
    }
  
  	//Sensor 3 saja yang mendeteksi gelap
  	 if (baca_sensor[0] > 34 && baca_sensor[1] > 34 && 
         baca_sensor[2] < 34 && baca_sensor[3] > 34 && 
         baca_sensor[4] > 34 && baca_sensor[5] > 34){
     
         LastError = 0;
       
         digitalWrite(pinEnable, HIGH);
         analogWrite(motor_kiri1, 255);
         analogWrite(motor_kiri2, 0);

         digitalWrite(pinEnable2, HIGH);
         analogWrite(motor_kanan1, 255);
         analogWrite(motor_kanan2, 0);
   }
    
    //Sensor 4 saja yang mendeteksi gelap
     if (baca_sensor[0] > 34 && baca_sensor[1] > 34 && 
         baca_sensor[2] > 34 && baca_sensor[3] < 34 && 
         baca_sensor[4] > 34 && baca_sensor[5] > 34){
  
         LastError = 0;
         try_PID(LastError);
   }
  
  	//Sensor 5 saja yang mendeteksi gelap
  	 if (baca_sensor[0] > 34 && baca_sensor[1] > 34 && 
         baca_sensor[2] > 34 && baca_sensor[3] > 34 && 
         baca_sensor[4] < 34 && baca_sensor[5] > 34){
     
        LastError = 1;
        try_PID(LastError);
    }
  
  //Sensor 6 saja yang mendeteksi gelap
  	 if (baca_sensor[0] > 34 && baca_sensor[1] > 34 && 
         baca_sensor[2] > 34 && baca_sensor[3] > 34 && 
         baca_sensor[4] > 34 && baca_sensor[5] < 34){
        
        LastError = 2;
        try_PID(LastError);
    }
}

void loop(){
  
  readsensor();
  
  //Sensor 1 dan 2 mendeteksi gelap, sisanya terang
    if (baca_sensor[0] < 34 && baca_sensor[1] < 34 && 
        baca_sensor[2] > 34 && baca_sensor[3] > 34 && 
        baca_sensor[4] > 34 && baca_sensor[5] > 34){
      
      LastError = -2;
      try_PID(LastError);
    }
  
  //Sensor 2 dan 3 mendeteksi gelap, sisanya terang
    if (baca_sensor[0] > 34 && baca_sensor[1] < 34 && 
        baca_sensor[2] < 34 && baca_sensor[3] > 34 && 
        baca_sensor[4] > 34 && baca_sensor[5] > 34){
      
      LastError = -1;
      try_PID(LastError); 
    }
  
  //Sensor 3 dan 4 mendeteksi gelap, sisanya terang 
    if (baca_sensor[0] > 34 && baca_sensor[1] > 34 && 
        baca_sensor[2] < 34 && baca_sensor[3] < 34 && 
        baca_sensor[4] > 34 && baca_sensor[5] > 34){
      	
      LastError = 0;
      
      digitalWrite(pinEnable, HIGH);
      analogWrite(motor_kiri1, 255);
      analogWrite(motor_kiri2, 0);

      digitalWrite(pinEnable2, HIGH);
      analogWrite(motor_kanan1, 255);
      analogWrite(motor_kanan2, 0);
    }
  
  //Sensor 4 dan 5 mendeteksi gelap, sisanya terang 
    if (baca_sensor[0] > 34 && baca_sensor[1] > 34 && 
        baca_sensor[2] > 34 && baca_sensor[3] < 34 && 
        baca_sensor[4] < 34 && baca_sensor[5] > 34){
      	
      LastError = 1;
      try_PID(LastError);
    }
  
  //Sensor 5 dan 6 mendeteksi gelap, sisanya terang 
  	if (baca_sensor[0] > 34 && baca_sensor[1] > 34 && 
        baca_sensor[2] > 34 && baca_sensor[3] > 34 && 
        baca_sensor[4] < 34 && baca_sensor[5] < 34){
      	
      LastError = 2;
      try_PID(LastError);
    }
  
  //Semua sensor mendeteksi terang 
  	if (baca_sensor[0] > 34 && baca_sensor[1] > 34 && 
        baca_sensor[2] > 34 && baca_sensor[3] > 34 && 
        baca_sensor[4] > 34 && baca_sensor[5] > 34){
      	
        digitalWrite(pinEnable, HIGH);
      //analogWrite(pin, value 0-255)
        analogWrite(motor_kiri1, 0);
        analogWrite(motor_kiri2, 0);
        
        digitalWrite(pinEnable2, HIGH);
        analogWrite(motor_kanan1, 0);
        analogWrite(motor_kanan2, 0);
    }
  If_Error();
}

void try_PID(int LastError){
  
  //Serial.print ("Error Sensor Detect : ");
  //Serial.println (LastError);
  
  
  int kp = 5, ki = 0, kd = 0; //kp range = 1-5
  int rate = error - LastError;//0-(-2) = 2
  //PID_value = (kp * LastError) + (kd * rate) + (ki / rate);
  PID_value =(kp * LastError);
  kecepatanMotorKanan = kecepatanSetPoint - PID_value;//255-60 = 195
  kecepatanMotorKiri  = kecepatanSetPoint + PID_value;//255+60 = 60
  
  kecepatanMotorKiri  = constrain(kecepatanMotorKiri, 0, 255);
  kecepatanMotorKanan = constrain(kecepatanMotorKanan, 0, 255);
  
  Serial.println (kecepatanMotorKanan);
  
  //Serial.print ("Error : ");
  //Serial.println (error);
  //Serial.print ("\n");
  //Serial.print ("rate  : ");
  //Serial.println (rate);
  //Serial.print ("\n");
  //Serial.print ("PID   : ");
  //Serial.println (PID_value);
  //Serial.print ("\n");
  //Serial.print ("Motor Kanan   : ");
  //Serial.println (kecepatanMotorKanan);
  //Serial.print ("\n");
  //Serial.print ("Motor Kiri    : ");
  //Serial.println (kecepatanMotorKiri);
  //Serial.print ("\n");
  //delay (1000);
 
  
  digitalWrite(pinEnable, HIGH);
  analogWrite(motor_kiri1,kecepatanMotorKiri);
  analogWrite(motor_kiri2, 0);

  digitalWrite(pinEnable2, HIGH);
  analogWrite(motor_kanan1, kecepatanMotorKanan);
  analogWrite(motor_kanan2, 0);
}