#include "shapefil_private.h"
#include <zephyr/fs/fs.h>
#include <zephyr/kernel.h>
#include <string.h>

static SAFile SADFOpen(const char *pszFilename, const char *pszAccess, void *pvUserData) {
    (void)pvUserData;
    SAFile f = k_malloc(sizeof(struct fs_file_t));
    if (!f) return NULL;
    fs_file_t_init(f);

    int flags = 0;
    if (strchr(pszAccess, 'r')) flags |= FS_O_READ;
    if (strchr(pszAccess, 'w')) flags |= FS_O_WRITE | FS_O_CREATE | FS_O_TRUNC;
    if (strchr(pszAccess, 'a')) flags |= FS_O_WRITE | FS_O_APPEND | FS_O_CREATE;

    if (fs_open(f, pszFilename, flags) < 0) {
        k_free(f);
        return NULL;
    }
    return f;
}

static SAOffset SADFRead(void *p, SAOffset size, SAOffset nmemb, SAFile file) {
    size_t total_read = 0;
    for (SAOffset i = 0; i < nmemb; i++) {
        ssize_t r = fs_read(file, (uint8_t *)p + i * size, size);
        if (r <= 0) break;
        total_read += r;
        if ((size_t)r < size) break; // Reached end of file
    }
    return total_read / size;
}

static SAOffset SADFWrite(const void *p, SAOffset size, SAOffset nmemb, SAFile file) {
    size_t total_written = 0;
    for (SAOffset i = 0; i < nmemb; i++) {
        ssize_t w = fs_write(file, (const uint8_t *)p + i * size, size);
        if (w <= 0) break;
        total_written += w;
        if ((size_t)w < size) break; // disk full
    }
    return total_written / size;
}

static SAOffset SADFSeek(SAFile file, SAOffset offset, int whence) {
    return (fs_seek(file, offset, whence) == 0) ? 0 : -1;
}

static SAOffset SADFTell(SAFile file) {
    off_t pos = fs_tell(file);
    return (pos >= 0) ? pos : -1;
}

static int SADFFlush(SAFile file) {
    return 0;
}

static int SADFClose(SAFile file) {
    int res = fs_close(file);
    k_free(file);
    return res;
}

static int SADRemove(const char *filename, void *pvUserData) {
    (void)pvUserData;
    return fs_unlink(filename);
}

static void SADError(const char *message) {
    printk("Shapelib Error: %s\n", message);
}

void SASetupDefaultHooks(SAHooks *psHooks) {
    psHooks->FOpen = SADFOpen;
    psHooks->FRead = SADFRead;
    psHooks->FWrite = SADFWrite;
    psHooks->FSeek = SADFSeek;
    psHooks->FTell = SADFTell;
    psHooks->FFlush = SADFFlush;
    psHooks->FClose = SADFClose;
    psHooks->Remove = SADRemove;

    psHooks->Error = SADError;
    psHooks->Atof = atof;
    psHooks->pvUserData = SHPLIB_NULLPTR;
}