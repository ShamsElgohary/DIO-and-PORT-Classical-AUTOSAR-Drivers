 /******************************************************************************
 *
 * Module: Port
 *
 * File Name: Port.h
 *
 * Description: Header file for TM4C123GH6PM Microcontroller - Port Driver.
 *
 * Author: Shams Elgohary
 ******************************************************************************/

#ifndef PORT_CFG_H_
#define PORT_CFG_H_

/*
 * Module Version 1.0.0
 */
#define PORT_CFG_SW_MAJOR_VERSION              (1U)
#define PORT_CFG_SW_MINOR_VERSION              (0U)
#define PORT_CFG_SW_PATCH_VERSION              (0U)

/*
 * AUTOSAR Version 4.0.3
 */
#define PORT_CFG_AR_RELEASE_MAJOR_VERSION     (4U)
#define PORT_CFG_AR_RELEASE_MINOR_VERSION     (0U)
#define PORT_CFG_AR_RELEASE_PATCH_VERSION     (3U)



/* Pre-compile option for Development Error Detect */
#define PORT_DEV_ERROR_DETECT                (STD_ON)

/* Pre-compile option to use Port_SetPinDirection API */
#define PORT_SET_PIN_DIRECTION_API           (STD_ON)

/* Pre-compile option to use Port_SetPinDirection API */
#define PORT_SET_PIN_MODE_API                (STD_ON)

/* Pre-compile option for Version Info API */
#define PORT_VERSION_INFO_API                (STD_ON)

/* Number of the configured Pins */
/* TivaC has a total of 43 Pins */
#define PortNumberOfPortPins                 (43U)


/*  Port ID's  */
#define PORTA_ID               (0U)
#define PORTB_ID               (1U)
#define PORTC_ID               (2U)
#define PORTD_ID               (3U)
#define PORTE_ID               (4U)
#define PORTF_ID               (5U)

/* Port Pin ID's */
#define PIN0_ID                (0U)
#define PIN1_ID                (1U)
#define PIN2_ID                (2U)
#define PIN3_ID                (3U)
#define PIN4_ID                (4U)
#define PIN5_ID                (5U)
#define PIN6_ID                (6U)
#define PIN7_ID                (7U)


#endif /* PORT_CFG_H_ */
