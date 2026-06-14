#ifndef PLAYLISTMANAGER_H
#define PLAYLISTMANAGER_H

#include <vector>
#include <memory>
#include <QString>
#include "playlist.h"

class PlaylistManager {
private:
    std::vector<std::unique_ptr<Playlist>> m_playlists;
    PlaylistManager();
    PlaylistManager(const PlaylistManager&) = delete;
    PlaylistManager& operator=(const PlaylistManager&) = delete;

public:
    static PlaylistManager& getInstance();
    void removePlaylist(const QString& name);
    void addPlaylist(const QString& name);
    size_t getPlaylistCount() const;
    Playlist* getPlaylist(size_t index);
};

#endif // PLAYLISTMANAGER_H