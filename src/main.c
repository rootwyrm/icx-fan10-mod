/*
  ICXFAN10 - Interposing Fan Controller for modifying ICX switches with quiet fans

  This implementation is actually quite complex, as it actually works cooperatively
  with the ICX instead of simply lying to it. It also is intended to support a WiFi
  status page for monitoring fans, and to support using the Teensy's RS232 
  capabilities to act as a serial console server.
  Because hey, why the hell not, right?

  IMPORTANT! THIS DOES NOT WORK WITH THE POWER SUPPLY FANS! THE POWER SUPPLIES 
  CONTAIN HIGHLY DANGEROUS VOLTAGES AND SHOCK HAZARDS, AND MUST ONLY BE SERVICED
  BY PROFESSIONALS. 
  YOU ARE NOT A PROFESSIONAL!!
  LEAVE THE POWER SUPPLY FANS ALONE.
*/

#include <FreqMeasureMulti.h>
#include <SD.h>
#include <SPI.h>
#include <Adafruit_NeoPixel_ZeroDMA.h>  // WARNING: Must use ZeroDMA!
#include <TeensyThreads.h>

#define MEASURE_TYPE FREQMEASUREMULTI_RAISING

// Serial port configurations
#define ESP8266_PORT Serial1
#define RS232_PORT   Serial2
#define SYSTTL_PORT  Serial5

#include "pin_config.h"

// Set all PWM signals to 25kHz
const float pwm_freq = 25000.0f;

// NeoPixel setup
#define NEOPIXEL_COUNT 8
Adafruit_NeoPixel_ZeroDMA neopixel8(NEOPIXEL_COUNT, NEOPIXEL_DATA, NEO_RGB + NEO_KHZ800);

// RESERVED: SDCard

// Initialize PWM inputs
FreqMeasureMulti icx_mod0_pwm, icx_mod1_pwm;

// Tachometer counters
volatile unsigned long fan_tach0_counter = 0;
volatile unsigned long fan_tach1_counter = 0;

// Setup Phase
void setup()
{
  // USB Interface while programming or debugging without system power
  Serial.begin(115200);
  while (!Serial) { 
    delay(100);
  }
  Serial.println("USB Console online.");
  // System TTL port
  SYSTTL_PORT.begin(115200);
  while (!SYSTTL_PORT) {
    delay(100);
  }
  SYSTTL_PORT.println("System TTL Console online.");
  // RS232 port
  RS232_PORT.begin(9600);
  while (!RS232_PORT) {
    delay(100);
  }
  // Send log to consoles
  log_console("RS232 port online.");

  // RESERVED: WiFi

  // RESERVED: SDCard

  // Setup neopixel
  neopixel8.begin();
  neopixel8.setBrightness(128);
  neopixel8.clear();
  // Do a color cycle test to prove life
  for(int i=0; i<4; i++) {
    // i=0, red; i=1, green; i=2, blue; i=3, clear
    for(int j=0; j<NEOPIXEL_COUNT; j++) {
      neopixel8.setPixelColor(j, neopixel8.Color(i==0?255:0, i==1?255:0, i==2?255:0, i==3?0:0));
      neopixel8.show();
      delay(250);
    }
  }
  // Clear pixels again
  neopixel8.clear();

  // Setup FAULT_LED
  pinMode(FAULT_LED, OUTPUT);
  // Test our fault LED
  log_console("Testing FAULT LED - you should see it blinking.");
  for (int i = 0; i < 20; i++)
  {
    digitalWrite(FAULT_LED, HIGH);
    delay(50);
    digitalWrite(FAULT_LED, LOW);
    delay(50);
  }

  // Set up outputs and initialize high
  pinMode(FAN0_PWM, OUTPUT);
  pinMode(FAN1_PWM, OUTPUT);
  analogWriteFrequency(FAN0_PWM, pwm_freq);
  analogWriteFrequency(FAN1_PWM, pwm_freq);
  // XXX: resolution 12 doesn't work as expected
  analogWriteResolution(8);
  // Always start fans at 100% to ensure we don't cook off.
  pwm_percent(FAN0_PWM, 100.0);
  pwm_percent(FAN1_PWM, 100.0);

  // Set frequency on and setup ICX PWM inputs
  analogWriteFrequency(ICX_MOD0_PWM, pwm_freq);
  analogWriteFrequency(ICX_MOD1_PWM, pwm_freq);
  icx_mod0_pwm.begin(ICX_MOD0_PWM);
  icx_mod1_pwm.begin(ICX_MOD1_PWM);

  // Set up tachometer outputs for ICX
  pinMode(ICX_MOD0_TACH0, OUTPUT_OPENDRAIN);
  pinMode(ICX_MOD0_TACH1, OUTPUT_OPENDRAIN);
  pinMode(ICX_MOD1_TACH0, OUTPUT_OPENDRAIN);
  pinMode(ICX_MOD1_TACH1, OUTPUT_OPENDRAIN);

  // Modification fan inputs; use care here.
  // Per Intel spec, normal is a 10k pullup from 12V supply; we're using a 1k from 5V
  pinMode(FAN0_TACH, INPUT);
  pinMode(FAN1_TACH, INPUT);

  // Attach interrupts to tachometer pins
  attachInterrupt(digitalPinToInterrupt(FAN0_TACH), fan0_tach_isr, FALLING);
  log_console("FAN0 Tachometer interrupt attached.");
  attachInterrupt(digitalPinToInterrupt(FAN1_TACH), fan1_tach_isr, FALLING);
  log_console("FAN1 Tachometer interrupt attached.");

  // Spawn threads
  log_console("Spawning operational threads...");
  threads.addThread(icx_module_control, "mod0");
  threads.addThread(icx_module_control, "mod1");

  log_console("System boot complete!");
}

volatile float fan0_pwm_output, fan1_pwm_output;
elapsedMillis mod10_pwm_update_timer;
elapsedMillis fan0_tach_timer, fan1_tach_timer;
volatile float icx_mod0_pwm, icx_mod1_pwm0;
volatile float fan0_pwm, fan1_pwm;
volatile int fan0_tach, fan1_tach, fan0_rpm, fan1_rpm;

elapsedMillis debug_print_timer;

void loop()
{

  // 2 pulses per revolution
  fan0_rpm = fan0_tach * 60 / 2;
  fan1_rpm = fan1_tach * 60 / 2;

  // Create a thread for module 0 operations


  // TODO: this should be moved to a thread that has an internal delay
  if (mod10_pwm_update_timer > 1000)
  {
    // Update the PWM outputs
    icx_module_pwm_update("mod0", ICX_MOD0_PWM);
    // TODO: Update the Neopixel
    icx_module_pwm_update("mod1", ICX_MOD1_PWM);
    // Update the Neopixel
    mod10_pwm_update_timer = 0;
  }

  // TODO: this could possibly be threaded too?
  if (fan0_tach_timer > 1000)
  {
    /* TODO: Update tachometer output */
    fan0_tach = 0;
    fan0_tach_timer = 0;
    // Update the Neopixel
  }
  if (fan1_tach_timer > 1000)
  {
    /* TODO: Update tachometer output */
    fan1_tach = 0;
    fan1_tach_timer = 0;
    // Update the Neopixel
  }

  if (debug_print_timer > 5000)
  {
    log_console("FAN0 RPM: " + fan0_rpm);
    log_console("FAN1 RPM: " + fan1_rpm);
    debug_print_timer = 0;
  }
}

/*
 * Shim function to write messages to both consoles
*/
void log_console(char *message)
{
  // Just writes to both consoles 
  Serial.println(message);
  SYSTTL_PORT.println(message);
}

/*
 icx_module_pwm
 Read commanded PWM from ICX host, and set PWM on modification fans to match
 NOTE: this function can be ignored when operating in autonomous temperature mode, but the PWM output does need to be sunk
*/
void icx_module_pwm_update(char mod, int pin)
{
  fan_ptr = (float*) malloc(size_t float);
  if (mod == "mod0")
  {
    if(icx_mod0_pwm.available()) {
      mod0_pwm = mod0_pwm + icx_mod0_pwm.read();
    }
    fan_ptr = mod0_pwm;
    if (mod0_pwm != fan_ptr)
    {
      // Update settings
      fan_ptr = mod0_pwm;
      analogWrite(FAN0_PWM, fan_ptr);
      log_console("FAN0 PWM updated to " + fan_ptr);
    }
  }
  elif (mod == "mod1")
  {
    if(icx_mod1_pwm.available()) {
      mod1_pwm = mod1_pwm + icx_mod1_pwm.read();
    }
    fan_ptr = mod1_pwm;
    if (mod1_pwm != fan_ptr)
    {
      // Update settings
      fan_ptr = mod1_pwm;
      analogWrite(FAN1_PWM, fan_ptr);
      log_console("FAN1 PWM updated to " + fan_ptr);
    }
  }
}

/*
 * Tachometer Counters; these are really basic
*/
void fan0_tach_isr()
{
  fan0_tach++;
}

void fan1_tach_isr()
{
  fan1_tach++;
}

// THREAD: icx_module_control
// Performs the ICX module control operations, including PWM and tachometer feeding
void icx_module_control(char mod)
{
  int PWM, tach0, tach1;
  float pwm_command;
  pwm_func_t* pwm_func = NULL;

  if (mod == "mod0") 
  {
    pwm_func = &icx_mod0_pwm;
    PWM = ICX_MOD0_PWM;
    tach0 = ICX_MOD0_TACH0;
    tach1 = ICX_MOD0_TACH1;
  }
  else if (mod == "mod1") 
  {
    pwm_func = &icx_mod1_pwm;
    PWM = ICX_MOD1_PWM;
    tach0 = ICX_MOD1_TACH0;
    tach1 = ICX_MOD1_TACH1;
  } 

  // Catch error
  if (pwm_func == NULL) {
    log_console("icx_module_control: could not find pwm_func!");
    ASSERT();
    return;
  }

  pwm_command = pwm_func->read();
  if (pwm_command != pwm_func->last_read()) {
    // Update the PWM output
    analogWrite(PWM, pwm_command);
    log_console("PWM updated to " + pwm_command);
  }

}

// THREAD: module0_status_led
// Monitors the MOD0_OK and MOD0_FAULT signals, and updates the status LED
void module0_status_led()
{
  int read_ok, read_fault;
  // Do a digital read of MOD0_OK and MOD0_FAULT
  read_ok = digitalRead(ICX_MOD0_OK);
  read_fault = digitalRead(ICX_MOD0_FAULT);

  // Update the status LED and neopixel LED 1
  if (read_ok == HIGH && read_fault == LOW) {
    digitalWrite(FAULT_LED, LOW);
    neopixel8.setPixelColor(1, neopixel8.Color(0, 255, 0, 0));
  } else if (read_ok == LOW && read_fault == HIGH) {
    digitalWrite(FAULT_LED, HIGH);
    neopixel8.setPixelColor(1, neopixel8.Color(255, 0, 0, 0));
  } else {
    digitalWrite(FAULT_LED, HIGH);
    neopixel8.setPixelColor(1, neopixel8.Color(255, 255, 0, 0));
  }
}