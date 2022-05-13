/*
 * sdram_ex.c
 *
 *  Created on: May 13, 2022
 *      Author: pujak
 */

#include "sdram_ex.h"

/* Private constants
 * --------------------------------------------*/
#define SDRAM_MODEREG_BURST_LENGTH_1                ((uint16_t)0x0000)
#define SDRAM_MODEREG_BURST_LENGTH_2                ((uint16_t)0x0001)
#define SDRAM_MODEREG_BURST_LENGTH_4                ((uint16_t)0x0002)
#define SDRAM_MODEREG_BURST_LENGTH_8                ((uint16_t)0x0004)
#define SDRAM_MODEREG_BURST_TYPE_SEQUENTIAL         ((uint16_t)0x0000)
#define SDRAM_MODEREG_BURST_TYPE_INTERLEAVED        ((uint16_t)0x0008)
#define SDRAM_MODEREG_CAS_LATENCY_2                 ((uint16_t)0x0020)
#define SDRAM_MODEREG_CAS_LATENCY_3                 ((uint16_t)0x0030)
#define SDRAM_MODEREG_OPERATING_MODE_STANDARD       ((uint16_t)0x0000)
#define SDRAM_MODEREG_WRITEBURST_MODE_PROGRAMMED    ((uint16_t)0x0000)
#define SDRAM_MODEREG_WRITEBURST_MODE_SINGLE        ((uint16_t)0x0200)

#define REFRESH_COUNT                               ((uint32_t)1386)
#define SDRAM_TIMEOUT                               ((uint32_t)0xFFFF)

/* Private function declarations
 * --------------------------------------------*/
void SDRAM_InitEx(SDRAM_HandleTypeDef *hsdram)
{
  FMC_SDRAM_CommandTypeDef cmd;
  __IO uint32_t tmpmrd = 0;
  uint32_t target;

  /* Decide the target bank */
  if (hsdram->Init.SDBank == FMC_SDRAM_BANK1)
    target = FMC_SDRAM_CMD_TARGET_BANK1;
  else if (hsdram->Init.SDBank == FMC_SDRAM_BANK2)
    target = FMC_SDRAM_CMD_TARGET_BANK2;
  else
    target = FMC_SDRAM_CMD_TARGET_BANK1_2;

  /* Step 1: Configure a clock configuration enable command */
  cmd.CommandMode = FMC_SDRAM_CMD_CLK_ENABLE;
  cmd.CommandTarget = target;
  cmd.AutoRefreshNumber = 1;
  cmd.ModeRegisterDefinition = 0;

  /* Send the command */
  HAL_SDRAM_SendCommand(hsdram, &cmd, SDRAM_TIMEOUT);

  /* Step 2: Insert 100 us minimum delay */
  /* Inserted delay is equal to 1 ms due to systick time base unit (ms) */
  HAL_Delay(1);

  /* Step 3: Configure a PALL (pre-charge all) command */
  cmd.CommandMode = FMC_SDRAM_CMD_PALL;
  cmd.CommandTarget = target;
  cmd.AutoRefreshNumber = 1;
  cmd.ModeRegisterDefinition = 0;

  /* Send the command */
  HAL_SDRAM_SendCommand(hsdram, &cmd, SDRAM_TIMEOUT);

  /* Step 4: Configure an Auto Refresh command */
  cmd.CommandMode = FMC_SDRAM_CMD_AUTOREFRESH_MODE;
  cmd.CommandTarget = target;
  cmd.AutoRefreshNumber = 8;
  cmd.ModeRegisterDefinition = 0;

  /* Send the command */
  HAL_SDRAM_SendCommand(hsdram, &cmd, SDRAM_TIMEOUT);

  /* Step 5: Program the external memory mode register */
  tmpmrd = (uint32_t) SDRAM_MODEREG_BURST_LENGTH_1 |
  SDRAM_MODEREG_BURST_TYPE_SEQUENTIAL | SDRAM_MODEREG_CAS_LATENCY_3 |
  SDRAM_MODEREG_OPERATING_MODE_STANDARD |
  SDRAM_MODEREG_WRITEBURST_MODE_SINGLE;

  cmd.CommandMode = FMC_SDRAM_CMD_LOAD_MODE;
  cmd.CommandTarget = target;
  cmd.AutoRefreshNumber = 1;
  cmd.ModeRegisterDefinition = tmpmrd;

  /* Send the command */
  HAL_SDRAM_SendCommand(hsdram, &cmd, SDRAM_TIMEOUT);

  /* Step 6: Set the refresh rate counter */
  /* Set the device refresh rate */
  HAL_SDRAM_ProgramRefreshRate(hsdram, REFRESH_COUNT);
}

