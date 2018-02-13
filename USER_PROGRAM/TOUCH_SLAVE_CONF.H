#ifndef __TOUCH_SLAVE_CONF_H_
#define __TOUCH_SLAVE_CONF_H_


#ifdef __cplusplus
 extern "C" 
{
#endif

	#define u8		unsigned char 
	#define u16		unsigned int   
	#define vu8		volatile unsigned char 
	#define vu16	volatile unsigned int 
	
/**********************************************************************
//			
//	Choice MCU type 
//
//
***********************************************************************/
	#define  BS83B08A 			(3)
	#define  BS83B12A 			(2)
	#define  BS83B16A 			(1)
	
	#define  USE_MCU_TYPE 		(2)
	
	
/**********************************************************************
//			
//	Choice intu type 
//
//
***********************************************************************/	
	#define  USE_INT_PIN 		(1)
	
#ifdef	USE_INT_PIN		
	#define MCU_INT_C  		_pac1
	#define MCU_INT  		_pa1
	
#endif

/**********************************************************************
//			
//	Choice MCU Wakeup 
//
//
***********************************************************************/
	#define  USE_WAKEUP_PIN 	(1)
	
#ifdef	USE_WAKEUP_PIN		
	#define MCU_WAKEUP_WU 		_pawu4
	#define MCU_WAKEUP_C 		_pac4
	#define MCU_WAKEUP  		_pa4
	
#endif

/**********************************************************************
//			
//	Choice MCU type 
//
//
***********************************************************************/	
	
	
	const u8 I2C_Address = 0x28;	
	const u8 Version = 0x18; //Ó²¼þ/Ü›¼þ°æ±¾Ì–
	const u8 SleepTime = 0x4E; //3s


#ifdef __cplusplus
}
#endif


#endif

