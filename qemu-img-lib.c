/**
 * Wrapper for qemu image functions. These functions are exported from this 
 * shared library.
 */

#include "qemu-common.h"
#include "osdep.h"
#include "block_int.h"
#include "qemu-img-lib.h"
#include <stdio.h>

#ifdef EMISCRITEN
#include <emscripten/emscripten.h>
#else
#define EMSCRIPTEN_KEEPALIVE 
#endif

/**
 * Function to open qemu image file.
 */
EMSCRIPTEN_KEEPALIVE __declspec(dllexport) void* qemu_img_open(const char *filename)
{

    BlockDriverState *bs;
    BlockDriver *drv;
    int ret;

    bdrv_init();

    bs = bdrv_new("");
    if (!bs)
        fprintf(stderr, "Not enough memory");

    drv = NULL;
    if ((ret = bdrv_open(bs, filename, BDRV_O_CACHE_WB, drv)) < 0) {
        fprintf(stderr, "Could not open (error: %d) '%s'", ret, filename);
    }

    fprintf(stderr, "Opened file %s\n", filename);
    //fprintf(stderr, "in QEMU int %d, long %d, ul %d, int64 %d, szt %d, off %d\n",
            //sizeof(int), sizeof(long), sizeof(unsigned long),
            //sizeof(int64_t), sizeof(size_t), sizeof(off_t));



    return bs;
}

/**
 * Function to read qemu image.
 */
EMSCRIPTEN_KEEPALIVE __declspec(dllexport) int qemu_img_read(void *bs, int64_t offset,
                    uint8_t *buf, size_t len)
{
    fprintf(stderr, "qemu_read off=%llu, len=%zd\n", (uint64_t)offset, len);
    return bdrv_pread((BlockDriverState *)bs, (uint64_t)offset, buf, len);
}

/**
 * Function to get image information.
 */
EMSCRIPTEN_KEEPALIVE __declspec(dllexport) int qemu_img_get_info(void *bs, int64_t *nsectors, 
                                    unsigned int *sect_size, int64_t *size)
{
    char fmt_name[128];

    bdrv_get_format(bs, fmt_name, sizeof(fmt_name));
    bdrv_get_geometry(bs, (uint64_t*)nsectors);
    *sect_size = 512;
    *size = *nsectors * (*sect_size);

    fprintf(stderr, "Qemu info: sect_size = %u, size=%zd\n", *sect_size, *size);
    return 0;
}