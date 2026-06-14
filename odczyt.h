#ifndef ODCZYT_H
#define ODCZYT_H

#include <QString>
class OdczytJSON {
public:
    bool wczytaj(const QString& sciezka);
};

class OdczytCSV {
public:
    bool wczytaj(const QString& sciezka);
};

class OdczytXML {
public:
    bool wczytaj(const QString& sciezka);
};

template <typename T>
bool wykonajOdczyt(const QString& sciezka) {
    T czytnik;
    return czytnik.wczytaj(sciezka);
}

#endif // ODCZYT_H