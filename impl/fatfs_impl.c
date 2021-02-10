/**
 * @author Cristóvão Zuppardo Rufino <cristovaozr@gmail.com>
 * @version 0.1
 *
 * @copyright Copyright Cristóvão Zuppardo Rufino (c) 2020
 * Please see LICENCE file to information regarding licensing
 */

#include "components/fatfs/source/ff.h"
#include "components/fatfs/source/diskio.h"

#include "include/device/device.h"
#include "include/device/spi.h"
#include "include/device/gpio.h"

#include "drivers/sdcard/sdcard.h"

#include "include/errors.h"

#include "ulibc/include/log.h"

#include <stdint.h>
#include <stddef.h>

static const struct sdcard *local_sdcard = NULL;

#define TAG "FFIMPL"

int32_t fatfs_impl_set_sdcard(const struct sdcard * const sdcard)
{
    int32_t ret = sdcard_init(sdcard);
    if (ret < 0) {
        goto exit;
    }
    
    local_sdcard = sdcard;

    exit:
    return ret;
}

// ----------------------------------------------------------------------------
// These functions are used by FatFs to access the SD card.
// Not all functions from the diskio.h headers are necessary, so only the ones
// that are are here
// For our purposes we will access the SD Card using SPI that the mbl_loader
// configures.
// ----------------------------------------------------------------------------

// http://elm-chan.org/fsw/ff/doc/dinit.html
DSTATUS disk_initialize (BYTE pdrv) {
    // Since this function will only be called after the SPI and SD Card
    // have been initialized (check mbl_cl_initialize), we just return that
    // the disk is ready
    (void) pdrv;
    if (local_sdcard) return RES_OK;
    return RES_ERROR;
}

// http://elm-chan.org/fsw/ff/doc/dstat.html
DSTATUS disk_status (BYTE pdrv) {
    // Since this function will only be called after the SPI and SD Card
    // have been initialized (check mbl_cl_initialize), we just return that
    // the disk is ready
    (void) pdrv;
    if (local_sdcard) return RES_OK;
    return RES_ERROR;
}

// http://elm-chan.org/fsw/ff/doc/dread.html
DRESULT disk_read (BYTE pdrv, BYTE* buff, LBA_t sector, UINT count) {
    // In this function we will read 'count' sectors from the SD card
    // starting in 'sector' address.
    // We will align the sector number to what the SD Card expects since
    // it could be a "Byte-Aligned" or "Sector-Aligned", depending on the version

    (void)pdrv;
    DRESULT ff_ret = RES_OK;

    do {
        int32_t ret = sdcard_read_block(local_sdcard, sector, buff);
        if (ret < 0) {
            ff_ret = RES_ERROR;
            goto exit;
        }
        buff += 512;
        sector++;
    } while(--count);

    exit:
	return ff_ret;
}