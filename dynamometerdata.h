#ifndef DYNAMOMETERDATA_H
#define DYNAMOMETERDATA_H

#include <QObject>
#include <QString>
#include <QTimer>



class dynamometerData : public QObject
{
    Q_OBJECT

    Q_PROPERTY(QString dynValue READ getDynValue NOTIFY valueChanged)
    Q_PROPERTY(QString dynState READ getDynState NOTIFY stateChanged)
    Q_PROPERTY(int packetsLost READ getPacketsLost NOTIFY packetsLostChanged)

public:
    explicit dynamometerData(QObject *parent = nullptr);


    QString getDynValue() const;
    QString getDynState() const;
    int getPacketsLost() const;


public slots:
    void updateFromMessage(const QString &state, QString &value);
    void updatePacketsLost(const int &packets);
    void handleTimeout();


signals:
    void valueChanged();
    void stateChanged();
    void packetsLostChanged();

private:
    QString m_value;
    QString m_state;
    int m_packetsLost = 0;
    QTimer *timeoutTimer;


    void setValue(const QString &value);
    void setState(const QString &state);

    void setupDynaTimers();
    void startTimeout();

};

#endif // DYNAMOMETERDATA_H
