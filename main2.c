#include <zephyr/kernel.h>       //Includes Zephyr's kernel functions, including sleep and timing functions.
#include <zephyr/device.h>       //Includes functions for accessing and checking hardware devices.
#include <zephyr/drivers/uart.h> //Includes the functions needed for UART communication.
#include <zephyr/drivers/led.h>  //Includes the functions needed to control LEDs.

#define UART_NODE DT_CHOSEN(zephyr_console)   //Finds the UART device selected as the Zephyr console in the device tree.

static const struct device *uart_dev = DEVICE_DT_GET(UART_NODE);   //Gets the UART device and stores a pointer to it.

#define RED_LED_NODE DT_ALIAS(led0)   //Finds the red LED using its device tree alias.
#define GREEN_LED_NODE DT_ALIAS(led1) //Finds the green LED using its device tree alias.

static const struct led_dt_spec red_led =
 LED_DT_SPEC_GET(RED_LED_NODE);   //Gets the hardware information for the red LED.

static const struct led_dt_spec green_led =
 LED_DT_SPEC_GET(GREEN_LED_NODE); //Gets the hardware information for the green LED.

static void uart_send_string(const char *str)   //Creates a function called uart_send_string that sends a string of characters through UART.
{
 while (*str) {   //Repeats while there are still characters left in the string.
 uart_poll_out(uart_dev, *str++);   //Sends the current character through UART and moves to the next character.
 }
}

int main(void)   //The main function where the program starts running.
{
 unsigned char rx_data;   //Creates a variable to store the character received from the PC.

 if (!device_is_ready(uart_dev)) {   //Checks whether the UART device is ready to be used.
 return 0;   //If the UART device is not ready, the program stops.
 }

 if (!led_is_ready_dt(&red_led) || !led_is_ready_dt(&green_led)) {   //Checks whether both the red and green LEDs are ready. || means OR.
 return 0;   //If either LED is not ready, the program stops.
 }

 led_off_dt(&red_led);   //Switches the red LED off when the program starts.
 led_off_dt(&green_led); //Switches the green LED off when the program starts.

 uart_send_string(   //Calls the function to send an introductory message to the PC.
 "\r\nFRDM-MCXN947 UART Example\r\n"   //Displays the program title, with new lines before and after it.
 "Press r for RED LED\r\n"             //Displays instructions telling the user to press r for the red LED.
 "Press g for GREEN LED\r\n\r\n"       //Displays instructions telling the user to press g for the green LED.
 );

 while (1) {   //Starts an infinite loop so the program continuously checks for characters from the PC.

 if (uart_poll_in(uart_dev, &rx_data) == 0) {   //Checks whether a character has been received through UART. A return value of 0 means a character was received.

 if (rx_data == '\r' || rx_data == '\n') {   //Checks whether the received character is a carriage return or a new line.
 continue;   //Ignores these characters and returns to the beginning of the loop.
 }

 uart_poll_out(uart_dev, rx_data);   //Sends the received character back to the PC (echoes the character).
 uart_send_string("\r\n");   //Sends a new line to the PC.

 led_off_dt(&red_led);   //Switches the red LED off before processing the new command.
 led_off_dt(&green_led); //Switches the green LED off before processing the new command.

 if (rx_data == 'r') {   //Checks whether the received character is the letter r.
 led_on_dt(&red_led);   //If the character is r, switches the red LED on.
 }
 else if (rx_data == 'g') {   //Otherwise, checks whether the received character is the letter g.
 led_on_dt(&green_led);   //If the character is g, switches the green LED on.
 }
 }

 k_sleep(K_MSEC(10));   //Pauses the program for 10 milliseconds before checking for another character.
 }

 return 0;   //Returns 0 to indicate successful program completion (although the infinite loop normally prevents this line being reached).
}