#include <driver/rtc_io.h>

// Built in button GPIO - adjust for your board
#define BUTTON_GPIO GPIO_NUM_35

// Select the deep sleep mode you want to test

//Will sleep for 30 seconds
#define DEEP_SLEEP_TIMER

//Will wake up when BUTTON_GPIO goes LOW
//#define DEEP_SLEEP_EXT0

//Will wake up when pins 15 or 12 go HIGH
//#define DEEP_SLEEP_EXT1_ANY_HIGH

//Will wake up when pins 15 or 12 are both LOW
//#define DEEP_SLEEP_EXT1_ALL_LOW

//Will wake up on touch of touch pin 9 (GPIO 32)
#define DEEP_SLEEP_TOUCH


// keep track of how many times we've come out of deep sleep
RTC_DATA_ATTR int sleep_count = 0;

// dump out the pin that woke us from EXT1 deep sleep
void show_ext1_wakeup_reason()
{
  uint64_t ext1_waskeup_reason = esp_sleep_get_ext1_wakeup_status();
  for (int i = 0; i < GPIO_NUM_MAX; i++)
  {
    if (ext1_waskeup_reason & (1ULL << i))
    {
      Serial.printf("GPIO %d\n", i);
    }
  }
}


// dump out the touch pin that caused the wake up
void show_touch_wakeup_reason()
{
  touch_pad_t tp = esp_sleep_get_touchpad_wakeup_status();
  Serial.printf("T %d\n", tp);
}

// work out why we were woken up
void show_wake_reason()
{
  sleep_count++;
  auto cause = esp_sleep_get_wakeup_cause();
  switch (cause)
  {
  case ESP_SLEEP_WAKEUP_UNDEFINED:
    Serial.println("Undefined"); // most likely a boot up after a reset or power cycly
    break;
  case ESP_SLEEP_WAKEUP_EXT0:
    Serial.println("Wakeup reason: EXT0");
    break;
  case ESP_SLEEP_WAKEUP_EXT1:
    Serial.println("Wakeup reason: EXT1");
    show_ext1_wakeup_reason();
    break;
  case ESP_SLEEP_WAKEUP_TIMER:
    Serial.println("Wakeup reason: TIMER");
    break;
  case ESP_SLEEP_WAKEUP_TOUCHPAD:
    Serial.println("Wakeup reason: TOUCHPAD");
    show_touch_wakeup_reason();
    break;
  case ESP_SLEEP_WAKEUP_ULP:
    Serial.println("Wakeup reason: ULP");
    break;
  default:
    Serial.printf("Wakeup reason: %d\n", cause);
  }
  Serial.printf("Count %d\n", sleep_count);
}

// touch handler - needed for waking from touch
void touch_isr_handler()
{
  Serial.println("Touch!");
}

// count down 3 seconds and then go to sleep
void enter_sleep()
{
  Serial.println("Going to sleep...");
  delay(1000);
  Serial.println("3");
  delay(1000);
  Serial.println("2");
  delay(1000);
  Serial.println("1");
  delay(1000);
#ifdef DEEP_SLEEP_TIMER  
  esp_sleep_enable_timer_wakeup(30000000);
#endif
#ifdef DEEP_SLEEP_EXT0 
  pinMode(BUTTON_GPIO, INPUT_PULLUP);
  rtc_gpio_hold_en(BUTTON_GPIO);
  esp_sleep_enable_ext0_wakeup(BUTTON_GPIO, LOW);
#endif
#ifdef DEEP_SLEEP_EXT1_ANY_HIGH
  pinMode(GPIO_NUM_15, INPUT_PULLDOWN);
  rtc_gpio_hold_en(GPIO_NUM_15);
  pinMode(GPIO_NUM_12, INPUT_PULLDOWN);
  rtc_gpio_hold_en(GPIO_NUM_12);
  esp_sleep_enable_ext1_wakeup((1 << GPIO_NUM_12) | (1 << GPIO_NUM_15), ESP_EXT1_WAKEUP_ANY_HIGH);
#endif
#ifdef DEEP_SLEEP_EXT1_ALL_LOW
  pinMode(GPIO_NUM_15, INPUT_PULLUP);
  rtc_gpio_hold_en(GPIO_NUM_15);
  pinMode(GPIO_NUM_12, INPUT_PULLUP);
  rtc_gpio_hold_en(GPIO_NUM_12);
  esp_sleep_enable_ext1_wakeup((1 << GPIO_NUM_12) | (1 << GPIO_NUM_15), ESP_EXT1_WAKEUP_ALL_LOW);
#endif
#ifdef DEEP_SLEEP_TOUCH
  touchAttachInterrupt(T9, touch_isr_handler, 50);
  esp_sleep_enable_touchpad_wakeup();
#endif
  esp_deep_sleep_start();
}

void setup(void)
{
  Serial.begin(115200);
  // we've just started up - show the reason why
  show_wake_reason();
  // enable the button on pin 35
  pinMode(BUTTON_GPIO, INPUT_PULLUP);
}

void loop()
{
  if (digitalRead(BUTTON_GPIO) == LOW)
  {
     Serial.println("Button pressed - enter sleep\n");
     enter_sleep();
  }
}
