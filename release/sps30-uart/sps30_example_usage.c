/*
 * Copyright (c) 2018, Sensirion AG
 * All rights reserved.
 *
 * Redistribution and use in source and binary forms, with or without
 * modification, are permitted provided that the following conditions are met:
 *
 * * Redistributions of source code must retain the above copyright notice, this
 *   list of conditions and the following disclaimer.
 *
 * * Redistributions in binary form must reproduce the above copyright notice,
 *   this list of conditions and the following disclaimer in the documentation
 *   and/or other materials provided with the distribution.
 *
 * * Neither the name of Sensirion AG nor the names of its
 *   contributors may be used to endorse or promote products derived from
 *   this software without specific prior written permission.
 *
 * THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDERS AND CONTRIBUTORS "AS IS"
 * AND ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT LIMITED TO, THE
 * IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE
 * ARE DISCLAIMED. IN NO EVENT SHALL THE COPYRIGHT HOLDER OR CONTRIBUTORS BE
 * LIABLE FOR ANY DIRECT, INDIRECT, INCIDENTAL, SPECIAL, EXEMPLARY, OR
 * CONSEQUENTIAL DAMAGES (INCLUDING, BUT NOT LIMITED TO, PROCUREMENT OF
 * SUBSTITUTE GOODS OR SERVICES; LOSS OF USE, DATA, OR PROFITS; OR BUSINESS
 * INTERRUPTION) HOWEVER CAUSED AND ON ANY THEORY OF LIABILITY, WHETHER IN
 * CONTRACT, STRICT LIABILITY, OR TORT (INCLUDING NEGLIGENCE OR OTHERWISE)
 * ARISING IN ANY WAY OUT OF THE USE OF THIS SOFTWARE, EVEN IF ADVISED OF THE
 * POSSIBILITY OF SUCH DAMAGE.
 */

#include <stdio.h>  // printf

#include "sensirion_uart.h"
#include "sps30.h"

/**
 * TO USE CONSOLE OUTPUT (PRINTF) AND WAIT (SLEEP) PLEASE ADAPT THEM TO YOUR
 * PLATFORM
 */
//#define printf(...)

int main(void) {
    struct sps30_measurement m;
    char serial[SPS30_MAX_SERIAL_LEN];
    int16_t ret;

    while (sensirion_uart_open() != 0) {
        printf("ERROR sensirion_uart_open\n");
        sensirion_sleep_usec(1000000); /* sleep for 1s */
    }

    /* Busy loop for initialization, because the main loop does not work without
     * a sensor.
     */
    printf("SEND sps30_probe\n");
    while (sps30_probe() != 0) {
        printf("ERROR sps30_probe\n");
        sensirion_sleep_usec(1000000); /* sleep for 1s */
    }

    struct sps30_version_information version_information;
    ret = sps30_read_version(&version_information);
    if (ret) {
        printf("ERROR (%d) sps30_read_version\n", ret);
    } else {
        printf("sps30_read_version FW: %u.%u HW: %u, SHDLC: %u.%u\n",
               version_information.firmware_major,
               version_information.firmware_minor,
               version_information.hardware_revision,
               version_information.shdlc_major,
               version_information.shdlc_minor);
    }

    ret = sps30_get_serial(serial);
    if (ret)
        printf("ERROR (%d) sps30_get_serial\n", ret);
    else
        printf("sps30_get_serial \"%s\"\n", serial);

    while (1) {

        printf("SEND sps30_start_measurement\n");
        ret = sps30_start_measurement();
        if (ret < 0) {
            printf("ERROR (%d) sps30_start_measurement\n", ret);
        }

        printf("measured values:\n" "pm1.0" "\tpm2.5" "\tpm4.0" "\tpm10.0" "\tnc0.5" "\tnc1.0" "\tnc2.5" "\tnc4.5" "\tnc10.0" "\tsize\n");

        for (int i = 0; i < 20; ++i) {

            // printf("SEND sps30_read_measurement\n");
            ret = sps30_read_measurement(&m);
            if (ret < 0) {
                printf("ERROR (%d) sps30_read_measurement\n", ret);
            } else {
                if (SPS30_IS_ERR_STATE(ret)) {
                    printf(
                        "ERROR Chip state %u - measurements may not be accurate\n",
                        SPS30_GET_ERR_STATE(ret));
                }

                printf( "%8.2f" "%8.2f" "%8.2f" "%8.2f" "%8.2f" "%8.2f" "%8.2f" "%8.2f" "%8.2f" "%8.2f\n",
                       m.mc_1p0, m.mc_2p5, m.mc_4p0, m.mc_10p0,
                       m.nc_0p5, m.nc_1p0, m.nc_2p5, m.nc_4p0, m.nc_10p0,
                       m.typical_particle_size);
            }

            sensirion_sleep_usec(1000000); /* sleep for 1s */
        }

        printf("SEND sps30_stop_measurement\n");
        ret = sps30_stop_measurement();
        if (ret) {
            printf("ERROR (%d) sps30_stop_measurement\n", ret);
        }

        if (version_information.firmware_major >= 2) {
            printf("SEND sps30_sleep\n");
            ret = sps30_sleep();
            if (ret) {
                printf("ERROR (%d) sps30_sleep\n", ret);
            }
        }

        printf("sleep for 60 seconds\n");
        sensirion_sleep_usec(1000000 * 60);

        if (version_information.firmware_major >= 2) {
            printf("SEND sps30_wake_up\n");
            ret = sps30_wake_up();
            if (ret) {
                printf("ERROR (%d) sps30_wake_up\n", ret);
            }
        }
    }

    printf("SEND sps30_stop_measurement\n");
    sps30_stop_measurement();

    if (sensirion_uart_close() != 0)
        printf("ERROR sensirion_uart_close\n");

    return 0;
}
