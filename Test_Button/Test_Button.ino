void setup() {
  // simple check for up to 5 buttons
  
  // light for buttons as output
 pinMode(13, OUTPUT);
 pinMode(12, OUTPUT);
 pinMode(11, OUTPUT);
 pinMode(10, OUTPUT);
 pinMode(9, OUTPUT);
  
  // buttons in input mode
  pinMode(3, INPUT);
  pinMode(4, INPUT);
  pinMode(5, INPUT);
  pinMode(6, INPUT);
  pinMode(7, INPUT);
  
  // buttons and both toggles countercurrent
  digitalWrite(3, HIGH);
  digitalWrite(4, HIGH);
  digitalWrite(5, HIGH);
  digitalWrite(6, HIGH);
  digitalWrite(7, HIGH);
}

void loop() {
  // check if any buttons or one of toggles is pressed; 
  // rephrase as loop if feeling less lazy
  if(digitalRead(3) == LOW){
    digitalWrite(9, HIGH);
  } else {
    digitalWrite(9, LOW);
  }

  if(digitalRead(4) == LOW){
    digitalWrite(10, HIGH);
  } else {
    digitalWrite(10, LOW);
  }
  
  if(digitalRead(5) == LOW){
    digitalWrite(11, HIGH);
  } else {
    digitalWrite(11, LOW);
  }
  
  if(digitalRead(6) == LOW){
    digitalWrite(12, HIGH);
  } else {
    digitalWrite(12, LOW);
  }
 
  if(digitalRead(7) == LOW){
    digitalWrite(13, HIGH);
  } else {
    digitalWrite(13, LOW);
  }
}
