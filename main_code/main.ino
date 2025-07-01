#include <Wire.h>
#include <Adafruit_GFX.h>
#include <Adafruit_SSD1306.h>

#define SCREEN_WIDTH 128
#define SCREEN_HEIGHT 64
#define OLED_RESET -1

#define ADC_INPUT 34       // ADC input for waveform
#define FREQ_INPUT 4       // GPIO for frequency measurement
#define CAPTURE_BTN 15      // Button to freeze waveform

#define SDA_2 17           // SDA for second OLED
#define SCL_2 16           // SCL for second OLED

Adafruit_SSD1306 display1(SCREEN_WIDTH, SCREEN_HEIGHT, &Wire, OLED_RESET);
Adafruit_SSD1306 display2(SCREEN_WIDTH, SCREEN_HEIGHT, &Wire1, OLED_RESET);

const int numSamples = 128;
int samples[numSamples];
bool freezeWaveform = false;

volatile uint32_t pulseCount = 0;
unsigned long previousMillis = 0;
const unsigned long interval = 1000;

float voltagePP = 0; // Store peak-to-peak voltage globally

void IRAM_ATTR countPulses() {
    pulseCount++;
}

void setup() {
    Serial.begin(115200);
    pinMode(ADC_INPUT, INPUT);
    pinMode(CAPTURE_BTN, INPUT_PULLUP);
    pinMode(FREQ_INPUT, INPUT_PULLDOWN);
    attachInterrupt(digitalPinToInterrupt(FREQ_INPUT), countPulses, RISING);

    // Initialize first OLED (Waveform)
    Wire.begin(21, 22);
    if (!display1.begin(SSD1306_SWITCHCAPVCC, 0x3C)) {
        Serial.println("SSD1306 allocation failed (OLED 1)");
        for (;;);
    }
    display1.clearDisplay();
    display1.display();

    // Initialize second OLED (Voltage & Frequency)
    Wire1.begin(SDA_2, SCL_2);
    if (!display2.begin(SSD1306_SWITCHCAPVCC, 0x3C, &Wire1)) {
        Serial.println("SSD1306 allocation failed (OLED 2)");
        for (;;);
    }
    display2.clearDisplay();
    display2.display();

    Serial.println("Frequency and Voltage Measurement Started...");
}

void loop() {
    if (digitalRead(CAPTURE_BTN) == LOW) {
        freezeWaveform = !freezeWaveform;
        delay(300);
        // <-- This function will print to Serial
      
    }

    // Always update voltage, even if waveform is frozen
    captureWaveform();

    if (!freezeWaveform) {
        drawWaveform();
    }

    displayVoltageAndFrequency();
}

void captureWaveform() {
    int minV = 4095, maxV = 0;
    
    for (int i = 0; i < numSamples; i++) {
        samples[i] = analogRead(ADC_INPUT);
        
        if (samples[i] < minV) minV = samples[i];
        if (samples[i] > maxV) maxV = samples[i];
        
        delayMicroseconds(10);
    }

    int peakToPeak = maxV - minV;
    voltagePP = (peakToPeak * 3.3*3.4) / 4095.0;  // Convert ADC value to voltage
}

void drawWaveform() {
    display1.clearDisplay();
    int step = numSamples / SCREEN_WIDTH;
    
    int minADC = 4095, maxADC = 0;
    
    // Find min and max ADC values in the buffer
    for (int i = 0; i < numSamples; i++) {
        if (samples[i] < minADC) minADC = samples[i];
        if (samples[i] > maxADC) maxADC = samples[i];
    }

    // Apply moving average filter to smooth noise
    int smoothedSamples[numSamples];
    int windowSize = 3;  // Adjust this for more/less smoothing
    for (int i = 0; i < numSamples; i++) {
        int sum = 0, count = 0;
        for (int j = -windowSize; j <= windowSize; j++) {
            int index = i + j;
            if (index >= 0 && index < numSamples) {  // Ensure valid index
                sum += samples[index];
                count++;
            }
        }
        smoothedSamples[i] = sum / count;  // Compute average
    }

    // Draw the waveform using the filtered values
    for (int i = 1; i < SCREEN_WIDTH; i++) {
        int index1 = (i - 1) * step;
        int index2 = i * step;
        if (index2 < numSamples) {
            display1.drawLine(i - 1, map(smoothedSamples[index1], minADC, maxADC, SCREEN_HEIGHT, 0),
                              i, map(smoothedSamples[index2], minADC, maxADC, SCREEN_HEIGHT, 0),
                              SSD1306_WHITE);
            
        }
    }

    display1.display();
}

void displayVoltageAndFrequency() {
    unsigned long currentMillis = millis();
    static uint32_t frequency = 0;

    if (currentMillis - previousMillis >= interval) {
        previousMillis = currentMillis;
        frequency = pulseCount/1.23;  // Using your frequency calculation method
        pulseCount = 0;
    }
     float voltage = analogRead(ADC_INPUT) * ((3.3) / 4095.0); 
     float vdc = voltage * 11.0;
    display2.clearDisplay();
    display2.setTextSize(1);
    display2.setTextColor(SSD1306_WHITE);
    
    display2.setCursor(0, 10);
    display2.print("Freq: ");
    display2.print(frequency);
    display2.println(" Hz");

    display2.setCursor(0, 30);
    display2.print("Vpp: ");
    display2.print(voltagePP, 2);
    display2.println(" V");

     display2.setCursor(0, 50);
    display2.print("VDC: ");
    display2.print(vdc, 2);

    display2.display();
}

