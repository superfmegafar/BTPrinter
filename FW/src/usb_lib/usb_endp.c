/******************** (C) COPYRIGHT 2008 STMicroelectronics ********************
* File Name          : usb_endp.c
* Author             : MCD Application Team
* Version            : V2.2.1
* Date               : 09/22/2008
* Description        : Endpoint routines
********************************************************************************
* THE PRESENT FIRMWARE WHICH IS FOR GUIDANCE ONLY AIMS AT PROVIDING CUSTOMERS
* WITH CODING INFORMATION REGARDING THEIR PRODUCTS IN ORDER FOR THEM TO SAVE TIME.
* AS A RESULT, STMICROELECTRONICS SHALL NOT BE HELD LIABLE FOR ANY DIRECT,
* INDIRECT OR CONSEQUENTIAL DAMAGES WITH RESPECT TO ANY CLAIMS ARISING FROM THE
* CONTENT OF SUCH FIRMWARE AND/OR THE USE MADE BY CUSTOMERS OF THE CODING
* INFORMATION CONTAINED HEREIN IN CONNECTION WITH THEIR PRODUCTS.
*******************************************************************************/

/* Includes ------------------------------------------------------------------*/
#include "usb_lib.h"
#include "usb_desc.h"
#include "usb_mem.h"
#include "hw_config.h"
#include "usb_istr.h"
#include "usb_bot.h"
#include "usb_app_config.h"

/* Private typedef -----------------------------------------------------------*/
/* Private define ------------------------------------------------------------*/
/* Private macro -------------------------------------------------------------*/
/* Private variables ---------------------------------------------------------*/
//u8 buffer_out[VIRTUAL_COM_PORT_DATA_SIZE];
//u32 count_out = 0;
u32 count_in = 0;

/* Private function prototypes -----------------------------------------------*/
/* Private functions ---------------------------------------------------------*/
/*******************************************************************************
* Function Name  : EP3_IN_Callback
* Description    :
* Input          : None.
* Output         : None.
* Return         : None.
*******************************************************************************/
void EP3_OUT_Callback(void)
{
#if(USB_DEVICE_CONFIG &_USE_USB_VIRTUAL_COMM_DEVICE)
   //int i;
  if (g_usb_type == USB_VIRTUAL_PORT)
  {
	  count_out = GetEPRxCount(ENDP3);
	  PMAToUserBufferCopy(buffer_out, ENDP3_RXADDR, count_out);
	  SetEPRxValid(ENDP3);

	  /*for (i = 0; i < count_out; i++)
	  {
	  PCUsart_InByte(buffer_out[i]);
	  }*/
	  //@todo....

	  //此处应用可以处理虚拟串口接收到的数据，由具体应用来修改。。。。
  }
#endif
}

/*******************************************************************************
* Function Name  : EP2_OUT_Callback.
* Description    : EP2 OUT Callback Routine.
* Input          : None.
* Output         : None.
* Return         : None.
*******************************************************************************/
void EP2_OUT_Callback(void)
{
#if(USB_DEVICE_CONFIG &_USE_USB_MASS_STOARGE_DEVICE)
	if (g_usb_type == USB_MASSSTORAGE)
	{
		Mass_Storage_Out();
	}
#endif
}

/*******************************************************************************
* Function Name  : EP1_IN_Callback
* Description    :
* Input          : None.
* Output         : None.
* Return         : None.
*******************************************************************************/
void EP1_IN_Callback(void)
{
#if(USB_DEVICE_CONFIG &_USE_USB_MASS_STOARGE_DEVICE)
	if (g_usb_type == USB_MASSSTORAGE)
	{
		Mass_Storage_In();
	}
	else
#endif
	{
		count_in = 0;
	}
}

/******************* (C) COPYRIGHT 2008 STMicroelectronics *****END OF FILE****/

