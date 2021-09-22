/*
 *
 * Module: Det
 *
 * File Name: Det.h
 * Description: Header file of the Development Error Tracer (Det) module.
 *              This header file is a STUB for the Det Module.
 *
 *  Created on: Aug 25, 2021
 *      Author: Shams
 */

#ifndef DET_H_
#define DET_H_


/* ID OF COMPANY -> Vendor ( Shams's ID = 5 ) */
#define DIO_VENDOR_ID       (5U)

/* MODULE ID */
#define DET_MODULE_ID       (15U)

/* DIO INSTANCE ID */
#define DET_INSTANCE_ID     (0U)


/*
 * Module Version 1.0.0
 */

#define DET_SW_MAJOR_VERSION        (1U)
#define DET_SW_MINOR_VERSION        (0U)
#define DET_SW_PATCH_VERSION        (0U)

/*
 * AUTOSAR Version 4.0.3
 */
#define DET_AR_MAJOR_VERSION        (4U)
#define DET_AR_MINOR_VERSION        (0U)
#define DET_AR_PATCH_VERSION        (3U)


/* CHECK STANDARD AUTOSAR TYPES */
#include "Std_Types.h"

/* AUTOSAR checking between Std Types and Dio Modules */
#if ((STD_TYPES_AR_RELEASE_MAJOR_VERSION != DET_AR_MAJOR_VERSION)\
 ||  (STD_TYPES_AR_RELEASE_MINOR_VERSION != DET_AR_MINOR_VERSION)\
 ||  (STD_TYPES_AR_RELEASE_PATCH_VERSION != DET_AR_PATCH_VERSION))
  #error "The AR version of Std_Types.h does not match the expected version"
#endif


/*******************************************************************************
 *                      External Variables                                     *
 *******************************************************************************/

extern uint16 Det_ModuleId;       /*   DET module ID   */
extern uint8 Det_InstanceId;      /*   DET instance ID */
extern uint8 Det_ApiId;           /*   DET API ID      */
extern uint8 Det_ErrorId;         /*   DET Error ID    */


/*******************************************************************************
 *                      Function Prototypes                                     *
 *******************************************************************************/
Std_ReturnType Det_ReportError (
        uint16 ModuleId,
        uint8 InstanceId,
        uint8 ApiId,
        uint8 ErrorId);




#endif /* DET_H_ */
