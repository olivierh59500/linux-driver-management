/*
 * This file is part of linux-driver-management.
 *
 * Copyright © 2016-2017 Ikey Doherty
 *
 * linux-driver-management is free software; you can redistribute it and/or
 * modify it under the terms of the GNU Lesser General Public License as
 * published by the Free Software Foundation; either version 2.1
 * of the License, or (at your option) any later version.
 */

#pragma once

#include <glib-object.h>

G_BEGIN_DECLS

typedef struct _LdmDevice LdmDevice;
typedef struct _LdmDeviceClass LdmDeviceClass;

/**
 * LdmDeviceType:
 * @LDM_DEVICE_TYPE_ANY: Placeholder value
 * @LDM_DEVICE_TYPE_AUDIO: Audio device
 * @LDM_DEVICE_TYPE_GPU: A graphics device (onboard, NVIDIA, etc.)
 * @LDM_DEVICE_TYPE_HID: Human interface device (mouse, keyboard, etc.)
 * @LDM_DEVICE_TYPE_IMAGE: Imaging device such as a camera
 * @LDM_DEVICE_TYPE_PCI: The device is connected via PCI
 * @LDM_DEVICE_TYPE_PRINTER: Printer
 * @LDM_DEVICE_TYPE_VIDEO: Video device, perhaps a webcam
 * @LDM_DEVICE_TYPE_USB: The device is connected via USB
 * @LDM_DEVICE_TYPE_WIRELESS: A wireless device, i.e. dongle or mouse
 *
 * Any device known to LDM may have one or more "types", defining the primary
 * use-case for these devices. Simple devices will tend to have a single distinct
 * type, such as "GPU". Composite devices, such as a USB camera, may have multiple
 * types, such s HID|IMAGE.
 *
 * As such - each #LdmDevice stores a composite type which may be queried by
 * the API.
 */
typedef enum {
        LDM_DEVICE_TYPE_ANY = 0,
        LDM_DEVICE_TYPE_AUDIO = 1 << 0,
        LDM_DEVICE_TYPE_GPU = 1 << 1,
        LDM_DEVICE_TYPE_HID = 1 << 2,
        LDM_DEVICE_TYPE_IMAGE = 1 << 3,
        LDM_DEVICE_TYPE_PCI = 1 << 4,
        LDM_DEVICE_TYPE_PRINTER = 1 << 5,
        LDM_DEVICE_TYPE_VIDEO = 1 << 6,
        LDM_DEVICE_TYPE_WIRELESS = 1 << 7,
        LDM_DEVICE_TYPE_USB = 1 << 8,
        LDM_DEVICE_TYPE_MAX = 1 << 9,
} LdmDeviceType;

#define LDM_TYPE_DEVICE ldm_device_get_type()
#define LDM_DEVICE(o) (G_TYPE_CHECK_INSTANCE_CAST((o), LDM_TYPE_DEVICE, LdmDevice))
#define LDM_IS_DEVICE(o) (G_TYPE_CHECK_INSTANCE_TYPE((o), LDM_TYPE_DEVICE))
#define LDM_DEVICE_CLASS(o) (G_TYPE_CHECK_CLASS_CAST((o), LDM_TYPE_DEVICE, LdmDeviceClass))
#define LDM_IS_DEVICE_CLASS(o) (G_TYPE_CHECK_CLASS_TYPE((o), LDM_TYPE_DEVICE))
#define LDM_DEVICE_GET_CLASS(o) (G_TYPE_INSTANCE_GET_CLASS((o), LDM_TYPE_DEVICE, LdmDeviceClass))

GType ldm_device_get_type(void);

/* API */
const gchar *ldm_device_get_modalias(LdmDevice *device);
const gchar *ldm_device_get_name(LdmDevice *device);
const gchar *ldm_device_get_path(LdmDevice *device);
const gchar *ldm_device_get_vendor(LdmDevice *device);

gboolean ldm_device_has_type(LdmDevice *device, guint mask);

G_END_DECLS

/*
 * Editor modelines  -  https://www.wireshark.org/tools/modelines.html
 *
 * Local variables:
 * c-basic-offset: 8
 * tab-width: 8
 * indent-tabs-mode: nil
 * End:
 *
 * vi: set shiftwidth=8 tabstop=8 expandtab:
 * :indentSize=8:tabSize=8:noTabs=true:
 */
