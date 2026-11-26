#include <zephyr/kernel.h>       //Includes Zephyr's kernel functions, including timers, sleep and timing functions.
#include <zephyr/drivers/led.h>  //Includes the functions needed to control the onboard LEDs.

#define LED1_NODE DT_ALIAS(led0)   //Finds the first LED using its device tree alias (red LED).
#define LED2_NODE DT_ALIAS(led1)   //Finds the second LED using its device tree alias (green LED).

static const struct led_dt_spec led1 = LED_DT_SPEC_GET(LED1_NODE);   //Gets the hardware information needed to control LED 1.
static const struct led_dt_spec led2 = LED_DT_SPEC_GET(LED2_NODE);   //Gets the hardware information needed to control LED 2.

static volatile bool timeout_flag = false;   //Creates a true/false variable to indicate whether the timer interrupt has occurred.

static bool led2_state = true;   //Creates a variable to remember the green LED state. Initially true (ON).

static void timeout_handler(struct k_timer *timer)   //Creates the timer callback function that runs when the timer expires.
{
 ARG_UNUSED(timer);   //Indicates that the timer parameter is not being used in this function.
 timeout_flag = true;   //Changes the timeout flag to true to indicate that the timer has expired.
}

K_TIMER_DEFINE( response_timer, timeout_handler, NULL );   //Creates and defines a software timer called response_timer, using timeout_handler when it expires and no stop callback.

int main(void)   //The main function where the program starts running.
{
 bool led1_state = false;   //Creates a true/false variable to remember whether the red LED is on or off. Initially false (OFF).

 if (!led_is_ready_dt(&led1))   //Checks whether LED 1 (red LED) is ready to be used.
 { return 0; }   //If LED 1 is not ready, the program stops.

 if (!led_is_ready_dt(&led2))   //Checks whether LED 2 (green LED) is ready to be used.
 { return 0; }   //If LED 2 is not ready, the program stops.

 led_on_dt(&led2);   //Switches the green LED on when the program starts.

 k_timer_start( &response_timer, K_SECONDS(10), K_NO_WAIT );   //Starts the timer with a 10-second delay and tells it not to repeat after expiring.

while (1) {   //Starts an infinite loop so the program continuously flashes the red LED.

 if (timeout_flag)   //Checks whether the timer has expired and the timeout flag is true.
 {
 timeout_flag = false;   //Resets the timeout flag to false so the timer event is not processed repeatedly.

 led2_state = !led2_state;   //Reverses the green LED state (ON becomes OFF, OFF becomes ON).

 if (led2_state)   //Checks whether the green LED state is true.
 { led_on_dt(&led2); }   //If true, switches the green LED on.

 else   //Runs if the green LED state is false.
 { led_off_dt(&led2); }   //Switches the green LED off.
 }

led1_state = !led1_state;   //Reverses the red LED state (ON becomes OFF, OFF becomes ON).

 if (led1_state)   //Checks whether the red LED state is true.
 { led_on_dt(&led1); }   //If true, switches the red LED on.

 else   //Runs if the red LED state is false.
 { led_off_dt(&led1); }   //Switches the red LED off.

 k_sleep(K_SECONDS(1));   //Pauses the program for 1 second before changing the red LED state again.
 }

 return 0;   //Returns 0 to indicate successful program completion (although the infinite loop normally prevents this line being reached).
}