/* -*- Mode: C; tab-width: 8; indent-tabs-mode: nil; c-basic-offset: 8 -*-
 *
 * Copyright (C) 2008 David Zeuthen <david@fubar.dk>
 *
 * This program is free software; you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published by
 * the Free Software Foundation; either version 2 of the License, or
 * (at your option) any later version.
 *
 * This program is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.
 *
 * You should have received a copy of the GNU General Public License
 * along with this program; if not, write to the Free Software
 * Foundation, Inc., 51 Franklin St, Fifth Floor, Boston, MA  02110-1301  USA
 *
 */

#ifndef __DEVKIT_DISKS_DEVICE_PRIVATE_H__
#define __DEVKIT_DISKS_DEVICE_PRIVATE_H__

#include <dbus/dbus-glib.h>
#include <gudev/gudev.h>
#include <atasmart.h>

#include "devkit-disks-types.h"

G_BEGIN_DECLS

struct Job;
typedef struct Job Job;

#define LSOF_DATA_STRUCT_TYPE (dbus_g_type_get_struct ("GValueArray",   \
                                                       G_TYPE_UINT,     \
                                                       G_TYPE_UINT,     \
                                                       G_TYPE_STRING,   \
                                                       G_TYPE_INVALID))

struct DevkitDisksDevicePrivate
{
        DBusGConnection *system_bus_connection;
        DevkitDisksDaemon *daemon;
        GUdevDevice *d;

        Job *job;

        char *object_path;
        char *native_path;
        guint64 device_detection_time;
        guint64 device_media_detection_time;

        gboolean removed;

        gboolean job_in_progress;
        char *job_id;
        uid_t job_initiated_by_uid;
        gboolean job_is_cancellable;
        double job_percentage;

        guint linux_md_poll_timeout_id;

        /* A list of current polling inhibitors (DevkitDisksInhibitor objects) */
        GList *polling_inhibitors;

        /* if non-zero, the id of the idle for emitting a 'change' signal */
        guint emit_changed_idle_id;

        /*****************/
        /* Disk spindown */
        /*****************/

        /* A list of current spindown configurators (DevkitDisksInhibitor objects)
         *
         * Each object will have a data element, @spindown-timeout-seconds, that is
         * the requested timeout for the inhibitor in question. It can be read via
         *
         *  GPOINTER_TO_INT (g_object_get_data (G_OBJECT (inhibitor), "spindown-timeout-seconds"));
         */
        GList *spindown_inhibitors;

        /* The timeout the disk is currently configured with, in seconds. This is 0 if spindown
         * is not enabled. Depending on the command-set used, a slightly different rounded value
         * may have been sent to the disk - for example, the ATA command-set has a rather peculiar
         * mapping, see the hdparm(1) man-page, option -S.
         *
         * This value is computed by considering all per-disk spindown inhibitors (set
         * via the DriveSetSpindownTimeout() method on the device) and all global spindown
         * inhibitors (set via the DriveSetAllSpindownTimeouts() method on the daemon).
         */
        gint spindown_timeout;

        /**************/
        /* Properties */
        /**************/

        char *device_file;
        dev_t dev;
        GPtrArray *device_file_by_id;
        GPtrArray *device_file_by_path;
        gboolean device_is_system_internal;
        gboolean device_is_partition;
        gboolean device_is_partition_table;
        gboolean device_is_removable;
        gboolean device_is_media_available;
        gboolean device_is_media_change_detected;
        gboolean device_is_media_change_detection_polling;
        gboolean device_is_media_change_detection_inhibitable;
        gboolean device_is_media_change_detection_inhibited;
        gboolean device_is_read_only;
        gboolean device_is_drive;
        gboolean device_is_optical_disc;
        gboolean device_is_luks;
        gboolean device_is_luks_cleartext;
        gboolean device_is_linux_md_component;
        gboolean device_is_linux_md;
        guint64 device_size;
        guint64 device_block_size;
        gboolean device_is_mounted;
        GPtrArray *device_mount_paths;
        uid_t device_mounted_by_uid;
        gboolean device_presentation_hide;
        gboolean device_presentation_nopolicy;
        char *device_presentation_name;
        char *device_presentation_icon_name;

        char *id_usage;
        char *id_type;
        char *id_version;
        char *id_uuid;
        char *id_label;

        char *partition_slave;
        char *partition_scheme;
        char *partition_type;
        char *partition_label;
        char *partition_uuid;
        GPtrArray *partition_flags;
        int partition_number;
        guint64 partition_offset;
        guint64 partition_size;

        char *partition_table_scheme;
        int partition_table_count;

        char *drive_vendor;
        char *drive_model;
        char *drive_revision;
        char *drive_serial;
        char *drive_wwn;
        char *drive_connection_interface;
        guint drive_connection_speed;
        GPtrArray *drive_media_compatibility;
        char *drive_media;
        gboolean drive_is_media_ejectable;
        gboolean drive_can_detach;
        gboolean drive_can_spindown;
        gboolean drive_is_rotational;
        guint drive_rotation_rate;
        char *drive_write_cache;
        char *drive_adapter;
        GPtrArray *drive_ports;

        gboolean optical_disc_is_blank;
        gboolean optical_disc_is_appendable;
        gboolean optical_disc_is_closed;
        guint optical_disc_num_tracks;
        guint optical_disc_num_audio_tracks;
        guint optical_disc_num_sessions;

        char *luks_holder;

        char *luks_cleartext_slave;
        uid_t luks_cleartext_unlocked_by_uid;

        char *linux_md_component_level;
        int linux_md_component_position;
        int linux_md_component_num_raid_devices;
        char *linux_md_component_uuid;
        char *linux_md_component_home_host;
        char *linux_md_component_name;
        char *linux_md_component_version;
        char *linux_md_component_holder;
        GPtrArray *linux_md_component_state;

        char *linux_md_state;
        char *linux_md_level;
        int linux_md_num_raid_devices;
        char *linux_md_uuid;
        char *linux_md_home_host;
        char *linux_md_name;
        char *linux_md_version;
        GPtrArray *linux_md_slaves;
        GPtrArray *linux_md_slaves_state;
        gboolean linux_md_is_degraded;
        char *linux_md_sync_action;
        double linux_md_sync_percentage;
        guint64 linux_md_sync_speed;

        gboolean drive_ata_smart_is_available;
        guint64 drive_ata_smart_time_collected;
        SkSmartOverall drive_ata_smart_status;
        void *drive_ata_smart_blob;
        gsize drive_ata_smart_blob_size;

        /* the following properties are not (yet) exported */
        char *dm_name;
        GPtrArray *slaves_objpath;
        GPtrArray *holders_objpath;
};

/* property setters */

void devkit_disks_device_set_job_in_progress (DevkitDisksDevice *device, gboolean value);
void devkit_disks_device_set_job_id (DevkitDisksDevice *device, const gchar *value);
void devkit_disks_device_set_job_initiated_by_uid (DevkitDisksDevice *device, guint value);
void devkit_disks_device_set_job_is_cancellable (DevkitDisksDevice *device, gboolean value);
void devkit_disks_device_set_job_percentage (DevkitDisksDevice *device, gdouble value);

void devkit_disks_device_set_device_detection_time (DevkitDisksDevice *device, guint64 value);
void devkit_disks_device_set_device_media_detection_time (DevkitDisksDevice *device, guint64 value);
void devkit_disks_device_set_device_file (DevkitDisksDevice *device, const gchar *value);
void devkit_disks_device_set_device_file_by_id (DevkitDisksDevice *device, GStrv value);
void devkit_disks_device_set_device_file_by_path (DevkitDisksDevice *device, GStrv value);
void devkit_disks_device_set_device_is_system_internal (DevkitDisksDevice *device, gboolean value);
void devkit_disks_device_set_device_is_partition (DevkitDisksDevice *device, gboolean value);
void devkit_disks_device_set_device_is_partition_table (DevkitDisksDevice *device, gboolean value);
void devkit_disks_device_set_device_is_removable (DevkitDisksDevice *device, gboolean value);
void devkit_disks_device_set_device_is_media_available (DevkitDisksDevice *device, gboolean value);
void devkit_disks_device_set_device_is_media_change_detected (DevkitDisksDevice *device, gboolean value);
void devkit_disks_device_set_device_is_media_change_detection_polling (DevkitDisksDevice *device, gboolean value);
void devkit_disks_device_set_device_is_media_change_detection_inhibitable (DevkitDisksDevice *device, gboolean value);
void devkit_disks_device_set_device_is_media_change_detection_inhibited (DevkitDisksDevice *device, gboolean value);
void devkit_disks_device_set_device_is_read_only (DevkitDisksDevice *device, gboolean value);
void devkit_disks_device_set_device_is_drive (DevkitDisksDevice *device, gboolean value);
void devkit_disks_device_set_device_is_optical_disc (DevkitDisksDevice *device, gboolean value);
void devkit_disks_device_set_device_is_luks (DevkitDisksDevice *device, gboolean value);
void devkit_disks_device_set_device_is_luks_cleartext (DevkitDisksDevice *device, gboolean value);
void devkit_disks_device_set_device_is_linux_md_component (DevkitDisksDevice *device, gboolean value);
void devkit_disks_device_set_device_is_linux_md (DevkitDisksDevice *device, gboolean value);
void devkit_disks_device_set_device_size (DevkitDisksDevice *device, guint64 value);
void devkit_disks_device_set_device_block_size (DevkitDisksDevice *device, guint64 value);
void devkit_disks_device_set_device_is_mounted (DevkitDisksDevice *device, gboolean value);
void devkit_disks_device_set_device_mount_paths (DevkitDisksDevice *device, GStrv value);
void devkit_disks_device_set_device_mounted_by_uid (DevkitDisksDevice *device, guint value);
void devkit_disks_device_set_device_presentation_hide (DevkitDisksDevice *device, gboolean value);
void devkit_disks_device_set_device_presentation_nopolicy (DevkitDisksDevice *device, gboolean value);
void devkit_disks_device_set_device_presentation_name (DevkitDisksDevice *device, const gchar *value);
void devkit_disks_device_set_device_presentation_icon_name (DevkitDisksDevice *device, const gchar *value);

void devkit_disks_device_set_id_usage (DevkitDisksDevice *device, const gchar *value);
void devkit_disks_device_set_id_type (DevkitDisksDevice *device, const gchar *value);
void devkit_disks_device_set_id_version (DevkitDisksDevice *device, const gchar *value);
void devkit_disks_device_set_id_uuid (DevkitDisksDevice *device, const gchar *value);
void devkit_disks_device_set_id_label (DevkitDisksDevice *device, const gchar *value);

void devkit_disks_device_set_partition_slave (DevkitDisksDevice *device, const gchar *value);
void devkit_disks_device_set_partition_scheme (DevkitDisksDevice *device, const gchar *value);
void devkit_disks_device_set_partition_type (DevkitDisksDevice *device, const gchar *value);
void devkit_disks_device_set_partition_label (DevkitDisksDevice *device, const gchar *value);
void devkit_disks_device_set_partition_uuid (DevkitDisksDevice *device, const gchar *value);
void devkit_disks_device_set_partition_flags (DevkitDisksDevice *device, GStrv value);
void devkit_disks_device_set_partition_number (DevkitDisksDevice *device, gint value);
void devkit_disks_device_set_partition_offset (DevkitDisksDevice *device, guint64 value);
void devkit_disks_device_set_partition_size (DevkitDisksDevice *device, guint64 value);

void devkit_disks_device_set_partition_table_scheme (DevkitDisksDevice *device, const gchar *value);
void devkit_disks_device_set_partition_table_count (DevkitDisksDevice *device, gint value);

void devkit_disks_device_set_drive_vendor (DevkitDisksDevice *device, const gchar *value);
void devkit_disks_device_set_drive_model (DevkitDisksDevice *device, const gchar *value);
void devkit_disks_device_set_drive_revision (DevkitDisksDevice *device, const gchar *value);
void devkit_disks_device_set_drive_serial (DevkitDisksDevice *device, const gchar *value);
void devkit_disks_device_set_drive_wwn (DevkitDisksDevice *device, const gchar *value);
void devkit_disks_device_set_drive_connection_interface (DevkitDisksDevice *device, const gchar *value);
void devkit_disks_device_set_drive_connection_speed (DevkitDisksDevice *device, guint value);
void devkit_disks_device_set_drive_media_compatibility (DevkitDisksDevice *device, GStrv value);
void devkit_disks_device_set_drive_media (DevkitDisksDevice *device, const gchar *value);
void devkit_disks_device_set_drive_is_media_ejectable (DevkitDisksDevice *device, gboolean value);
void devkit_disks_device_set_drive_can_detach (DevkitDisksDevice *device, gboolean value);
void devkit_disks_device_set_drive_can_spindown (DevkitDisksDevice *device, gboolean value);
void devkit_disks_device_set_drive_is_rotational (DevkitDisksDevice *device, gboolean value);
void devkit_disks_device_set_drive_rotation_rate (DevkitDisksDevice *device, guint value);
void devkit_disks_device_set_drive_write_cache (DevkitDisksDevice *device, const gchar *value);
void devkit_disks_device_set_drive_adapter (DevkitDisksDevice *device, const gchar *value);
void devkit_disks_device_set_drive_ports (DevkitDisksDevice *device, GStrv value);

void devkit_disks_device_set_optical_disc_is_blank (DevkitDisksDevice *device, gboolean value);
void devkit_disks_device_set_optical_disc_is_appendable (DevkitDisksDevice *device, gboolean value);
void devkit_disks_device_set_optical_disc_is_closed (DevkitDisksDevice *device, gboolean value);
void devkit_disks_device_set_optical_disc_num_tracks (DevkitDisksDevice *device, guint value);
void devkit_disks_device_set_optical_disc_num_audio_tracks (DevkitDisksDevice *device, guint value);
void devkit_disks_device_set_optical_disc_num_sessions (DevkitDisksDevice *device, guint value);

void devkit_disks_device_set_luks_holder (DevkitDisksDevice *device, const gchar *value);

void devkit_disks_device_set_luks_cleartext_slave (DevkitDisksDevice *device, const gchar *value);
void devkit_disks_device_set_luks_cleartext_unlocked_by_uid (DevkitDisksDevice *device, guint value);

void devkit_disks_device_set_linux_md_component_level (DevkitDisksDevice *device, const gchar *value);
void devkit_disks_device_set_linux_md_component_position (DevkitDisksDevice *device, gint value);
void devkit_disks_device_set_linux_md_component_num_raid_devices (DevkitDisksDevice *device, gint value);
void devkit_disks_device_set_linux_md_component_uuid (DevkitDisksDevice *device, const gchar *value);
void devkit_disks_device_set_linux_md_component_home_host (DevkitDisksDevice *device, const gchar *value);
void devkit_disks_device_set_linux_md_component_name (DevkitDisksDevice *device, const gchar *value);
void devkit_disks_device_set_linux_md_component_version (DevkitDisksDevice *device, const gchar *value);
void devkit_disks_device_set_linux_md_component_holder (DevkitDisksDevice *device, const gchar *value);
void devkit_disks_device_set_linux_md_component_state (DevkitDisksDevice *device, GStrv value);

void devkit_disks_device_set_linux_md_state (DevkitDisksDevice *device, const gchar *value);
void devkit_disks_device_set_linux_md_level (DevkitDisksDevice *device, const gchar *value);
void devkit_disks_device_set_linux_md_num_raid_devices (DevkitDisksDevice *device, gint value);
void devkit_disks_device_set_linux_md_uuid (DevkitDisksDevice *device, const gchar *value);
void devkit_disks_device_set_linux_md_home_host (DevkitDisksDevice *device, const gchar *value);
void devkit_disks_device_set_linux_md_name (DevkitDisksDevice *device, const gchar *value);
void devkit_disks_device_set_linux_md_version (DevkitDisksDevice *device, const gchar *value);
void devkit_disks_device_set_linux_md_slaves (DevkitDisksDevice *device, GStrv value);
void devkit_disks_device_set_linux_md_is_degraded (DevkitDisksDevice *device, gboolean value);
void devkit_disks_device_set_linux_md_sync_action (DevkitDisksDevice *device, const gchar *value);
void devkit_disks_device_set_linux_md_sync_percentage (DevkitDisksDevice *device, gdouble value);
void devkit_disks_device_set_linux_md_sync_speed (DevkitDisksDevice *device, guint64 value);

void devkit_disks_device_set_dm_name (DevkitDisksDevice *device, const gchar *value);
void devkit_disks_device_set_slaves_objpath (DevkitDisksDevice *device, GStrv value);
void devkit_disks_device_set_holders_objpath (DevkitDisksDevice *device, GStrv value);

void devkit_disks_device_set_drive_ata_smart_is_available (DevkitDisksDevice *device, gboolean value);
void devkit_disks_device_set_drive_ata_smart_time_collected (DevkitDisksDevice *device, guint64 value);
void devkit_disks_device_set_drive_ata_smart_status (DevkitDisksDevice *device, SkSmartOverall value);
void devkit_disks_device_set_drive_ata_smart_blob_steal (DevkitDisksDevice *device, gchar *blob, gsize blob_size);


G_END_DECLS

#endif /* __DEVKIT_DISKS_DEVICE_PRIVATE_H__ */
