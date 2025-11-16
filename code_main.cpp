#include <Adafruit_CircuitPlayground.h>

//indexes for each feature 
#define POSTURE_LED 0  
#define SOUND_LED 1
#define BREAK_LED 2
//#define MUTE_DURATION 300000  //5 minutes break default
#define MUTE_DURATION 30000 // 30 s for test
unsigned long mutestarttime=-MUTE_DURATION;


// Posture Corrector Class 
class PostureCorrector {
  public:
    float zThreshold; // Threshold for Z-axis deviation from 9.8 m/s²
    bool enabled;

    PostureCorrector() {
      zThreshold = 30.0;   // Threshold for deviation (in m/s²)
      enabled = false;
    }

    

    void update(bool ismute) {
      if (!enabled  || ismute) return;     //enabled is a boolean variable for which the value is determined based on the user interaction in the void setupfeatures loop

      // Get Z-axis acceleration value
      float z = CircuitPlayground.motionZ();

      // Convert acceleration from g to m/s²
      float z_ms2 = z * 9.80665;

      // Print Z value to the serial monitor
      Serial.print("Z-axis Acceleration: ");
      Serial.print(z_ms2);
      Serial.println(" m/s²");

        // Check if Z acceleration deviates significantly from 9.8 m/s²
        if (abs(z_ms2 - 9.8) >= zThreshold) {
        // Beep using the onboard speaker
        CircuitPlayground.playTone(1000, 200); // 1000 Hz tone for 200 milliseconds
        for(int i=0;i<10;i++)
        {
          CircuitPlayground.setPixelColor(i, 255, 0, 0);   //all red
        }  
        delay(2000); 
        CircuitPlayground.clearPixels();   // Clear previous colors
      }

      // Delay to avoid flooding the serial monitor
      delay(500);
      
    }
};

// Sound Level Detector Class 
class SoundLevelDetector 
{
  private:
    unsigned long lastAverageTime;
    float accumulatedSound;
    int readings;

    void displaySoundLevel(float soundLevel)   //function to display the sound level at the current time
    {
      CircuitPlayground.clearPixels(); // Clear previous colors
      // Map sound level to number of LEDs
      int numLEDs = map(soundLevel, 10, 100, 1, 10);
      numLEDs = constrain(numLEDs, 0, 10);
      for (int i = 0; i < 10; i++) {
        if (i < numLEDs) {
          CircuitPlayground.setPixelColor(i, 0, 0, 255); // Blue
        } else {
          CircuitPlayground.setPixelColor(i, 0); // Off
        }
      }
    }
  public:
    float soundThreshold;
    bool enabled;
    unsigned long averagingTime; // in milliseconds

    SoundLevelDetector()
     {
      soundThreshold = 60.0;     // sound threshold
      averagingTime = 4000;      // Averaging over 4 seconds
      enabled = false;
      
      lastAverageTime = 0;
      accumulatedSound = 0;
      readings = 0;
    }


    void update( bool ismute) {
      if (!enabled  || ismute) return; 

      // Read sound level
      float soundLevel = CircuitPlayground.mic.soundPressureLevel(50); // 50ms sample

      // Display sound level using LEDs
        displaySoundLevel(soundLevel);

      accumulatedSound += soundLevel;
      readings++;

      unsigned long currentTime = millis();
      if (currentTime - lastAverageTime >= averagingTime) { //if averaging time has elapsed 
        // Compute average
        float averageSound = accumulatedSound / readings;  //averaging

        // Print average sound level
        Serial.print("Average Sound Level: ");
        Serial.println(averageSound);

        

        if (averageSound > soundThreshold)
         {
          //beep twice for loud sound 
          for(int j=0;j<2;j++)
          { 
          CircuitPlayground.playTone(1000, 200);
          for(int i=0;i<10;i++)
          {
            CircuitPlayground.setPixelColor(i, 255, 0, 0);}  //all red
            delay(100);
          }

          CircuitPlayground.clearPixels(); // Clear previous colors
          
         }

        // Reset for next averaging period
        lastAverageTime = currentTime;
        accumulatedSound = 0;
        readings = 0;
      }
    }

  
};

// Break Reminder Class
class BreakReminder 
{
  private:
    unsigned long lastReminderTime;

  public:
    unsigned long interval; // in milliseconds
    bool enabled;

    BreakReminder() //constructor  // does not take any values
    {
      //interval = 30 * 60 * 1000; // Default 30 minutes
      interval =   120000; // 30 s for test
      enabled = false;
    }

    void begin() 
    {
      lastReminderTime = millis();
      
    }

    void update(bool ismute) 
    {
      if (!enabled||ismute) 
        return;  //return void if the feature is disabled 

      unsigned long currentTime = millis();
      if (currentTime - lastReminderTime >= interval)  //if time interval has elapsed from the last reminder 
      {
          //triple beep for the break reminder function
        for(int j=0;j<3;j++)
        {
          CircuitPlayground.playTone(2000, 500);
         for(int i=0;i<10;i++)
          {CircuitPlayground.setPixelColor(i, 255, 0, 0);}  //all red
        delay(200);
        }

        CircuitPlayground.clearPixels();
        mutestarttime = currentTime;
        // Reset timer
        lastReminderTime = currentTime + MUTE_DURATION ;
        
      }
    }

  
};

// Create objects for each class
PostureCorrector postureCorrector;
SoundLevelDetector soundLevelDetector;
BreakReminder breakReminder;

void setup() {
  CircuitPlayground.begin();
  Serial.begin(9600);

  // Enter setup phase
  setupFeatures();
}

void loop()
 {
  // Check for reset condition
  // If both buttons are pressed together for more than 2 seconds, reset
  static unsigned long buttonPressTime = 0;
  if (CircuitPlayground.leftButton() && CircuitPlayground.rightButton()) 
  {
    if (buttonPressTime == 0) 
    {
      buttonPressTime = millis();
    } 
    else if (millis() - buttonPressTime > 2000)
    {
      // Reset
      setupFeatures();
    }
  } else {
    buttonPressTime = 0;
  }

  bool isMute = false;
  if ((millis()- mutestarttime) < MUTE_DURATION)
    {isMute = true;}
  // Update features
  postureCorrector.update(isMute);
  soundLevelDetector.update(isMute);
  breakReminder.update(isMute);

  delay(50); // Small delay to avoid overwhelming the CPU
}

void setupFeatures() {
  // Variables to store user selections
  bool postureEnabled = false;
  bool soundEnabled = false;
  bool breakEnabled = false;

  // Step 1: Feature Selection
  // Use LEDs 0-2 to represent the features
  // Posture Corrector - LED 0
  // Sound Level Detector - LED 1
  // Break Reminder - LED 2

  // Loop through features (feature 1,2,3)
  for (int i = 0; i < 3; i++) 
  {
    // Blink the LED to indicate the feature
    for (int j = 0; j < 3; j++) //the led corresponding to a specific feature is made to blink thrice to indicate that this feauture is currently being configured
    {  
      CircuitPlayground.setPixelColor(i, 255, 255, 255); // White
      delay(200);
      CircuitPlayground.setPixelColor(i, 0); // Off
      delay(200);
    }
    // Wait for user input
    CircuitPlayground.setPixelColor(i, 255, 255, 255); // Lights up in white for first time use 
    bool selected = false;
    unsigned long lastButtonPressTime = 0;
    bool selectionMade = false;

    while (!selectionMade) {  //functions with right button (selection)
      if (CircuitPlayground.leftButton()) {  //if the left button is pressed (on/off)
        if (millis() - lastButtonPressTime > 300) { // Debounce
          selected = !selected;  //state if reversed each time the left button is pressed  // the loop allows for activation and deactivation
          if (selected) {
            CircuitPlayground.setPixelColor(i, 0, 255, 0); // the corresponding led lights up in green
          } else {
            CircuitPlayground.setPixelColor(i, 255, 255, 255); // White
          }
          lastButtonPressTime = millis();
        }
      }
      if (CircuitPlayground.rightButton()) {
        if (millis() - lastButtonPressTime > 300) { // Debounce  //debouncing is used to ensure that a single button press is registered in a time interval of 300ms
                                                                  // this is to avoid mispress due to mechanical or electrical noise
          // Move to next feature
          selectionMade = true;
          lastButtonPressTime = millis();
        }
      }
      delay(50); // Small delay
    }
    // Store selection
    if (i == 0) postureEnabled = selected;
    else if (i == 1) soundEnabled = selected;
    else if (i == 2) breakEnabled = selected;
    // Turn off LED
    CircuitPlayground.setPixelColor(i, 0);
  }

  // Initialize features with user selections
  postureCorrector.enabled = postureEnabled;
  soundLevelDetector.enabled = soundEnabled;
  breakReminder.enabled = breakEnabled;

  // Start features
  postureCorrector.begin();
  soundLevelDetector.begin();
  breakReminder.begin();

  
}
