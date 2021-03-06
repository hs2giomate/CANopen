/*
 * \brief CAN module object for generic STM microcontroller.
 *
 * \details CanOpenNode driver implementation based on STMCubeMX HAL.
 *
 * @file        CO_driver.c
 * @ingroup     CO_driver
 * @author      Janez Paternoster
 * @author      Andrii Shylenko
 * @copyright   2004 - 2015 Janez Paternoster
 *
 * This file is part of CANopenNode, an opensource CANopen Stack.
 * Project home page is <https://github.com/CANopenNode/CANopenNode>.
 * For more information on CANopen see <http://www.can-cia.org/>.
 *
 * CANopenNode is free and open source software: you can redistribute
 * it and/or modify it under the terms of the GNU General Public License
 * as published by the Free Software Foundation, either version 2 of the
 * License, or (at your option) any later version.
 *
 * This program is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE. See the
 * GNU General Public License for more details.
 *
 * You should have received a copy of the GNU General Public License
 * along with this program. If not, see <http://www.gnu.org/licenses/>.
 *
 * Following clarification and special exception to the GNU General Public
 * License is included to the distribution terms of CANopenNode:
 *
 * Linking this library statically or dynamically with other modules is
 * making a combined work based on this library. Thus, the terms and
 * conditions of the GNU General Public License cover the whole combination.
 *
 * As a special exception, the copyright holders of this library give
 * you permission to link this library with independent modules to
 * produce an executable, regardless of the license terms of these
 * independent modules, and to copy and distribute the resulting
 * executable under terms of your choice, provided that you also meet,
 * for each linked independent module, the terms and conditions of the
 * license of that module. An independent module is a module which is
 * not derived from or based on this library. If you modify this
 * library, you may extend this exception to your version of the
 * library, but you are not obliged to do so. If you do not wish
 * to do so, delete this exception statement from your version.
 */

/*-----------------------------------------------------------------------------
 * INCLUDE SECTION
 *----------------------------------------------------------------------------*/
//#include "can.h" /* Include HAL interfaces generated by cube MX. */

#include "CO_driver.h"
#include "CO_Emergency.h"
#include "hal_can_async.h"

/*-----------------------------------------------------------------------------
 * LOCAL (static) DEFINITIONS
 *----------------------------------------------------------------------------*/
/*\brief pointer to CO_CanModule used in CubeMX CAN Rx interrupt routine*/
static CO_CANmodule_t* RxFifo_Callback_CanModule_p = NULL;
/*\brief TxHeader object used for transmission */
//static CAN_TxHeaderTypeDef TxHeader;
struct can_message msgHeader;

/*-----------------------------------------------------------------------------
 * LOCAL FUNCTION PROTOTYPES
 *----------------------------------------------------------------------------*/
//static void prepareTxHeader(struct can_message *msgHeader, CO_CANtx_t *buffer);

/*-----------------------------------------------------------------------------
 * LOCAL FUNCTIONS
 *----------------------------------------------------------------------------*/

/*!*****************************************************************************
 * \author  Andrii Shylenko
 * \date 	10.11.2018
 *
 * \brief prepares CAN Tx header based on the ID, RTR and data count.
 * \param [in]	TxHeader pointer to @CAN_TxHeaderTypeDef object
 * \param [in]	buffer ponyer to CO_CANtx_t with CANopen configuration data
 *
 * \ingroup CO_driver
 ******************************************************************************/
void prepareTxHeader(struct can_message *msgHeader, CO_CANtx_t *buffer)
{
	/* Map buffer data to the HAL CAN tx header data*/
	//TxHeader->ExtId = 0u;
	msgHeader->id=0;
	//TxHeader->IDE = 0;
	msgHeader->fmt=CAN_FMT_STDID;
	
	//TxHeader->DLC = buffer->DLC;
	msgHeader->len=buffer->DLC;
	
	//TxHeader->StdId = ( buffer->ident >> 2 );
	msgHeader->id=( buffer->ident >> 2 );
	
	//TxHeader->RTR = ( buffer->ident & 0x2 );
	msgHeader->type=( buffer->ident & 0x2 );
}

/*-----------------------------------------------------------------------------
 * GLOBAL FUNCTIONS - see descriptions in header file
 *----------------------------------------------------------------------------*/

//TODO move callbacks to the CO_driver.c and implement callback init routine

/* \brief 	Cube MX callbacks for Fifo0 and Fifo1
 * \details It is assumed that only one CANmodule is (CO->CANmodule[0]) is used.
 */


void CAN_RxFifo1MsgPendingCallback(void)
{
	if(RxFifo_Callback_CanModule_p != NULL)
	{
		CO_CANinterrupt_Rx(RxFifo_Callback_CanModule_p);
	}
	else
	{
		;//TODO add assert here
	}
}

void CO_CANsetConfigurationMode(struct can_async_descriptor *CANbaseAddress){
	/* Put CAN module in configuration mode */
	/* HAL is responsible for that */
}

/******************************************************************************/
CO_ReturnError_t CO_CANsetNormalMode(CO_CANmodule_t *CANmodule){
	/* Put CAN module in normal mode */
	int32_t		error_CAN_hal;
	CO_ReturnError_t Error = CO_ERROR_NO;
	error_CAN_hal=can_async_enable(CANmodule->CANBaseDescriptor);

	/* Enable CAN interrupts */

	//    HAL_NVIC_SetPriority(CAN1_TX_IRQn, 1, 0);
	//    HAL_NVIC_EnableIRQ(CAN1_TX_IRQn);
	//    HAL_NVIC_SetPriority(CAN1_RX0_IRQn, 1, 0);
	//    HAL_NVIC_EnableIRQ(CAN1_RX0_IRQn);
	//    HAL_NVIC_SetPriority(CAN1_RX1_IRQn, 1, 0);
	//    HAL_NVIC_EnableIRQ(CAN1_RX1_IRQn);

	if (error_CAN_hal != CO_ERROR_NO)
	{
		//_Error_Handler(__FILE__, __LINE__);
		return CO_ERROR_HAL;
	}
	CANmodule->CANnormal = true;
	return Error;
}

/******************************************************************************/
CO_ReturnError_t CO_CANmodule_init(
		CO_CANmodule_t         *CANmodule,
		struct can_async_descriptor    *HALCanObject,
		CO_CANrx_t              rxArray[],
		uint16_t                rxSize,
		CO_CANtx_t              txArray[],
		uint16_t                txSize,
		uint16_t                CANbitRate)
{
	uint16_t i;
	int32_t		error_CAN_hal;

	/* verify arguments */
	if(CANmodule==NULL || rxArray==NULL || txArray==NULL)
	{
		return CO_ERROR_ILLEGAL_ARGUMENT;
	}
	else
	{
		;//do nothing
	}

	RxFifo_Callback_CanModule_p = CANmodule;

	/* Configure object variables */
	CANmodule->CANBaseDescriptor = (struct can_async_descriptor*)HALCanObject;
	CANmodule->rxArray = rxArray;
	CANmodule->rxSize = rxSize;
	CANmodule->txArray = txArray;
	CANmodule->txSize = txSize;
	CANmodule->CANnormal = false;
	CANmodule->useCANrxFilters = false;
	CANmodule->bufferInhibitFlag = false;
	CANmodule->firstCANtxMessage = true;
	CANmodule->CANtxCount = 0U;
	CANmodule->errOld = 0U;
	CANmodule->em = NULL;

	for(i=0U; i<rxSize; i++)
	{
		rxArray[i].ident = 0U;
		rxArray[i].pFunct = NULL;
	}

	for(i=0U; i<txSize; i++)
	{
		txArray[i].bufferFull = false;
	}

	/* Configure CAN module registers */
	/* Configuration is handled by CubeMX HAL*/
	CO_CANmodule_disable(CANmodule);
	//HAL_CAN_MspDeInit(CANmodule->CANBaseDescriptor);
	error_CAN_hal=can_async_enable(HALCanObject);
	//HAL_CAN_MspInit(CANmodule->CANBaseDescriptor); /* NVIC and GPIO */
/*
	CANmodule->CANBaseDescriptor->Instance = CAN1;
	CANmodule->CANBaseDescriptor->Init.Mode = CAN_MODE_NORMAL;
	CANmodule->CANBaseDescriptor->Init.SyncJumpWidth = CAN_SJW_1TQ;
	CANmodule->CANBaseDescriptor->Init.TimeTriggeredMode = DISABLE;
	CANmodule->CANBaseDescriptor->Init.AutoBusOff = DISABLE;
	CANmodule->CANBaseDescriptor->Init.AutoWakeUp = DISABLE;
	CANmodule->CANBaseDescriptor->Init.AutoRetransmission = ENABLE;
	CANmodule->CANBaseDescriptor->Init.ReceiveFifoLocked = DISABLE;
	CANmodule->CANBaseDescriptor->Init.TransmitFifoPriority = DISABLE;
	CANmodule->CANBaseDescriptor->Init.TimeSeg2 = CAN_BS2_2TQ;
	CANmodule->CANBaseDescriptor->Init.TimeSeg1 = CAN_BS1_13TQ;
*/
	/* Can speed configuration. */
	/* Based on the values obtained from http://bittiming.can-wiki.info */
	/* Assuming CAN clock is 80 MHz */

	/*
    Bit	rate	Acurracy	Prescaler   nr. timequanta  Seg.1	Seg.2	Sample point  CAN_BUS_TIME
    1000		0.0000		5			16				13		2		87.5		  0x001c0004
    500		    0.0000		10			16				13		2		87.5		  0x001c0009
    250		 	0.0000		20			16				13		2		87.5		  0x001c0013
    125		    0.0000		40			16				13		2		87.5		  0x001c0027
    100		 	0.0000		50			16				13		2		87.5		  0x001c0031
    50 		 	0.0000		100			16				13		2		87.5		  0x001c0063
    20 		 	0.0000		250			16				13		2		87.5		  0x0007018f
    10 		 	0.0000		500			16				13		2		87.5		  0x001c01f3
	 */

	uint32_t Prescaler = 500;

	switch(CANbitRate) {
	case 1000:
		Prescaler = 5;
		break;
	case 500:
		Prescaler = 10;
		break;
	case 250:
		Prescaler = 20;
		break;
	case 125:
		Prescaler = 40;
		break;
	case 100:
		Prescaler = 50;
		break;
	case 50:
		Prescaler = 100;
		break;
	case 20:
		Prescaler = 250;
		break;
	case 10:
		Prescaler = 500;
		break;

	default :
		return  CO_ERROR_ILLEGAL_BAUDRATE;
	}

//	CANmodule->CANBaseDescriptor->Init.Prescaler = Prescaler;

	if (error_CAN_hal != CO_ERROR_NO)
	{
		//_Error_Handler(__FILE__, __LINE__);
		return CO_ERROR_HAL;
	}

	return CO_ERROR_NO;
}


/******************************************************************************/
void CO_CANmodule_disable(CO_CANmodule_t *CANmodule){
	/* turn off the module */
	/* handled by CubeMX HAL*/
	/*
	HAL_CAN_DeactivateNotification(CANmodule->CANbaseAddress ,
			CAN_IT_RX_FIFO0_MSG_PENDING |
			CAN_IT_RX_FIFO1_MSG_PENDING |
			CAN_IT_TX_MAILBOX_EMPTY);
	HAL_CAN_Stop(CANmodule->CANbaseAddress);
	*/
	can_async_enable(CANmodule->CANBaseDescriptor);
}


/******************************************************************************/
uint16_t CO_CANrxMsg_readIdent(struct can_message *msg){
	return (uint16_t) msg->id;
}


/******************************************************************************/
CO_ReturnError_t CO_CANrxBufferInit(
		CO_CANmodule_t         *CANmodule,
		uint16_t                index,
		uint16_t                ident,
		uint16_t                mask,
		bool_t                  rtr,
		void                   *object,
		void                  (*pFunct)(void *object, const CO_CANrxMsg_t *message))
{
	CO_ReturnError_t ret = CO_ERROR_NO;

	if((CANmodule!=NULL) && (object!=NULL) && (pFunct!=NULL) && (index < CANmodule->rxSize)){
		/* buffer, which will be configured */
		CO_CANrx_t *buffer = &CANmodule->rxArray[index];

		/* Configure object variables */
		buffer->object = object;
		buffer->pFunct = pFunct;

		/* CAN identifier and CAN mask, bit aligned with CAN module. Different on different microcontrollers. */
		buffer->ident = (ident & 0x07FF) << 2;
		if (rtr)
		{
			buffer->ident |= 0x02;
		}
		buffer->mask = (mask & 0x07FF) << 2;
		buffer->mask |= 0x02;

		/* Set CAN hardware module filter and mask. */
		if(CANmodule->useCANrxFilters)
		{
			/* TODO Configure CAN module hardware filters */
		}
		else
		{
			/*no hardware filters*/
			//CAN_FilterTypeDef FilterConfig;
			struct can_filter  filter;
			filter.id   = 0x0;
			filter.mask = 0;
			can_async_set_filter(CANmodule->CANBaseDescriptor, 0, CAN_FMT_STDID, &filter);
		}
	}
	else
	{
		ret = CO_ERROR_ILLEGAL_ARGUMENT;
	}
	return ret;
}


/******************************************************************************/
CO_CANtx_t *CO_CANtxBufferInit(
		CO_CANmodule_t         *CANmodule,
		uint16_t                index,
		uint16_t                ident,
		bool_t                  rtr,
		uint8_t                 noOfBytes,
		bool_t                  syncFlag)
{
	CO_CANtx_t *buffer = NULL;

	if((CANmodule != NULL) && (index < CANmodule->txSize)){
		/* get specific buffer */
		buffer = &CANmodule->txArray[index];

		/* CAN identifier, DLC and rtr, bit aligned with CAN module transmit buffer.*/

		buffer->ident &= 0x7FF;
		buffer->ident = ident << 2;
		if (rtr) buffer->ident |= 0x02;

		buffer->DLC = noOfBytes;
		buffer->bufferFull = false;
		buffer->syncFlag = syncFlag;
	}

	return buffer;
}

/******************************************************************************/
CO_ReturnError_t CO_CANsend(CO_CANmodule_t *CANmodule, CO_CANtx_t *buffer)
{
	CO_ReturnError_t err = CO_ERROR_NO;
	//struct can_async_descriptor *canHandler=malloc(sizeof(struct can_async_descriptor))
	struct can_async_descriptor *canHandler=(struct can_async_descriptor *)CANmodule->CANBaseDescriptor;
	//canHandler= (*struct can_async_descriptor )CANmodule->CANBaseDescriptor;
	/* Verify overflow */
	if(buffer->bufferFull){
		if(!CANmodule->firstCANtxMessage){
			/* don't set error, if bootup message is still on buffers */
			CO_errorReport((CO_EM_t*)CANmodule->em, CO_EM_CAN_TX_OVERFLOW, CO_EMC_CAN_OVERRUN, buffer->ident);
		}
		err = CO_ERROR_TX_OVERFLOW;
	}

	uint32_t TxMailboxNum;
	/* if CAN TX buffer is free, send message */
	CO_LOCK_CAN_SEND();

	prepareTxHeader(&msgHeader, buffer);

	if ((CANmodule->CANtxCount == 0) &&
			((uint32_t)hri_can_read_TXFQS_TFFL_bf(CAN_0.dev.hw) > 0 )) {
		CANmodule->bufferInhibitFlag = buffer->syncFlag;

		if( can_async_write(canHandler, &msgHeader)!= CO_ERROR_NO)
		{
			err = CO_ERROR_HAL;
		}
		else
		{
			;/*do nothing*/
		}
	}
	/* if no buffer is free, message will be sent in the task */
	else
	{
		buffer->bufferFull = true;
		CANmodule->CANtxCount++;
	}
	CO_UNLOCK_CAN_SEND();

	return err;
}


/******************************************************************************/
void CO_CANclearPendingSyncPDOs(CO_CANmodule_t *CANmodule)
{
	uint32_t tpdoDeleted = 0U;

	CO_LOCK_CAN_SEND();
	/* Abort message from CAN module, if there is synchronous TPDO.
	 * Take special care with this functionality. */
	/* TODO */

	/*
      if ((state = HAL_CAN_IsTxMessagePending(CANmodule->CANbaseAddress) && (CANmodule->bufferInhibitFlag))
      {
    	HAL_CAN_AbortTxRequest(CANmodule->);
      }
	 */

	if(/*messageIsOnCanBuffer && */CANmodule->bufferInhibitFlag){
		/* clear TXREQ */
		CANmodule->bufferInhibitFlag = false;
		tpdoDeleted = 1U;
	}
	/* delete also pending synchronous TPDOs in TX buffers */
	if(CANmodule->CANtxCount != 0U){
		uint16_t i;
		CO_CANtx_t *buffer = &CANmodule->txArray[0];
		for(i = CANmodule->txSize; i > 0U; i--){
			if(buffer->bufferFull){
				if(buffer->syncFlag){
					buffer->bufferFull = false;
					CANmodule->CANtxCount--;
					tpdoDeleted = 2U;
				}
			}
			buffer++;
		}
	}
	CO_UNLOCK_CAN_SEND();


	if(tpdoDeleted != 0U){
		CO_errorReport((CO_EM_t*)CANmodule->em, CO_EM_TPDO_OUTSIDE_WINDOW, CO_EMC_COMMUNICATION, tpdoDeleted);
	}
}


/******************************************************************************/
void CO_CANverifyErrors(CO_CANmodule_t *CANmodule){
	CO_EM_t* em = (CO_EM_t*)CANmodule->em;
	struct can_async_descriptor *canHandler= (struct can_async_descriptor *)CANmodule->CANBaseDescriptor;
	uint32_t HalCanErrorCode = canHandler->dev.cb->irq_handler();

	if(CANmodule->errOld != HalCanErrorCode)
	{
		CANmodule->errOld = HalCanErrorCode;
		if(HalCanErrorCode & HAL_CAN_ERROR_BOF)
		{                               /* bus off */
			CO_errorReport(em, CO_EM_CAN_TX_BUS_OFF, CO_EMC_BUS_OFF_RECOVERED, HalCanErrorCode);
		}
		else{                                               /* not bus off */
			CO_errorReset(em, CO_EM_CAN_TX_BUS_OFF, HalCanErrorCode);

			if(HalCanErrorCode & HAL_CAN_ERROR_EWG)
			{     											/* bus warning */
				CO_errorReport(em, CO_EM_CAN_BUS_WARNING, CO_EMC_NO_ERROR, HalCanErrorCode);
			}
			else
			{
				//do nothing
			}
			if(HalCanErrorCode & HAL_CAN_ERROR_EPV)
			{      											/* TX/RX bus passive */
				if(!CANmodule->firstCANtxMessage)
				{
					CO_errorReport(em, CO_EM_CAN_TX_BUS_PASSIVE, CO_EMC_CAN_PASSIVE, HalCanErrorCode);
				}
				else
				{
					//do nothing
				}
			}
			else{
				bool_t isError = CO_isError(em, CO_EM_CAN_TX_BUS_PASSIVE);
				if(isError)
				{
					CO_errorReset(em, CO_EM_CAN_TX_BUS_PASSIVE, HalCanErrorCode);
					CO_errorReset(em, CO_EM_CAN_TX_OVERFLOW, HalCanErrorCode);
				}
				else
				{
					//do nothing
				}
			}

			if(HalCanErrorCode & HAL_CAN_ERROR_NONE)
			{      											 /* no error */
				CO_errorReset(em, CO_EM_CAN_BUS_WARNING, HalCanErrorCode);
			}
			else
			{
				//do nothing
			}

		}

		if((HalCanErrorCode & HAL_CAN_ERROR_RX_FOV0) || (HalCanErrorCode & HAL_CAN_ERROR_RX_FOV1))
		{                                 					/* CAN RX bus overflow */
			CO_errorReport(em, CO_EM_CAN_RXB_OVERFLOW, CO_EMC_CAN_OVERRUN, HalCanErrorCode);
		}
		else
		{
			//do nothing
		}
	}
}

/*Interrupt handlers*/
/******************************************************************************/
void CO_CANinterrupt_Rx(const CO_CANmodule_t *CANmodule)
{
	/* receive interrupt */

	static CO_CANrxMsg_t CANmessage;
	bool_t msgMatched = false;
	CO_CANrx_t *MsgBuff = CANmodule->rxArray; /* receive message buffer from CO_CANmodule_t object. */
	HAL_CAN_GetRxMessage(CANmodule->CANBaseDescriptor, CAN_RX_FIFO0, &CANmessage.RxHeader, &CANmessage.data[0]);

	/*dirty hack, consider change to a pointer here*/
	CANmessage.DLC = (uint8_t)CANmessage.RxHeader.DLC;
	CANmessage.ident = CANmessage.RxHeader.StdId;

	uint32_t index;
	/* Search rxArray form CANmodule for the same CAN-ID. */
	for (index = 0; index < CANmodule->rxSize; index++)
	{
		uint16_t msg = (((uint16_t)(CANmessage.RxHeader.StdId << 2)) | (uint16_t)(CANmessage.RxHeader.RTR));
		if (((msg ^ MsgBuff->ident) & MsgBuff->mask) == 0)
		{
			msgMatched = true;
			break;
		}
		MsgBuff++;
	}

	/* Call specific function, which will process the message */
	if(msgMatched && (MsgBuff != NULL) && (MsgBuff->pFunct != NULL))
	{
		MsgBuff->pFunct(MsgBuff->object, &CANmessage);
	}

	//TODO filters handing

	//        if(CANmodule->useCANrxFilters){
	//            /* CAN module filters are used. Message with known 11-bit identifier has */
	//           /* been received */
	//            index = rcvMsg;  /* get index of the received message here. Or something similar */
	//            if(index < CANmodule->rxSize){
	//                buffer = &CANmodule->rxArray[index];
	//                /* verify also RTR */
	//                if(( RxHeader.RTR ) == CAN_RTR_DATA){
	//                    msgMatched = true;
	//                }
	//                if((( RxHeader. rcvMsgIdent ^ buffer->ident) & buffer->mask) == 0U){
	//                    msgMatched = true;
	//                }
	//            }
	//        }
	//        else{
	//            /* CAN module filters are not used, message with any standard 11-bit identifier */
	//            /* has been received. Search rxArray form CANmodule for the same CAN-ID. */
	//            buffer = &CANmodule->rxArray[0];
	//            for(index = CANmodule->rxSize; index > 0U; index--){
	//                if(((rcvMsgIdent ^ buffer->ident) & buffer->mask) == 0U){
	//                    msgMatched = true;
	//                    break;
	//                }
	//                buffer++;
	//            }
	//        }

	/*CubeMx HAL is responsible for clearing interrupt flags and all the dirty work. */
}


void CO_CANpolling_Tx(CO_CANmodule_t *CANmodule)
{
	if (HAL_CAN_GetTxMailboxesFreeLevel((CAN_HandleTypeDef*)CANmodule->CANBaseDescriptor) > 0)
	{
		/* First CAN message (bootup) was sent successfully */
		CANmodule->firstCANtxMessage = false;
		/* Clear flag from previous message */
		CANmodule->bufferInhibitFlag = false;
		/* Are there any new messages waiting to be send */
		if(CANmodule->CANtxCount > 0U)
		{
			uint16_t i;             /* index of transmitting message */

			/* first buffer */
			CO_CANtx_t *buffer = &CANmodule->txArray[0];
			/* search through whole array of pointers to transmit message buffers. */
			for(i = CANmodule->txSize; i > 0U; i--)
			{
				/* if message buffer is full, send it. */
				if(buffer->bufferFull)
				{

					/* Copy message to CAN buffer */
					CANmodule->bufferInhibitFlag = buffer->syncFlag;

					uint32_t TxMailboxNum;

					prepareTxHeader(&TxHeader, buffer);
					if( HAL_CAN_AddTxMessage(CANmodule->CANBaseDescriptor,
							&TxHeader,
							&buffer->data[0],
							&TxMailboxNum) != HAL_OK)
					{
						;//do nothing
					}
					else
					{
						buffer->bufferFull = false;
						CANmodule->CANtxCount--;
					}

					break;                      /* exit for loop */
				}
				else
				{
					/*do nothing*/;
				}
				buffer++;
			}/* end of for loop */

			/* Clear counter if no more messages */
			if(i == 0U)
			{
				CANmodule->CANtxCount = 0U;
			}
			else
			{
				/*do nothing*/;
			}
		}
	}
}
