#ifndef __MINIX_DRIVER_H__
#define __MINIX_DRIVER_H__

/* Types and constants shared between the generic and device dependent
 * device driver code.
 */

#define _POSIX_SOURCE      1	/* tell headers to include POSIX stuff */
#define _MINIX             1	/* tell headers to include MINIX stuff */
#define _SYSTEM		   1	/* get negative error number in <errno.h> */

/* The following are so basic, all the *.c files get them automatically. */
#include <minix/config.h>	/* MUST be first */
#include <minix/ansi.h>		/* MUST be second */
#include <minix/type.h>
#include <minix/ipc.h>
#include <minix/com.h>
#include <minix/callnr.h>
#include <sys/types.h>
#include <minix/const.h>
#include <minix/syslib.h>
#include <minix/sysutil.h>
#include <minix/endpoint.h>

#include <string.h>
#include <limits.h>
#include <stddef.h>
#include <errno.h>
#include <sys/param.h>

#include <minix/partition.h>
#include <minix/u64.h>

typedef int thread_id_t;

/* Info about and entry points into the device dependent code. */
struct driver {
  _PROTOTYPE( char *(*dr_name), (void) );
  _PROTOTYPE( int (*dr_open), (struct driver *dp, message *m_ptr) );
  _PROTOTYPE( int (*dr_close), (struct driver *dp, message *m_ptr) );
  _PROTOTYPE( int (*dr_ioctl), (struct driver *dp, message *m_ptr) );
  _PROTOTYPE( struct device *(*dr_prepare), (int device) );
  _PROTOTYPE( int (*dr_transfer), (int proc_nr, int opcode, u64_t position,
					iovec_t *iov, unsigned nr_req) );
  _PROTOTYPE( void (*dr_cleanup), (void) );
  _PROTOTYPE( void (*dr_geometry), (struct partition *entry) );
  _PROTOTYPE( void (*dr_alarm), (struct driver *dp, message *m_ptr) );
  _PROTOTYPE( int (*dr_cancel), (struct driver *dp, message *m_ptr) );
  _PROTOTYPE( int (*dr_select), (struct driver *dp, message *m_ptr) );
  _PROTOTYPE( int (*dr_other), (struct driver *dp, message *m_ptr) );
  _PROTOTYPE( void (*dr_hw_int), (struct driver *dp, message *m_ptr) );
  _PROTOTYPE( int (*dr_thread), (dev_t dev, thread_id_t *threadp) );
};

/* Base and size of a partition in bytes. */
struct device {
  u64_t dv_base;
  u64_t dv_size;
};

#define DRIVER_STD	0	/* Use the standard reply protocol */
#define DRIVER_ASYN	1	/* Use the new asynchronous protocol */

#define MAX_NR_OPEN_DEVICES 256

#define IS_DEV_MINOR_RQ(type) (IS_DEV_RQ(type) && (type) != DEV_STATUS)

/* Functions defined by libdriver. These can be used for both singlethreaded
 * and multithreaded drivers.
 */
_PROTOTYPE( void driver_announce, (void) );
_PROTOTYPE( char *no_name, (void) );
_PROTOTYPE( int do_nop, (struct driver *dp, message *m_ptr) );
_PROTOTYPE( struct device *nop_prepare, (int device) );
_PROTOTYPE( void nop_cleanup, (void) );
_PROTOTYPE( void nop_task, (void) );
_PROTOTYPE( void nop_alarm, (struct driver *dp, message *m_ptr) );
_PROTOTYPE( int nop_cancel, (struct driver *dp, message *m_ptr) );
_PROTOTYPE( int nop_select, (struct driver *dp, message *m_ptr) );
_PROTOTYPE( int do_diocntl, (struct driver *dp, message *m_ptr) );
_PROTOTYPE( int nop_ioctl, (struct driver *dp, message *m_ptr) );

#ifndef _DRIVER_MT_API
/* Additional functions for the singlethreaded version. These allow the driver
 * to either use the stock driver_task(), or implement its own message loop.
 * To avoid accidents, these functions are not exposed when minix/driver_mt.h
 * has been included previously.
 */
_PROTOTYPE( int driver_receive, (endpoint_t src, message *m_ptr,
	int *status_ptr) );
_PROTOTYPE( int driver_receive_mq, (message *m_ptr, int *status_ptr) );
_PROTOTYPE( void driver_handle_msg, (struct driver *dp, int type,
	message *m_ptr, int ipc_status) );
_PROTOTYPE( void driver_terminate, (void) );
_PROTOTYPE( void driver_task, (struct driver *dr, int type) );
_PROTOTYPE( int driver_mq_queue, (message *m_ptr, int status) );
#endif /* !_DRIVER_MT_API */

/* Parameters for the disk drive. */
#define SECTOR_SIZE      512	/* physical sector size in bytes */
#define SECTOR_SHIFT       9	/* for division */
#define SECTOR_MASK      511	/* and remainder */

#define CD_SECTOR_SIZE  2048	/* sector size of a CD-ROM in bytes */

/* Size of the DMA buffer buffer in bytes. */
#define DMA_BUF_SIZE	(DMA_SECTORS * SECTOR_SIZE)

#endif /* __MINIX_DRIVER_H__ */
