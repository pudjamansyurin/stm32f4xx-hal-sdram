# STM32F4 HAL SDRAM Extended

## How to use the module

#### **`main.c`**

```c
#include "stm32f4xx-hal-sdram/sdram.h"

int main(void)
{
  /* Reset of all peripherals, Initializes the Flash interface and the Systick. */
  HAL_Init();

  /* Configure the system clock */
  SystemClock_Config();

  /* Initialize all configured peripherals */
  MX_GPIO_Init();
  MX_FMC_Init();

  /* Initialize SDRAM extended */
  SDRAM_InitEx(&hsdram1);

  /* Super loop */
  while(1) {
    
  }
}
```
