#include <Wire.h>
#include <Adafruit_GFX.h>
#include <Adafruit_SSD1306.h>
#include <Adafruit_MLX90614.h>
#define SCREEN_WIDTH 128
#define SCREEN_HEIGHT 64
#define OLED_RESET -1 // Reset pin # (or -1 if sharing Arduino reset pin)
#define SCREEN_ADDRESS 0x3C // Screen Address in memory
Adafruit_SSD1306 display(SCREEN_WIDTH, SCREEN_HEIGHT, &Wire, OLED_RESET); // Initializes the OLED screen
Adafruit_MLX90614 mlx = Adafruit_MLX90614(); // Initializes the ir sensor
int incomingByte; // Reads incoming serial data
const int buzzer = 2; // Constant value of 2 indicating D2 output on Arduino Nanoe
float finalTemp; // Stores final temp
char finalUnit; // Stores unit used
double tempRead;
double tempAdj;
void setup() {
  Serial.begin(9600); // Starts serial comm with baud rate of 9600
  mlx.begin(); // Starts the IR temperature reading
  display.begin(SSD1306_SWITCHCAPVCC, SCREEN_ADDRESS); // Start the OLED display
  display.clearDisplay();
  pinMode(buzzer, OUTPUT); // Sets D2 to output to Buzzer
}
void loop() {
  int counter = 200; // Counter used to record temperatures for around 6.5 seconds
  display.clearDisplay();
  // Text at the top of the screen
  display.setTextSize(2);
  display.setTextColor(WHITE);
  display.setCursor(0, 16);
  display.println("Please use \n QR Code");
  display.display();
  12
  // see if there's incoming serial data:
  if (Serial.available() > 0) {
    // read the oldest byte in the serial buffer:
    incomingByte = Serial.read();
    // if it's a capital C (ASCII 67), print to display in Celsius:
    if (incomingByte == 'C') {
      // Initial beep to let the user know that their temperature is being recorded
      digitalWrite(buzzer, HIGH); // Turn on Buzzer
      delay(100); // Delay for 1/10 sec
      digitalWrite(buzzer, LOW); // Turn on Buzzer
      // Loops until counter = 0, which is approximately 5 seconds
      while (counter) {
        display.clearDisplay();
        // Text at the top of the screen
        display.setTextSize(1);
        display.setTextColor(WHITE);
        display.setCursor(0, 0);
        display.println("Temperature:");
        // Text in the middle of the temperature reading
        display.setTextSize(3);
        display.setCursor(10, 16);
        tempRead = mlx.readObjectTempC();
        tempAdj = 2.52 + tempRead;
        display.println(tempAdj, 1);
        display.setCursor(90, 16);
        display.println("C");
        // Wait text at the bottom of the screen
        display.setTextSize(1);
        display.setTextColor(WHITE);
        display.setCursor(0, 45);
        display.println("Wait for the tone");
        finalTemp = tempAdj;
        counter = counter - 1;
        if (counter % 2 == 0)
          display.display();
      }
      finalUnit = 'C';
      Serial.print(finalTemp, 1);
      FinalPrint();
    }
    // if it's an F (ASCII 70), print to display in Fahrenheit:
    if (incomingByte == 'F') {
      // Initial beep to let the user know that their temperature is being recorded
      digitalWrite(buzzer, HIGH); // Turn on Buzzer
      delay(100); // Delay for 1/10 sec
      digitalWrite(buzzer, LOW); // Turn on Buzzer
      // Loops until counter = 0, which is approximately 5 seconds
      while (counter) {
        display.clearDisplay();
        // Text at the top of the screen
        display.setTextSize(1);
        display.setTextColor(WHITE);
        display.setCursor(0, 0);
        display.println("Temperature:");
        // Text in the middle of the temperature reading
        display.setTextSize(3);
        display.setCursor(10, 16);
        tempRead = mlx.readObjectTempF();
        13
        tempAdj = 4.53 + tempRead;
        display.println(tempAdj, 1);
        display.setCursor(90, 16);
        display.println("F");
        // Wait text at the bottom of the screen
        display.setTextSize(1);
        display.setTextColor(WHITE);
        display.setCursor(0, 45);
        display.println("Wait for the tone");
        finalTemp = tempAdj;
        counter = counter - 1;
        if (counter % 2 == 0)
          display.display();
      }
      finalUnit = 'F';
      Serial.print(finalTemp, 1);
      FinalPrint();
    }
  }
}
// Function used to print the final display with the final temperature, unit and indication of a fever
void FinalPrint() {
  // Final print screen with the buzzer in the background to tell the user they are completed
  display.clearDisplay();
  display.display();
  // Prints final temperature of user
  display.setTextSize(3);
  display.setCursor(10, 16);
  display.println(finalTemp, 1);
  display.setCursor(90, 16);
  if (finalUnit == 'C') {
    display.println("C");
    // Checks and prints if the user has a fever of over 38 C
    if (finalTemp > 33.7) {
      display.setTextSize(2);
      display.setTextColor(WHITE);
      display.setCursor(0, 45);
      display.println(" FEVER");
      display.display();
      // If fever beep faster
      digitalWrite(buzzer, HIGH); // Turn on Buzzer
      delay(250); // Delay for 1/4 sec
      digitalWrite(buzzer, LOW); // Turn off Buzzer
      delay(250);
      digitalWrite(buzzer, HIGH);
      delay(250);
      digitalWrite(buzzer, LOW);
      delay(250);
      digitalWrite(buzzer, HIGH);
      delay(250);
      digitalWrite(buzzer, LOW);
      delay(250);
      digitalWrite(buzzer, HIGH);
      delay(250);
      digitalWrite(buzzer, LOW);
      delay(250);
      digitalWrite(buzzer, HIGH);
      delay(250);
      digitalWrite(buzzer, LOW);
      delay(2500); // Total delay of 5 seconds for user to see their final
      temperature
    }
    else {
      14
      display.display();
      digitalWrite(buzzer, HIGH); // Turn on Buzzer
      delay(500); // Delay for 1/2 sec
      digitalWrite(buzzer, LOW); // Turn off Buzzer
      delay(500);
      digitalWrite(buzzer, HIGH);
      delay(500);
      digitalWrite(buzzer, LOW);
      delay(500);
      digitalWrite(buzzer, HIGH);
      delay(500);
      digitalWrite(buzzer, LOW);
      delay(2500); // Total delay of 5 seconds for user to see their final
      temperature
    }
  }
  else if (finalUnit == 'F') {
    display.println("F");
    // Checks and prints if the user has a fever of over 100.4 F
    if (finalTemp > 92.7) {
      display.setTextSize(2);
      display.setTextColor(WHITE);
      display.setCursor(0, 45);
      display.println(" FEVER");
      display.display();
      // If fever beep faster
      digitalWrite(buzzer, HIGH); // Turn on Buzzer
      delay(250); // Delay for 1/4 sec
      digitalWrite(buzzer, LOW); // Turn off Buzzer
      delay(250);
      digitalWrite(buzzer, HIGH);
      delay(250);
      digitalWrite(buzzer, LOW);
      delay(250);
      digitalWrite(buzzer, HIGH);
      delay(250);
      digitalWrite(buzzer, LOW);
      delay(250);
      digitalWrite(buzzer, HIGH);
      delay(250);
      digitalWrite(buzzer, LOW);
      delay(250);
      digitalWrite(buzzer, HIGH);
      delay(250);
      digitalWrite(buzzer, LOW);
      delay(2500); // Total delay of 5 seconds for user to see their final
      temperature
    }
    else {
      display.display();
      digitalWrite(buzzer, HIGH); // Turn on Buzzer
      delay(500); // Delay for 1/2 sec
      digitalWrite(buzzer, LOW); // Turn off Buzzer
      delay(500);
      digitalWrite(buzzer, HIGH);
      delay(500);
      digitalWrite(buzzer, LOW);
      delay(500);
      digitalWrite(buzzer, HIGH);
      delay(500);
      digitalWrite(buzzer, LOW);
      delay(2500); // Total delay of 5 seconds for user to see their final
      temperature
    }
  }
}
