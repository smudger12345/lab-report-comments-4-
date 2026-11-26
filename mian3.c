#include <zephyr/kernel.h>       //Includes Zephyr's kernel functions, including sleep and timing functions.
#include <zephyr/device.h>       //Includes functions for accessing and checking hardware devices.
#include <zephyr/drivers/gpio.h> //Includes functions needed to configure GPIO pins and interrupts.
#include <zephyr/drivers/led.h>  //Includes functions needed to control the onboard LEDs.

#define BUTTON_PIN 13   //Defines pin 13 as the GPIO pin connected to the SW1 button.

static const struct device *gpio4 =
 DEVICE_DT_GET(DT_NODELABEL(gpio4));   //Gets GPIO controller 4 and stores a pointer to the device.

#define RED_LED_NODE DT_ALIAS(led0)    //Finds the red LED using its device tree alias.
#define BLUE_LED_NODE DT_ALIAS(led2)   //Finds the blue LED using its device tree alias.

static const struct led_dt_spec red_led =
 LED_DT_SPEC_GET(RED_LED_NODE);   //Gets the hardware information needed to control the red LED.

static const struct led_dt_spec blue_led =
 LED_DT_SPEC_GET(BLUE_LED_NODE);  //Gets the hardware information needed to control the blue LED.

static bool red_state = false;   //Creates a true/false variable to remember whether the red LED is currently on or off. Initially false (OFF).

static void button_callback( const struct device *dev, struct gpio_callback *cb, uint32_t pins)   //Creates the callback function that runs when the button interrupt occurs.
{
 ARG_UNUSED(dev);   //Indicates that the dev parameter is not being used in this function.
 ARG_UNUSED(cb);    //Indicates that the cb parameter is not being used in this function.

 if (pins & BIT(BUTTON_PIN))   //Checks whether the interrupt came from button pin 13.
 {
 red_state = !red_state;   //Reverses the red_state variable (ON becomes OFF, OFF becomes ON).

 if (red_state)   //Checks whether the red_state variable is true.
 { led_on_dt(&red_led); }   //If true, switches the red LED on.

 else   //Runs if red_state is false.
 { led_off_dt(&red_led); }   //Switches the red LED off.
 }
}

static struct gpio_callback button_cb;   //Creates a GPIO callback structure that stores the button interrupt callback information.

int main(void)   //The main function where the program starts running.
{
 int ret;   //Creates an integer variable used to store return values from functions.

 bool blue_state = false;   //Creates a true/false variable to remember whether the blue LED is on or off. Initially false (OFF).

 if (!device_is_ready(gpio4))   //Checks whether GPIO controller 4 is ready to be used.
 { return 0; }   //If GPIO controller 4 is not ready, the program stops.

 if (!led_is_ready_dt(&red_led))   //Checks whether the red LED is ready to be used.
 { return 0; }   //If the red LED is not ready, the program stops.

 if (!led_is_ready_dt(&blue_led))   //Checks whether the blue LED is ready to be used.
 { return 0; }   //If the blue LED is not ready, the program stops.

 ret = gpio_pin_configure( gpio4, BUTTON_PIN, GPIO_INPUT );   //Configures pin 13 of GPIO controller 4 as an input for the button.
 if (ret < 0)   //Checks whether configuring the button pin failed.
 { return 0; }   //If configuration fails, the program stops.

ret = gpio_pin_interrupt_configure( gpio4, BUTTON_PIN, GPIO_INT_EDGE_TO_ACTIVE);   //Configures an interrupt that occurs when the button changes to its active state.
 if (ret < 0)   //Checks whether configuring the interrupt failed.
 { return 0; }   //If interrupt configuration fails, the program stops.

gpio_init_callback( &button_cb, button_callback, BIT(BUTTON_PIN) );   //Initialises the callback structure, linking the button interrupt to the button_callback function and pin 13.

ret = gpio_add_callback( gpio4, &button_cb );   //Registers the callback with GPIO controller 4 so it can respond to button interrupts.
 if (ret < 0)   //Checks whether registering the callback failed.
 { return 0; }   //If registering the callback fails, the program stops.

 led_off_dt(&red_led);    //Makes sure the red LED is initially switched off.
 led_off_dt(&blue_led);   //Makes sure the blue LED is initially switched off.

 while (1) {   //Starts an infinite loop so the blue LED continues flashing.

 blue_state = !blue_state;   //Reverses the blue_state variable (ON becomes OFF, OFF becomes ON).

 if (blue_state)   //Checks whether the blue_state variable is true.
 { led_on_dt(&blue_led); }   //If true, switches the blue LED on.

 else   //Runs if blue_state is false.
 { led_off_dt(&blue_led); }   //Switches the blue LED off.

 k_sleep(K_MSEC(250));   //Pauses the program for 250 milliseconds before changing the blue LED state again.
 }

 return 0;   //Returns 0 to indicate successful program completion (although the infinite loop normally prevents this line being reached).
}