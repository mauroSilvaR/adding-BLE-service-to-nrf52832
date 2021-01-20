/**
 * Copyright (c) 2013 - 2020, Nordic Semiconductor ASA
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
#include "sdk_common.h"//obrigat�rio
#if NRF_MODULE_ENABLED(BLE_LBS)//obrigat�rio
#include "ble_lbs.h"//vai ser sempre o .h do servi�o que estou criando
#include "ble_srv_common.h"//obrigat�rio
#include "nrf_log.h"

/**@brief Function for handling the Write event.
 *
 * @param[in] p_lbs      LED Button Service structure.
 * @param[in] p_ble_evt  Event received from the BLE stack.
 */
static void on_write(ble_lbs_t * p_lbs, ble_evt_t const * p_ble_evt)
{   
    ble_gatts_evt_write_t const * p_evt_write = &p_ble_evt->evt.gatts_evt.params.write;
    
    if (   (p_evt_write->handle == p_lbs->led_char_handles.value_handle)
        && (p_evt_write->len == 1)
        && (p_lbs->led_write_handler != NULL))
    {
        p_lbs->led_write_handler(p_ble_evt->evt.gap_evt.conn_handle, p_lbs, p_evt_write->data[0]);
			 
    }
		 if (   (p_evt_write->handle == p_lbs->led2_char_handles.value_handle)
        && (p_evt_write->len == 1)
        && (p_lbs->led2_write_handler != NULL))
		 
    {
        p_lbs->led2_write_handler(p_ble_evt->evt.gap_evt.conn_handle, p_lbs, p_evt_write->data[0]);
			
    }
		
		
}


void ble_lbs_on_ble_evt(ble_evt_t const * p_ble_evt, void * p_context)
{    
    ble_lbs_t * p_lbs = (ble_lbs_t *)p_context;
     
    switch (p_ble_evt->header.evt_id)
    {
        case BLE_GATTS_EVT_WRITE:
            on_write(p_lbs, p_ble_evt);
				     
            break;

        default:
            // No implementation needed.
				     
            break;
    }
}

//fun��o para inicializar os servi�os de bot�o e led!!!!!
//parametros p_lbs=ponteiro para instancia do servi�o do main
//p_lbs_init  ponteiro para a inst�ncia de inicializa��o que est� no main
uint32_t ble_lbs_init(ble_lbs_t * p_lbs, const ble_lbs_init_t * p_lbs_init)
{    //posso colocar o exemplo colocando essa func s� pra retornar um erro se o cara passar algo vazio na fun��o
	   if (p_lbs == NULL || p_lbs_init == NULL)
    {
        return NRF_ERROR_NULL;
			   
    }
	
	
    uint32_t              err_code;
    ble_uuid_t            ble_uuid;
    ble_add_char_params_t add_char_params;

    // Initialize service structure.
    p_lbs->led_write_handler = p_lbs_init->led_write_handler;//digo que o handler da struc de inicializa��o vai ser igual ao
	                                                           //handler da struc do servi�o em si
                                                        //n�o faz sentido?N�o! Mas n�s seguimos a nordic pra n�o dar problema
		p_lbs->led2_write_handler = p_lbs_init->led2_write_handler;
    // Adiciono o servi�o na pilha BLE!!!!.
    ble_uuid128_t base_uuid = {LBS_UUID_BASE};//falo que o UUID base do meu servi�o vai ser o uuid base que falei l� no .h
		                                          //dessa forma mudando no .h eu j� mudo aqui sem esfor�o 
    err_code = sd_ble_uuid_vs_add(&base_uuid, &p_lbs->uuid_type);//essa fun��o adiciona
		                                                             //o uuid base pilha BLE
    VERIFY_SUCCESS(err_code);
     
    ble_uuid.type = p_lbs->uuid_type;
    ble_uuid.uuid = LBS_UUID_SERVICE;
		
    //adiciono as especifica��es do servi�o na tabela GATT, ou seja adiciono as defin~��es que j� fiz na gatt
    err_code = sd_ble_gatts_service_add(BLE_GATTS_SRVC_TYPE_PRIMARY, &ble_uuid, &p_lbs->service_handle);
    VERIFY_SUCCESS(err_code);
     
    // Add Button characteristic.
    memset(&add_char_params, 0, sizeof(add_char_params));
    add_char_params.uuid              = LBS_UUID_BUTTON_CHAR;//falo qual � o UUID do Bot�o
    add_char_params.uuid_type         = p_lbs->uuid_type;//acho que fala qual o tipo se � serv, carac etc. Acho que sempre ser� igual
    add_char_params.init_len          = sizeof(uint8_t);//acho que fala o tamanho pra envio.acho que n�o muda
    add_char_params.max_len           = sizeof(uint8_t);//acho que fala o tamanho m�ximo pra envio.Acho que n�o muda
    add_char_params.char_props.read   = 1;//acho que l� estado do bot�o
    add_char_params.char_props.notify = 1;//define que as propriedadades da caracter�stica v�o poder ser enviadas
    
    add_char_params.read_access       =SEC_OPEN;//define que pode ser lido, se mudar para SEC_NO_ACCESS s� funciona no app, se eu enviar o estado ele ainda vai ler
    add_char_params.cccd_write_access = SEC_OPEN;//define que pode ser escrito

    err_code = characteristic_add(p_lbs->service_handle,
                                  &add_char_params,
                                  &p_lbs->button_char_handles);
    if (err_code != NRF_SUCCESS)
    {    
        return err_code;
    }

    //adiciono a caracter�stica do LED
    memset(&add_char_params, 0, sizeof(add_char_params));
    add_char_params.uuid             = LBS_UUID_LED_CHAR;//UUID do srrvi�o do led defnido l� em cima
    add_char_params.uuid_type        = p_lbs->uuid_type;//tipo do atributo, diz que isso � uma caracter�stica.
    add_char_params.init_len         = sizeof(uint8_t);//acho que � o tamanho da caracter�stica
    add_char_params.max_len          = sizeof(uint8_t);//acho que � o tamanho m�ximo da caracter�stica
    add_char_params.char_props.read  = 1;//define que as propriedadades da caracter�stica v�o poder ser lidas
    add_char_params.char_props.write = 1;//define que as propriedadades da caracter�stica v�o poder ser escritas
		

    add_char_params.read_access  = SEC_OPEN;//define a seguran�a pra leitura das propriedades da caracter�stica
    add_char_params.write_access = SEC_OPEN;//define a seguran�a pra escrita das propriedades da caracter�stica
		
		err_code=characteristic_add(p_lbs->service_handle, &add_char_params, &p_lbs->led_char_handles);
		if (err_code != NRF_SUCCESS)
    {    
        return err_code;
    }
				
    //adiciono caracter�stica do LED 2
    memset(&add_char_params, 0, sizeof(add_char_params));
    add_char_params.uuid             = LBS_UUID_LED2_CHAR;//UUID do srrvi�o do led defnido l� em cima
    add_char_params.uuid_type        = p_lbs->uuid_type;//tipo do atributo, diz que isso � uma caracter�stica.
    add_char_params.init_len         = sizeof(uint8_t);//acho que � o tamanho da caracter�stica
    add_char_params.max_len          = sizeof(uint8_t);//acho que � o tamanho m�ximo da caracter�stica
    add_char_params.char_props.read  = 1;//define que as propriedadades da caracter�stica v�o poder ser lidas
    add_char_params.char_props.write = 1;//define que as propriedadades da caracter�stica v�o poder ser escritas
		
    add_char_params.read_access  = SEC_OPEN;//define a seguran�a pra leitura das propriedades da caracter�stica
    add_char_params.write_access = SEC_OPEN;//define a seguran�a pra escrita das propriedades da caracter�stica
		
    return characteristic_add(p_lbs->service_handle, &add_char_params, &p_lbs->led2_char_handles);
}


uint32_t ble_lbs_on_button_change(uint16_t conn_handle, ble_lbs_t * p_lbs, uint8_t button_state)
{
    ble_gatts_hvx_params_t params;
    uint16_t len = sizeof(button_state);

    memset(&params, 0, sizeof(params));
    params.type   = BLE_GATT_HVX_NOTIFICATION;
    params.handle = p_lbs->button_char_handles.value_handle;
    params.p_data = &button_state;
    params.p_len  = &len;

    return sd_ble_gatts_hvx(conn_handle, &params);
}
#endif // NRF_MODULE_ENABLED(BLE_LBS)
