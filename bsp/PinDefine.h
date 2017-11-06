
#ifndef _PINDEFINE_H_
#define _PINDEFINE_H_

#include "gpio_map.h"

/* pin define */ 

/* pin for main board 1/2 data switch */
#define PIN_USB_A_SEL  MAKE_GPIO_NUM(PORTA_OFS, 0)

/* pin for main board 1/2 enable switch */
#define PIN_EN_A       MAKE_GPIO_NUM(PORTA_OFS, 1)
#define PIN_EN_B       MAKE_GPIO_NUM(PORTA_OFS, 2)

/* 74238, for main board 1/8 enable switch */
#define PIN_238_A      MAKE_GPIO_NUM(PORTA_OFS, 6)
#define PIN_238_B      MAKE_GPIO_NUM(PORTA_OFS, 7)
#define PIN_238_C      MAKE_GPIO_NUM(PORTB_OFS, 0)

#define PIN_238_SEL    MAKE_GPIO_NUM(PORTB_OFS, 1)
#define PIN_238_EN     MAKE_GPIO_NUM(PORTB_OFS, 2)

/* pin for main board 1/8 data switch */
#define PIN_C0         MAKE_GPIO_NUM(PORTA_OFS, 3)
#define PIN_C1         MAKE_GPIO_NUM(PORTA_OFS, 4)
#define PIN_C2         MAKE_GPIO_NUM(PORTA_OFS, 5)

/* pin for slave board 1/8 switch */
#define PIN_SLAVE_C0   MAKE_GPIO_NUM(PORTC_OFS, 15)
#define PIN_SLAVE_C1   MAKE_GPIO_NUM(PORTC_OFS, 14)
#define PIN_SLAVE_C2   MAKE_GPIO_NUM(PORTC_OFS, 13)

#endif

