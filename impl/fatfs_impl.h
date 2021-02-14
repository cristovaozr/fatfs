/**
 * @author Cristóvão Zuppardo Rufino <cristovaozr@gmail.com>
 * @version 0.1
 *
 * @copyright Copyright Cristóvão Zuppardo Rufino (c) 2020-2021
 * Please see LICENCE file to information regarding licensing
 */

#ifndef COMPONENTS_FATFS_IMPL_FATFS_IMPL_H_
#define COMPONENTS_FATFS_IMPL_FATFS_IMPL_H_

#include <stdint.h>

/**
 * @brief Sets the SDCARD object to FatFs
 *
 * @param sdcard SDCARD object
 * @return E_SUCCESS on success
 */
extern int32_t fatfs_impl_set_sdcard(const struct sdcard * const sdcard);

#endif // COMPONENTS_FATFS_IMPL_FATFS_IMPL_H_
