# adding-BLE-service-to-nrf52832
This repository contains small changes to the BLE_APP_BLINKY from nordik SDK 17. There is a simple addition of a second LED as a Characteristic in the LED service.
Just 3 files are changed from the original Blinky example from nordic SDK, main.c,ble_lds.h and ble_lds.c the rest can be left as is. 

Since many folders and libraries are necessary for the inner workings of the project is highly advisable to just make a copy of the original example folder and alter just alter
these 3 files before building the project in Keil.

