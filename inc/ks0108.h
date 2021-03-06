/*
		128x64 KS0108 LCD Driver for STM32F4
*/

#include <stm32f4xx.h>

/* LCD Connection Descriptions */
#define KS0108_RST_PIN	3
#define KS0108_RST_PORT	GPIOA_BASE

#define KS0108_RW_PIN	8
#define KS0108_RW_PORT	GPIOA_BASE

#define KS0108_DI_PIN	15
#define KS0108_DI_PORT	GPIOA_BASE

#define KS0108_EN_PIN	0
#define KS0108_EN_PORT	GPIOB_BASE

#define KS0108_CS1_PIN	4
#define KS0108_CS1_PORT	GPIOB_BASE

#define KS0108_CS2_PIN	5
#define KS0108_CS2_PORT	GPIOB_BASE

#define KS0108_DT_PIN	4
#define KS0108_DT_PORT	GPIOE_BASE

/* LCD Definitions */
#define KS0108_Y_ADRESS		0xB8	/* Swapped with X address for real coordinate system */
#define KS0108_X_ADRESS		0x40	/* Swapped with Y address for real coordinate system */
#define KS0108_START_LINE	0xC0
#define KS0108_DISPLAY_ON	0x3F
#define KS0108_DISPLAY_OFF	0x3E

/* Port Definitions */
#define KS0108_PIN_INPUT	0x00
#define KS0108_PIN_OUTPUT	0x01
#define KS0108_PORT_INPUT	0x00000000
#define KS0108_PORT_OUTPUT	0x00005555

#define KS0108_RST	(*(volatile unsigned short*)( 0x42000000U | ((KS0108_RST_PORT+0x14) << 5) | ((KS0108_RST_PIN << 2))))	/* ODR */
#define KS0108_RW	(*(volatile unsigned short*)( 0x42000000U | ((KS0108_RW_PORT+0x14) << 5) | ((KS0108_RW_PIN << 2))))		/* ODR */
#define KS0108_DI	(*(volatile unsigned short*)( 0x42000000U | ((KS0108_DI_PORT+0x14) << 5) | ((KS0108_DI_PIN << 2))))		/* ODR */
#define KS0108_EN	(*(volatile unsigned short*)( 0x42000000U | ((KS0108_EN_PORT+0x14) << 5) | ((KS0108_EN_PIN << 2))))		/* ODR */
#define KS0108_CS1	(*(volatile unsigned short*)( 0x42000000U | ((KS0108_CS1_PORT+0x14) << 5) | ((KS0108_CS1_PIN << 2))))	/* ODR */
#define KS0108_CS2	(*(volatile unsigned short*)( 0x42000000U | ((KS0108_CS2_PORT+0x14) << 5) | ((KS0108_CS2_PIN << 2))))	/* ODR */

#define KS0108_RST_DIR(x)	((*(volatile unsigned int*)KS0108_RST_PORT)=((*(volatile unsigned int*)KS0108_RST_PORT)&~(0x03U<<(KS0108_RST_PIN*2)))|(x<<(KS0108_RST_PIN*2)))	/* MODER */
#define KS0108_RW_DIR(x)	((*(volatile unsigned int*)KS0108_RW_PORT)=((*(volatile unsigned int*)KS0108_RW_PORT)&~(0x03U<<(KS0108_RW_PIN*2)))|(x<<(KS0108_RW_PIN*2)))		/* MODER */
#define KS0108_DI_DIR(x)	((*(volatile unsigned int*)KS0108_DI_PORT)=((*(volatile unsigned int*)KS0108_DI_PORT)&~(0x03U<<(KS0108_DI_PIN*2)))|(x<<(KS0108_DI_PIN*2)))		/* MODER */
#define KS0108_EN_DIR(x)	((*(volatile unsigned int*)KS0108_EN_PORT)=((*(volatile unsigned int*)KS0108_EN_PORT)&~(0x03U<<(KS0108_EN_PIN*2)))|(x<<(KS0108_EN_PIN*2)))		/* MODER */
#define KS0108_CS1_DIR(x)	((*(volatile unsigned int*)KS0108_CS1_PORT)=((*(volatile unsigned int*)KS0108_CS1_PORT)&~(0x03U<<(KS0108_CS1_PIN*2)))|(x<<(KS0108_CS1_PIN*2)))	/* MODER */
#define KS0108_CS2_DIR(x)	((*(volatile unsigned int*)KS0108_CS2_PORT)=((*(volatile unsigned int*)KS0108_CS2_PORT)&~(0x03U<<(KS0108_CS2_PIN*2)))|(x<<(KS0108_CS2_PIN*2)))	/* MODER */
#define KS0108_PORT_DIR(x)	((*(volatile unsigned int*)KS0108_DT_PORT)= \
							((*(volatile unsigned int*)KS0108_DT_PORT)&~(0x0000FFFFU<<(KS0108_DT_PIN*2)))|(x<<(KS0108_DT_PIN*2)))	/* MODER */
#define KS0108_L()	(KS0108_CS1=0, KS0108_CS2=1)
#define KS0108_R()	(KS0108_CS1=1, KS0108_CS2=0)
#define KS0108_N()	(KS0108_CS1=0, KS0108_CS2=0)
#define KS0108_DATA	((unsigned char)(((*((volatile unsigned short*)KS0108_DT_PORT+0x10)&(0x00FFU<<KS0108_DT_PIN)))>>KS0108_DT_PIN))	/* IDR */
#define KS0108_LDATA(x)	((*(volatile unsigned short*)(KS0108_DT_PORT+0x14)) = \
						((*(volatile unsigned short*)(KS0108_DT_PORT+0x14))&~(0x00FFU<<KS0108_DT_PIN))|(x<<KS0108_DT_PIN))
							