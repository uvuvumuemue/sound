#include "playlistmanager.h"
PlaylistManager::PlaylistManager() {

}
PlaylistManager& PlaylistManager::getInstance() {
    static PlaylistManager instance;
    return instance;
}

void PlaylistManager::addPlaylist(const QString& name) {
    auto newPlaylist = std::make_unique<Playlist>();
    newPlaylist->name = name;
    m_playlists.push_back(std::move(newPlaylist));
}
size_t PlaylistManager::getPlaylistCount() const {
    return m_playlists.size();
}

Playlist* PlaylistManager::getPlaylist(size_t index) {
    if (index < m_playlists.size()) {
        return m_playlists[index].get();
    }
    return nullptr;
}
void PlaylistManager::removePlaylist(const QString& name) {
    for (size_t i = 0; i < m_playlists.size(); ++i) {
        if (m_playlists[i] && m_playlists[i]->name == name) {
            m_playlists.erase(m_playlists.begin() + i);
            break;
        }
    }
}