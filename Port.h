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
#ifndef PORT_H_
#define PORT_H_


/* ID OF Vendor ( Shams's Company  = 5 ) */
#define PORT_VENDOR_ID       (5U)

/* MODULE ID */
#define PORT_MODULE_ID       (124U)

/* PORT INSTANCE ID */
#define PORT_INSTANCE_ID     (0U)


/*
 * Module Version 1.0.0
 */

#define PORT_SW_MAJOR_VERSION        (1U)
#define PORT_SW_MINOR_VERSION        (0U)
#define PORT_SW_PATCH_VERSION        (0U)

/*
 * AUTOSAR Version 4.0.3
 */
#define PORT_AR_RELEASE_MAJOR_VERSION        (4U)
#define PORT_AR_RELEASE_MINOR_VERSION        (0U)
#define PORT_AR_RELEASE_PATCH_VERSION        (3U)


/*
 * PORT STATUS
 */

#define PORT_INITIALIZED                (1U)
#define PORT_NOT_INITIALIZED            (0U)


/* CHECK STANDARD AUTOSAR TYPES */
#include "Std_Types.h"

/* AUTOSAR checking between Std Types and Dio Modules */
#if ((STD_TYPES_AR_RELEASE_MAJOR_VERSION != PORT_AR_RELEASE_MAJOR_VERSION)\
 ||  (STD_TYPES_AR_RELEASE_MINOR_VERSION != PORT_AR_RELEASE_MINOR_VERSION)\
 ||  (STD_TYPES_AR_RELEASE_PATCH_VERSION != PORT_AR_RELEASE_PATCH_VERSION))
  #error "The AUTOSAR version of Std_Types.h does not match the expected version"
#endif

/* Port Pre-Compile Configuration Header file */
#include "Port_Cfg.h"

/* AUTOSAR Version checking between Dio_Cfg.h and Dio.h files */
#if ((PORT_CFG_AR_RELEASE_MAJOR_VERSION != PORT_AR_RELEASE_MAJOR_VERSION)\
 ||  (PORT_CFG_AR_RELEASE_MINOR_VERSION != PORT_AR_RELEASE_MINOR_VERSION)\
 ||  (PORT_CFG_AR_RELEASE_PATCH_VERSION != PORT_AR_RELEASE_PATCH_VERSION))
  #error "The AUTOSAR version of Port_Cfg.h does not match the expected version"
#endif

/* Software Version checking between Dio_Cfg.h and Dio.h files */
#if ((PORT_CFG_SW_MAJOR_VERSION != PORT_SW_MAJOR_VERSION)\
 ||  (PORT_CFG_SW_MINOR_VERSION != PORT_SW_MINOR_VERSION)\
 ||  (PORT_CFG_SW_PATCH_VERSION != PORT_SW_PATCH_VERSION))
  #error "The Software version of Port_Cfg.h does not match the expected version"
#endif

/* NON-AUTOSAR FILES */
#include "Common_Macros.h"



/******************************************************************************
 *                      API Service Id Macros                                 *
 ******************************************************************************/

/* Service ID for Port_Init */
#define PORT_INIT_SID                                (uint8)0x00

/* Service ID for Port_SetPinDirection */
#define PORT_SET_PIN_DIRECTION_SID                   (uint8)0x01

/* Service ID for Port_RefreshPortDirectiont */
#define PORT_REFRESH_PORT_DIRECTION_SID              (uint8)0x02

/* Service ID for Port GetVersionInfo */
#define PORT_GET_VERSION_INFO_SID                    (uint8)0x03

/* Service ID for Port_SetPinMode */
#define PORT_SET_PIN_MODE_SID                        (uint8)0x04



/*******************************************************************************
 *                      DET Error Codes                                        *
 *******************************************************************************/

/* DET code to report Incorrect Port Pin ID passed  */
#define PORT_E_PARAM_PIN                    (uint8)0x0A

/* DET code to report Port Pin not configured as changeable */
#define PORT_E_DIRECTION_UNCHANGEABLE       (uint8)0x0B

/* DET code to report API Port_Init service called with wrong parameter */
#define PORT_E_PARAM_CONFIG                 (uint8)0x0C

/* DET code to report API Port_SetPinMode service called when mode is invalid. */
#define PORT_E_PARAM_INVALID_MODE           (uint8)0x0D

/* DET code to report API Port_SetPinMode service called when mode is unchangeable. */
#define PORT_E_MODE_UNCHANGEABLE            (uint8)0x0E

/* DET code to report API service called without module initialization */
#define PORT_E_UNINIT                       (uint8)0x0F

/* DET code to report APIs called with a Null Pointer */
#define PORT_E_PARAM_POINTER                (uint8)0x10

/*******************************************************************************
 *                              Module Definitions                             *
 *******************************************************************************/

/* Enable Digital or Analog Mode */
#define DISABLE_DIGITAL                      (0U)
#define ENABLE_DIGITAL                       (1U)
#define ENABLE_ANALOG                        (2U)


/* Is Pin Direction changeable or not */
#define DIR_CHANGEABLE                       (1U)
#define DIR_UNCHANGEABLE                     (0U)

/* Is Pin Mode changeable or not */
#define MODE_CHANGEABLE                      (1U)
#define MODE_UNCHANGEABLE                    (0U)
   
/* Used to clear PCTL bits for pin to be DIO pin */   
#define PIN_CLEAR_PCTL_M                     (0xF)

/* This part of the code is here and not in configurations to protect the static code */
/*  Port ID's  */
#define PORTA_ID_M                (uint8)0
#define PORTB_ID_M                (uint8)1
#define PORTC_ID_M                (uint8)2
#define PORTD_ID_M                (uint8)3
#define PORTE_ID_M                (uint8)4
#define PORTF_ID_M                (uint8)5

/* Port Pin ID's */
#define PIN0_ID_M                 (uint8)0
#define PIN1_ID_M                 (uint8)1
#define PIN2_ID_M                 (uint8)2
#define PIN3_ID_M                 (uint8)3
#define PIN4_ID_M                 (uint8)4
#define PIN5_ID_M                 (uint8)5
#define PIN6_ID_M                 (uint8)6
#define PIN7_ID_M                 (uint8)7

/******************************************************************************
 *                      PORT MODULE DATA TYPES                                 *
 ******************************************************************************/


/* Numeric ID for Port Pins, MCU platform based (43 pins)  */
typedef uint8 Port_PinType;

/* Numeric ID for Port Pin modes */
typedef uint8 Port_PinModeType;

/* Type for defining the direction of a Port Pin */
typedef enum
{
    PORT_PIN_IN , PORT_PIN_OUT
}Port_PinDirectionType;

/* NOT IN SWS ( IMPLEMENTATION SPECIFIC ) */
/* Type to configure the internal resistors */
typedef enum
{
    R_OFF,PULL_DOWN,PULL_UP
}Port_PinInternalResistor;


/* NOT IN SWS ( IMPLEMENTATION SPECIFIC ) */
/* Stucture That Consists of the Configuration structure for the pin */
typedef struct
{
    /* PORT ID */
    uint8 Port_Num;
    /* Pin Channel ID */
    uint8 Pin_Num;
    /* Pin Direction */
    Port_PinDirectionType Direction;
    /* Configure internal resistor if there is*/
    Port_PinInternalResistor Resistor;
    /* Pin Mode */
    Port_PinModeType PCTL_Mode;
    /* Enable Digital or Analog mode */
    uint8 DEN_NAMSEL;
    /* Initial Value of Pin */
    uint8 InitialValue;
    /* Change Pin Direction During Runtime Configuration */
    uint8 PortPinDirectionChangeable;
    /* Change Mode During Runtime Configuration */
    uint8 PortPinModeChangeable;
}Port_ConfigPin;


/* IMPLEMENTATION SPECIFIC */
/* Type of the external data structure containing the initialization data for this module */
typedef struct
{
    Port_ConfigPin Pins[PortNumberOfPortPins];
}Port_ConfigType;



/*******************************************************************************
 *                      Function Prototypes                                    *
 *******************************************************************************/

/* Initializes the Port Driver module */
void Port_Init( const Port_ConfigType* ConfigPtr );

#if (PORT_SET_PIN_DIRECTION_API == STD_ON )
/* Sets the port pin direction during runtime */
void Port_SetPinDirection(Port_PinType Pin, Port_PinDirectionType Direction);
#endif

/* Refreshes direction of all configured ports to the configured direction */
void Port_RefreshPortDirection(void);

#if (PORT_VERSION_INFO_API == STD_ON )
/* Returns the version information of this module */
void Port_GetVersionInfo( Std_VersionInfoType* versioninfo );
#endif

#if (PORT_SET_PIN_MODE_API == STD_ON)
/* Sets the port pin mode */
void Port_SetPinMode( Port_PinType Pin, Port_PinModeType Mode );
#endif

/*******************************************************************************
 *                       External Variables                                    *
 *******************************************************************************/

/* Extern PostBuild structure to be used by the PORT module */
extern const Port_ConfigType Port_Configuration;


#endif /* PORT_H_ */



