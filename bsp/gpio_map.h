#ifndef _GPIO_MAP_H_
#define _GPIO_MAP_H_

#include "stm32f10x_gpio.h"

//port num
#define PORTA_OFS      0
#define PORTB_OFS      1
#define PORTC_OFS      2
#define PORTD_OFS      3
#define PORTE_OFS      4


#define GPIO_PORT_SHIFT        8
#define GPIO_OFS_SHIFT         0

#define MAKE_GPIO_NUM(p, o)   (((p) << GPIO_PORT_SHIFT) | ((o) << GPIO_OFS_SHIFT))

#define GPIO_PORT_MASK         0x0000ff00
#define GPIO_OFS_MASK          0x000000ff

#define GRAB_PORT(x)           (((x) & GPIO_PORT_MASK) >> GPIO_PORT_SHIFT)
#define GRAB_OFS(x)            (((x) & GPIO_OFS_MASK) >> GPIO_OFS_SHIFT)

//macro
#define SET_GPIO_BIT(x)     (((GPIO_TypeDef *)(GPIOA_BASE + 0x0400 * GRAB_PORT(x)))->BSRR = (1ul << GRAB_OFS(x)))

#define CLR_GPIO_BIT(x)     (((GPIO_TypeDef *)(GPIOA_BASE + 0x0400 * GRAB_PORT(x)))->BRR = (1ul << GRAB_OFS(x)))

#define GET_GPIO_BIT(x)   	((((GPIO_TypeDef *)(GPIOA_BASE + 0x0400 * GRAB_PORT(x)))->IDR >> GRAB_OFS(x)) & 0x01)

#define GET_GPIO_TYPE(x)    ((GPIO_TypeDef *)(GPIOA_BASE + 0x0400 * GRAB_PORT(x)))

#define GET_GPIO_PIN(x)     (1ul << GRAB_OFS(x))


#endif

