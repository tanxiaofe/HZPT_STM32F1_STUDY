#ifndef      __BSP_ILI9341_LCD_H
#define	     __BSP_ILI9341_LCD_H



/////////////////////////////////////�û�������///////////////////////////////////	 
//����2���궨�壬������Ļ����ʾ��ʽ��IO�ٶ�
#define USE_HORIZONTAL  0	//�����Ƿ�ʹ�ú��� 		0,��ʹ��.1,ʹ��.
#define LCD_FAST_IO     1 	//�����Ƿ�ʹ�ÿ���IO	0,��ʵ��.1,ʹ��
/////////////////////////                                                                                                                                /////////////////////////////////////////////////////////	 
#include "main.h"
#include "gpio.h"
//TFTLCD������Ҫ���õĺ���		   
extern uint16_t  POINT_COLOR;//Ĭ�Ϻ�ɫ    
extern uint16_t  BACK_COLOR; //������ɫ.Ĭ��Ϊ��ɫ
//����LCD�ĳߴ�
#if USE_HORIZONTAL==1	//ʹ�ú���
#define LCD_W 320
#define LCD_H 240
#else
#define LCD_W 240
#define LCD_H 320
#endif
#define 			ILI9341_LESS_PIXEL	  							240			//Һ�����϶̷�������ؿ��
#define 			ILI9341_MORE_PIXEL	 								320			//Һ�����ϳ���������ؿ��

////////////////////////////////////////////////////////////////////
//-----------------LCD�˿ڶ���---------------- 

#define	LCD_LED(X) HAL_GPIO_WritePin(GPIOD, GPIO_PIN_2,X)

#ifdef LCD_FAST_IO //����IO

#define	LCD_CS_SET  HAL_GPIO_WritePin( GPIOD,GPIO_PIN_8,GPIO_PIN_SET)//GPIOD->BSRR=1<<8    //Ƭѡ�˿�  	PD8(GND)
#define	LCD_RS_SET	HAL_GPIO_WritePin( GPIOD,GPIO_PIN_1,GPIO_PIN_SET)//GPIOD->BSRR=1<<1    //����/����   	PD1	   
#define	LCD_WR_SET	HAL_GPIO_WritePin( GPIOD,GPIO_PIN_9,GPIO_PIN_SET)//GPIOD->BSRR=1<<9    //д����		PD9
#define	LCD_RD_SET	HAL_GPIO_WritePin( GPIOD,GPIO_PIN_0,GPIO_PIN_SET)//GPIOD->BSRR=1<<0    //������		PD0(VCC)
#define	LCD_RST_SET	HAL_GPIO_WritePin( GPIOD,GPIO_PIN_10,GPIO_PIN_SET)//GPIOD->BSRR=1<<10   //��λ			PD10
#define	LCD_RST_BL	HAL_GPIO_WritePin( GPIOD,GPIO_PIN_2,GPIO_PIN_SET)//GPIOD->BSRR=1<<2    //����			PD2	
	
#define	LCD_CS_CLR 	HAL_GPIO_WritePin( GPIOD,GPIO_PIN_8,GPIO_PIN_RESET)//GPIOD->BRR=1<<8     //Ƭѡ�˿�  	 
#define	LCD_RS_CLR	HAL_GPIO_WritePin( GPIOD,GPIO_PIN_1,GPIO_PIN_RESET)//GPIOD->BRR=1<<1     //����/����   	   
#define	LCD_WR_CLR	HAL_GPIO_WritePin( GPIOD,GPIO_PIN_9,GPIO_PIN_RESET)//GPIOD->BRR=1<<9     //д����			 
#define	LCD_RD_CLR	HAL_GPIO_WritePin( GPIOD,GPIO_PIN_0,GPIO_PIN_RESET)//GPIOD->BRR=1<<0     //������			 
#define	LCD_RST_CLR	HAL_GPIO_WritePin( GPIOD,GPIO_PIN_10,GPIO_PIN_RESET)//GPIOD->BRR=1<<10    //��λ	   
		  

#else //����IO

#define	LCD_CS(X) GPIO_WriteBit(GPIOD, GPIO_PIN_8, (BitAction)X)    //Ƭѡ�˿� 
#define	LCD_RS(X) GPIO_WriteBit(GPIOD, GPIO_PIN_1, (BitAction)X)    //����/����        	   
#define	LCD_WR(X) GPIO_WriteBit(GPIOD, GPIO_PIN_9, (BitAction)X)	//д����	 
#define	LCD_RD(X) GPIO_WriteBit(GPIOD, GPIO_PIN_0, (BitAction)X)	//������	 
#define	LCD_RST(X) GPIO_WriteBit(GPIOD, GPIO_PIN_10, (BitAction)X)	//��λ			 
#endif


//PD4��Ϊ��λ�Ĵ���ʱ����SCK��PD3����������SI,PD11������洢ʱ��
#define DATAOUT(x){\
 Write_u16_data(GPIOD,GPIO_ReadOutputData(GPIOD)&0X07ff);\
if(x>>15)	  	HAL_GPIO_WritePin( GPIOD,GPIO_PIN_3,GPIO_PIN_SET);else HAL_GPIO_WritePin( GPIOD,GPIO_PIN_3,GPIO_PIN_RESET);HAL_GPIO_WritePin( GPIOD,GPIO_PIN_4,GPIO_PIN_SET);HAL_GPIO_WritePin( GPIOD,GPIO_PIN_4,GPIO_PIN_RESET);\
if((x>>14)&1) HAL_GPIO_WritePin( GPIOD,GPIO_PIN_3,GPIO_PIN_SET);else HAL_GPIO_WritePin( GPIOD,GPIO_PIN_3,GPIO_PIN_RESET);HAL_GPIO_WritePin( GPIOD,GPIO_PIN_4,GPIO_PIN_SET);HAL_GPIO_WritePin( GPIOD,GPIO_PIN_4,GPIO_PIN_RESET);\
if((x>>13)&1) HAL_GPIO_WritePin( GPIOD,GPIO_PIN_3,GPIO_PIN_SET);else HAL_GPIO_WritePin( GPIOD,GPIO_PIN_3,GPIO_PIN_RESET);HAL_GPIO_WritePin( GPIOD,GPIO_PIN_4,GPIO_PIN_SET);HAL_GPIO_WritePin( GPIOD,GPIO_PIN_4,GPIO_PIN_RESET);\
if((x>>12)&1) HAL_GPIO_WritePin( GPIOD,GPIO_PIN_3,GPIO_PIN_SET);else HAL_GPIO_WritePin( GPIOD,GPIO_PIN_3,GPIO_PIN_RESET);HAL_GPIO_WritePin( GPIOD,GPIO_PIN_4,GPIO_PIN_SET);HAL_GPIO_WritePin( GPIOD,GPIO_PIN_4,GPIO_PIN_RESET);\
if((x>>11)&1) HAL_GPIO_WritePin( GPIOD,GPIO_PIN_3,GPIO_PIN_SET);else HAL_GPIO_WritePin( GPIOD,GPIO_PIN_3,GPIO_PIN_RESET);HAL_GPIO_WritePin( GPIOD,GPIO_PIN_4,GPIO_PIN_SET);HAL_GPIO_WritePin( GPIOD,GPIO_PIN_4,GPIO_PIN_RESET);\
if((x>>10)&1) HAL_GPIO_WritePin( GPIOD,GPIO_PIN_3,GPIO_PIN_SET);else HAL_GPIO_WritePin( GPIOD,GPIO_PIN_3,GPIO_PIN_RESET);HAL_GPIO_WritePin( GPIOD,GPIO_PIN_4,GPIO_PIN_SET);HAL_GPIO_WritePin( GPIOD,GPIO_PIN_4,GPIO_PIN_RESET);\
if((x>>9)&1)  HAL_GPIO_WritePin( GPIOD,GPIO_PIN_3,GPIO_PIN_SET);else HAL_GPIO_WritePin( GPIOD,GPIO_PIN_3,GPIO_PIN_RESET);HAL_GPIO_WritePin( GPIOD,GPIO_PIN_4,GPIO_PIN_SET);HAL_GPIO_WritePin( GPIOD,GPIO_PIN_4,GPIO_PIN_RESET);\
if((x>>8)&1)  HAL_GPIO_WritePin( GPIOD,GPIO_PIN_3,GPIO_PIN_SET);else HAL_GPIO_WritePin( GPIOD,GPIO_PIN_3,GPIO_PIN_RESET);HAL_GPIO_WritePin( GPIOD,GPIO_PIN_4,GPIO_PIN_SET);HAL_GPIO_WritePin( GPIOD,GPIO_PIN_4,GPIO_PIN_RESET);\
if((x>>7)&1)  HAL_GPIO_WritePin( GPIOD,GPIO_PIN_3,GPIO_PIN_SET);else HAL_GPIO_WritePin( GPIOD,GPIO_PIN_3,GPIO_PIN_RESET);HAL_GPIO_WritePin( GPIOD,GPIO_PIN_4,GPIO_PIN_SET);HAL_GPIO_WritePin( GPIOD,GPIO_PIN_4,GPIO_PIN_RESET);\
if((x>>6)&1)  HAL_GPIO_WritePin( GPIOD,GPIO_PIN_3,GPIO_PIN_SET);else HAL_GPIO_WritePin( GPIOD,GPIO_PIN_3,GPIO_PIN_RESET);HAL_GPIO_WritePin( GPIOD,GPIO_PIN_4,GPIO_PIN_SET);HAL_GPIO_WritePin( GPIOD,GPIO_PIN_4,GPIO_PIN_RESET);\
if((x>>5)&1)  HAL_GPIO_WritePin( GPIOD,GPIO_PIN_3,GPIO_PIN_SET);else HAL_GPIO_WritePin( GPIOD,GPIO_PIN_3,GPIO_PIN_RESET);HAL_GPIO_WritePin( GPIOD,GPIO_PIN_4,GPIO_PIN_SET);HAL_GPIO_WritePin( GPIOD,GPIO_PIN_4,GPIO_PIN_RESET);\
if((x>>4)&1)  HAL_GPIO_WritePin( GPIOD,GPIO_PIN_3,GPIO_PIN_SET);else HAL_GPIO_WritePin( GPIOD,GPIO_PIN_3,GPIO_PIN_RESET);HAL_GPIO_WritePin( GPIOD,GPIO_PIN_4,GPIO_PIN_SET);HAL_GPIO_WritePin( GPIOD,GPIO_PIN_4,GPIO_PIN_RESET);\
if((x>>3)&1)  HAL_GPIO_WritePin( GPIOD,GPIO_PIN_3,GPIO_PIN_SET);else HAL_GPIO_WritePin( GPIOD,GPIO_PIN_3,GPIO_PIN_RESET);HAL_GPIO_WritePin( GPIOD,GPIO_PIN_4,GPIO_PIN_SET);HAL_GPIO_WritePin( GPIOD,GPIO_PIN_4,GPIO_PIN_RESET);\
if((x>>2)&1)  HAL_GPIO_WritePin( GPIOD,GPIO_PIN_3,GPIO_PIN_SET);else HAL_GPIO_WritePin( GPIOD,GPIO_PIN_3,GPIO_PIN_RESET);HAL_GPIO_WritePin( GPIOD,GPIO_PIN_4,GPIO_PIN_SET);HAL_GPIO_WritePin( GPIOD,GPIO_PIN_4,GPIO_PIN_RESET);\
if((x>>1)&1)  HAL_GPIO_WritePin( GPIOD,GPIO_PIN_3,GPIO_PIN_SET);else HAL_GPIO_WritePin( GPIOD,GPIO_PIN_3,GPIO_PIN_RESET);HAL_GPIO_WritePin( GPIOD,GPIO_PIN_4,GPIO_PIN_SET);HAL_GPIO_WritePin( GPIOD,GPIO_PIN_4,GPIO_PIN_RESET);\
if((x>>0)&1)  HAL_GPIO_WritePin( GPIOD,GPIO_PIN_3,GPIO_PIN_SET);else HAL_GPIO_WritePin( GPIOD,GPIO_PIN_3,GPIO_PIN_RESET);HAL_GPIO_WritePin( GPIOD,GPIO_PIN_4,GPIO_PIN_SET);HAL_GPIO_WritePin( GPIOD,GPIO_PIN_4,GPIO_PIN_RESET);\
HAL_GPIO_WritePin( GPIOD,GPIO_PIN_11,GPIO_PIN_SET);\
}

//////////////////////////////////////////////////////////////////////
//������ɫ
#define WHITE         	 0xFFFF
#define BLACK         	 0x0000	  
#define BLUE         	 0x001F  
#define BRED             0XF81F
#define GRED 			 0XFFE0
#define GBLUE			 0X07FF
#define RED           	 0xF800
#define MAGENTA       	 0xF81F
#define GREEN         	 0x07E0
#define CYAN          	 0x7FFF
#define YELLOW        	 0xFFE0
#define BROWN 			 0XBC40 //��ɫ
#define BRRED 			 0XFC07 //�غ�ɫ
#define GRAY  			 0X8430 //��ɫ
//GUI��ɫ

#define DARKBLUE      	 0X01CF	//����ɫ
#define LIGHTBLUE      	 0X7D7C	//ǳ��ɫ  
#define GRAYBLUE       	 0X5458 //����ɫ
//������ɫΪPANEL����ɫ 
 
#define LIGHTGREEN     	 0X841F //ǳ��ɫ
//#define LIGHTGRAY      0XEF5B //ǳ��ɫ(PANNEL)
#define LGRAY 			 0XC618 //ǳ��ɫ(PANNEL),���屳��ɫ

#define LGRAYBLUE        0XA651 //ǳ����ɫ(�м����ɫ)
#define LBBLUE           0X2B12 //ǳ����ɫ(ѡ����Ŀ�ķ�ɫ)
	    															  
extern uint16_t BACK_COLOR, POINT_COLOR ;  
uint16_t GPIO_ReadOutputData(GPIO_TypeDef* GPIOx);
void Write_u16_data(GPIO_TypeDef* GPIOx,uint16_t data);
void LCD_Init(void);
void LCD_DisplayOn(void);
void LCD_DisplayOff(void);
void LCD_Clear(uint16_t Color);	 
void LCD_DrawPoint(uint16_t x,uint16_t y);	//����
uint16_t  LCD_ReadPoint(uint16_t x,uint16_t y); 	//����
void Draw_Circle(uint16_t x0,uint16_t y0,uint8_t r);
void LCD_DrawLine(uint16_t x1, uint16_t y1, uint16_t x2, uint16_t y2);
void LCD_DrawRectangle(uint16_t x1, uint16_t y1, uint16_t x2, uint16_t y2);		   
void LCD_Fill(uint16_t xsta,uint16_t ysta,uint16_t xend,uint16_t yend,uint16_t color);
void LCD_ShowChar(uint16_t x,uint16_t y,uint8_t num,uint8_t size,uint8_t mode);	//��ʾһ���ַ�
void LCD_ShowNum(uint16_t x,uint16_t y,uint32_t num,uint8_t len,uint8_t size);  	//��ʾһ������
void LCD_Show2Num(uint16_t x,uint16_t y,uint16_t num,uint8_t len,uint8_t size,uint8_t mode);//��ʾ2������
void LCD_ShowString(uint16_t x,uint16_t y,const uint8_t *p);		 	//��ʾһ���ַ���,16����
void LCD_SetCursor(uint16_t xsta, uint16_t ysta,uint16_t xend,uint16_t yend);
void ILI9341_DrawPixel ( uint16_t usX, uint16_t usY, uint16_t uColor );
void LCD_WriteReg(uint8_t LCD_Reg, uint16_t LCD_RegValue);
uint16_t LCD_ReadReg(uint8_t LCD_Reg);
void LCD_WriteRAM_Prepare(void);
void LCD_WriteRAM(uint16_t RGB_Code);
void TEST_FONT(void);
void Test_Show_CH_Font16(uint16_t x,uint16_t y,uint8_t index,uint16_t color);
void Test_Show_CH_Font24(uint16_t x,uint16_t y,uint8_t index,uint16_t color);		
uint16_t LCD_ReadRAM(void);		   
uint16_t LCD_BGR2RGB(uint16_t c);


//д8λ���ݺ���
//�ú궨��,����ٶ�.
#if LCD_FAST_IO==1 //����IO
#define LCD_WR_DATA(data){\
LCD_RS_SET;\
LCD_CS_CLR;\
DATAOUT(data);\
LCD_WR_CLR;\
LCD_WR_SET;\
LCD_CS_SET;\
} 
#else//����IO
#define LCD_WR_DATA(data){\
LCD_RS=1;\
LCD_CS=0;\
DATAOUT(data);\
LCD_WR=0;\
LCD_WR=1;\
LCD_CS=1;\
} 	
#endif	

#define LCD_WR_TDATA(data){\
LCD_RS=1;\
LCD_CS=0;\
DATAOUT(data & 0xFF00);\
LCD_WR=0;\
LCD_WR=1;\
DATAOUT(data << 8);\
LCD_WR=0;\
LCD_WR=1;\
LCD_CS=1;\
}																			 
							  		 

	 






#endif /* __BSP_ILI9341_ILI9341_H */


