#include "playlist.h"

Playlist::Playlist(QObject *parent)
    : QObject{parent}
{

}

void Playlist::add_song(QString song_name)
{
    songs.push_back(song_name);
}

void Playlist::remove_song(unsigned int index)
{
    songs.erase(songs.begin() + index);
}
QString Playlist::get_random_song()
{
    srand (time(NULL));
    return songs[rand() % songs.size()-1];
}

QString Playlist::get_next_song(unsigned int index)
{
    if(index+1>=songs.size()){
        return songs[0];
    }
    return songs[index+1];
}

QString Playlist::get_previous_song(unsigned int index)
{
    if(index-1<=0){
        return songs[songs.size()-1];
    }
    return songs[index-1];
}

QString Playlist::get_song(unsigned int index)
{
    return songs[index];
}