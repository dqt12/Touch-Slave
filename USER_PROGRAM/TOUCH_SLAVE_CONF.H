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
//	USE_MCU_TYPE : 1:BS83B16A 2:BS83B12A 3:BS83B08A
//
***********************************************************************/
	#define  BS83B08A 			(3)
	#define  BS83B12A 			(2)
	#define  BS83B16A 			(1)
	
	#define  USE_MCU_TYPE 		(2)
	
	
/**********************************************************************
//			
//	Choice IRQ pin 
//	USE_INT_PIN  : 0:disable 	1:enable
//	MCU_INT_C : pin control
//	MCU_INT   : pin  register
***********************************************************************/	
	#define USE_IRQ_PIN 		(1)
	#define MCU_IRQ_C  			_pac1
	#define MCU_IRQ  			_pa1

/**********************************************************************
//			
//	Choice MCU Wakeup pin (only supprot PA4/INT)
//	USE_WAKEUP_PIN  : 0:disable 	1:enable
//	USE_WAKEUP_PULLHIGH  : 0:disable(outside pullhigh) 	1:enable(inside pullhigh) 
//	MCU_WAKEUP_TYPE : 0x00:disable  		0x01:rising edge
//					  0x02:trailing edge 	0x03:rising and trailing edge 
***********************************************************************/
	#define  USE_WAKEUP_PIN 		(1)
	#define  USE_WAKEUP_PULLHIGH 	(1)	
	#define  MCU_WAKEUP_TYPE    	(0x02) 
	

/**********************************************************************
//			
//	Choice MCU I2C PIN Pullhigh
//	USE_I2C_PULLHIGH  : 0:disable(outside pullhigh) 	1:enable(inside pullhigh) 
***********************************************************************/
	#define  USE_I2C_PULLHIGH 		(0)	
							   
/**********************************************************************
//			
//	system inf 
//
***********************************************************************/	
	const u8 I2C_Address 	= 0x28;//I2C从机地址
	const u8 SysVersion 	= 0x18; //硬件/軟件版本號
	const u8 SleepTime 		= 0x4E; //3s
	


#ifdef __cplusplus
}
#endif


#endif

