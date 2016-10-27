/*

	Pin configurations (input, output)

*/

#include<pinDefinitions.h>
#include<avr/interrupt.h>

#define BVV(bit, val) ((val)?_BV(bit):0)

/********** Digital Mode **********/

#define DigitalB_OUTPUT(pin)  DDRB |= _BV(pin)
#define DigitalB_INPUT(pin)   DDRB &= ~_BV(pin)
#define DigitalB_HIGH(pin)   PORTB |= _BV(pin)
#define DigitalB_LOW(pin)  PORTB &= ~_BV(pin)
#define DigitalB_read(pin) ((bit_is_set(PINB, pin))? HIGH:LOW)


#define DigitalD_OUTPUT(pin)  DDRD |= _BV(pin)
#define DigitalD_INPUT(pin)   DDRD &= ~_BV(pin)
#define DigitalD_HIGH(pin)   PORTD |= _BV(pin)
#define DigitalD_LOW(pin)  PORTD &= ~_BV(pin)
#define DigitalD_read(pin) ((bit_is_set(PIND, pin))? HIGH:LOW)

/********** Analog Mode **********/

void Analog_INPUT0(){

    ADMUX =   BVV(REFS0, 1) | BVV(REFS1, 0) /* AVCC as reference voltage */
            | BVV(ADLAR, 0) /* Left justify */
            | BVV(MUX0, 0) | BVV(MUX1, 0) | BVV(MUX2, 0) | BVV(MUX3, 0); /* Read ADC0 */       //HACER UNA FUNCION PARA OPTIMIZAR ESTO

    ADCSRA =   BVV(ADEN, 1) /* Enable */
             | BVV(ADSC, 0) /* Don't start conversion */
             | BVV(ADATE, 0) /* No auto-update */
             | BVV(ADIE, 0) | BVV(ADIF, 0) /* No interrupt */
             | BVV(ADPS0, 1) | BVV(ADPS1, 1) | BVV(ADPS2, 1); /* clk/128 */
}

void Analog_INPUT1(){
    ADMUX =   BVV(REFS0, 1) | BVV(REFS1, 0) /* AVCC as reference voltage */
            | BVV(ADLAR, 0) /* Left justify */
            | BVV(MUX0, 1) | BVV(MUX1, 0) | BVV(MUX2, 0) | BVV(MUX3, 0); /* Read ADC0 */       //HACER UNA FUNCION PARA OPTIMIZAR ESTO

    ADCSRA =   BVV(ADEN, 1) /* Enable */
             | BVV(ADSC, 0) /* Don't start conversion */
             | BVV(ADATE, 0) /* No auto-update */
             | BVV(ADIE, 0) | BVV(ADIF, 0) /* No interrupt */
             | BVV(ADPS0, 1) | BVV(ADPS1, 1) | BVV(ADPS2, 1); /* clk/128 */
}

void Analog_INPUT2(){
    ADMUX =   BVV(REFS0, 1) | BVV(REFS1, 0) /* AVCC as reference voltage */
            | BVV(ADLAR, 0) /* Left justify */
            | BVV(MUX0, 0) | BVV(MUX1, 1) | BVV(MUX2, 0) | BVV(MUX3, 0); /* Read ADC2 */       //HACER UNA FUNCION PARA OPTIMIZAR ESTO

    ADCSRA =   BVV(ADEN, 1) /* Enable */
             | BVV(ADSC, 0) /* Don't start conversion */
             | BVV(ADATE, 0) /* No auto-update */
             | BVV(ADIE, 0) | BVV(ADIF, 0) /* No interrupt */
             | BVV(ADPS0, 1) | BVV(ADPS1, 1) | BVV(ADPS2, 1); /* clk/128 */
}

void Analog_INPUT3(){
    ADMUX =   BVV(REFS0, 1) | BVV(REFS1, 0) /* AVCC as reference voltage */
            | BVV(ADLAR, 0) /* Left justify */
            | BVV(MUX0, 1) | BVV(MUX1, 1) | BVV(MUX2, 0) | BVV(MUX3, 0); /* Read ADC0 */       //HACER UNA FUNCION PARA OPTIMIZAR ESTO

    ADCSRA =   BVV(ADEN, 1) /* Enable */
             | BVV(ADSC, 0) /* Don't start conversion */
             | BVV(ADATE, 0) /* No auto-update */
             | BVV(ADIE, 0) | BVV(ADIF, 0) /* No interrupt */
             | BVV(ADPS0, 1) | BVV(ADPS1, 1) | BVV(ADPS2, 1); /* clk/128 */
}

void Analog_INPUT4(){
    ADMUX =   BVV(REFS0, 1) | BVV(REFS1, 0) /* AVCC as reference voltage */
            | BVV(ADLAR, 0) /* Left justify */
            | BVV(MUX0, 0) | BVV(MUX1, 0) | BVV(MUX2, 1) | BVV(MUX3, 0); /* Read ADC0 */       //HACER UNA FUNCION PARA OPTIMIZAR ESTO

    ADCSRA =   BVV(ADEN, 1) /* Enable */
             | BVV(ADSC, 0) /* Don't start conversion */
             | BVV(ADATE, 0) /* No auto-update */
             | BVV(ADIE, 0) | BVV(ADIF, 0) /* No interrupt */
             | BVV(ADPS0, 1) | BVV(ADPS1, 1) | BVV(ADPS2, 1); /* clk/128 */
}

void Analog_INPUT5(){
    ADMUX =   BVV(REFS0, 1) | BVV(REFS1, 0) /* AVCC as reference voltage */
            | BVV(ADLAR, 0) /* Left justify */
            | BVV(MUX0, 1) | BVV(MUX1, 0) | BVV(MUX2, 1) | BVV(MUX3, 0); /* Read ADC0 */       //HACER UNA FUNCION PARA OPTIMIZAR ESTO

    ADCSRA =   BVV(ADEN, 1) /* Enable */
             | BVV(ADSC, 0) /* Don't start conversion */
             | BVV(ADATE, 0) /* No auto-update */
             | BVV(ADIE, 0) | BVV(ADIF, 0) /* No interrupt */
             | BVV(ADPS0, 1) | BVV(ADPS1, 1) | BVV(ADPS2, 1); /* clk/128 */
}

void Analog_INPUT6(){
    ADMUX =   BVV(REFS0, 1) | BVV(REFS1, 0) /* AVCC as reference voltage */
            | BVV(ADLAR, 0) /* Left justify */
            | BVV(MUX0, 0) | BVV(MUX1, 1) | BVV(MUX2, 1) | BVV(MUX3, 0); /* Read ADC0 */       //HACER UNA FUNCION PARA OPTIMIZAR ESTO

    ADCSRA =   BVV(ADEN, 1) /* Enable */
             | BVV(ADSC, 0) /* Don't start conversion */
             | BVV(ADATE, 0) /* No auto-update */
             | BVV(ADIE, 0) | BVV(ADIF, 0) /* No interrupt */
             | BVV(ADPS0, 1) | BVV(ADPS1, 1) | BVV(ADPS2, 1); /* clk/128 */
}

int Analog_read(){
    ADCSRA |= _BV(ADSC);
    loop_until_bit_is_clear(ADCSRA, ADSC);
    uint8_t low  = ADCL;
    uint8_t high = ADCH;

    return (high << 8) | low;
}
