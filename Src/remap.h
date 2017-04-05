#ifndef __REMAP_H__
#define __REMAP_H__
#include "include.h"




#define AFIO_OFFSET                 (AFIO_BASE - PERIPH_BASE)
#define EVCR_OFFSET                 (AFIO_OFFSET + 0x00)
#define EVOE_BitNumber              ((uint8_t)0x07)
#define EVCR_EVOE_BB                (PERIPH_BB_BASE + (EVCR_OFFSET * 32) + (EVOE_BitNumber * 4))
#define EVCR_PORTPINCONFIG_MASK     ((uint16_t)0xFF80)
#define LSB_MASK                    ((uint16_t)0xFFFF)
#define DBGAFR_POSITION_MASK        ((uint32_t)0x000F0000)
#define DBGAFR_SWJCFG_MASK          ((uint32_t)0xF0FFFFFF)
#define DBGAFR_LOCATION_MASK        ((uint32_t)0x00200000)
#define DBGAFR_NUMBITS_MASK         ((uint32_t)0x00100000)

#define GPIO_Remap_SPI1            ((uint32_t)0x00000001)  /* SPI1 Alternate Function mapping */
#define GPIO_Remap_I2C1            ((uint32_t)0x00000002)  /* I2C1 Alternate Function mapping */
#define GPIO_Remap_USART1          ((uint32_t)0x00000004)  /* USART1 Alternate Function mapping */
#define GPIO_Remap_USART2          ((uint32_t)0x00000008)  /* USART2 Alternate Function mapping */
#define GPIO_PartialRemap_USART3   ((uint32_t)0x00140010)  /* USART3 Partial Alternate Function mapping */
#define GPIO_FullRemap_USART3      ((uint32_t)0x00140030)  /* USART3 Full Alternate Function mapping */
#define GPIO_PartialRemap_TIM1     ((uint32_t)0x00160040)  /* TIM1 Partial Alternate Function mapping */
#define GPIO_FullRemap_TIM1        ((uint32_t)0x001600C0)  /* TIM1 Full Alternate Function mapping */
#define GPIO_PartialRemap1_TIM2    ((uint32_t)0x00180100)  /* TIM2 Partial1 Alternate Function mapping */
#define GPIO_PartialRemap2_TIM2    ((uint32_t)0x00180200)  /* TIM2 Partial2 Alternate Function mapping */
#define GPIO_FullRemap_TIM2        ((uint32_t)0x00180300)  /* TIM2 Full Alternate Function mapping */
#define GPIO_PartialRemap_TIM3     ((uint32_t)0x001A0800)  /* TIM3 Partial Alternate Function mapping */
#define GPIO_FullRemap_TIM3        ((uint32_t)0x001A0C00)  /* TIM3 Full Alternate Function mapping */
#define GPIO_Remap_TIM4            ((uint32_t)0x00001000)  /* TIM4 Alternate Function mapping */
#define GPIO_Remap1_CAN1           ((uint32_t)0x001D4000)  /* CAN Alternate Function mapping */
#define GPIO_Remap2_CAN1           ((uint32_t)0x001D6000)  /* CAN Alternate Function mapping */
#define GPIO_Remap_PD01            ((uint32_t)0x00008000)  /* PD01 Alternate Function mapping */
#define GPIO_Remap_TIM5CH4_LSI     ((uint32_t)0x00200001)  /* LSI connected to TIM5 Channel4 input capture for calibration */
#define GPIO_Remap_ADC1_ETRGINJ    ((uint32_t)0x00200002)  /* ADC1 External Trigger Injected Conversion remapping */
#define GPIO_Remap_ADC1_ETRGREG    ((uint32_t)0x00200004)  /* ADC1 External Trigger Regular Conversion remapping */
#define GPIO_Remap_ADC2_ETRGINJ    ((uint32_t)0x00200008)  /* ADC2 External Trigger Injected Conversion remapping */
#define GPIO_Remap_ADC2_ETRGREG    ((uint32_t)0x00200010)  /* ADC2 External Trigger Regular Conversion remapping */
#define GPIO_Remap_SWJ_NoJTRST     ((uint32_t)0x00300100)  /* Full SWJ Enabled (JTAG-DP + SW-DP) but without JTRST */
#define GPIO_Remap_SWJ_JTAGDisable ((uint32_t)0x00300200)  /* JTAG-DP Disabled and SW-DP Enabled */
#define GPIO_Remap_SWJ_Disable     ((uint32_t)0x00300400)  /* Full SWJ Disabled (JTAG-DP + SW-DP) */


void GPIO_PinRemapConfig(uint32_t GPIO_Remap, FunctionalState NewState);


#endif

