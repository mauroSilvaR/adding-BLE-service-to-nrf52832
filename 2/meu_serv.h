/**
 * Copyright (c) 2015 - 2020, Nordic Semiconductor ASA
 *
 * All rights reserved.
 *
 * Redistribution and use in source and binary forms, with or without modification,
 * are permitted provided that the following conditions are met:
 *
 * 1. Redistributions of source code must retain the above copyright notice, this
 *    list of conditions and the following disclaimer.
 *
 * 2. Redistributions in binary form, except as embedded into a Nordic
 *    Semiconductor ASA integrated circuit in a product or a software update for
 *    such product, must reproduce the above copyright notice, this list of
 *    conditions and the following disclaimer in the documentation and/or other
 *    materials provided with the distribution.
 *
 * 3. Neither the name of Nordic Semiconductor ASA nor the names of its
 *    contributors may be used to endorse or promote products derived from this
 *    software without specific prior written permission.
 *
 * 4. This software, with or without modification, must only be used with a
 *    Nordic Semiconductor ASA integrated circuit.
 *
 * 5. Any software provided in binary form under this license must not be reverse
 *    engineered, decompiled, modified and/or disassembled.
 *
 * THIS SOFTWARE IS PROVIDED BY NORDIC SEMICONDUCTOR ASA "AS IS" AND ANY EXPRESS
 * OR IMPLIED WARRANTIES, INCLUDING, BUT NOT LIMITED TO, THE IMPLIED WARRANTIES
 * OF MERCHANTABILITY, NONINFRINGEMENT, AND FITNESS FOR A PARTICULAR PURPOSE ARE
 * DISCLAIMED. IN NO EVENT SHALL NORDIC SEMICONDUCTOR ASA OR CONTRIBUTORS BE
 * LIABLE FOR ANY DIRECT, INDIRECT, INCIDENTAL, SPECIAL, EXEMPLARY, OR
 * CONSEQUENTIAL DAMAGES (INCLUDING, BUT NOT LIMITED TO, PROCUREMENT OF SUBSTITUTE
 * GOODS OR SERVICES; LOSS OF USE, DATA, OR PROFITS; OR BUSINESS INTERRUPTION)
 * HOWEVER CAUSED AND ON ANY THEORY OF LIABILITY, WHETHER IN CONTRACT, STRICT
 * LIABILITY, OR TORT (INCLUDING NEGLIGENCE OR OTHERWISE) ARISING IN ANY WAY OUT
 * OF THE USE OF THIS SOFTWARE, EVEN IF ADVISED OF THE POSSIBILITY OF SUCH DAMAGE.
 *
 */
/** @file
 *
 * @defgroup ble_lbs LED Button Service Server
 * @{
 * @ingroup ble_sdk_srv
 *
 * @brief LED Button Service Server module.
 *
 * @details This module implements a custom LED Button Service with an LED and Button Characteristics.
 *          During initialization, the module adds the LED Button Service and Characteristics
 *          to the BLE stack database.
 *
 *          The application must supply an event handler for receiving LED Button Service
 *          events. Using this handler, the service notifies the application when the
 *          LED value changes.
 *
 *          The service also provides a function for letting the application notify
 *          the state of the Button Characteristic to connected peers.
 *
 * @note    The application must register this module as BLE event observer using the
 *          NRF_SDH_BLE_OBSERVER macro. Example:
 *          @code
 *              ble_hids_t instance;
 *              NRF_SDH_BLE_OBSERVER(anything, BLE_HIDS_BLE_OBSERVER_PRIO,
 *                                   ble_hids_on_ble_evt, &instance);
 *          @endcode
 */

#ifndef MEU_SERV_H__
#define MEU_SERV_H__

#include <stdint.h>
#include <stdbool.h>
#include "ble.h"
#include "ble_srv_common.h"
#include "nrf_sdh_ble.h"

#ifdef __cplusplus
extern "C" {
#endif

/**@brief   Macro for defining a ble_lbs instance.
 *
 * @param   _name   Name of the instance.
 * @hideinitializer
 */
#define MEU_SERV_DEF(_name)      /*essa macro é só pra poder criar a instância na main */                                                                    \
static meu_serv_t _name;                                                                             \
NRF_SDH_BLE_OBSERVER(_name ## _obs,                                                                 \
                     BLE_LBS_BLE_OBSERVER_PRIO, /*talvez não precise*/                                                    \
                     meu_serv_on_ble_evt, &_name)//talvez não precise// SUUUUUUPER importante!!!!! você precisa declara o seu serviço aqui!!!
//50f0539f-3bcb-4417-8120-08206b5cb054 from https://www.uuidgenerator.net/version4
#define SERV_UUID_BASE        {0x54, 0xB0, 0x5C, 0x6B, 0x20, 0x08, 0x20, 0x81, \
                              0x17, 0x44, 0xCB, 0x3B, 0x9F, 0x53, 0xF0, 0x50}
#define SERV_UUID_SERVICE     0x2077
#define LBS_UUID_LED_CHAR     0x1525															
#define SERV_UUID_LOG         0x2272


// Forward declaration of the ble_lbs_t type. Uma forward declaration é como se fosse uma prototipagem de uma função numa classe de python
typedef struct meu_serv_s meu_serv_t;

//essas são prototipagens de funções que vou usar na main
typedef void (*meu_serv_write_handler_t) (uint16_t conn_handle, meu_serv_t * p_lbs, uint8_t new_state);


/** @brief LED Button Service init structure. This structure contains all options and data needed for
 *        initialization of the service.*/
typedef struct//struct pra iniciar. repare que nessa eu não fiz forward declaration.
{
    meu_serv_write_handler_t log_write_handler; /**< Event handler to be called when the LED Characteristic is written. */
	  
} meu_serv_init_t;

/**@brief LED Button Service structure. This structure contains various status information for the service. */
struct meu_serv_s//struct do serviço em si aqui vc declara os "bloquinhos" tendo em mente o protocolo ATT
{
    uint16_t                    service_handle;      /**< Handle of your custom Service (as provided by the BLE stack). */
    ble_gatts_char_handles_t    log_char_handles;    /**< Handles related to the log Characteristic. */
    uint8_t                     uuid_type;           /**< UUID type for your Service. */
    meu_serv_write_handler_t    log_write_handler;   /**< Event handler to be called when the LOG Characteristic is written. */

};


/**@brief Function for initializing the meu_serv Service.somente um protótipo
 *
 * @param[out] p_lbs      LED Button Service structure. This structure must be supplied by
 *                        the application. It is initialized by this function and will later
 *                        be used to identify this particular service instance.
 * @param[in] p_lbs_init  Information needed to initialize the service.
 *
 * @retval NRF_SUCCESS If the service was initialized successfully. Otherwise, an error code is returned.
 */
uint32_t meu_serv_init(meu_serv_t * p_lbs, const meu_serv_init_t * p_lbs_init);


/**@brief Function for handling the application's BLE stack events.
 *somente um protótipo...implementação no arquivo c
 * @details This function handles all events from the BLE stack that are of interest to the LED Button Service.
 *
 * @param[in] p_ble_evt  Event received from the BLE stack.
 * @param[in] p_context  LED Button Service structure.
 */
void meu_serv_on_ble_evt(ble_evt_t const * p_ble_evt, void * p_context);


/**@brief Function for sending a button state notification.Pode ser que eu use...,as por enquanto não
 *
 ' @param[in] conn_handle   Handle of the peripheral connection to which the button state notification will be sent.
 * @param[in] p_lbs         LED Button Service structure.
 * @param[in] button_state  New button state.
 *
 * @retval NRF_SUCCESS If the notification was sent successfully. Otherwise, an error code is returned.
 */
//uint32_t ble_lbs_on_button_change(uint16_t conn_handle, ble_lbs_t * p_lbs, uint8_t button_state);
//ESSA FUNÇÃO ESTÁ PROTOTIPADA AQUI E ESCRITA NA ble_lbs.c

#ifdef __cplusplus
}
#endif

#endif // BLE_LBS_H__

/** @} */
