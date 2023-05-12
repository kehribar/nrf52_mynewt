// ----------------------------------------------------------------------------
//
//
// ----------------------------------------------------------------------------
#include "ble.h"
#include <stdio.h>
#include <string.h>
#include "bsp/bsp.h"
#include "sysinit/sysinit.h"
#include "os/os.h"
#include "log/log.h"
#include "host/ble_hs.h"
#include "services/gap/ble_svc_gap.h"
#include "xprintf.h"

// ----------------------------------------------------------------------------
static const char* device_name = "Custom Device Name";

// ----------------------------------------------------------------------------
static int adv_event(struct ble_gap_event* event, void* arg);

// ----------------------------------------------------------------------------
static void adv_start()
{
  struct ble_gap_adv_params adv_params;
  struct ble_hs_adv_fields fields;

  // Set adv parameters
  memset(&adv_params, 0, sizeof(adv_params));
  adv_params.conn_mode = BLE_GAP_CONN_MODE_NON;
  adv_params.disc_mode = BLE_GAP_DISC_MODE_GEN;

  // Fill the fields with advertising data - flags, tx power level, name
  memset(&fields, 0, sizeof(fields));
  fields.flags = BLE_HS_ADV_F_DISC_GEN;
  fields.tx_pwr_lvl_is_present = 1;
  fields.tx_pwr_lvl = BLE_HS_ADV_TX_PWR_LVL_AUTO;
  fields.name = (uint8_t *)device_name;
  fields.name_len = strlen(device_name);
  fields.name_is_complete = 1;
  ble_gap_adv_set_fields(&fields);

  // ...
  xprintf("%s: Starting advertising...\r\n", __func__);

  // As own address type we use hard-coded value, because we generate
  // NRPA and by definition it's random
  ble_gap_adv_start(
    BLE_OWN_ADDR_RANDOM, NULL, 10000, &adv_params, adv_event, NULL
  );
}

// ----------------------------------------------------------------------------
static void set_ble_addr()
{
  ble_addr_t addr;

  // Generate new non-resolvable private address
  ble_hs_id_gen_rnd(1, &addr);

  // Set generated address
  ble_hs_id_set_rnd(addr.val);
}

// ----------------------------------------------------------------------------
static int adv_event(struct ble_gap_event* event, void* arg)
{
  switch(event->type)
  {
    case BLE_GAP_EVENT_ADV_COMPLETE:
    {
      xprintf(
        "%s: Advertising completed, termination code: %d\r\n",
        __func__, event->adv_complete.reason
      );
      adv_start();
      break;
    }
    default:
    {
      xprintf("%s: Advertising event not handled\r\n", __func__);
      break;
    }
  }

  return 0;
}

// ----------------------------------------------------------------------------
static void on_sync(void)
{
  set_ble_addr();
  adv_start();
}

// ----------------------------------------------------------------------------
static void on_reset(int reason)
{
  xprintf("%s: Resetting state; reason=%d\n", __func__, reason);
}

// ----------------------------------------------------------------------------
void ble_init()
{
  // ...
  ble_hs_cfg.sync_cb = on_sync;
  ble_hs_cfg.reset_cb = on_reset;
  ble_svc_gap_device_name_set(device_name);
}
