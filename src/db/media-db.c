/*
 * media-db.c
 * 
 * Copyright 2013 Ikey Doherty <ikey.doherty@gmail.com>
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
 * Foundation, Inc., 51 Franklin Street, Fifth Floor, Boston,
 * MA 02110-1301, USA.
 * 
 * 
 */
#include "media-db.h"

G_DEFINE_TYPE_WITH_PRIVATE(MediaDB, media_db, G_TYPE_OBJECT);

/* MediaInfo API */
void free_media_info(gpointer p_info)
{
        MediaInfo *info;

        info = (MediaInfo*)p_info;
        if (info->title)
                g_free(info->title);
        if (info->artist)
                g_free(info->artist);
        if (info->album)
                g_free(info->album);
        if (info->genre)
                g_free(info->genre);
        if (info->path)
                g_free(info->path);
}

/* Initialisation */
static void media_db_class_init(MediaDBClass *klass)
{
        GObjectClass *g_object_class;

        g_object_class = G_OBJECT_CLASS(klass);
        g_object_class->dispose = &media_db_dispose;
}

static void media_db_init(MediaDB *self)
{
        const gchar *config;
        self->priv = media_db_get_instance_private(self);

        /* Our storage location */
        config = g_get_user_config_dir();
        self->priv->storage_path = g_strdup_printf("%s/%s", config,
                CONFIG_NAME);

        /* Open the database */
        self->priv->db = gdbm_open(self->priv->storage_path, 0,
                GDBM_WRCREAT, 0600, NULL);
        if (!self->priv->db)
                g_error("Failed to initialise database!");
}

static void media_db_dispose(GObject *object)
{
        MediaDB *self;

        self = MEDIA_DB(object);
        if (self->priv->storage_path)
                g_free(self->priv->storage_path);

        gdbm_close(self->priv->db);
        /* Destruct */
        G_OBJECT_CLASS (media_db_parent_class)->dispose (object);
}

/* Utility; return a new MediaDB */
MediaDB* media_db_new(void)
{
        MediaDB *self;

        self = g_object_new(MEDIA_DB_TYPE, NULL);
        return MEDIA_DB(self);
}
