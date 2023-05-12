// ----------------------------------------------------------------------------
//
//
// ----------------------------------------------------------------------------
#include <stdio.h>
#include <string.h>
#include "bsp/bsp.h"
#include "sysinit/sysinit.h"
#include "os/os.h"
#include "ble.h"
#include "xprintf.h"

// ----------------------------------------------------------------------------
#include <nrfx.h>
#include <nrf_gpio.h>
#include <nrf_uart.h>
#include <nrfx_uart.h>

// ----------------------------------------------------------------------------
static void hardware_init();
static nrfx_uart_t dbg_uart_inst = NRFX_UART_INSTANCE(0);

// ----------------------------------------------------------------------------
#define BLINK_TASK_PRIO (10)
#define BLINK_STACK_SIZE (64)

// ----------------------------------------------------------------------------
struct os_task blink_task;
os_stack_t blink_task_stack[BLINK_STACK_SIZE];

// ----------------------------------------------------------------------------
uint32_t readCounter1ms()
{
  return os_get_uptime_usec() / 1000;
}

// ----------------------------------------------------------------------------
uint32_t calculateDeltaCounter1ms(uint32_t ref)
{
  const uint32_t now = readCounter1ms();
  if(ref > now)
  {
    return 0;
  }

  return (now - ref);
}

// ----------------------------------------------------------------------------
void blink_task_func(void* arg)
{
  // ...
  nrf_gpio_cfg_output(26);

  // ...
  while(1)
  {
    // ...
    os_time_delay(os_time_ms_to_ticks32(500));

    // ...
    nrf_gpio_pin_toggle(26);
  }
}

// ----------------------------------------------------------------------------
int main(int argc, char const *argv[])
{
  // ...
  hardware_init();

  // ...
  while(1)
  {
    os_eventq_run(os_eventq_dflt_get());
  }

  // ...
  return 0;
}

// ----------------------------------------------------------------------------
void dbg_sendchar(uint8_t ch)
{
  nrfx_uart_tx(&dbg_uart_inst, &ch, 1);
  while(nrfx_uart_tx_in_progress(&dbg_uart_inst) != 0);
}

// ----------------------------------------------------------------------------
static void dbg_init()
{
  // ...
  nrfx_uart_config_t p_config = NRFX_UART_DEFAULT_CONFIG(
    NRF_GPIO_PIN_MAP(1, 12), // RX pin
    NRF_GPIO_PIN_MAP(1, 11)  // TX pin
  );

  // ...
  nrfx_uart_init(&dbg_uart_inst, &p_config, NULL);
  xdev_out(dbg_sendchar);
}

// ----------------------------------------------------------------------------
static void hardware_init()
{
  // ...
  sysinit();

  // ...
  dbg_init();

  // ...
  os_task_init(
    &blink_task, "blink_task", blink_task_func, NULL, BLINK_TASK_PRIO,
    OS_WAIT_FOREVER, blink_task_stack, BLINK_STACK_SIZE
  );

  // ...
  xprintf("\r\n");
  xprintf("[%s, %s]\r\n", __DATE__, __TIME__);
  xprintf("System boot OK!\r\n");

  // ...
  ble_init();
}
