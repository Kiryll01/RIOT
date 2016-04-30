/*
 * Copyright (C) 2016 Unwired Devices
 *
 * This file is subject to the terms and conditions of the GNU Lesser General
 * Public License v2.1. See the file LICENSE in the top level directory for more
 * details.
 */

/**
 * @defgroup    boards_unwd-range-l1 Unwired Range-l1
 * @ingroup     boards
 * @brief       Board specific files for the unwd-range-l1 board.
 * @{
 *
 * @file
 * @brief       Board specific definitions for the unwd-range-l1 board.
 *
 * @author      Cr0s
 */

#ifndef BOARD_H_
#define BOARD_H_

#include "board_common.h"

#ifdef __cplusplus
extern "C" {
#endif

/**
 * @name SX1276 configuration
 * @{
 */
#define SX1276_DIO0 GPIO_PIN(PORT_A, 8)
#define SX1276_DIO1 GPIO_PIN(PORT_A, 9)
#define SX1276_DIO2 GPIO_PIN(PORT_A, 10)
#define SX1276_DIO3 GPIO_PIN(PORT_A, 11)

/** Antenna mode (RX/TX) switching pin */
#define SX1276_ANTSW GPIO_PIN(PORT_A, 12)

/** SX1276 SPI */
#define SX1276_SPI SPI_1
#define SX1276_SPI_NSS GPIO_PIN(PORT_B, SPI_1_PIN_NSS)
#define SX1276_SPI_MODE SPI_CONF_FIRST_RISING
#define SX1276_SPI_SPEED SPI_SPEED_1MHZ
/** Unused pins */
#define SX1276_DIO4 NULL
#define SX1276_DIO5 NULL
#define SX1276_RESET NULL

/** @} */

/**
 * @name xtimer configuration
 * @{
 */
#define XTIMER              TIMER_DEV(0)
#define XTIMER_CHAN         (0)
#define XTIMER_OVERHEAD     (6)
#define XTIMER_BACKOFF      (3)
/** @} */

#ifdef __cplusplus
}
#endif

#endif /* BOARD_H_ */
/** @} */