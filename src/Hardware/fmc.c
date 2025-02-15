/**
   @copyright None
   @file      fmc.c
   @author    Comment Vivre
   @date      2023-12-29
   @brief     None
*/

#include <main.h>

NAND_HandleTypeDef hnand1;
SDRAM_HandleTypeDef hsdram1;
FMC_SDRAM_CommandTypeDef command;

/**
 * @function     MX_FMC_Init
 * @brief        None
 * @param[in]    None
 * @return       None
 * @date         2024-01-06
*/
void MX_FMC_Init(void)
{
  FMC_NAND_PCC_TimingTypeDef ComSpaceTiming = {0};
  FMC_NAND_PCC_TimingTypeDef AttSpaceTiming = {0};
  FMC_SDRAM_TimingTypeDef SdramTiming = {0};
  /** Perform the NAND1 memory initialization sequence
  */
  hnand1.Instance = FMC_NAND_DEVICE;
  /* hnand1.Init */
  hnand1.Init.NandBank = FMC_NAND_BANK3;
  hnand1.Init.Waitfeature = FMC_NAND_PCC_WAIT_FEATURE_ENABLE;
  hnand1.Init.MemoryDataWidth = FMC_NAND_MEM_BUS_WIDTH_8;
  hnand1.Init.EccComputation = FMC_NAND_ECC_DISABLE;
  hnand1.Init.ECCPageSize = FMC_NAND_ECC_PAGE_SIZE_512BYTE;
  hnand1.Init.TCLRSetupTime = 10;
  hnand1.Init.TARSetupTime = 10;
  /* hnand1.Config */
  hnand1.Config.PageSize = 0;
  hnand1.Config.SpareAreaSize = 0;
  hnand1.Config.BlockSize = 0;
  hnand1.Config.BlockNbr = 0;
  hnand1.Config.PlaneNbr = 0;
  hnand1.Config.PlaneSize = 0;
  hnand1.Config.ExtraCommandEnable = DISABLE;
  /* ComSpaceTiming */
  ComSpaceTiming.SetupTime = 10;
  ComSpaceTiming.WaitSetupTime = 10;
  ComSpaceTiming.HoldSetupTime = 10;
  ComSpaceTiming.HiZSetupTime = 10;
  /* AttSpaceTiming */
  AttSpaceTiming.SetupTime = 10;
  AttSpaceTiming.WaitSetupTime = 10;
  AttSpaceTiming.HoldSetupTime = 10;
  AttSpaceTiming.HiZSetupTime = 10;
  
  if (HAL_NAND_Init(&hnand1, &ComSpaceTiming, &AttSpaceTiming) != HAL_OK)
  {
    Error_Handler( );
  }
  
  /** Perform the SDRAM1 memory initialization sequence
  */
  hsdram1.Instance = FMC_SDRAM_DEVICE;
  /* hsdram1.Init */
  hsdram1.Init.SDBank = FMC_SDRAM_BANK1;
  hsdram1.Init.ColumnBitsNumber = FMC_SDRAM_COLUMN_BITS_NUM_9;
  hsdram1.Init.RowBitsNumber = FMC_SDRAM_ROW_BITS_NUM_13;
  hsdram1.Init.MemoryDataWidth = FMC_SDRAM_MEM_BUS_WIDTH_32;
  hsdram1.Init.InternalBankNumber = FMC_SDRAM_INTERN_BANKS_NUM_4;
  hsdram1.Init.CASLatency = FMC_SDRAM_CAS_LATENCY_2;
  hsdram1.Init.WriteProtection = FMC_SDRAM_WRITE_PROTECTION_DISABLE;
  hsdram1.Init.SDClockPeriod = FMC_SDRAM_CLOCK_PERIOD_2;
  hsdram1.Init.ReadBurst = FMC_SDRAM_RBURST_ENABLE;
  hsdram1.Init.ReadPipeDelay = FMC_SDRAM_RPIPE_DELAY_0;
  /* SdramTiming */
  SdramTiming.LoadToActiveDelay = 2;
  SdramTiming.ExitSelfRefreshDelay = 8;
  SdramTiming.SelfRefreshTime = 6;
  SdramTiming.RowCycleDelay = 6;
  SdramTiming.WriteRecoveryTime = 4;
  SdramTiming.RPDelay = 2;
  SdramTiming.RCDDelay = 2;
  
  if (HAL_SDRAM_Init(&hsdram1, &SdramTiming) != HAL_OK)
  {
    Error_Handler( );
  }
  
  SDRAM_Initialization_Sequence(&hsdram1, &command);
}


/**
   @function     HAL_FMC_MspInit
   @brief        None
   @param[in]    None
   @return       None
   @date         2023-12-29
*/

static uint8_t FMC_Initialized = 0;
void HAL_FMC_MspInit(void)
{
  GPIO_InitTypeDef GPIO_InitStruct = {0};
  
  if (FMC_Initialized)
  { return; }
  
  FMC_Initialized = 1;
  LL_RCC_SetFMCClockSource(LL_RCC_FMC_CLKSOURCE_HCLK);
  /* Peripheral clock enable */
  __HAL_RCC_FMC_CLK_ENABLE();
  /** FMC GPIO Configuration
    PI6   ------> FMC_D28     PI5   ------> FMC_NBL3     PI4   ------> FMC_NBL2
    PG9   ------> FMC_NCE     PD5   ------> FMC_NWE      PD4   ------> FMC_NOE
    PI1   ------> FMC_D25     PI0   ------> FMC_D24      PI7   ------> FMC_D29
    PE1   ------> FMC_NBL1    PD6   ------> FMC_NWAIT    PI2   ------> FMC_D26
    PH15  ------> FMC_D23     PH14  ------> FMC_D22      PE0   ------> FMC_NBL0
    PI3   ------> FMC_D27     PG15  ------> FMC_SDNCAS   PD0   ------> FMC_D2
    PH13  ------> FMC_D21     PI9   ------> FMC_D30      PD1   ------> FMC_D3
    PI10  ------> FMC_D31     PG8   ------> FMC_SDCLK    PF2   ------> FMC_A2
    PF1   ------> FMC_A1      PF0   ------> FMC_A0       PG5   ------> FMC_BA1
    PF3   ------> FMC_A3      PG4   ------> FMC_BA0      PG2   ------> FMC_A12
    PF5   ------> FMC_A5      PF4   ------> FMC_A4       PC0   ------> FMC_SDNWE
    PC2   ------> FMC_SDNE0   PC3   ------> FMC_SDCKE0   PE10  ------> FMC_D7
    PF13  ------> FMC_A7      PF14  ------> FMC_A8       PE9   ------> FMC_D6
    PE11  ------> FMC_D8      PH10  ------> FMC_D18      PH11  ------> FMC_D19
    PD15  ------> FMC_D1      PD14  ------> FMC_D0       PF12  ------> FMC_A6
    PF15  ------> FMC_A9      PE12  ------> FMC_D9       PE15  ------> FMC_D12
    PH9   ------> FMC_D17     PH12  ------> FMC_D20      PD11  ------> FMC_CLE
    PD12  ------> FMC_ALE     PF11  ------> FMC_SDNRAS   PG0   ------> FMC_A10
    PE8   ------> FMC_D5      PE13  ------> FMC_D10      PH8   ------> FMC_D16
    PD10  ------> FMC_D15     PD9   ------> FMC_D14      PG1   ------> FMC_A11
    PE7   ------> FMC_D4      PE14  ------> FMC_D11      PD8   ------> FMC_D13
  */
  /* GPIO_InitStruct */
  GPIO_InitStruct.Pin = GPIO_PIN_6|GPIO_PIN_5|GPIO_PIN_4|GPIO_PIN_1
                          |GPIO_PIN_0|GPIO_PIN_7|GPIO_PIN_2|GPIO_PIN_3
                          |GPIO_PIN_9|GPIO_PIN_10;
  GPIO_InitStruct.Mode = GPIO_MODE_AF_PP;
  GPIO_InitStruct.Pull = GPIO_PULLUP;
  GPIO_InitStruct.Speed = GPIO_SPEED_FREQ_VERY_HIGH;
  GPIO_InitStruct.Alternate = GPIO_AF12_FMC;

  HAL_GPIO_Init(GPIOI, &GPIO_InitStruct);

  /* GPIO_InitStruct */
  GPIO_InitStruct.Pin = GPIO_PIN_9|GPIO_PIN_15|GPIO_PIN_8|GPIO_PIN_5
                          |GPIO_PIN_4|GPIO_PIN_2|GPIO_PIN_0|GPIO_PIN_1;
  GPIO_InitStruct.Mode = GPIO_MODE_AF_PP;
  GPIO_InitStruct.Pull = GPIO_PULLUP;
  GPIO_InitStruct.Speed = GPIO_SPEED_FREQ_VERY_HIGH;
  GPIO_InitStruct.Alternate = GPIO_AF12_FMC;

  HAL_GPIO_Init(GPIOG, &GPIO_InitStruct);

  /* GPIO_InitStruct */
  GPIO_InitStruct.Pin = GPIO_PIN_5|GPIO_PIN_4|GPIO_PIN_6|GPIO_PIN_0
                          |GPIO_PIN_1|GPIO_PIN_15|GPIO_PIN_14|GPIO_PIN_10
                          |GPIO_PIN_9|GPIO_PIN_8;
  GPIO_InitStruct.Mode = GPIO_MODE_AF_PP;
  GPIO_InitStruct.Pull = GPIO_PULLUP;
  GPIO_InitStruct.Speed = GPIO_SPEED_FREQ_VERY_HIGH;
  GPIO_InitStruct.Alternate = GPIO_AF12_FMC;

  HAL_GPIO_Init(GPIOD, &GPIO_InitStruct);

  /* GPIO_InitStruct */
  GPIO_InitStruct.Pin = GPIO_PIN_1|GPIO_PIN_0|GPIO_PIN_10|GPIO_PIN_9
                          |GPIO_PIN_11|GPIO_PIN_12|GPIO_PIN_15|GPIO_PIN_8
                          |GPIO_PIN_13|GPIO_PIN_7|GPIO_PIN_14;
  GPIO_InitStruct.Mode = GPIO_MODE_AF_PP;
  GPIO_InitStruct.Pull = GPIO_PULLUP;
  GPIO_InitStruct.Speed = GPIO_SPEED_FREQ_VERY_HIGH;
  GPIO_InitStruct.Alternate = GPIO_AF12_FMC;

  HAL_GPIO_Init(GPIOE, &GPIO_InitStruct);

  /* GPIO_InitStruct */
  GPIO_InitStruct.Pin = GPIO_PIN_15|GPIO_PIN_14|GPIO_PIN_13|GPIO_PIN_10
                          |GPIO_PIN_11|GPIO_PIN_9|GPIO_PIN_12|GPIO_PIN_8;
  GPIO_InitStruct.Mode = GPIO_MODE_AF_PP;
  GPIO_InitStruct.Pull = GPIO_PULLUP;
  GPIO_InitStruct.Speed = GPIO_SPEED_FREQ_VERY_HIGH;
  GPIO_InitStruct.Alternate = GPIO_AF12_FMC;

  HAL_GPIO_Init(GPIOH, &GPIO_InitStruct);

  /* GPIO_InitStruct */
  GPIO_InitStruct.Pin = GPIO_PIN_2|GPIO_PIN_1|GPIO_PIN_0|GPIO_PIN_3
                          |GPIO_PIN_5|GPIO_PIN_4|GPIO_PIN_13|GPIO_PIN_14
                          |GPIO_PIN_12|GPIO_PIN_15|GPIO_PIN_11;
  GPIO_InitStruct.Mode = GPIO_MODE_AF_PP;
  GPIO_InitStruct.Pull = GPIO_PULLUP;
  GPIO_InitStruct.Speed = GPIO_SPEED_FREQ_VERY_HIGH;
  GPIO_InitStruct.Alternate = GPIO_AF12_FMC;

  HAL_GPIO_Init(GPIOF, &GPIO_InitStruct);

  /* GPIO_InitStruct */
  GPIO_InitStruct.Pin = GPIO_PIN_0|GPIO_PIN_2|GPIO_PIN_3;
  GPIO_InitStruct.Mode = GPIO_MODE_AF_PP;
  GPIO_InitStruct.Pull = GPIO_PULLUP;
  GPIO_InitStruct.Speed = GPIO_SPEED_FREQ_VERY_HIGH;
  GPIO_InitStruct.Alternate = GPIO_AF12_FMC;

  HAL_GPIO_Init(GPIOC, &GPIO_InitStruct);

  /* GPIO_InitStruct */
  GPIO_InitStruct.Pin = GPIO_PIN_11|GPIO_PIN_12;
  GPIO_InitStruct.Mode = GPIO_MODE_AF_PP;
  GPIO_InitStruct.Pull = GPIO_PULLUP;
  GPIO_InitStruct.Speed = GPIO_SPEED_FREQ_VERY_HIGH;

  HAL_GPIO_Init(GPIOD, &GPIO_InitStruct);
  /* Peripheral interrupt init */
  HAL_NVIC_SetPriority(FMC_IRQn, 0, 0);
  HAL_NVIC_EnableIRQ(FMC_IRQn);
}
void HAL_NAND_MspInit(NAND_HandleTypeDef * nandHandle)
{
  HAL_FMC_MspInit();
}
void HAL_SDRAM_MspInit(SDRAM_HandleTypeDef * sdramHandle)
{
  HAL_FMC_MspInit();
}
