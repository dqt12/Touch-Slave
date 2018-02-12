#include    "USER_PROGRAM.H"         

#define u8		unsigned char 
#define u16		unsigned int   

#define vu8		volatile unsigned char 
#define vu16	volatile unsigned int 



// choose mcu
//#define  BS83B08A 	3
#define  BS83B12A 	2
//#define  BS83B16A 	1

#if	 BS83B08A
//I2C size
#define I2C_MAXNUM		8
//page size
const u8 IC_PAGE_SIZS[]={3,8,1,1,1,2};
const u8 Version = 0x18; //Ӳ��/ܛ���汾̖
// MCU_INT_CONFIG
#define MCU_INT_c  		_pac1
#define MCU_INT  		_pa1
#endif

#if	 BS83B12A
//I2C size
#define I2C_MAXNUM		12
//page size
const u8 IC_PAGE_SIZS[]={3,12,2,1,2,2};
const u8 Version = 0x1C; //Ӳ��/ܛ���汾̖
// MCU_INT_CONFIG
#define MCU_INT_c  		_pac1
#define MCU_INT  		_pa1
#endif

#if	 BS83B16A
//I2C size
#define I2C_MAXNUM		16
//page size
const u8 IC_PAGE_SIZS[]={3,16,2,1,2,2};
const u8 Version = 0x1F; //Ӳ��/ܛ���汾̖
// MCU_INT_CONFIG
#define MCU_INT_c  		_pac1
#define MCU_INT  		_pa1
#endif


//++++++++++++++++++++++MAIN PROGAM+++++++++++++++++++++++
#define TRUE 	1
#define FALSE 	0
#define NTH 	0x0F
#define RESET 	0x0A

const u8 DeviceAddress = 0x28;
const u8 IC_PAGE_HEAD[]={0xA0,0xB0,0xC0,0xD0,0xE0,0xF0};

_TKS_FLAGA_type      FLAG;

#define FLAG_Err    FLAG.bits.b0
#define FLAG_End    FLAG.bits.b1
//#define          	FLAG.bits.b2
//#define       	FLAG.bits.b3
//#define           FLAG.bits.b4
//#define           FLAG.bits.b5
//#define           FLAG.bits.b6

struct I2C_LIST
{
//FOR HEAD
	vu8 WordAddr;		
	vu8 SUM;	
	vu8 TxNum;
	vu8 RxNum;
	vu8 CRC;
	vu8 CRC_BUF;		
}I2C;

u8 I2C_Data[I2C_MAXNUM];
u8 MCU_STATE;
u8 I2C_STATE;

void DATA_LIST_UPDATA(void)
{
	u8 *p;
	
	I2C.SUM = IC_PAGE_SIZS[I2C.WordAddr];//ȡ퓔�����
	
   	switch(I2C.WordAddr)//ȡ퓔�����ָ�
	{
		case 0 :p = &GLOBE_VARIES[0];break;
		case 1 :p = &GLOBE_VARIES[3];break;
		case 2 :p = &KEY_IO_SEL[0];break;
		case 3 :p = &MCU_STATE;break;
		default :p = &GLOBE_VARIES[0];I2C.SUM = 0;break;
	}
	
	
	for(I2C.RxNum=0;I2C.RxNum<I2C.SUM;I2C.RxNum++)
	{	
		*(p+I2C.RxNum) = I2C_Data[I2C.RxNum];	//�����|�ؔ���
	}	
	
}

//��ʼ��I2C����
void I2C_LIST_INIT(void)
{
	I2C.WordAddr = 0x00;
	I2C.SUM = 0;	
	I2C.RxNum = 0;
	I2C.TxNum = 0;
	I2C.CRC = 0;
	I2C.CRC_BUF = 0;
	FLAG_End = FALSE;
	FLAG_Err = FALSE;
}

//��ʼ��ϵ�y����
void SYS_LIST_INIT(void)
{
	MCU_STATE = NTH;
	I2C_STATE = NTH;
	
	FLAG_Err = TRUE;//ʹ��ͨ���e�`���ܳ�ʼ��I2C����
}

//==============================================
void USER_PROGRAM_INITIAL()
{	
//	_wdtc = 0xAF;	//wdt = 8s 
	_wdtc = 0b10101100;	//wdt = 1s 
	
	_pawu=0;		//0:��ֹ�½��ػ���;   1:ʹ��
	_papu=0;		//0:��ֹ����;         1:ʹ������
	_pa=0x00;	
	MCU_INT = 1;
	_pac =0x05;		//0:���;      		  1:����
		 
	
	MCU_INT = 1;		
	MCU_INT_c = 0;	
	
	
//���ö�ʱ��	
//	_tmrc = 0b00000111;//12Mhz,1/128
//	_tmr = 131;//256-156=100,1/12M*128=10.7us; 10.7us*100=1.07ms
//	_te = 1;	//��ʱ/�������жϿ���λ
//	_ton = 1;	//��ʱ/����������/ֹͣ����λ
		
//����IIC	
//	_papu0 = 1;  	//SDA
//	_papu2 = 1;  	//SCL

	_simc0 = 0b11000010;
	_sima = DeviceAddress;//�ӻ���ַ
	
//I2C��ʱ����	
	_i2ctoc = 0b10111111; //64ms
	_simf = 0;
	_sime = 1;	
	
	SYS_LIST_INIT();

	_emi = 1;	//���жϿ���λ	
}

//==============================================
//**********************************************
//==============================================
void USER_PROGRAM()
{
	if(MCU_STATE == RESET)// ϵ�y�O�ã�����O�Þ��λ���t�R�Ϗ�λ
	{
		MCU_INT = 0;
		_wdtc = 0xFF;//MCU RESET //64MS
	}
	else 
	{
		MCU_STATE = NTH;
	}
	
	if(FLAG_Err) // ͨ���e�`����λI2C����
	{
		I2C_LIST_INIT();
	}
//--------------------------------------------------------------------	
	GET_KEY_BITMAP();//���°��Iֵ
	#ifndef	BS83B08A	
	//ȡʹ�ܵİ��Iֵ
	KEY_DATA[0] =KEY_DATA[0]&KEY_IO_SEL[0] ;
	KEY_DATA[1] =KEY_DATA[1]&KEY_IO_SEL[1] ;	
	//�а��I����INT׃��
	 if(KEY_DATA[0] || KEY_DATA[1]) 
		MCU_INT = 0;
  	 else
	 	MCU_INT = 1;	
	#endif	
	
	#ifdef	BS83B08A
	//ȡʹ�ܵİ��Iֵ
	KEY_DATA[0] =KEY_DATA[0]&KEY_IO_SEL[0] ;
	//�а��I����INT׃��
	 if(KEY_DATA[0]) 
		MCU_INT = 0;
  	 else
	 	MCU_INT = 1;
	#endif		


//	----------------------------------------------------------------
	if(FLAG_End)				//һ֡���ݽ�����ɱ�־
	{
		//����ַ�D�Q��퓴a	
		I2C.WordAddr >>=4;
		I2C.WordAddr -=0x0A;
		//���յ�����Ч������
		I2C.RxNum-=2;
	
		//���Д� 퓴a�Ƿ��Ɍ���ǰ��� �� ����Ч�������Ƿ����퓴�С
		if((I2C.WordAddr <= 3) && (I2C.RxNum == IC_PAGE_SIZS[I2C.WordAddr])) 
		{	
			I2C.CRC_BUF=~I2C.CRC_BUF;//ȡ��Ӌ������
			
			if( I2C.CRC == I2C.CRC_BUF  )//��CCRC
			{
				I2C_STATE = TRUE; //�������_���I
				
				DATA_LIST_UPDATA();//�����|�ؔ���ֵ
				
			}
			else I2C_STATE = FALSE;	//�����e�`���I					
		}	
		else I2C_STATE = FALSE;	//�����e�`���I	
			
		FLAG_End = FALSE;
	}	
//	----------------------------------------------------------------
}




void __attribute((interrupt(0x10))) IIC_ISR(void)
{ 
 	u8 data;	
 	_simf = 0;		//�����־λ				
 	if(_i2ctof)//_i2ctof=1:I2Cͨ�ų�ʱ
 	{
 		_i2ctoen = 1;
 		_i2ctof = 0;
 		FLAG_Err = TRUE;
 	}
 	else 
 		{
			if(_haas)//_haas=1:slave receive SLAVE_ADDR match							
			{
				//��ַƥ����ж������x��λ,������д�ӻ����Ƕ��ӻ�

				if(_srw)//srw=1:slave send data to master (frist data).
				{							
					
					if(I2C.WordAddr == 0x00)
					{
						I2C.SUM = IC_PAGE_SIZS[0];
						I2C.TxNum = IC_PAGE_HEAD[0];
						I2C.CRC_BUF = 0;
					}
					
						
					switch(I2C.TxNum)
					{	
						case 0xA0 :data = GLOBE_VARIES[0];break;
						case 0xB0 :data = GLOBE_VARIES[3];;break;
						case 0xC0 :data = KEY_IO_SEL[0];break;
						case 0xD0 :data = MCU_STATE;break;
						case 0xE0 :data = KEY_DATA[0];break;
						case 0xF0 :data = I2C_STATE;I2C_STATE = NTH;break;
						default : data = 0xAA;;break;
					}
					_htx = 1; //_htx=1:	
					_simd = data;
					
					I2C.CRC_BUF += data;
					
					I2C.SUM += I2C.TxNum;
					
					I2C.TxNum++;
					if(I2C.TxNum >= I2C.SUM) 
						I2C.TxNum = 0xFF;
							
				}else//srw=0:slave receive data for master
					{	
						_htx = 0;//_htx=0:
						_txak = 0;//_txak=0:slave enable ACK
						_acc = _simd;//dummy
						
						I2C.RxNum = 0;//begin to conut
						I2C.TxNum = 0;
						I2C.SUM = 0;
						FLAG_End = FALSE;
					} 
			}
			else//_haas=0:slave receive SLAVE_ADDR match,then communication data 
			{	
										  
				if(_htx)//_htx=1:slave send data to master
				{								
					if(_rxak)//_rxak=1:slave send data,then master NACK;
					{							
						_htx = 0;
						_txak = 0;	
						_acc = _simd;//dummy

						I2C.WordAddr = 0x00;
						I2C.SUM = 0;
						I2C.TxNum = 0;
					}
					else//_rxak=1:slave send data,then master ACK
					{				
						//slave continue to send data to master (second and next data).							
						switch(I2C.TxNum)
						{
		
						//	case 0xA0 :data = GLOBE_VARIES[0];break;
							case 0xA1 :data = GLOBE_VARIES[1];break;
							case 0xA2 :data = GLOBE_VARIES[2];break;
						//	case 0xB0 :data = GLOBE_VARIES[3];;break;
							case 0xB1 :data = GLOBE_VARIES[4];break;	
							case 0xB2 :data = GLOBE_VARIES[5];break; 
							case 0xB3 :data = GLOBE_VARIES[6];break;
							case 0xB4 :data = GLOBE_VARIES[7];break;
							case 0xB5 :data = GLOBE_VARIES[8];break;
							case 0xB6 :data = GLOBE_VARIES[9];break;
							case 0xB7 :data = GLOBE_VARIES[10];break;
							//case 0xD0 :data = MCU_STATE[0];break;
							//case 0xD1 :data = MCU_STATE[1];break;

							#ifndef	 BS83B08A	
							case 0xB8 :data = GLOBE_VARIES[11];break;
							case 0xB9 :data = GLOBE_VARIES[12];break;	
							case 0xBA :data = GLOBE_VARIES[13];break;	
							case 0xBB :data = GLOBE_VARIES[14];break;
							
								#ifdef	 BS83B16A
								case 0xBC :data = GLOBE_VARIES[15];break;
								case 0xBD :data = GLOBE_VARIES[16];break;
								case 0xBE :data = GLOBE_VARIES[17];break;
								case 0xBF :data = GLOBE_VARIES[18];break;
								#endif
							
						//	case 0xC0 :data = KEY_IO_SEL[0];break;
							case 0xC1 :data = KEY_IO_SEL[1];break;
						//	case 0xE0 :data = KEY_DATA[0];break;
							case 0xE1 :data = KEY_DATA[1];break;
						//	case 0xF0 :data =SYS_INF[0];break;
							#endif
						
							case 0xF1 :data = Version;break;	
							case 0xFF :data =~I2C.CRC_BUF;break;	
								
							default : data = 0xAA ;break;
						}	
						
						_simd = data;
						
						I2C.CRC_BUF += data;
							
						I2C.TxNum++;
						if(I2C.TxNum >= I2C.SUM) 
							I2C.TxNum = 0xFF;
	
					}			
				}
				else//_htx=0:slave receive data for master
				{//for slave send data to master
					data = _simd;
					_txak = 0;
					
					if(I2C.RxNum < 1)
					{
						//for slave send data to master
						//ȡ��ַ
						I2C.WordAddr = data&0xf0;					
	
					   	switch(I2C.WordAddr)
						{	
					//		case 0xA0 :I2C.SUM = IC_PAGE_SIZS[0];I2C.TxNum = IC_PAGE_HEAD[0];break;
							case 0xB0 :I2C.SUM = IC_PAGE_SIZS[1];I2C.TxNum = IC_PAGE_HEAD[1];break;
							case 0xC0 :I2C.SUM = IC_PAGE_SIZS[2];I2C.TxNum = IC_PAGE_HEAD[2];break;
							case 0xD0 :I2C.SUM = IC_PAGE_SIZS[3];I2C.TxNum = IC_PAGE_HEAD[3];break;
							case 0xE0 :I2C.SUM = IC_PAGE_SIZS[4];I2C.TxNum = IC_PAGE_HEAD[4];break;
							case 0xF0 :I2C.SUM = IC_PAGE_SIZS[5];I2C.TxNum = IC_PAGE_HEAD[5];break;
							default   :I2C.SUM = IC_PAGE_SIZS[0];I2C.TxNum = IC_PAGE_HEAD[0];break;
						}
						I2C.CRC_BUF = 0;
						I2C.RxNum++;

					}
					else if(I2C.RxNum <= I2C.SUM)
						 { 
							I2C_Data[I2C.RxNum - 1] = data;
							I2C.CRC_BUF += data;
							I2C.RxNum++;
						 }
						 else 
						 {
						 	I2C.CRC = data;
						 	FLAG_End = TRUE;
						 	I2C.RxNum++;
						 }
						 
						
				}		
			}
		}	
}




//void __attribute((interrupt(0x0c))) ISR_tmr0 (void)
//{		
////	LED1 = ~LED1;		
//			
//	//----------------------------------------------------
//	if(Cnt20ms > 0)//20ms ������
//	{				
//			Cnt20ms--;
//	}
//	else
//	{
//			Cnt20ms = 19;	
//			Flag20ms = 1;			//20ms ��־ 26MS
//	}	
//}

