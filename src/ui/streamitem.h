#ifndef STREAMITEM_H
#define STREAMITEM_H

#include <QListWidgetItem>
#include <QObject>
#include <QEvent>
#include <QDebug>
#include "../model/channel.h"

class Channel;

class StreamItem: public QObject, public QListWidgetItem{

    Q_OBJECT
protected:
    Channel* channel;

public:
    //explicit StreamItem(QWidget *parent = 0);
    StreamItem(Channel*);
    ~StreamItem();

    bool online() const;

    const QString getName();

    const QString getUriName();

    const QString getInfo();

    const QString getGame();

    virtual bool operator< (const QListWidgetItem& other) const;

    Channel* getChannel() const;

public slots:
        void update();
        void updateIcon();
};

#endif // STREAMITEM_H