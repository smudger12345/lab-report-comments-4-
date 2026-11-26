//To create a simple LED level indicator. As the analogue input voltage increases, the ADC reading increases, and more LEDs switch on.
#include <zephyr/kernel.h>       //Includes Zephyr's kernel functions, including sleep and timing functions.
#include <zephyr/drivers/adc.h>   //Includes the functions needed to use the ADC.
#include <zephyr/drivers/gpio.h>  //Includes the functions needed to control GPIO pins (LEDs).

#define ADC_NODE DT_PATH(zephyr_user)   //Creates a reference to the zephyr,user node in the app.overlay file.

static const struct adc_dt_spec adc_channel =
 ADC_DT_SPEC_GET(ADC_NODE);   //Gets the ADC configuration from the device tree and stores it in adc_channel.

static int16_t adc_value;   //Stores the ADC conversion result as a 16-bit integer.

#define LED1_NODE DT_NODELABEL(teach_led1)   //Refers to LED 1 in the app.overlay file.
#define LED2_NODE DT_NODELABEL(teach_led2)   //Refers to LED 2 in the app.overlay file.
#define LED3_NODE DT_NODELABEL(teach_led3)   //Refers to LED 3 in the app.overlay file.
#define LED4_NODE DT_NODELABEL(teach_led4)   //Refers to LED 4 in the app.overlay file.

static const struct gpio_dt_spec led1 = GPIO_DT_SPEC_GET(LED1_NODE, gpios);   //Gets the GPIO pin information for LED 1.
static const struct gpio_dt_spec led2 = GPIO_DT_SPEC_GET(LED2_NODE, gpios);   //Gets the GPIO pin information for LED 2.
static const struct gpio_dt_spec led3 = GPIO_DT_SPEC_GET(LED3_NODE, gpios);   //Gets the GPIO pin information for LED 3.
static const struct gpio_dt_spec led4 = GPIO_DT_SPEC_GET(LED4_NODE, gpios);   //Gets the GPIO pin information for LED 4.

int main(void)   //The main function where the program starts running.
{
 int ret;   //Creates an integer variable used to store return values from functions.

 if (!adc_is_ready_dt(&adc_channel))   //Checks whether the ADC device is ready to be used.
 { return 0; }   //If the ADC is not ready, the program stops.

 ret = adc_channel_setup_dt(&adc_channel);   //Sets up the ADC channel using the configuration from the device tree.
 if (ret < 0) { return 0; }   //If setup fails, the program stops.

 struct adc_sequence sequence = {   //Creates an ADC sequence structure that stores the settings for an ADC reading.
 .buffer = &adc_value,   //Specifies where the ADC conversion result should be stored.
 .buffer_size = sizeof(adc_value),   //Specifies the size of the memory available to store the ADC result.
 };

 ret = adc_sequence_init_dt( &adc_channel, &sequence );   //Initialises the ADC sequence using the ADC configuration.
 if (ret < 0) { return 0; }   //If initialisation fails, the program stops.

 gpio_pin_configure_dt( &led1, GPIO_OUTPUT_INACTIVE );   //Configures LED 1 as an output and initially switches it off.
 gpio_pin_configure_dt( &led2, GPIO_OUTPUT_INACTIVE );   //Configures LED 2 as an output and initially switches it off.
 gpio_pin_configure_dt( &led3, GPIO_OUTPUT_INACTIVE );   //Configures LED 3 as an output and initially switches it off.
 gpio_pin_configure_dt( &led4, GPIO_OUTPUT_INACTIVE );   //Configures LED 4 as an output and initially switches it off.

while (1) {   //Starts an infinite loop so the program continuously reads the ADC.

 ret = adc_read_dt( &adc_channel, &sequence );   //Reads the current analogue input and converts it into a digital value.

 if (ret == 0) {   //Checks whether the ADC reading was successful.

 gpio_pin_set_dt( &led1, adc_value > 819 );   //Turns LED 1 on if the ADC value is greater than 819.
 gpio_pin_set_dt( &led2, adc_value > 1638 );   //Turns LED 2 on if the ADC value is greater than 1638.
 gpio_pin_set_dt( &led3, adc_value > 2457 );   //Turns LED 3 on if the ADC value is greater than 2457.
 gpio_pin_set_dt( &led4, adc_value > 3276 );   //Turns LED 4 on if the ADC value is greater than 3276.

 }

 k_sleep(K_MSEC(50));   //Pauses the program for 50 milliseconds before taking another ADC reading.
 }

 return 0;   //Returns 0 to indicate successful program completion (although the infinite loop normally prevents this line being reached).
}