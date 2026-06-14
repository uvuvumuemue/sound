#include "zapis.h"
#include "playlistmanager.h"
#include <QFile>
#include <QTextStream>
#include <QJsonDocument>
#include <QJsonObject>
#include <QJsonArray>
#include <QXmlStreamWriter>

bool ZapisJSON::zapisz(const QString& sciezka) {
    QJsonArray playlistsArray;
    size_t count = PlaylistManager::getInstance().getPlaylistCount();

    for (size_t i = 0; i < count; ++i) {
        Playlist* p = PlaylistManager::getInstance().getPlaylist(i);
        if (!p) continue;

        QJsonObject playlistObj;
        playlistObj["name"] = p->name;

        QJsonArray songsArray;
        for (const QString& song : p->songs) {
            songsArray.append(song);
        }
        playlistObj["songs"] = songsArray;
        playlistsArray.append(playlistObj);
    }

    QFile file(sciezka);
    if (!file.open(QIODevice::WriteOnly)) return false;

    file.write(QJsonDocument(playlistsArray).toJson());
    return true;
}

bool ZapisCSV::zapisz(const QString& sciezka) {
    QFile file(sciezka);
    if (!file.open(QIODevice::WriteOnly | QIODevice::Text)) return false;
    QTextStream out(&file);

    size_t count = PlaylistManager::getInstance().getPlaylistCount();
    for (size_t i = 0; i < count; ++i) {
        Playlist* p = PlaylistManager::getInstance().getPlaylist(i);
        if (!p) continue;
        if (p->songs.empty()) {
            out << p->name << ",\n";
        } else {
            for (const QString& song : p->songs) {
                out << p->name << "," << song << "\n";
            }
        }
    }
    return true;
}
bool ZapisXML::zapisz(const QString& sciezka) {
    QFile file(sciezka);
    if (!file.open(QIODevice::WriteOnly | QIODevice::Text)) return false;

    QXmlStreamWriter xmlWriter(&file);
    xmlWriter.setAutoFormatting(true);

    xmlWriter.writeStartDocument();
    xmlWriter.writeStartElement("Playlists");

    size_t count = PlaylistManager::getInstance().getPlaylistCount();
    for (size_t i = 0; i < count; ++i) {
        Playlist* p = PlaylistManager::getInstance().getPlaylist(i);
        if (!p) continue;

        xmlWriter.writeStartElement("Playlist");
        xmlWriter.writeAttribute("name", p->name);

        for (const QString& song : p->songs) {
            xmlWriter.writeTextElement("Song", song);
        }

        xmlWriter.writeEndElement();
    }

    xmlWriter.writeEndElement();
    xmlWriter.writeEndDocument();

    return true;
}