/*
 * Module: DIO
 *
 * File Name : Dio.h
 * Description: Header file for TM4C123GH6PM Microcontroller - Dio Driver
 *      Author: Shams Elgohary
 */

#ifndef DIO_H_
#define DIO_H_

/* ID OF COMPANY -> Vendor ( Shams's ID = 5 ) */
#define DIO_VENDOR_ID       (5U)

/* MODULE ID */
#define DIO_MODULE_ID       (120U)

/* DIO INSTANCE ID */
#define DIO_INSTANCE_ID     (0U)


/*
 * Module Version 1.0.0
 */

#define DIO_SW_MAJOR_VERSION        (1U)
#define DIO_SW_MINOR_VERSION        (0U)
#define DIO_SW_PATCH_VERSION        (0U)

/*
 * AUTOSAR Version 4.0.3
 */
#define DIO_AR_RELEASE_MAJOR_VERSION        (4U)
#define DIO_AR_RELEASE_MINOR_VERSION        (0U)
#define DIO_AR_RELEASE_PATCH_VERSION        (3U)


/*
 * DIO STATUS
 */

#define DIO_INITIALIZED                (1U)
#define DIO_NOT_INITIALIZED            (0U)

/* CHECK STANDARD AUTOSAR TYPES */
#include "Std_Types.h"

/* AUTOSAR checking between Std Types and Dio Modules */
#if ((STD_TYPES_AR_RELEASE_MAJOR_VERSION != DIO_AR_RELEASE_MAJOR_VERSION)\
 ||  (STD_TYPES_AR_RELEASE_MINOR_VERSION != DIO_AR_RELEASE_MINOR_VERSION)\
 ||  (STD_TYPES_AR_RELEASE_PATCH_VERSION != DIO_AR_RELEASE_PATCH_VERSION))
  #error "The AR version of Std_Types.h does not match the expected version"
#endif

/* Dio Pre-Compile Configuration Header file */
#include "Dio_Cfg.h"

/* AUTOSAR Version checking between Dio_Cfg.h and Dio.h files */
#if ((DIO_CFG_AR_RELEASE_MAJOR_VERSION != DIO_AR_RELEASE_MAJOR_VERSION)\
 ||  (DIO_CFG_AR_RELEASE_MINOR_VERSION != DIO_AR_RELEASE_MINOR_VERSION)\
 ||  (DIO_CFG_AR_RELEASE_PATCH_VERSION != DIO_AR_RELEASE_PATCH_VERSION))
  #error "The AR version of Dio_Cfg.h does not match the expected version"
#endif

/* Software Version checking between Dio_Cfg.h and Dio.h files */
#if ((DIO_CFG_SW_MAJOR_VERSION != DIO_SW_MAJOR_VERSION)\
 ||  (DIO_CFG_SW_MINOR_VERSION != DIO_SW_MINOR_VERSION)\
 ||  (DIO_CFG_SW_PATCH_VERSION != DIO_SW_PATCH_VERSION))
  #error "The SW version of Dio_Cfg.h does not match the expected version"
#endif

/* NON-AUTOSAR FILES */
#include "Common_Macros.h"


/******************************************************************************
 *                      API Service Id Macros                                 *
 ******************************************************************************/

/* Service ID for DIO read Channel */
#define DIO_READ_CHANNEL_SID           (uint8)0x00

/* Service ID for DIO write Channel */
#define DIO_WRITE_CHANNEL_SID          (uint8)0x01

/* Service ID for DIO read Port */
#define DIO_READ_PORT_SID              (uint8)0x02

/* Service ID for DIO write Port */
#define DIO_WRITE_PORT_SID             (uint8)0x03

/* Service ID for DIO read Channel Group */
#define DIO_READ_CHANNEL_GROUP_SID     (uint8)0x04

/* Service ID for DIO write Channel Group */
#define DIO_WRITE_CHANNEL_GROUP_SID    (uint8)0x05

/* Service ID for DIO GetVersionInfo */
#define DIO_GET_VERSION_INFO_SID       (uint8)0x12

/* Service ID for DIO Init Channel */
#define DIO_INIT_SID                   (uint8)0x10

/* Service ID for DIO flip Channel */
#define DIO_FLIP_CHANNEL_SID           (uint8)0x11


/*******************************************************************************
 *                      DET Error Codes                                        *
 *******************************************************************************/

/* DET code to report Invalid Channel */
#define DIO_E_PARAM_INVALID_CHANNEL_ID (uint8)0x0A

/* Dio_Init API service called with NULL pointer parameter */
#define DIO_E_PARAM_CONFIG             (uint8)0x10

/* DET code to report Invalid Port */
#define DIO_E_PARAM_INVALID_PORT_ID    (uint8)0x14

/* DET code to report Invalid Channel Group */
#define DIO_E_PARAM_INVALID_GROUP      (uint8)0x1F

/* API service called with a NULL pointer. In case of error the API service shall return
 * immediately without any further action, beside reporting this development error
 */
#define DIO_E_PARAM_POINTER             (uint8)0x20

/* API service used without module initialization is reported using following  */
#define DIO_E_UNINIT                   (uint8)0xF0


/******************************************************************************
 *                      DIO MODULE DATA TYPES                                 *
 ******************************************************************************/

/* Numeric ID of a DIO channel */
typedef uint8 Dio_ChannelType;

/* Numeric ID of a DIO Port */
typedef uint8 Dio_PortType;

/* Possible levels a DIO channel can have (input or output) */
typedef uint8 Dio_LevelType;

/* Value of a DIO port */
typedef uint8 Dio_PortLevelType;


/* Definition of a Structure of a Channel Group */
typedef struct
{
  /* Mask which defines the positions of the channel group */
  uint8 mask;
  /* Position of the group from LSB */
  uint8 offset;
  /* Port ID which the Channel group is defined. */
  Dio_PortType PortIndex;
} Dio_ChannelGroupType;

/* NOT IN SWS ( IMPLEMENTATION SPECIFIC ) */
/* Stucture That Consists of the Port and Channel Id */
typedef struct
{
    /* Member contains the ID of the Port that this channel belongs to */
    Dio_PortType Port_Num;
    /* Member contains the ID of the Channel*/
    Dio_ChannelType Ch_Num;
}Dio_ConfigChannel;


/* Data Structure required for initializing the Dio Driver (IMPLEMENTATION SPECIFIC) */
typedef struct Dio_ConfigType
{
    Dio_ConfigChannel Channels[DIO_CONFIGURED_CHANNLES];
} Dio_ConfigType;


/*******************************************************************************
 *                      Function Prototypes                                    *
 *******************************************************************************/

/* Function for DIO read Channel API */
Dio_LevelType Dio_ReadChannel(Dio_ChannelType ChannelId);

/* Function for DIO Write Channel API */
void Dio_WriteChannel(Dio_ChannelType ChannelId,Dio_LevelType Level);

/* Function for DIO Read Port API */
Dio_PortLevelType Dio_ReadPort(Dio_PortType PortId);

/* Function for DIO Write Port API */
void Dio_WritePort( Dio_PortType PortId, Dio_PortLevelType Level );

/* Function for DIO Initialization API */
void Dio_Init(const Dio_ConfigType * ConfigPtr);

#if (DIO_FLIP_CHANNEL_API == STD_ON)
/* Function for DIO flip channel API */
Dio_LevelType Dio_FlipChannel(Dio_ChannelType ChannelId);
#endif

/* Function for DIO Get Version Info API */
#if (DIO_VERSION_INFO_API == STD_ON)
void Dio_GetVersionInfo(Std_VersionInfoType *versioninfo);
#endif


/*******************************************************************************
 *                       External Variables                                    *
 *******************************************************************************/

/* Extern PostBuild structure to be used by Dio and other modules */
extern const Dio_ConfigType Dio_Configuration;

#endif /* DIO_H_ */
