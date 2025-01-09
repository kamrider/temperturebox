#include "bsp_usart.h"

// ���ڽ��ջ�����
uint8_t USART1_RX_BUF[USART1_MAX_RECV_LEN];
uint8_t USART2_RX_BUF[USART2_MAX_RECV_LEN];
uint16_t USART1_RX_STA = 0;
uint16_t USART2_RX_STA = 0;

void USART1_Config(void)
{
	GPIO_InitTypeDef GPIO_InitStructure;
	USART_InitTypeDef USART_InitStructure;
	NVIC_InitTypeDef NVIC_InitStructure;

	// �򿪴���GPIO��ʱ��
	DEBUG_USART_GPIO_APBxClkCmd(DEBUG_USART_GPIO_CLK, ENABLE);
	
	// �򿪴��������ʱ��
	DEBUG_USART_APBxClkCmd(DEBUG_USART_CLK, ENABLE);

	// ��USART Tx��GPIO����Ϊ���츴��ģʽ
	GPIO_InitStructure.GPIO_Pin = DEBUG_USART_TX_GPIO_PIN;
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_AF_PP;
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
	GPIO_Init(DEBUG_USART_TX_GPIO_PORT, &GPIO_InitStructure);

  // ��USART Rx��GPIO����Ϊ��������ģʽ
	GPIO_InitStructure.GPIO_Pin = DEBUG_USART_RX_GPIO_PIN;
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_IN_FLOATING;
	GPIO_Init(DEBUG_USART_RX_GPIO_PORT, &GPIO_InitStructure);
	
	// ���ô��ڵĹ�������
	// ���ò�����
	USART_InitStructure.USART_BaudRate = DEBUG_USART_BAUDRATE;
	// ���� �������ֳ�
	USART_InitStructure.USART_WordLength = USART_WordLength_8b;
	// ����ֹͣλ
	USART_InitStructure.USART_StopBits = USART_StopBits_1;
	// ����У��λ
	USART_InitStructure.USART_Parity = USART_Parity_No ;
	// ����Ӳ��������
	USART_InitStructure.USART_HardwareFlowControl = USART_HardwareFlowControl_None;
	// ���ù���ģʽ���շ�һ��
	USART_InitStructure.USART_Mode = USART_Mode_Rx | USART_Mode_Tx;
	// ��ɴ��ڵĳ�ʼ������
	USART_Init(DEBUG_USARTx, &USART_InitStructure);

	// �����ж�
	NVIC_InitStructure.NVIC_IRQChannel = USART1_IRQn;
	NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority = 3;
	NVIC_InitStructure.NVIC_IRQChannelSubPriority = 3;
	NVIC_InitStructure.NVIC_IRQChannelCmd = ENABLE;
	NVIC_Init(&NVIC_InitStructure);

	// ʹ�ܴ��ڽ����ж�
	USART_ITConfig(DEBUG_USARTx, USART_IT_RXNE, ENABLE);
	// ʹ�ܴ���
	USART_Cmd(DEBUG_USARTx, ENABLE);	    
}

void USART2_Config(void)
{
	GPIO_InitTypeDef GPIO_InitStructure;
	USART_InitTypeDef USART_InitStructure;
	NVIC_InitTypeDef NVIC_InitStructure;

	// �򿪴���GPIO��ʱ��
	DATA_USART_GPIO_APBxClkCmd(DATA_USART_GPIO_CLK, ENABLE);
	// �򿪴��������ʱ��
	DATA_USART_APBxClkCmd(DATA_USART_CLK, ENABLE);

	// ��USART Tx��GPIO����Ϊ���츴��ģʽ
	GPIO_InitStructure.GPIO_Pin = DATA_USART_TX_GPIO_PIN;
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_AF_PP;
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
	GPIO_Init(DATA_USART_TX_GPIO_PORT, &GPIO_InitStructure);

	// ��USART Rx��GPIO����Ϊ��������ģʽ
	GPIO_InitStructure.GPIO_Pin = DATA_USART_RX_GPIO_PIN;
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_IN_FLOATING;
	GPIO_Init(DATA_USART_RX_GPIO_PORT, &GPIO_InitStructure);

	USART_InitStructure.USART_BaudRate = DATA_USART_BAUDRATE;
	USART_InitStructure.USART_WordLength = USART_WordLength_8b;
	USART_InitStructure.USART_StopBits = USART_StopBits_1;
	USART_InitStructure.USART_Parity = USART_Parity_No;
	USART_InitStructure.USART_HardwareFlowControl = USART_HardwareFlowControl_None;
	USART_InitStructure.USART_Mode = USART_Mode_Rx | USART_Mode_Tx;
	USART_Init(DATA_USARTx, &USART_InitStructure);

	NVIC_InitStructure.NVIC_IRQChannel = USART2_IRQn;
	NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority = 3;
	NVIC_InitStructure.NVIC_IRQChannelSubPriority = 2;
	NVIC_InitStructure.NVIC_IRQChannelCmd = ENABLE;
	NVIC_Init(&NVIC_InitStructure);

	USART_ITConfig(DATA_USARTx, USART_IT_RXNE, ENABLE);
	USART_Cmd(DATA_USARTx, ENABLE);
}

void USART1_SendByte(uint8_t ch)
{
	USART_SendData(DEBUG_USARTx, ch);
	while(USART_GetFlagStatus(DEBUG_USARTx, USART_FLAG_TXE) == RESET);
}

void USART2_SendByte(uint8_t ch)
{
	USART_SendData(DATA_USARTx, ch);
	while(USART_GetFlagStatus(DATA_USARTx, USART_FLAG_TXE) == RESET);
}

void USART1_SendString(char *str)
{
	while(*str)
	{
		USART1_SendByte(*str++);
	}
}

void USART2_SendString(char *str)
{
	while(*str)
	{
		USART2_SendByte(*str++);
	}
}

// �ض���printf������1
int fputc(int ch, FILE *f)
{
	USART1_SendByte((uint8_t)ch);
	return ch;
}

// ����1�жϷ�����
void USART1_IRQHandler(void)
{
	uint8_t temp;
	
	if(USART_GetITStatus(DEBUG_USARTx, USART_IT_RXNE) != RESET)
	{
		temp = USART_ReceiveData(DEBUG_USARTx);
		if(USART1_RX_STA < USART1_MAX_RECV_LEN)
		{
			USART1_RX_BUF[USART1_RX_STA++] = temp;
		}
	}
}

// ����2�жϷ�����
void USART2_IRQHandler(void)
{
	uint8_t temp;
	
	if(USART_GetITStatus(DATA_USARTx, USART_IT_RXNE) != RESET)
	{
		temp = USART_ReceiveData(DATA_USARTx);
		if(USART2_RX_STA < USART2_MAX_RECV_LEN)
		{
			USART2_RX_BUF[USART2_RX_STA++] = temp;
		}
	}
}

void USART2_CLR_Buf(void)
{
	memset(USART2_RX_BUF, 0, USART2_MAX_RECV_LEN);
	USART2_RX_STA = 0;
}

void USART_Config(void)
{
	GPIO_InitTypeDef GPIO_InitStructure;
	USART_InitTypeDef USART_InitStructure;
	
	// ʹ��USART1ʱ��
	DEBUG_USART_APBxClkCmd(DEBUG_USART_CLK, ENABLE);
	
	// ʹ��USART1 GPIOʱ��
	RCC_APB2PeriphClockCmd(DEBUG_USART_GPIO_CLK, ENABLE);
	
	// ����USART1 TX����
	GPIO_InitStructure.GPIO_Pin = DEBUG_USART_TX_GPIO_PIN;
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_AF_PP;
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
	GPIO_Init(DEBUG_USART_TX_GPIO_PORT, &GPIO_InitStructure);
	
	// ����USART1 RX����
	GPIO_InitStructure.GPIO_Pin = DEBUG_USART_RX_GPIO_PIN;
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_IN_FLOATING;
	GPIO_Init(DEBUG_USART_RX_GPIO_PORT, &GPIO_InitStructure);
	
	// ����USART1����
	USART_InitStructure.USART_BaudRate = DEBUG_USART_BAUDRATE;
	USART_InitStructure.USART_WordLength = USART_WordLength_8b;
	USART_InitStructure.USART_StopBits = USART_StopBits_1;
	USART_InitStructure.USART_Parity = USART_Parity_No;
	USART_InitStructure.USART_HardwareFlowControl = USART_HardwareFlowControl_None;
	USART_InitStructure.USART_Mode = USART_Mode_Rx | USART_Mode_Tx;
	USART_Init(DEBUG_USARTx, &USART_InitStructure);
	
	USART_Cmd(DEBUG_USARTx, ENABLE);
}

void USART2_Init(uint32_t baudrate)
{
	GPIO_InitTypeDef GPIO_InitStructure;
	USART_InitTypeDef USART_InitStructure;
	NVIC_InitTypeDef NVIC_InitStructure;
	
	// ʹ��USART2ʱ��
	RCC_APB1PeriphClockCmd(RCC_APB1Periph_USART2, ENABLE);
	
	// ʹ��USART2 GPIOʱ��
	RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOA, ENABLE);
	
	// ����USART2 TX���� (PA2)
	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_2;
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_AF_PP;
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
	GPIO_Init(GPIOA, &GPIO_InitStructure);
	
	// ����USART2 RX���� (PA3)
	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_3;
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_IN_FLOATING;
	GPIO_Init(GPIOA, &GPIO_InitStructure);
	
	// ����USART2�ж�
	NVIC_InitStructure.NVIC_IRQChannel = USART2_IRQn;
	NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority = 3;
	NVIC_InitStructure.NVIC_IRQChannelSubPriority = 2;
	NVIC_InitStructure.NVIC_IRQChannelCmd = ENABLE;
	NVIC_Init(&NVIC_InitStructure);
	
	// ����USART2����
	USART_InitStructure.USART_BaudRate = baudrate;
	USART_InitStructure.USART_WordLength = USART_WordLength_8b;
	USART_InitStructure.USART_StopBits = USART_StopBits_1;
	USART_InitStructure.USART_Parity = USART_Parity_No;
	USART_InitStructure.USART_HardwareFlowControl = USART_HardwareFlowControl_None;
	USART_InitStructure.USART_Mode = USART_Mode_Rx | USART_Mode_Tx;
	USART_Init(USART2, &USART_InitStructure);
	
	// ʹ��USART2�����ж�
	USART_ITConfig(USART2, USART_IT_RXNE, ENABLE);
	
	// ʹ��USART2
	USART_Cmd(USART2, ENABLE);
	
	// ��ս��ջ�����
	USART2_CLR_Buf();
}
