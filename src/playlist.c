/*
 * $Id$
 *
 * (c) 2004 by Kalle Wallin <kaw@linux.se>
 * (c) 2008 Max Kellermann <max@duempel.org>
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
 * You should have received a copy of the GNU General Public License
 * along with this program; if not, write to the Free Software
 * Foundation, Inc., 59 Temple Place, Suite 330, Boston, MA  02111-1307  USA
 *
 */

#include "playlist.h"
#include "mpdclient.h"
#include "ncmpc.h"

#include <string.h>

#define ENABLE_PLCHANGES

#define MPD_ERROR(c) (c==NULL || c->connection==NULL || c->connection->error)

gint
mpdclient_playlist_free(mpdclient_playlist_t *playlist)
{
	guint i;

	for (i = 0; i < playlist->list->len; ++i) {
		struct mpd_song *song = g_array_index(playlist->list, struct mpd_song *, i);
		mpd_freeSong(song);
	}

	g_array_free(playlist->list, TRUE);
	memset(playlist, 0, sizeof(mpdclient_playlist_t));
	return 0;
}

struct mpd_song *
playlist_get_song(mpdclient_t *c, gint idx)
{
	if (idx < 0 || (guint)idx >= c->playlist.list->len)
		return NULL;

	return g_array_index(c->playlist.list, struct mpd_song *, idx);
}

struct mpd_song *
playlist_lookup_song(mpdclient_t *c, gint id)
{
	guint i;

	for (i = 0; i < c->playlist.list->len; ++i) {
		struct mpd_song *song = g_array_index(c->playlist.list,
						      struct mpd_song *, i);
		if (song->id == id)
			return song;
	}

	return NULL;
}

gint
playlist_get_index(mpdclient_t *c, struct mpd_song *song)
{
	guint i;

	for (i = 0; i < c->playlist.list->len; ++i) {
		if (g_array_index(c->playlist.list, struct mpd_song *, i)
		    == song)
			return (gint)i;
	}

	return -1;
}

gint
playlist_get_index_from_id(mpdclient_t *c, gint id)
{
	guint i;

	for (i = 0; i < c->playlist.list->len; ++i) {
		struct mpd_song *song = g_array_index(c->playlist.list,
						      struct mpd_song *, i);
		if (song->id == id)
			return (gint)i;
	}

	return -1;
}

gint
playlist_get_index_from_file(mpdclient_t *c, gchar *filename)
{
	guint i;

	for (i = 0; i < c->playlist.list->len; ++i) {
		struct mpd_song *song = g_array_index(c->playlist.list,
						      struct mpd_song *, i);
		if(strcmp(song->file, filename) == 0)
			return (gint)i;
	}

	return -1;
}