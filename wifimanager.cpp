#include "wifimanager.h"
#include <QProcess>
#include <QDebug>
#include <QRegularExpression>


WiFiManager::WiFiManager(QObject *parent) : QObject(parent) {
    // Inizializza il processo e connetti il segnale di completamento
    connect(&process, QOverload<int, QProcess::ExitStatus>::of(&QProcess::finished),
            this, &WiFiManager::handleProcessFinished);
    /*connect(&process, &QProcess::readyReadStandardOutput,
            this, &WiFiManager::handleReadyRead);*/

    //connect(&timeoutTimer, &QTimer::timeout, this, &WiFiManager::onTimeout);

    qDebug() << "WiFiManager costruito e segnale 'finished' connesso.";
}

/*void WiFiManager::connectToNetwork(const QString &ssid, const QString &password)
{
    // Utilizzo di un comando di sistema per connettersi alla rete WiFi
    QString command = QString("nmcli dev wifi connect %1 password %2").arg(ssid).arg(password);
    QProcess process;
    process.start(command);
    process.waitForFinished();

    if (process.exitCode() == 0) {
        updateConnectionStatus("Connesso");
    } else {
        updateConnectionStatus("Connessione fallita");
    }
}

void WiFiManager::updateConnectionStatus(const QString &status)
{
    emit connectionStatusChanged(status);
}
*/

void WiFiManager::getAvailableNetworks()
{
    setBusy(true);
    //QProcess process;

    // Ottieni l'ambiente di sistema e aggiungi le variabili necessarie
    //QProcessEnvironment env = QProcessEnvironment::systemEnvironment();
    //process.setProcessEnvironment(env);
    qDebug() << "Chiamata getAvailableNetworks()";
    if (process.state() == QProcess::Running) {
        qDebug() << "Un processo di scansione � gi� in esecuzione, lo fermo...";
        process.terminate();  // Se un processo di scansione � gi� in corso, termina prima di avviarne uno nuovo
        if (!process.waitForFinished(3000)) {  // Attendi al massimo 3 secondi per la terminazione
            qDebug() << "Il processo non ha terminato nei tempi previsti. Procedo con kill().";
            process.kill();  // Se `terminate()` non riesce, forza la terminazione
            process.waitForFinished();
        }
        qDebug() << "Processo di scansione precedente terminato.";
    }else{
       qDebug() << "Nessun processo esistente";
    }
    qDebug() << "Avvio un nuovo processo di scansione delle reti WiFi.";

    QString program = "sudo";
    QStringList arguments;
    arguments << "iwlist" << "wlan0" << "scan";

    process.start(program, arguments);

    if (!process.waitForStarted()) {
        qDebug() << "Il processo iwlist non � riuscito a partire.";
        setBusy(false);
    } else {
        qDebug() << "Processo di scansione avviato.";
        //timeoutTimer.start(5000);
    }


  /*  QStringList networks;
   QString program = "iwlist";
   QStringList arguments;
   arguments << "wlan0" << "scan";

    int exitCode = QProcess::execute(program, arguments);
    if (exitCode == 0) {
        QProcess process;
        process.start("iwlist wlan0 scan");
        process.waitForFinished();
        QString output = process.readAllStandardOutput();
        networks = parseNetworks(output);
        qDebug() << "Reti trovate (sincrono):" << networks;
    } else {
        qDebug() << "Il processo sincrono iwlist non � riuscito a partire.";
    }
    return networks;*/

}

void WiFiManager::handleReadyRead()
{
    QString output = process.readAllStandardOutput();
    qDebug() << "Output (readyRead):" << output;
}

bool WiFiManager::isBusy() const
{
    return m_busy;
}


void WiFiManager::setBusy(bool busy)
{
    if (m_busy != busy) {
        m_busy = busy;
        emit busyChanged(m_busy);
    }
}


void WiFiManager::stopNetworkScan()
{
    qDebug() << "Chiamata stopNetworkScan()";
    if (process.state() == QProcess::Running) {
        qDebug() << "Un processo di scansione � gi� in esecuzione, lo fermo...";
        process.terminate();  // Termina il processo in modo controllato
        if (!process.waitForFinished(2000)) {  // Attendi fino a 2 secondi per la terminazione
            qDebug() << "Il processo non ha terminato nei tempi previsti. Procedo con kill().";
            process.kill();  // Forza la terminazione se non risponde
        }
        qDebug() << "Processo di scansione terminato.";
    } else {
        qDebug() << "Nessun processo di scansione in esecuzione.";
    }
}

void WiFiManager::handleProcessFinished(int exitCode, QProcess::ExitStatus exitStatus)
{
    //timeoutTimer.stop();  // Ferma il timer quando il processo finisce

    setBusy(false);
    qDebug() << "handleProcessFinished() chiamato con codice di uscita:" << exitCode << ", stato di uscita:" << exitStatus;
    if (exitStatus == QProcess::NormalExit && exitCode == 0) {
        QString output = process.readAllStandardOutput();
        qDebug() << "Output iwlist:" << output;  // Stampa l'output grezzo di nmcli
        QStringList networks = parseNetworks(output);
        if (!networks.isEmpty()) {
            qDebug() << "Reti trovate:" << networks;
            emit availableNetworksChanged(networks);
        } else {
            qDebug() << "Nessuna rete trovata. Non emetto il segnale.";
        }

    } else{
        qDebug() << "Errore nel processo nmcli, codice di uscita:" << exitCode;
        emit availableNetworksChanged(QStringList());  // Invia una lista vuota in caso di errore
    }


}

QStringList WiFiManager::parseNetworks(const QString &nmcliOutput)
{
    QSet<QString> uniqueNetworks;  // QSet per evitare duplicati
    QStringList lines = nmcliOutput.split("\n", Qt::SkipEmptyParts);

    // Stampa ogni linea per verificare cosa stiamo analizzando
    /*qDebug() << "Linee output nmcli:";
    for (const QString &line : lines) {
        qDebug() << line;
    }*/

    for (const QString &line : lines) {
        if (line.contains("ESSID:")) {
            QString ssid = line.section("ESSID:", 1, 1).replace("\"", "").trimmed();
            // Filtra eventuali caratteri nulli (\x00) o SSID vuoti
            if (!ssid.isEmpty() && !ssid.contains("\\x00")) {
                uniqueNetworks.insert(ssid);
            }
        }
    }

    QStringList networks = QStringList(uniqueNetworks.begin(), uniqueNetworks.end());
    qDebug() << "Reti trovate:" << networks;  // Stampa la lista delle reti trovate
    return networks;
}


/*void WiFiManager::onTimeout()
{
    qDebug() << "Timeout del processo raggiunto. Termino il processo iwlist.";
    if (process.state() == QProcess::Running) {
        process.kill();  // Forza la terminazione del processo
        process.waitForFinished();  // Assicurati che il processo sia completamente terminato
        qDebug() << "Processo terminato a causa del timeout.";
    }
}
*/

void WiFiManager::connectToNetwork(const QString &ssid, const QString &password)
{
    // Costruisci il comando di connessione (usiamo nmcli come esempio)
    QString command = QString("nmcli dev wifi connect %1").arg(ssid);
    if (!password.isEmpty()) {
        command += QString(" password %1").arg(password);
    }

    QProcess process;
    process.start(command);
    process.waitForFinished();

    if (process.exitCode() == 0) {
        emit connectionStatusChanged("Connesso");
    } else {
        emit connectionStatusChanged("Connessione fallita");
    }
}



WiFiManager::~WiFiManager() {
    qDebug() << "DIstruttore";
    if (process.state() == QProcess::Running) {
        process.kill();  // Termina il processo se � ancora in esecuzione
        process.waitForFinished();  // Attendi la terminazione
    }
}
