
#ifndef _SWITCH_H_
#define _SWITCH_H_

/**
 * Initilize switch.
 */
extern int init_switch(void);

/**
 * Switch port on main board.
 *
 * @param port: port on main board(0 ~ 7)
 *
 * @return: RT_TRUE on OK, RT_FALSE on error
 */
extern rt_bool_t switch_main_board(int port);

/**
 * Disable main board
 */
extern void disable_main_board(void);

/**
 * Disable all slave boards connected to main board.
 */
extern void disable_all_slave_board(void);

/**
 * Enable slave board connected to main board.
 *
 * @param main_port: port on main board(0 ~ 7)
 *
 * @return: RT_TRUE on OK, RT_FALSE on error
 */
extern rt_bool_t enable_slave_board(int main_port);

/**
 * Switch port on slave board.
 *
 * @param slave_port: port on slave board(0 ~ 15)
 *
 * @return: RT_TRUE on OK, RT_FALSE on error
 */
extern rt_bool_t switch_slave_board(int slave_port);

#endif

