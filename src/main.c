#include "stm32f0xx.h"

#define TIMx TIM1
#define TIMx_BASE TIM1_BASE

// prototypes
void ConfigureLEDGPIO(void);
void ConfigureTIM(void);
void Delay(uint32_t);

int main (void) {
    
    ConfigureLEDGPIO();
    ConfigureTIM();
    GPIOC->BSRR |= 1<<9; // turn on green led
    // Delay(10);
    GPIOC->BSRR = 0;
    // Delay(10);


    while (1) {
        __WFI();
    }

    // Return 0 to satisfy compiler
    return 0;
}

__INLINE void ConfigureLEDGPIO(void)
{
  RCC->AHBENR |= RCC_AHBENR_GPIOCEN; // enable GPIOC peripheral clock
  GPIOC->MODER = (GPIOC->MODER & ~(GPIO_MODER_MODER9)) | GPIO_MODER_MODER9_0; // set pin to general IO mode
}

__INLINE void ConfigureTIM(void)
{
  RCC->APB2ENR |= RCC_APB2ENR_TIM1EN; // enable peripheral clock of Timer X
  RCC->AHBENR |= RCC_AHBENR_GPIOAEN; // enable peripheral clock of GPIOA
  
  // PA8 - CH1
  GPIOA->MODER |= (GPIOA->MODER & ~(GPIO_MODER_MODER8)) | (GPIO_MODER_MODER8_1); // set pin to alternate function
  GPIOA->AFR[1] |= (0x02 << GPIO_AFRH_AFSEL8_Pos); // select AF2 in AFRH
  TIMx->CCMR1 |= TIM_CCMR1_OC1M_1 | TIM_CCMR1_OC1M_2 |\
                 TIM_CCMR1_OC1PE; // pwm mode 1 on OC1, enable preload
  TIMx->CCER |= TIM_CCER_CC1E; // active high polarity on OC1
  
  // PA9 - CH2
  GPIOA->MODER |= (GPIOA->MODER & ~(GPIO_MODER_MODER9)) | (GPIO_MODER_MODER9_1); // set pin to alternate function  
  GPIOA->AFR[1] |= (0x02 << GPIO_AFRH_AFSEL9_Pos); // select AF2 in AFRH
  TIMx->CCMR1 |= TIM_CCMR1_OC2M_1 | TIM_CCMR1_OC2M_2 |\
                 TIM_CCMR1_OC2PE; // pwm mode 1 on OC2, enable preload
  TIMx->CCER |= TIM_CCER_CC2E; // active high polarity on OC2

  // Both
  TIMx->PSC = 47; // prescaler - APBCLK/48 -> 1 MHz
  TIMx->ARR = 8; // timer clk is 1 MHz -> period = (8+1)/1 MHz
  TIMx->CCR1 = 4; // signal high during 4 us
  TIMx->CCR2 = 5; // signal high during 4 us
  TIMx->BDTR |= TIM_BDTR_MOE; // enable output
  TIMx->CR1 |= TIM_CR1_CEN; // enable counter, edge align mode, set as upcounter
  TIMx->EGR |= TIM_EGR_UG; // for update generation
}
  
void Delay(uint32_t time_ms)
{
  for (uint32_t i=0; i < time_ms*1000; i++);
}