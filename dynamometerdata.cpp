#include "dynamometerdata.h"
#include <QDebug>


dynamometerData::dynamometerData(QObject *parent) : QObject(parent) {
    setupDynaTimers();
}


QString dynamometerData::getDynValue() const { return m_value; }

QString dynamometerData::getDynState() const { return m_state; }

int dynamometerData::getPacketsLost() const { return m_packetsLost; }


void dynamometerData::setValue(const QString &value) {
    if (m_value != value) {
        m_value = value;
        qDebug() << "emit valueChanged:" << m_value;
        emit valueChanged();
    }
}

void dynamometerData::setState(const QString &state) {
    if (m_state != state) {
        m_state = state;
        qDebug() << "emit stateChanged:" << m_state;
        emit stateChanged();
    }
}

void dynamometerData::updateFromMessage(const QString &state, QString &value) {
    qDebug() << "updateFromMessage: stato=" << state << "valore=" << value;
    setState(state);
    setValue(value);
    startTimeout();
}


void dynamometerData::handleTimeout() {
    setValue("------");
    setState("X");  // oppure "Timeout" o stato dedicato
}


void dynamometerData::updatePacketsLost(const int &packets) {
    if (m_packetsLost != packets) {
        m_packetsLost = packets;
        emit packetsLostChanged();
    }
}

void dynamometerData::setupDynaTimers(){
    // timeout messaggi validi
    timeoutTimer = new QTimer(this);
    timeoutTimer->setInterval(5000);  // 5s di timeout
    timeoutTimer->setSingleShot(true);

    connect(timeoutTimer, &QTimer::timeout, this, [=]() {
        handleTimeout();
    });
}

void dynamometerData::startTimeout() {
    timeoutTimer->start();
}

