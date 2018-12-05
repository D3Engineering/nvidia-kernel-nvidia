/*
 * Copyright (c) 2018, NVIDIA CORPORATION.  All rights reserved.
 *
 * This program is free software; you can redistribute it and/or modify it
 * under the terms and conditions of the GNU General Public License,
 * version 2, as published by the Free Software Foundation.
 *
 * This program is distributed in the hope it will be useful, but WITHOUT
 * ANY WARRANTY; without even the implied warranty of MERCHANTABILITY or
 * FITNESS FOR A PARTICULAR PURPOSE.  See the GNU General Public License for
 * more details.
 *
 * You should have received a copy of the GNU General Public License
 * along with this program.  If not, see <http://www.gnu.org/licenses/>.
 */

#ifndef DRM_TEGRA_UDRM_IOCTL_H
#define DRM_TEGRA_UDRM_IOCTL_H

#include <drm/drm.h>

#if defined(__cplusplus)
extern "C" {
#endif

#define DRM_TEGRA_UDRM_DMABUF_MMAP              0x00
#define DRM_TEGRA_UDRM_DMABUF_DESTROY_MAPPINGS  0x01
#define DRM_TEGRA_UDRM_CLOSE_NOTIFY             0x02
#define DRM_TEGRA_UDRM_SEND_VBLANK_EVENT        0x03
#define DRM_TEGRA_UDRM_DROP_MASTER_NOTIFY       0x04

struct drm_tegra_udrm_dmabuf_mmap {
	int fd;
	unsigned long offset;
};

struct drm_tegra_udrm_dmabuf_destroy_mappings {
	int fd;
};

struct drm_tegra_udrm_close_notify {
	int eventfd;
	int clear;
};

struct drm_tegra_udrm_send_vblank_event {
	struct drm_event_vblank vblank;
};

struct drm_tegra_udrm_drop_master_notify {
	int eventfd;
	int clear;
};

#define TEGRA_UDRM_IOCTL(dir, name, str) \
	DRM_##dir(DRM_COMMAND_BASE + DRM_TEGRA_UDRM_##name, \
		struct drm_tegra_udrm_##str)

/* In order to facilitate mmap() on the dumb buffer, UMD issues this
 * ioctl from handling of DRM_IOCTL_MODE_MAP_DUMB with dmabuf fd
 * corresponding to dumb buffer. Driver will return fake offset
 * to UMD which can be used by DRM clients in mmap(.., offset).
 *
 * In parameters -
 *     fd: dmabuf fd.
 *
 * Out parameters -
 *     offset: offset to be used in mmap().
 */
#define DRM_IOCTL_TEGRA_UDRM_DMABUF_MMAP \
	TEGRA_UDRM_IOCTL(IOWR, DMABUF_MMAP, dmabuf_mmap)


/* UMD issues this ioctl from handling of DRM_IOCTL_MODE_DESTROY_DUMB
 * to destroy dmabuf fd <=> offset mapping created by driver in
 * DRM_IOCTL_TEGRA_UDRM_DMABUF_MMAP.
 *
 * In parameters -
 *     fd: dmabuf fd.
 */
#define DRM_IOCTL_TEGRA_UDRM_DMABUF_DESTROY_MAPPINGS \
	TEGRA_UDRM_IOCTL(IOW, DMABUF_DESTROY_MAPPINGS, \
		dmabuf_destroy_mappings)

/* UMD issues this ioctl with eventfd and then does poll() on
 * eventfd. Driver signals this eventfd from .preclose method.
 *
 * In parameters -
 *     eventfd: fd created using eventfd().
 *     clear: When set driver will no longer signal the eventfd.
 */
#define DRM_IOCTL_TEGRA_UDRM_CLOSE_NOTIFY \
	TEGRA_UDRM_IOCTL(IOW, CLOSE_NOTIFY, close_notify)

/* UMD issues this ioctl with vblank event. Driver injects
 * this event into DRM framework so that it get delivered to DRM
 * clients waiting on poll/read on drivers fd.
 *
 * In parameters -
 *    vblank: drm_event_vblank structure.
 */
#define DRM_IOCTL_TEGRA_UDRM_SEND_VBLANK_EVENT \
	TEGRA_UDRM_IOCTL(IOW, SEND_VBLANK_EVENT, send_vblank_event)

/* UMD issues this ioctl with eventfd and then does poll() on
 * eventfd. Driver signals this eventfd from .master_drop method.
 *
 * In parameters -
 *     eventfd: fd created using eventfd().
 *     clear: When set driver will no longer signal the eventfd.
 */
#define DRM_IOCTL_TEGRA_UDRM_DROP_MASTER_NOTIFY \
	TEGRA_UDRM_IOCTL(IOW, DROP_MASTER_NOTIFY, drop_master_notify)

#if defined(__cplusplus)
}
#endif

#endif