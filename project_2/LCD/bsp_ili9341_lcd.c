/***************************************************/
/**********���������������ILI9341******************/
/***************************************************/
#include "bsp_ili9341_lcd.h"
#include "fonts.h" 
#include "tim.h"
//������ɫ,������ɫ
uint16_t POINT_COLOR = 0x0000,BACK_COLOR = 0xFFFF;  
uint16_t DeviceCode;	 
uint16_t LCD_X_LENGTH = ILI9341_LESS_PIXEL;
uint16_t LCD_Y_LENGTH = ILI9341_MORE_PIXEL;

/*******************************************************
�����ܣ�GPIODд��u16����
�Ρ�������
����ֵ����
********************************************************/
void Write_u16_data(GPIO_TypeDef* GPIOx,uint16_t data)
{
	uint16_t buff;
	buff = ~data;
	GPIOx->BSRR = (buff<<16)|data;
	
}
/*******************************************************
�����ܣ���ȡGPIO�������
�Ρ�������
����ֵ����
********************************************************/
uint16_t GPIO_ReadOutputData(GPIO_TypeDef* GPIOx)
{
  /* Check the parameters */
  assert_param(IS_GPIO_ALL_PERIPH(GPIOx));

  return ((uint16_t)GPIOx->ODR);
}
#if LCD_FAST_IO==1 //����IO

/*******************************************************
�����ܣ�LCD��λ
�Ρ�������
����ֵ����
********************************************************/
void LCD_RESET(void)
{ 
	LCD_RST_SET;//�ߵ�ƽ  
 	HAL_Delay_s(1);
	LCD_RST_CLR;//�͵�ƽ
	HAL_Delay_s(20);
	LCD_RST_SET;//�ߵ�ƽ  
	HAL_Delay_s(20);
}
#else//����IO

/*******************************************************
�����ܣ�LCD��λ
�Ρ�������
����ֵ����
********************************************************/
void LCD_RESET(void)
{ 
	LCD_RST(1);  
 	HAL_Delay_s(1);
	LCD_RST(0);//�͵�ƽ
	HAL_Delay_s(20);
	LCD_RST(1);//�ߵ�ƽ  
	HAL_Delay_s(20);
} 	
#endif

#if LCD_FAST_IO==1 //����IO

/*******************************************************
�����ܣ�д�Ĵ�������
�Ρ�����data���Ĵ���ֵ
����ֵ����
********************************************************/
void LCD_WR_REG(uint8_t data)
{ 
	LCD_RS_CLR;//д��ַ  
 	LCD_CS_CLR; 
	DATAOUT(data); 
	LCD_WR_CLR; 
	LCD_WR_SET; 
 	LCD_CS_SET;   
}
#else//����IO

/*******************************************************
�����ܣ�д�Ĵ�������
�Ρ�����data���Ĵ���ֵ
����ֵ����
********************************************************/
void LCD_WR_REG(uint8_t data)
{ 
	LCD_RS(0);//д��ַ  
 	LCD_CS(0); 
	DATAOUT(data); 
	LCD_WR(0); 
	LCD_WR(1); 
 	LCD_CS(1);   
} 	
#endif	

/*******************************************************
�����ܣ�д�Ĵ���
�Ρ�����LCD_Reg:�Ĵ�����ַ
	    LCD_RegValue:Ҫд�������
����ֵ����
********************************************************/
void LCD_WriteReg(uint8_t LCD_Reg, uint16_t LCD_RegValue)
{	
	LCD_WR_REG(LCD_Reg);  
	LCD_WR_DATA(LCD_RegValue);	    		 
}	   
 
/*******************************************************
�����ܣ���ʼдGRAM
�Ρ�������
����ֵ����
********************************************************/
void LCD_WriteRAM_Prepare(void)
{
	LCD_WR_REG(0x2c);
}	 


/*******************************************************
�����ܣ�LCDдGRAM
�Ρ�����RGB_Code:��ɫֵ
����ֵ����
********************************************************/
void LCD_WriteRAM(uint16_t RGB_Code)
{							    
	LCD_WR_DATA(RGB_Code);//дʮ��λGRAM
}



/*******************************************************
�����ܣ���ILI93xx����������ΪGBR��ʽ��������д��
		��ʱ��ΪRGB��ʽ��ͨ���ú���ת��
�Ρ�����c:GBR��ʽ����ɫֵ
����ֵ��RGB��ʽ����ɫֵ
********************************************************/
uint16_t LCD_BGR2RGB(uint16_t c)
{
	uint16_t  r,g,b,rgb;   
	b=(c>>0)&0x1f;
	g=(c>>5)&0x3f;
	r=(c>>11)&0x1f;	 
	rgb=(b<<11)+(g<<5)+(r<<0);		 
	return(rgb);
}		 


/*******************************************************
�����ܣ����ù��λ��
�Ρ�����Xpos:������
		Ypos:������
����ֵ��
********************************************************/
__inline void LCD_SetCursor(uint16_t xsta, uint16_t ysta,uint16_t xend,uint16_t yend)
{ 


	LCD_WR_REG(0x2a);
	LCD_WR_DATA(xsta>>8);
	LCD_WR_DATA(xsta);
	LCD_WR_DATA(xend>>8);
	LCD_WR_DATA(xend);

	LCD_WR_REG(0x2b);
	LCD_WR_DATA(ysta>>8);
	LCD_WR_DATA(ysta);
	LCD_WR_DATA(yend>>8);
	LCD_WR_DATA(yend);

	LCD_WriteRAM_Prepare();     //��ʼд��GRAM
}


/*******************************************************
�����ܣ�����
�Ρ�����x:0~239
		y:0~319
		POINT_COLOR:�˵����ɫ
����ֵ����
********************************************************/
void LCD_DrawPoint(uint16_t x,uint16_t y)
{
	LCD_SetCursor(x,y,x,y);
	LCD_WR_DATA(POINT_COLOR); 
}

/*******************************************************
�����ܣ���ʼ��lcd
�Ρ�������
����ֵ����
��  ע���ó�ʼ���������Գ�ʼ������ILI93XXҺ��,������������
	    �ǻ���ILI9320��!�������ͺŵ�����оƬ��û�в���! 
********************************************************/
void LCD_Init(void)
{ 


	LCD_RESET();//��λ
	LCD_RST_BL;  					 
	HAL_Delay_s(5); // HAL_Delay_s 5 ms 
	LCD_WriteReg(0x0000,0x0001);
	HAL_Delay_s(5); // HAL_Delay_s 5 ms 
 
	//************* Start Initial Sequence **********//   ILI9341+IVO2.8
	LCD_WR_REG(0xCF);  
	LCD_WR_DATA(0x00); 
	LCD_WR_DATA(0xD9); 
	LCD_WR_DATA(0X30); 
	 
	LCD_WR_REG(0xED);  
	LCD_WR_DATA(0x64); 
	LCD_WR_DATA(0x03); 
	LCD_WR_DATA(0X12); 
	LCD_WR_DATA(0X81); 
	 
	LCD_WR_REG(0xE8);  
	LCD_WR_DATA(0x85); 
	LCD_WR_DATA(0x10); 
	LCD_WR_DATA(0x78); 
	 
	LCD_WR_REG(0xCB);  
	LCD_WR_DATA(0x39); 
	LCD_WR_DATA(0x2C); 
	LCD_WR_DATA(0x00); 
	LCD_WR_DATA(0x34); 
	LCD_WR_DATA(0x02); 
	 
	LCD_WR_REG(0xF7);  
	LCD_WR_DATA(0x20); 
	 
	LCD_WR_REG(0xEA);  
	LCD_WR_DATA(0x00); 
	LCD_WR_DATA(0x00); 
	 
	LCD_WR_REG(0xC0);    //Power control 
	LCD_WR_DATA(0x21);   //VRH[5:0] 
	 
	LCD_WR_REG(0xC1);    //Power control 
	LCD_WR_DATA(0x12);   //SAP[2:0];BT[3:0] 
	 
	LCD_WR_REG(0xC5);    //VCM control 
	LCD_WR_DATA(0x32); 
	LCD_WR_DATA(0x3C); 
	 
	LCD_WR_REG(0xC7);    //VCM control2 
	LCD_WR_DATA(0XC1); 
	 
	LCD_WR_REG(0x36);    // Memory Access Control 
	LCD_WR_DATA(0x08); 
	 
	LCD_WR_REG(0x3A);   
	LCD_WR_DATA(0x55); 
	
	LCD_WR_REG(0xB1);   
	LCD_WR_DATA(0x00);   
	LCD_WR_DATA(0x18); 
	 
	LCD_WR_REG(0xB6);    // Display Function Control 
	LCD_WR_DATA(0x0A); 
	LCD_WR_DATA(0xA2); 
	 
	LCD_WR_REG(0xF2);    // 3Gamma Function Disable 
	LCD_WR_DATA(0x00); 
	 
	LCD_WR_REG(0x26);    //Gamma curve selected 
	LCD_WR_DATA(0x01); 
	 
	LCD_WR_REG(0xE0);    //Set Gamma 
	LCD_WR_DATA(0x0F); 
	LCD_WR_DATA(0x20); 
	LCD_WR_DATA(0x1E); 
	LCD_WR_DATA(0x09); 
	LCD_WR_DATA(0x12); 
	LCD_WR_DATA(0x0B); 
	LCD_WR_DATA(0x50); 
	LCD_WR_DATA(0XBA); 
	LCD_WR_DATA(0x44); 
	LCD_WR_DATA(0x09); 
	LCD_WR_DATA(0x14); 
	LCD_WR_DATA(0x05); 
	LCD_WR_DATA(0x23); 
	LCD_WR_DATA(0x21); 
	LCD_WR_DATA(0x00); 
	 
	LCD_WR_REG(0XE1);    //Set Gamma 
	LCD_WR_DATA(0x00); 
	LCD_WR_DATA(0x19); 
	LCD_WR_DATA(0x19); 
	LCD_WR_DATA(0x00); 
	LCD_WR_DATA(0x12); 
	LCD_WR_DATA(0x07); 
	LCD_WR_DATA(0x2D); 
	LCD_WR_DATA(0x28); 
	LCD_WR_DATA(0x3F); 
	LCD_WR_DATA(0x02); 
	LCD_WR_DATA(0x0A); 
	LCD_WR_DATA(0x08); 
	LCD_WR_DATA(0x25); 
	LCD_WR_DATA(0x2D); 
	LCD_WR_DATA(0x0F); 
	 
	LCD_WR_REG(0x11);    //Exit Sleep 
	HAL_Delay_s(120); 
	LCD_WR_REG(0x29);    //Display on 

	LCD_LED(GPIO_PIN_SET);//��������	 
	LCD_Clear(WHITE);
}  		  
  

/*******************************************************
�����ܣ���������
�Ρ�����Color:Ҫ���������ɫ
����ֵ����
********************************************************/
void LCD_Clear(uint16_t Color)
{
    LCD_Fill(0,0,239,319,Color);
}  


/*******************************************************
�����ܣ���ָ�����������ָ����ɫ
�Ρ����������С:(xend-xsta)*(yend-ysta)
����ֵ����
********************************************************/
void LCD_Fill(uint16_t xsta,uint16_t ysta,uint16_t xend,uint16_t yend,uint16_t color)
{          
	uint16_t i,j;
	uint16_t xlen=0;
	LCD_SetCursor(xsta,ysta,xend,yend);
	
#if USE_HORIZONTAL==1
	xlen=yend-ysta+1;	   
	for(i=xsta;i<=xend;i++)
	{  
		for(j=0;j<xlen;j++)LCD_WR_DATA(color);//?????? 	  
	}
#else
	xlen=xend-xsta+1;	   
	for(i=ysta;i<=yend;i++)
	{ 
		for(j=0;j<xlen;j++)LCD_WR_DATA(color);//?????? 	    
	}
#endif						  	    
}  


/*******************************************************
�����ܣ�����
�Ρ�����x1,y1:�������
		x2,y2:�յ����� 
����ֵ����
********************************************************/
void LCD_DrawLine(uint16_t x1, uint16_t y1, uint16_t x2, uint16_t y2)
{
	uint16_t t; 
	int xerr=0,yerr=0,delta_x,delta_y,distance; 
	int incx,incy,uRow,uCol; 

	delta_x=x2-x1; //������������ 
	delta_y=y2-y1; 
	uRow=x1; 
	uCol=y1; 
	if(delta_x>0)incx=1; //���õ������� 
	else if(delta_x==0)incx=0;//��ֱ�� 
	else {incx=-1;delta_x=-delta_x;} 
	if(delta_y>0)incy=1; 
	else if(delta_y==0)incy=0;//ˮƽ�� 
	else{incy=-1;delta_y=-delta_y;} 
	if( delta_x>delta_y)distance=delta_x; //ѡȡ�������������� 
	else distance=delta_y; 
	for(t=0;t<=distance+1;t++ )//������� 
	{  
		LCD_DrawPoint(uRow,uCol);//���� 
		xerr+=delta_x ; 
		yerr+=delta_y ; 
		if(xerr>distance) 
		{ 
			xerr-=distance; 
			uRow+=incx; 
		} 
		if(yerr>distance) 
		{ 
			yerr-=distance; 
			uCol+=incy; 
		} 
	}  
}    


/*******************************************************
�����ܣ�������
�Ρ�����x1,y1:�������
		x2,y2:�յ����� 
����ֵ����
********************************************************/
void LCD_DrawRectangle(uint16_t x1, uint16_t y1, uint16_t x2, uint16_t y2)
{
	LCD_DrawLine(x1,y1,x2,y1);
	LCD_DrawLine(x1,y1,x1,y2);
	LCD_DrawLine(x1,y2,x2,y2);
	LCD_DrawLine(x2,y1,x2,y2);
}



/*******************************************************
�����ܣ���ָ��λ�û�һ��ָ����С��Բ
�Ρ�������x,y):���ĵ�
		r    :�뾶
����ֵ����
********************************************************/
void Draw_Circle(uint16_t x0,uint16_t y0,uint8_t r)
{
	int a,b;
	int di;
	a=0;b=r;	  
	di=3-(r<<1);             //�ж��¸���λ�õı�־
	while(a<=b)
	{
		LCD_DrawPoint(x0-b,y0-a);             //3           
		LCD_DrawPoint(x0+b,y0-a);             //0           
		LCD_DrawPoint(x0-a,y0+b);             //1       
		LCD_DrawPoint(x0-b,y0-a);             //7           
		LCD_DrawPoint(x0-a,y0-b);             //2             
		LCD_DrawPoint(x0+b,y0+a);             //4               
		LCD_DrawPoint(x0+a,y0-b);             //5
		LCD_DrawPoint(x0+a,y0+b);             //6 
		LCD_DrawPoint(x0-b,y0+a);             
		a++;
		//ʹ��Bresenham�㷨��Բ     
		if(di<0)di +=4*a+6;	  
		else
		{
			di+=10+4*(a-b);   
			b--;
		} 
		LCD_DrawPoint(x0+a,y0+b);
	}
} 


/*******************************************************
�����ܣ���ָ��λ����ʾһ���ַ�
�Ρ�����x:0~234
		y:0~308
		num:Ҫ��ʾ���ַ�:" "--->"~"
		size:�����С 12/16
		mode:���ӷ�ʽ(1)���Ƿǵ��ӷ�ʽ(0)
		��ָ��λ����ʾһ���ַ�
		x:0~234
		y:0~308
		num:Ҫ��ʾ���ַ�:" "--->"~"
		size:�����С 12/16
		mode:���ӷ�ʽ(1)���Ƿǵ��ӷ�ʽ(0)
����ֵ����
********************************************************/
void LCD_ShowChar(uint16_t x,uint16_t y,uint8_t num,uint8_t size,uint8_t mode)
{  
#if USE_HORIZONTAL==1
//#ifdef USE_HORIZONTAL
#define MAX_CHAR_POSX 312
#define MAX_CHAR_POSY 232 
#else     
#define MAX_CHAR_POSX 232
#define MAX_CHAR_POSY 312
#endif 
    uint8_t temp;
    uint8_t pos,t;
	uint16_t x0=x;
	uint16_t colortemp=POINT_COLOR;      
    if(x>MAX_CHAR_POSX||y>MAX_CHAR_POSY)return;	    
	//���ô���		   
	num=num-' ';//�õ�ƫ�ƺ��ֵ
	if(!mode) //�ǵ��ӷ�ʽ
	{
		for(pos=0;pos<size;pos++)
		{
			if(size==48)temp=asc2_1024[num][pos];//����1206����
			else temp=asc2_1608[num][pos];		 //����1608����
			for(t=0;t<size/2;t++)
		    {                 
		        if(temp&0x01)POINT_COLOR=colortemp;
				else POINT_COLOR=BACK_COLOR;
				LCD_DrawPoint(x,y);	
				temp>>=1; 
				x++;
		    }
			x=x0;
			y++;
		}	
	}else//���ӷ�ʽ
	{
		for(pos=0;pos<size;pos++)
		{
			if(size==48)temp=asc2_1024[num][pos];//����1206����
			else temp=asc2_1608[num][pos];		 //����1608����
			for(t=0;t<size/2;t++)
		    {                 
		        if(temp&0x01)LCD_DrawPoint(x+t,y+pos);//��һ����     
		        temp>>=1; 
		    }
		}
	}
	POINT_COLOR=colortemp;	    	   	 	  
}   



/*******************************************************
�����ܣ�m^n����
�Ρ�����m:����
		n:�η�
����ֵ����
********************************************************/
uint32_t mypow(uint8_t m,uint8_t n)
{
	uint32_t result=1;	 
	while(n--)result*=m;    
	return result;
}			 


/*******************************************************
�����ܣ���ʾ���֣���λΪ0,����ʾ
�Ρ�����x,y :�������	 
		len :���ֵ�λ��
		size:�����С
		color:��ɫ
		num:��ֵ(0~4294967295);
����ֵ����
********************************************************/
void LCD_ShowNum(uint16_t x,uint16_t y,uint32_t num,uint8_t len,uint8_t size)
{         	
	uint8_t t,temp;
	uint8_t enshow=0;						   
	for(t=0;t<len;t++)
	{
		temp=(num/mypow(10,len-t-1))%10;
		if(enshow==0&&t<(len-1))
		{
			if(temp==0)
			{
				LCD_ShowChar(x+(size/2)*t,y,' ',size,0);
				continue;
			}else enshow=1; 
		 	 
		}
	 	LCD_ShowChar(x+(size/2)*t,y,temp+'0',size,0); 
	}
} 
	

/*******************************************************
�����ܣ���ʾ����,��λΪ0,������ʾ
�Ρ�����x,y :�������	 
		len :���ֵ�λ��
		size:�����С
		color:��ɫ
		num:��ֵ(0~4294967295);
		mode:
			[7]:0,�����;1,���0.
			[6:1]:����
			[0]:0,�ǵ�����ʾ;1,������ʾ
����ֵ����
********************************************************/
void LCD_Show2Num(uint16_t x,uint16_t y,uint16_t num,uint8_t len,uint8_t size,uint8_t mode)
{         	
	uint8_t t,temp;	
	uint8_t enshow=0;	
	for(t=0;t<len;t++)
	{
		temp=(num/mypow(10,len-t-1))%10;
	 	if(enshow==0&&t<(len-1))
		{
			if(temp==0)
			{
				if(mode&0X80)LCD_ShowChar(x+(size/2)*t,y,'0',size,mode&0X01);  
				else LCD_ShowChar(x+(size/2)*t,y,' ',size,mode&0X01);  
 				continue;
			}else enshow=1; 
		 	 
		}
	 	LCD_ShowChar(x+(size/2)*t,y,temp+'0',size,mode&0X01); 
	}
} 


/*******************************************************
�����ܣ���ʾ�ַ���
�Ρ�����x,y:�������  	 
		*p:�ַ�����ʼ��ַ
����ֵ����
��  ע����16����
********************************************************/
void LCD_ShowString(uint16_t x,uint16_t y,const uint8_t *p)
{         
    while(*p!='\0')
    {       
        if(x>MAX_CHAR_POSX){x=0;y+=16;}
        if(y>MAX_CHAR_POSY){y=x=0;LCD_Clear(WHITE);}
        LCD_ShowChar(x,y,*p,16,0);
        x+=8;
        p++;
    }  
}

/**
 * @brief  ��ILI9341��ʾ����ĳһ����ĳ����ɫ�������
 * @param  usX �����ض�ɨ�跽���¸õ��X����
 * @param  usY �����ض�ɨ�跽���¸õ��Y����
 * @note ��ʹ��LCD_SetBackColor��LCD_SetTextColor��LCD_SetColors����������ɫ
 * @retval ��
 */
void ILI9341_DrawPixel ( uint16_t usX, uint16_t usY, uint16_t uColor )
{
    if ( ( usX < LCD_X_LENGTH ) && ( usY < LCD_Y_LENGTH ) )
    {
        LCD_SetCursor ( usX,usY,1,1);
				LCD_Fill( usX ,usY,1,1,uColor);
    }
}

/*******************************************************
�����ܣ���LCD�ϴ�����
�Ρ�����x��y����������
		color�������ɫ
����ֵ����
********************************************************/
void LCD_Draw_Point(uint16_t x,uint16_t y,uint16_t color)
{
	uint16_t temp;
	temp=POINT_COLOR;
	POINT_COLOR=color;
	LCD_DrawPoint(x,y);
	POINT_COLOR=temp;
}


/*******************************************************
�����ܣ���ָ��λ�� ��ʾ1��16*16�ĺ���
�Ρ�����x��y��������ʾ��λ��
		index:tfont��������ĵڼ�������
		color:������ֵ���ɫ
����ֵ����
********************************************************/
void Test_Show_CH_Font16(uint16_t x,uint16_t y,uint8_t index,uint16_t color)
{   			    
	uint8_t temp,t,t1;
	uint16_t y0=y;				   
    for(t=0;t<32;t++)//ÿ��16*16�ĺ��ֵ��� ��32���ֽ�
    {   
		if(t<16)temp=tfont16[index*2][t];      //ǰ16���ֽ�
		else temp=tfont16[index*2+1][t-16];    //��16���ֽ�	                          
        for(t1=0;t1<8;t1++)
		{
			if(temp&0x80)LCD_Draw_Point(x,y,color);//��ʵ�ĵ�
			else LCD_Draw_Point(x,y,BACK_COLOR);   //���հ׵㣨ʹ�ñ���ɫ��
			temp<<=1;
			y++;
			if((y-y0)==16)
			{
				y=y0;
				x++;
				break;
			}
		}  	 
    }          
}


/*******************************************************
�����ܣ���ָ��λ�� ��ʾ1��24*24�ĺ���
�Ρ�����x��y��������ʾ��λ��
		index:tfont��������ĵڼ�������
		color:������ֵ���ɫ
����ֵ����
********************************************************/
void Test_Show_CH_Font24(uint16_t x,uint16_t y,uint8_t index,uint16_t color)
{   			    
	uint8_t temp,t,t1;
	uint16_t y0=y;				   
    for(t=0;t<72;t++)//ÿ��24*24�ĺ��ֵ��� ��72���ֽ�
    {   
		if(t<24)temp=tfont24[index*3][t];           //ǰ24���ֽ�
		else if(t<48)temp=tfont24[index*3+1][t-24]; //��24���ֽ�	                          
        else temp=tfont24[index*3+2][t-48];         //��24���ֽ�
	    for(t1=0;t1<8;t1++)
		{
			if(temp&0x80)LCD_Draw_Point(x,y,color);//��ʵ�ĵ�
			else LCD_Draw_Point(x,y,BACK_COLOR);   //���հ׵㣨ʹ�ñ���ɫ��
			temp<<=1;
			y++;
			if((y-y0)==48)
			{
				y=y0;
				x++;
				break;
			}
		}  	 
    }          
}


/*******************************************************
�����ܣ���ʾ����
�Ρ�������
����ֵ����
********************************************************/
void TEST_FONT(void)
{
	uint8_t t;
	uint8_t x,x1;
	x=20;
	x1=0;
	for(t=0;6;t++)//11������
	{
		if(t==1)Test_Show_CH_Font24(50,50,10,0);
		else     Test_Show_CH_Font24(x1,50,t,0);
		x+=16;
		x1+=24;
		
	}
}



