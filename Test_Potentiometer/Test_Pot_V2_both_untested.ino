//The following lines define the pins & variables we'll be using 

int potPin = A3;
int ledPin = 13;
int val=0;

void setup() {
pinMode (ledPin, OUTPUT);//Sets the LED pin as an output
}

void loop() {
val = analogRead(potPin); //reads the analogue voltage from the pot via ADC and stores it in the variable


digitalWrite(ledPin, HIGH); //turn the LED output pin high turning the LED on 


delay(val); //waits an amount of time dependent on the potentiometer position


digitalWrite(ledPin, LOW); //turn the LED output pin low turning the LED off


delay(val); //waits an amount of time dependent on the potentiometer position


}
