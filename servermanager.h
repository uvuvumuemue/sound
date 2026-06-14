#ifndef SERVERMANAGER_H
#define SERVERMANAGER_H

#include <vector>
#include <QString>

class ServerManager {
private:
    std::vector<QString> m_servers;
    ServerManager() {}
    ServerManager(const ServerManager&) = delete;
    ServerManager& operator=(const ServerManager&) = delete;

public:
    static ServerManager& getInstance();
    bool addServer(const QString& url);
    void removeServer(const QString& url);

    std::vector<QString> getServers() const;
};

#endif // SERVERMANAGER_H