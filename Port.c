 /******************************************************************************
 *
 * Module: Port
 *
 * File Name: Port.c
 *
 * Description: Source file for TM4C123GH6PM Microcontroller - Port Driver.
 *
 * Author: Shams Elgohary
 ******************************************************************************/

#include "Port.h"
#include "Port_Regs.h"

#if (PORT_DEV_ERROR_DETECT == STD_ON)

#include "Det.h"
/* AUTOSAR Version checking between Det and Port Modules */
#if ((DET_AR_MAJOR_VERSION != PORT_AR_RELEASE_MAJOR_VERSION)\
 || (DET_AR_MINOR_VERSION != PORT_AR_RELEASE_MINOR_VERSION)\
 || (DET_AR_PATCH_VERSION != PORT_AR_RELEASE_PATCH_VERSION))
  #error "The AR version of Port.h does not match the expected version"
#endif

#endif


STATIC const Port_ConfigPin * PinConfigArray = NULL_PTR;
STATIC uint8 Port_Status = PORT_NOT_INITIALIZED;


/************************************************************************************
* Service Name: Port_Init
* Service ID[hex]: 0x00
* Sync/Async: Synchronous
* Reentrancy: Non reentrant
* Parameters (in): ConfigPtr - Pointer to post-build configuration data
* Parameters (inout): None
* Parameters (out): None
* Return value: None
* Description: Function to Initialize the Port Driver Module.
************************************************************************************/
void Port_Init( const Port_ConfigType* ConfigPtr )
{
#if (PORT_DEV_ERROR_DETECT == STD_ON)
    /* check if the input configuration pointer is not a NULL_PTR */
    if (NULL_PTR == ConfigPtr)
    {
        Det_ReportError(PORT_MODULE_ID, PORT_INSTANCE_ID, PORT_INIT_SID, PORT_E_PARAM_CONFIG);
    }
    else
#endif
{
    /* Used to point at the base address of the specified Port*/
    volatile uint32 *Port_BasePtr  = NULL_PTR ;

    /* Index to be used in the array of configurations */
    Port_PinType PinID = 0 ;

    /* Address of the first element in the array of configurations = &Pins[0]*/
    PinConfigArray = ConfigPtr->Pins;

    while(PinID < PortNumberOfPortPins)
    {

        /* Activate Clock for specified Port */
        SYSCTL_REGCGC2_REG |= (1<<PinConfigArray[PinID].Port_Num);

        /* Port_BasePtr will point to the base address of the specified Port */
        /* Delay of 3 clock cycles needed after activating clock */
        switch (PinConfigArray[PinID].Port_Num)
        {
        case PORTA_ID_M :
            Port_BasePtr =  (volatile uint32 *) GPIO_PORTA_BASE_ADDRESS ;
            break;
        case PORTB_ID_M :
            Port_BasePtr =  (volatile uint32 *) GPIO_PORTB_BASE_ADDRESS ;
            break;
        case PORTC_ID_M :
            Port_BasePtr =  (volatile uint32 *) GPIO_PORTC_BASE_ADDRESS ;
            break;
        case PORTD_ID_M :
            Port_BasePtr =  (volatile uint32 *) GPIO_PORTD_BASE_ADDRESS ;
            break;
        case PORTE_ID_M :
            Port_BasePtr =  (volatile uint32 *) GPIO_PORTE_BASE_ADDRESS ;
            break;
        case PORTF_ID_M :
            Port_BasePtr =  (volatile uint32 *) GPIO_PORTF_BASE_ADDRESS ;
            break;
        default:
            break;
        }

        /* If Pin is PD7 or PF0 Unlock the register */
        if( ( PinConfigArray[PinID].Pin_Num == PIN7_ID_M && PinConfigArray[PinID].Port_Num == PORTD_ID_M ) \
         || ( PinConfigArray[PinID].Pin_Num == PIN0_ID_M && PinConfigArray[PinID].Port_Num == PORTF_ID_M ) )
        {
            /* Unlock Commit Register */
            ACCESS_REG_OFFSET(Port_BasePtr , PORT_LOCK_REG_OFFSET) = 0x4C4F434B;
            /* Set Corresponding Bits in GPIOCR  to allow changes to these pins */
            SET_BIT ( ACCESS_REG_OFFSET(Port_BasePtr , PORT_COMMIT_REG_OFFSET) , PinConfigArray[PinID].Pin_Num );
        }
        /* JTAG PINS PC0 ,PC1 PC2, PC3 */
        else if ( (PinConfigArray[PinID].Pin_Num <= PIN3_ID_M)  && (PinConfigArray[PinID].Port_Num == PORTC_ID_M) )
        {
            PinID = PinID + 1;
            continue; /* Violation of Misa Rules, Reason : Protection as these pins can lock the MicroController */
        }
        else
        {
        /* DO NOTHING (OTHER PINS DO NOT NEED UNLOCKING) */
        }

        if ( 0 < PinConfigArray[PinID].PCTL_Mode && PinConfigArray[PinID].PCTL_Mode < 16)
        {
            /* PCTL is more than 0 less than 16 (User chooses a mode to enable ) -> Set AFSEL bit and the appropriate PCTL bits */
            SET_BIT ( ACCESS_REG_OFFSET(Port_BasePtr , PORT_ALT_FUNC_REG_OFFSET ) , PinConfigArray[PinID].Pin_Num );
            /* Each PCTL pin has 4 bits.. PIN0 has 4 LSB bits PIN7 has 4 MSB bits */
            ACCESS_REG_OFFSET(Port_BasePtr , PORT_CTL_REG_OFFSET ) |= ( PinConfigArray[PinID].PCTL_Mode << (4 * PinConfigArray[PinID].Pin_Num) );
        }
        else
        {
            /* PCTL_MODE=0 -> GPIO Pin , Disable Alternative Fn. and clear the 4 PCTL corresponding to the specified pin */
            CLEAR_BIT ( ACCESS_REG_OFFSET(Port_BasePtr , PORT_ALT_FUNC_REG_OFFSET ) , PinConfigArray[PinID].Pin_Num );
            /* Each PCTL pin has 4 bits.. PIN0 has 4 LSB bits PIN7 has 4 MSB bits */
            ACCESS_REG_OFFSET(Port_BasePtr , PORT_CTL_REG_OFFSET ) &= ~( PIN_CLEAR_PCTL_M << (4 * PinConfigArray[PinID].Pin_Num) );
        }

        switch(PinConfigArray[PinID].Direction)
        {
        case PORT_PIN_OUT:
            /* PIN IS CONFIGURED AS OUTPUT -> SET DIR BIT */
            SET_BIT ( ACCESS_REG_OFFSET(Port_BasePtr , PORT_DIR_REG_OFFSET ) , PinConfigArray[PinID].Pin_Num );
            break;
        case PORT_PIN_IN:
            /* PIN IS CONFIGURED AS INPUT -> CLEAR DIR BIT */
            CLEAR_BIT ( ACCESS_REG_OFFSET(Port_BasePtr , PORT_DIR_REG_OFFSET ) , PinConfigArray[PinID].Pin_Num );
            break;
        default:
            break;
        }

        switch(PinConfigArray[PinID].Resistor)
        {
        case PULL_UP:
            /* Activate Pull Up resistor for specified Pin */
            SET_BIT ( ACCESS_REG_OFFSET(Port_BasePtr , PORT_PULL_UP_REG_OFFSET ) , PinConfigArray[PinID].Pin_Num );
            break;
        case PULL_DOWN:
            /* Activate Pull Down resistor for specified Pin */
            CLEAR_BIT ( ACCESS_REG_OFFSET(Port_BasePtr , PORT_PULL_DOWN_REG_OFFSET ) , PinConfigArray[PinID].Pin_Num );
            break;
        case R_OFF:
            /* Do Nothing */
            break;
        default:
            break;
        }

        switch(PinConfigArray[PinID].DEN_NAMSEL)
        {
        case ENABLE_DIGITAL:
            SET_BIT( ACCESS_REG_OFFSET(Port_BasePtr , PORT_DIGITAL_ENABLE_REG_OFFSET ) , PinConfigArray[PinID].Pin_Num );   /* Enable Digital MODE */
            break;
        case ENABLE_ANALOG:
            SET_BIT( ACCESS_REG_OFFSET(Port_BasePtr , PORT_ANALOG_MODE_SEL_REG_OFFSET ) , PinConfigArray[PinID].Pin_Num );  /* Enable ANALOG MODE */
            break;
        default:
            break;
        }

        switch(PinConfigArray[PinID].InitialValue == STD_HIGH)
        {
        case STD_HIGH:
            SET_BIT( ACCESS_REG_OFFSET(Port_BasePtr , PORT_DATA_REG_OFFSET ) , PinConfigArray[PinID].Pin_Num );
            break;
        case STD_LOW:
            CLEAR_BIT( ACCESS_REG_OFFSET(Port_BasePtr , PORT_DATA_REG_OFFSET ) , PinConfigArray[PinID].Pin_Num );
            break;
        default:
            break;
        }

        /* Next PinID */
        PinID = PinID + 1;
      }

    Port_Status =  PORT_INITIALIZED;
    }
}


/************************************************************************************
* Service Name: Port_SetPinDirection
* Service ID[hex]: 0x01
* Sync/Async: Synchronous
* Reentrancy: Reentrant
* Parameters (in): Pin       - Port Pin ID number
*                  Direction - Port Pin Direction
* Parameters (inout): None
* Parameters (out): None
* Return value: None
* Description: Sets the port pin direction.
************************************************************************************/
#if (PORT_SET_PIN_DIRECTION_API == STD_ON )

void Port_SetPinDirection(Port_PinType Pin, Port_PinDirectionType Direction)
{
#if (PORT_DEV_ERROR_DETECT == STD_ON)
    if( Port_Status == PORT_NOT_INITIALIZED  )
    {
        /* Port Not Initialized  */
        Det_ReportError(PORT_MODULE_ID, PORT_INSTANCE_ID, PORT_SET_PIN_DIRECTION_SID, PORT_E_UNINIT);
    }

    else if (PortNumberOfPortPins <= Pin)
    {
        /* check for incorrect Port Pin ID passed  */
        Det_ReportError(PORT_MODULE_ID, PORT_INSTANCE_ID, PORT_SET_PIN_DIRECTION_SID, PORT_E_PARAM_PIN);
    }

    else if (PinConfigArray[Pin].PortPinDirectionChangeable != STD_ON)
    {
        /* Pin direction can' be changed at runtime */
        Det_ReportError(PORT_MODULE_ID, PORT_INSTANCE_ID, PORT_SET_PIN_DIRECTION_SID, PORT_E_DIRECTION_UNCHANGEABLE);
    }

    else
#endif
    {
    /* Used to point at the base address of the specified Port*/
    volatile uint32 *Port_BasePtr  = NULL_PTR ;

    /* Port_BasePtr will point to the base address of the specified Port */
    switch (PinConfigArray[Pin].Port_Num)
     {

     case PORTA_ID_M :
         Port_BasePtr =  (volatile uint32 *) GPIO_PORTA_BASE_ADDRESS ;
         break;
     case PORTB_ID_M :
         Port_BasePtr =  (volatile uint32 *) GPIO_PORTB_BASE_ADDRESS ;
         break;
     case PORTC_ID_M :
         Port_BasePtr =  (volatile uint32 *) GPIO_PORTC_BASE_ADDRESS ;
         break;
     case PORTD_ID_M :
         Port_BasePtr =  (volatile uint32 *) GPIO_PORTD_BASE_ADDRESS ;
         break;
     case PORTE_ID_M :
         Port_BasePtr =  (volatile uint32 *) GPIO_PORTE_BASE_ADDRESS ;
         break;
     case PORTF_ID_M :
         Port_BasePtr =  (volatile uint32 *) GPIO_PORTF_BASE_ADDRESS ;
         break;
     default:
         break;
     }

    switch( PinConfigArray[Pin].Direction)
    {
    case PORT_PIN_OUT:
        /* PIN IS CONFIGURED AS OUTPUT -> SET DIR BIT */
        SET_BIT ( ACCESS_REG_OFFSET(Port_BasePtr , PORT_DIR_REG_OFFSET ) , PinConfigArray[Pin].Pin_Num );
        break;
    case PORT_PIN_IN:
        /* PIN IS CONFIGURED AS INPUT -> CLEAR DIR BIT */
        CLEAR_BIT ( ACCESS_REG_OFFSET(Port_BasePtr , PORT_DIR_REG_OFFSET ) , PinConfigArray[Pin].Pin_Num );
        break;

    default:
        break;
    }
    }
}

#endif




/************************************************************************************
* Service Name: Port_RefreshPortDirection
* Service ID[hex]: 0x02
* Sync/Async: Synchronous
* Reentrancy: Non-Reentrant
* Parameters (in):
* Parameters (inout): None
* Parameters (out): None
* Return value: None
* Description: Refreshes port direction.
************************************************************************************/
void Port_RefreshPortDirection(void)
{
#if (PORT_DEV_ERROR_DETECT == STD_ON)
    if( Port_Status == PORT_NOT_INITIALIZED  )
    {
        /* Port Not Initialized  */
        Det_ReportError(PORT_MODULE_ID, PORT_INSTANCE_ID, PORT_REFRESH_PORT_DIRECTION_SID, PORT_E_UNINIT);
    }
    else
#endif
    {
        /* Used to point at the base address of the specified Port*/
        volatile uint32 *Port_BasePtr  = NULL_PTR ;

        /* Index to be used in the array of configurations */
        Port_PinType PinID = 0 ;

        while(PinID < PortNumberOfPortPins)
        {

            /* Port_BasePtr will point to the base address of the specified Port */
            switch (PinConfigArray[PinID].Port_Num)
            {

            case PORTA_ID_M :
                Port_BasePtr =  (volatile uint32 *) GPIO_PORTA_BASE_ADDRESS ;
                break;
            case PORTB_ID_M :
                Port_BasePtr =  (volatile uint32 *) GPIO_PORTB_BASE_ADDRESS ;
                break;
            case PORTC_ID_M :
                Port_BasePtr =  (volatile uint32 *) GPIO_PORTC_BASE_ADDRESS ;
                break;
            case PORTD_ID_M :
                Port_BasePtr =  (volatile uint32 *) GPIO_PORTD_BASE_ADDRESS ;
                break;
            case PORTE_ID_M :
                Port_BasePtr =  (volatile uint32 *) GPIO_PORTE_BASE_ADDRESS ;
                break;
            case PORTF_ID_M :
                Port_BasePtr =  (volatile uint32 *) GPIO_PORTF_BASE_ADDRESS ;
                break;
            default:
                break;
            }

            /* JTAG PINS PC0 ,PC1 PC2, PC3  or Pin Direction is Changeable*/
            if( (PinConfigArray[PinID].Pin_Num <= PIN3_ID_M  && PinConfigArray[PinID].Port_Num == PORTC_ID_M )|| PinConfigArray[PinID].PortPinDirectionChangeable == DIR_CHANGEABLE)
            {
                /* DO NOTHING -> SKIP THESE PINS AND LEAVE THEM UNCHANGED */
            }

            else
            {
                switch(PinConfigArray[PinID].Direction)
                {
                case PORT_PIN_OUT:
                    /* PIN IS CONFIGURED AS OUTPUT -> SET DIR BIT */
                    SET_BIT ( ACCESS_REG_OFFSET(Port_BasePtr , PORT_DIR_REG_OFFSET ) , PinConfigArray[PinID].Pin_Num );
                    break;
                case PORT_PIN_IN:
                    /* PIN IS CONFIGURED AS INPUT -> CLEAR DIR BIT */
                    CLEAR_BIT ( ACCESS_REG_OFFSET(Port_BasePtr , PORT_DIR_REG_OFFSET ) , PinConfigArray[PinID].Pin_Num );
                    break;
                default:
                    break;
                }
            }

            PinID = PinID + 1;
        }

    }
}


/************************************************************************************
* Service Name: Port_GetVersionInfo
* Service ID[hex]: 0x03
* Sync/Async: Synchronous
* Reentrancy: Non-Reentrant
* Parameters (in):
* Parameters (inout): None
* Parameters (out): versioninfo - Pointer to where to store the version information of this module.
* Return value: None
* Description: Returns the version information of this module.
************************************************************************************/
#if (PORT_VERSION_INFO_API == STD_ON )
void Port_GetVersionInfo( Std_VersionInfoType* versioninfo )
{
#if (PORT_DEV_ERROR_DETECT == STD_ON)

    if( Port_Status == PORT_NOT_INITIALIZED  )
    {
        /* Port Not Initialized  */
        Det_ReportError(PORT_MODULE_ID, PORT_INSTANCE_ID, PORT_GET_VERSION_INFO_SID, PORT_E_UNINIT);
    }

    else if(NULL_PTR == versioninfo)
    {
        /* Check if input pointer is not Null pointer */
        Det_ReportError(PORT_MODULE_ID, PORT_INSTANCE_ID, PORT_GET_VERSION_INFO_SID, PORT_E_PARAM_POINTER);
    }

    else
#endif
    {
        /* COPY VENDOR ID, MODULE ID, SW VERSION */
        versioninfo->vendorID = PORT_VENDOR_ID ;
        versioninfo->moduleID = PORT_MODULE_ID;
        versioninfo->sw_major_version = PORT_SW_MAJOR_VERSION;
        versioninfo->sw_minor_version = PORT_SW_MINOR_VERSION;
        versioninfo->sw_patch_version = PORT_SW_PATCH_VERSION;
    }
}
#endif


/************************************************************************************
* Service Name: Port_GetVersionInfo
* Service ID[hex]: 0x04
* Sync/Async: Synchronous
* Reentrancy: Reentrant
* Parameters (in): Pin  - Port Pin ID number
*                  Mode - New Port Pin mode to be set on port pin
* Parameters (inout): None
* Parameters (out): None
* Return value: None
* Description: Sets the port pin mode.
************************************************************************************/
#if (PORT_SET_PIN_MODE_API == STD_ON)

void Port_SetPinMode( Port_PinType Pin, Port_PinModeType Mode )
{
#if (PORT_DEV_ERROR_DETECT == STD_ON)
    if( Port_Status == PORT_NOT_INITIALIZED  )
    {
        /* Port Not Initialized  */
        Det_ReportError(PORT_MODULE_ID, PORT_INSTANCE_ID, PORT_SET_PIN_MODE_SID, PORT_E_UNINIT);
    }
    else if (PortNumberOfPortPins <= Pin)
    {
        /* check for incorrect Port Pin ID passed  */
        Det_ReportError(PORT_MODULE_ID, PORT_INSTANCE_ID, PORT_SET_PIN_MODE_SID, PORT_E_PARAM_PIN);
    }
    else if (PinConfigArray[Pin].PortPinModeChangeable != STD_ON)
    {
        /* Pin direction can' be changed at runtime */
        Det_ReportError(PORT_MODULE_ID, PORT_INSTANCE_ID, PORT_SET_PIN_MODE_SID, PORT_E_DIRECTION_UNCHANGEABLE);
    }
    else if (Mode >= 16)
    {
        /*Invalid Pin Mode */
        Det_ReportError(PORT_MODULE_ID, PORT_INSTANCE_ID, PORT_SET_PIN_MODE_SID, PORT_E_PARAM_INVALID_MODE);
    }

    else
#endif
    {
        /* Used to point at the base address of the specified Port*/
        volatile uint32 *Port_BasePtr  = NULL_PTR ;

        /* Port_BasePtr will point to the base address of the specified Port */
        switch (PinConfigArray[Pin].Port_Num)
         {

         case PORTA_ID_M :
             Port_BasePtr =  (volatile uint32 *) GPIO_PORTA_BASE_ADDRESS ;
             break;
         case PORTB_ID_M :
             Port_BasePtr =  (volatile uint32 *) GPIO_PORTB_BASE_ADDRESS ;
             break;
         case PORTC_ID_M :
             Port_BasePtr =  (volatile uint32 *) GPIO_PORTC_BASE_ADDRESS ;
             break;
         case PORTD_ID_M :
             Port_BasePtr =  (volatile uint32 *) GPIO_PORTD_BASE_ADDRESS ;
             break;
         case PORTE_ID_M :
             Port_BasePtr =  (volatile uint32 *) GPIO_PORTE_BASE_ADDRESS ;
             break;
         case PORTF_ID_M :
             Port_BasePtr =  (volatile uint32 *) GPIO_PORTF_BASE_ADDRESS ;
             break;
         default:
             break;
         }

        if ( Mode > 0 )
        {
            /* PCTL is more than 0 less than 16 (User chooses a mode to enable ) -> Set AFSEL bit and the appropriate PCTL bits */
            SET_BIT ( ACCESS_REG_OFFSET(Port_BasePtr , PORT_ALT_FUNC_REG_OFFSET ) , PinConfigArray[Pin].Pin_Num );
            /* Each PCTL pin has 4 bits.. PIN0 has 4 LSB bits PIN7 has 4 MSB bits */
            ACCESS_REG_OFFSET(Port_BasePtr , PORT_CTL_REG_OFFSET ) |= ( Mode << (4 * PinConfigArray[Pin].Pin_Num) );
        }
        else
        {
            /* PCTL_MODE=0 -> GPIO Pin , Disable Alternative Fn. and clear the 4 PCTL corresponding to the specified pin */
            CLEAR_BIT ( ACCESS_REG_OFFSET(Port_BasePtr , PORT_ALT_FUNC_REG_OFFSET ) , PinConfigArray[Pin].Pin_Num );
            /* Each PCTL pin has 4 bits.. PIN0 has 4 LSB bits PIN7 has 4 MSB bits */
            ACCESS_REG_OFFSET(Port_BasePtr , PORT_CTL_REG_OFFSET ) &= ~( PIN_CLEAR_PCTL_M << (4 * PinConfigArray[Pin].Pin_Num) );
        }
    }
}

#endif
