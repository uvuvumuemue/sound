#ifndef ZAPIS_H
#define ZAPIS_H

#include <QString>

class Zapis {
public:
    virtual ~Zapis() = default;

    virtual bool zapisz(const QString& sciezka) = 0;
};

class ZapisJSON : public Zapis {
public:
    bool zapisz(const QString& sciezka) override;
};

class ZapisCSV : public Zapis {
public:
    bool zapisz(const QString& sciezka) override;
};

class ZapisXML : public Zapis {
public:
    bool zapisz(const QString& sciezka) override;
};

#endif // ZAPIS_H