#include "servermanager.h"
#include <algorithm>
ServerManager& ServerManager::getInstance() {
    static ServerManager instance;
    return instance;
}

bool ServerManager::addServer(const QString& url) {
    if (std::find(m_servers.begin(), m_servers.end(), url) == m_servers.end()) {
        m_servers.push_back(url);
        return true;
    }
    return false;
}

void ServerManager::removeServer(const QString& url) {
    auto it = std::remove(m_servers.begin(), m_servers.end(), url);
    if (it != m_servers.end()) {
        m_servers.erase(it, m_servers.end());
    }
}

std::vector<QString> ServerManager::getServers() const {
    return m_servers;
}