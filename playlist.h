#ifndef PLAYLIST_H
#define PLAYLIST_H

#include <QObject>
#include <stdlib.h>
#include <time.h>
#include <vector>
class Playlist : public QObject
{
    Q_OBJECT
public:
    QString name;
    std::vector<QString> songs;
    explicit Playlist(QObject *parent = nullptr);
public slots:
    Q_INVOKABLE void add_song(QString song_name);
    Q_INVOKABLE void remove_song(unsigned int index);
    Q_INVOKABLE QString get_random_song();
    Q_INVOKABLE QString get_next_song(unsigned int index);
    Q_INVOKABLE QString get_previous_song(unsigned int index);
    Q_INVOKABLE QString get_song(unsigned int index);

};

#endif // PLAYLIST_H