#include <zephyr/dt-bindings/adc/adc.h>  //Includes Zephyr's ADC (Analogue-to-Digital Converter) definitions.
/ { zephyr,user {     //Creates a node called user under Zephyr's zephyr node. This is commonly used to define hardware resources that your application wants to access.
 io-channels = <&lpadc0 0>;   //to use channel 0 of the LPADC0 analogue-to-digital converter.
 };
 teach_leds {   //This section tells Zephyr which GPIO pins are connected to your four LEDs.
 compatible = "gpio-leds";
 teach_led1: led_1 { gpios = <&gpio4 12 GPIO_ACTIVE_HIGH>; };  //slecets GPIO contrller 4 slects pin number 12 on gpio controller 
 teach_led2: led_2 { gpios = <&gpio0 22 GPIO_ACTIVE_HIGH>; };
 teach_led3: led_3 { gpios = <&gpio0 15 GPIO_ACTIVE_HIGH>; };
 teach_led4: led_4 { gpios = <&gpio0 14 GPIO_ACTIVE_HIGH>; };
 };
 };
&lpadc0 {   //low  power analogue to digital converter
 status = "okay";   //Enables the ADC peripheral so Zephyr can use it.
 #address-cells = <1>;  //child nodes use one cell to represent their address.
 #size-cells = <0>;  //child nodes do not use a separate size value.
 channel@0 {  //Creates a child node representing ADC channel 0.
 reg = <0>;
 zephyr,gain = "ADC_GAIN_1";   //the input is measured without additional amplification or attenuation from the gain setting.
 zephyr,reference = "ADC_REF_EXTERNAL0";   //the ADC uses the external reference input called EXT0.
 zephyr,acquisition-time = <ADC_ACQ_TIME_DEFAULT>;  //Determines how long the ADC samples the input voltage before performing the conversion.
 zephyr,resolution = <12>;  //Sets the ADC resolution to 12 bits.
 zephyr,input-positive = <0>;  //Selects the positive analogue input for the ADC channel.
 };
};
