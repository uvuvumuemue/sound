#include "odczyt.h"
#include "playlistmanager.h"
#include <QFile>
#include <QTextStream>
#include <QJsonDocument>
#include <QJsonObject>
#include <QJsonArray>
#include <QXmlStreamReader>


bool OdczytJSON::wczytaj(const QString& sciezka) {
    QFile file(sciezka);
    if (!file.open(QIODevice::ReadOnly)) return false;

    QJsonDocument doc = QJsonDocument::fromJson(file.readAll());
    QJsonArray playlistsArray = doc.array();

    for (int i = 0; i < playlistsArray.size(); ++i) {
        QJsonObject obj = playlistsArray[i].toObject();
        PlaylistManager::getInstance().addPlaylist(obj["name"].toString());
        Playlist* p = PlaylistManager::getInstance().getPlaylist(PlaylistManager::getInstance().getPlaylistCount() - 1);
        QJsonArray songsArray = obj["songs"].toArray();
        for (int j = 0; j < songsArray.size(); ++j) {
            if (p) p->add_song(songsArray[j].toString());
        }
    }
    return true;
}

bool OdczytCSV::wczytaj(const QString& sciezka) {
    QFile file(sciezka);
    if (!file.open(QIODevice::ReadOnly | QIODevice::Text)) return false;
    QTextStream in(&file);

    QString currentPlaylist = "";
    Playlist* p = nullptr;
    while (!in.atEnd()) {
        QStringList parts = in.readLine().split(",");
        if (parts.size() >= 1 && !parts[0].isEmpty()) {
            if (parts[0] != currentPlaylist) {
                PlaylistManager::getInstance().addPlaylist(parts[0]);
                p = PlaylistManager::getInstance().getPlaylist(PlaylistManager::getInstance().getPlaylistCount() - 1);
                currentPlaylist = parts[0];
            }
            if (parts.size() >= 2 && !parts[1].isEmpty() && p) {
                p->add_song(parts[1]);
            }
        }
    }
    return true;
}
bool OdczytXML::wczytaj(const QString& sciezka) {
    QFile file(sciezka);
    if (!file.open(QIODevice::ReadOnly | QIODevice::Text)) return false;

    QXmlStreamReader xmlReader(&file);
    Playlist* p = nullptr;

    while (!xmlReader.atEnd() && !xmlReader.hasError()) {
        QXmlStreamReader::TokenType token = xmlReader.readNext();
        if (token == QXmlStreamReader::StartElement) {

            if (xmlReader.name().toString() == "Playlist") {
                QString name = xmlReader.attributes().value("name").toString();
                PlaylistManager::getInstance().addPlaylist(name);
                p = PlaylistManager::getInstance().getPlaylist(PlaylistManager::getInstance().getPlaylistCount() - 1);
            }
            else if (xmlReader.name().toString() == "Song" && p) {
                p->add_song(xmlReader.readElementText());
            }
        }
    }
    return !xmlReader.hasError();
}