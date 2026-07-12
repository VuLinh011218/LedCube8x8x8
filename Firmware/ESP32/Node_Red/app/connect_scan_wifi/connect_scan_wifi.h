#ifndef WIFI_H_
#define WIFI_H_

/***********************************************************************************************************************
 * Includes   <System Includes> , "Project Includes"
 **********************************************************************************************************************/

#include <string.h>
#include "freertos/FreeRTOS.h"
#include "freertos/task.h"
#include "freertos/event_groups.h"
#include "esp_system.h"
#include "esp_wifi.h"
#include "esp_event.h"
#include "esp_log.h"
#include "nvs_flash.h"

#include "lwip/err.h"
#include "lwip/sys.h"

#ifdef __cplusplus
extern "C"
{
#endif

    /*******************************************************************************************************************
     * Public APIs
     ******************************************************************************************************************/

    /**
     * @brief Scan for nearby WiFi networks and print the results.
     */
    void wifi_scan_and_print(void);

    /**
     * @brief Initialize WiFi Station.
     */
    void wifi_init_sta(void);

    /**
     * @brief Task light flashes to indicate WiFi status.
     */
    void wifi_blink_task(void *arg);

#ifdef __cplusplus
}
#endif

#endif /* WIFI_H_ */
