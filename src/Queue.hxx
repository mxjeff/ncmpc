/* ncmpc (Ncurses MPD Client)
 * (c) 2004-2018 The Music Player Daemon Project
 * Project homepage: http://musicpd.org
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
 * You should have received a copy of the GNU General Public License along
 * with this program; if not, write to the Free Software Foundation, Inc.,
 * 51 Franklin Street, Fifth Floor, Boston, MA 02110-1301 USA.
 */

#ifndef QUEUE_HXX
#define QUEUE_HXX

#include "Compiler.h"

#include <mpd/client.h>

#include <vector>
#include <memory>

#include <assert.h>

struct SongDeleter {
	void operator()(struct mpd_song *song) const {
		mpd_song_free(song);
	}
};

struct MpdQueue {
	/* queue version number (obtained from mpd_status) */
	unsigned version = 0;

	using Vector = std::vector<std::unique_ptr<struct mpd_song, SongDeleter>>;

	/* the list */
	Vector items;

	using size_type = Vector::size_type;

	size_type size() const {
		return items.size();
	}

	bool empty() const {
		return items.empty();
	}

	/** remove and free all songs in the playlist */
	void clear();

	const struct mpd_song &operator[](size_type i) const {
		assert(i < size());

		return *items[i];
	}

	struct mpd_song &operator[](size_type i) {
		assert(i < size());

		return *items[i];
	}

	gcc_pure
	const struct mpd_song *GetChecked(int i) const;

	void push_back(const struct mpd_song &song) {
		items.emplace_back(mpd_song_dup(&song));
	}

	void Replace(size_type i, const struct mpd_song &song) {
		items[i].reset(mpd_song_dup(&song));
	}

	void RemoveIndex(size_type i) {
		items.erase(std::next(items.begin(), i));
	}

	void Move(unsigned dest, unsigned src);

	gcc_pure
	int Find(const struct mpd_song &song) const;

	gcc_pure
	int FindId(unsigned id) const;

	gcc_pure
	int FindUri(const char *uri) const;

	gcc_pure
	int FindUri(const struct mpd_song &song) const {
		return FindUri(mpd_song_get_uri(&song));
	}
};

#endif