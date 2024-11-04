#include "wifimanager.h"
#include <QProcess>
#include <QDebug>
#include <QRegularExpression>


WiFiManager::WiFiManager(QObject *parent) : QObject(parent) {
    // Inizializza il processo e connetti il segnale di completamento
    connect(&scanProcess, QOverload<int, QProcess::ExitStatus>::of(&QProcess::finished),
            this, &WiFiManager::handleProcessFinished);
    /*connect(&process, &QProcess::readyReadStandardOutput,
            this, &WiFiManager::handleReadyRead);*/

    //connect(&timeoutTimer, &QTimer::timeout, this, &WiFiManager::onTimeout);

    qDebug() << "WiFiManager costruito e segnale 'finished' connesso.";


    // Configura il timer di connessione
    connectionTimer.setInterval(this->CONNECT_TIMEOUT_MS); // Timeout di 30 secondi
    connectionTimer.setSingleShot(true);
    connect(&connectionTimer, &QTimer::timeout, this, &WiFiManager::handleConnectionTimeout);

    // Configura il timer di scansione delle reti
    scanNetworksTimer.setInterval(100); //
    scanNetworksTimer.setSingleShot(true);
    connect(&scanNetworksTimer, &QTimer::timeout, this, &WiFiManager::handleSavedNetworksTimeout);



}






// Emetti il segnale con lo stato della connessione
void WiFiManager::emitConnectionStatus(ConnectionStatus status) {
    emit connectionStatusChanged(status);
}

void WiFiManager::scanNetworks()
{
    setBusy(true);
    if (this->scanForSavedNetworks){
        getSavedNetworks();

    } else {
        startScanNetworks();
    }
}

void WiFiManager::runIwlistCommand() {
    QString program = "sudo";
    QStringList arguments;
    arguments << "iwlist" << "wlan0" << "scan";

    scanProcess.start(program, arguments);

    if (!scanProcess.waitForStarted()) {
        qDebug() << "Il processo iwlist non � riuscito a partire.";
        setBusy(false);
    } else {
        qDebug() << "Processo di scansione avviato.";
        //timeoutTimer.start(5000);
    }
}

void WiFiManager::startScanNetworks()
{
    setBusy(true);
    //QProcess process;

    // Ottieni l'ambiente di sistema e aggiungi le variabili necessarie
    //QProcessEnvironment env = QProcessEnvironment::systemEnvironment();
    //process.setProcessEnvironment(env);
    qDebug() << "Chiamata getAvailableNetworks()";
    if (scanProcess.state() == QProcess::Running) {
        qDebug() << "Un processo di scansione � gi� in esecuzione, lo fermo...";
        scanProcess.terminate();  // Se un processo di scansione � gi� in corso, termina prima di avviarne uno nuovo
        if (!scanProcess.waitForFinished(3000)) {  // Attendi al massimo 3 secondi per la terminazione
            qDebug() << "Il processo non ha terminato nei tempi previsti. Procedo con kill().";
            scanProcess.kill();  // Se `terminate()` non riesce, forza la terminazione
            scanProcess.waitForFinished();
        }
        qDebug() << "Processo di scansione precedente terminato.";
    }else{
       qDebug() << "Nessun processo esistente";
    }
    qDebug() << "Avvio un nuovo processo di scansione delle reti WiFi.";

    runIwlistCommand();

}

void WiFiManager::handleReadyRead()
{
    QString output = scanProcess.readAllStandardOutput();
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
    if (scanProcess.state() == QProcess::Running) {
        qDebug() << "Un processo di scansione � gi� in esecuzione, lo fermo...";
        scanProcess.terminate();  // Termina il processo in modo controllato
        if (!scanProcess.waitForFinished(2000)) {  // Attendi fino a 2 secondi per la terminazione
            qDebug() << "Il processo non ha terminato nei tempi previsti. Procedo con kill().";
            scanProcess.kill();  // Forza la terminazione se non risponde
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
        QString iwlistOutput = scanProcess.readAllStandardOutput();
        qDebug() << "Output iwlist:" << iwlistOutput;  // Stampa l'output grezzo di nmcli
        QList<NetworkInfo> networks = parseNetworks(iwlistOutput);

        if (!networks.isEmpty()) {

            QVariantList variantNetworks;
            for (const NetworkInfo &network : networks) {
                variantNetworks.append(toVariantMap(network));
            }

            qDebug() << "Reti trovate:" << variantNetworks;
            emit availableNetworksChanged(variantNetworks);
        } else {
            qDebug() << "Nessuna rete trovata. Non emetto il segnale.";
        }

    } else{
        qDebug() << "Errore nel processo nmcli, codice di uscita:" << exitCode;
        emit availableNetworksChanged(QVariantList());  // Invia una lista vuota in caso di errore
    }


}


QList<WiFiManager::NetworkInfo> WiFiManager::parseNetworks(const QString &iwlistOutput) {
    QList<NetworkInfo> networks;
    QSet<QString> uniqueNetworks;

    QStringList lines = iwlistOutput.split("\n", Qt::SkipEmptyParts);
    NetworkInfo currentNetwork;

    for (const QString &line : lines) {
        if (line.contains("ESSID:")) {
            currentNetwork.ssid = line.section("ESSID:", 1, 1).replace("\"", "").trimmed();
            currentNetwork.isSaved = this->savedNetworks.contains(currentNetwork.ssid);
            qDebug() << "Rete trovata:" << currentNetwork.ssid << (currentNetwork.isSaved ? "(Salvata)" : "(Non salvata)");


        }
        if (line.contains("Encryption key:on")) {
            currentNetwork.requiresPassword = true;
        }
        if (line.contains("Cell")) {
            if (!currentNetwork.ssid.isEmpty() &&
                !currentNetwork.ssid.contains("\\x00") &&
                !uniqueNetworks.contains(currentNetwork.ssid)) {

                networks.append(currentNetwork);


                uniqueNetworks.insert(currentNetwork.ssid);
            }
            // Reset per la prossima cella
            currentNetwork = NetworkInfo();
        }
    }

    // fine linee, salvo se ho trovato una rete valida
    if (!currentNetwork.ssid.isEmpty() &&
        !currentNetwork.ssid.contains("\\x00") &&
        !uniqueNetworks.contains(currentNetwork.ssid)) {

        networks.append(currentNetwork);
        uniqueNetworks.insert(currentNetwork.ssid);
    }
    // Stampa dell'intera lista
    for (const NetworkInfo &network : networks) {
        qDebug() << network.isSaved;
    }
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

    // Controlla se c'� gi� un processo di connessione in corso
    if (this->connectProcess.state() == QProcess::Running) {
        qDebug() << "Connessione gi� in corso. Attendere il completamento.";
        //emit connectionStatusChanged("Connessione gi� in corso");
        return;
    }

    // Escapa l'SSID per gestire eventuali caratteri speciali
    QString escapedSsid = ssid;
    escapedSsid.replace("\"", "\\\""); // Sostituisce le virgolette con sequenze di escape

    // Controllo SSID non vuoto
    if (escapedSsid.isEmpty()) {
        qDebug() << "Errore: SSID non valido";
        emitConnectionStatus(WiFiManager::SsidEmpty);
        return;
    }

    // Definisci il programma e gli argomenti per QProcess
    QString program = "sudo";
    QStringList arguments;
    arguments << "nmcli" << "dev" << "wifi" << "connect" << escapedSsid;

    if (!password.isEmpty()) {
        arguments << "password" << password;
    }

    // Collegamento del segnale finished per evitare il blocco
    connect(&this->connectProcess, QOverload<int, QProcess::ExitStatus>::of(&QProcess::finished),
            this, &WiFiManager::handleConnectionFinished, Qt::UniqueConnection);

    // Avvia il processo in modo asincrono
    QString commandString = program + " " + arguments.join(" ");
    if (!password.isEmpty()) {
        commandString.replace(password, "****");
    }

    qDebug() << "Eseguendo il comando:" << commandString;
    this->connectionKilled = false;
    this->connectProcess.start(program, arguments);

    // Verifica se il processo � stato avviato correttamente
    if (!this->connectProcess.waitForStarted()) {
        qDebug() << "Errore: impossibile avviare il processo";
        this->connectProcess.kill();
        this->connectionKilled = true;
        emitConnectionStatus(WiFiManager::ErrorProcessStart);
        return;
    }

    if (this->connectionTimer.isActive()) {
        this->connectionTimer.stop();
    }
    // Avvia il timer di connessione
    this->connectionTimer.start();


    // Aggiorna lo stato a "in connessione" appena il processo viene avviato
   emitConnectionStatus(WiFiManager::Connecting);
}


void WiFiManager::handleConnectionTimeout() {
    if (this->connectProcess.state() == QProcess::Running) {
        this->connectProcess.kill();
        this->connectionKilled = true;
        qDebug() << "Errore: il processo di connessione ha superato il timeout";
        emitConnectionStatus(WiFiManager::Timeout);
    }
}

void WiFiManager::handleConnectionFinished(int exitCode, QProcess::ExitStatus exitStatus)
{
    if (this->connectionTimer.isActive()) {
        this->connectionTimer.stop();
    }
    if (this->connectionKilled) {
        // Se il processo � stato terminato dal timeout, non emettere ulteriori segnali
        return;
    }

    if (exitCode == 0 && exitStatus == QProcess::NormalExit) {
        qDebug() << "Connesso";
        this->scanForSavedNetworks = true;
        emitConnectionStatus(WiFiManager::Connected);
    } else {
        QString errorOutput = this->connectProcess.readAllStandardError();
        qDebug() << "Connessione fallita, errore:" << errorOutput;

        if (errorOutput.contains("Secrets were required, but not provided") ||
            (errorOutput.contains("802-11-wireless-security.psk: property is invalid"))) {
            // Errore specifico per password errata o mancante
            emitConnectionStatus(WiFiManager::WrongPassword); // Emetti il segnale per richiedere la password
        } else {
            qDebug() << "Errore di connessione generico.";
            emitConnectionStatus(WiFiManager::ConnectionFailed);
        }
    }
}


QVariantMap WiFiManager::toVariantMap(const NetworkInfo &network) {
    QVariantMap map;
    map["ssid"] = network.ssid;
    map["requiresPassword"] = network.requiresPassword;
    map["networkKnown"] = network.isSaved;
    return map;
}


void WiFiManager::getSavedNetworks()
{

    // Controlla se c'� gi� un processo di connessione in corso
    if (this->savedNetworksProcess.state() == QProcess::Running) {
        emit savedNetworksChanged("Ricerca gi� in corso");
        return;
    }

    // Definisci il programma e gli argomenti per QProcess
    QString program = "nmcli";
    QStringList arguments;
    arguments << "-t" << "-f" << "NAME,UUID,TYPE" << "connection" << "show";

    // Collegamento del segnale finished per evitare il blocco
    connect(&this->savedNetworksProcess, QOverload<int, QProcess::ExitStatus>::of(&QProcess::finished),
            this, &WiFiManager::handleSavedNetworksFinished, Qt::UniqueConnection);

    // Avvia il processo in modo asincrono
    qDebug() << "Eseguendo il comando ricerca reti salvate";
    this->savedNetworksProcess.start(program, arguments);

    // Verifica se il processo � stato avviato correttamente
    if (!this->savedNetworksProcess.waitForStarted()) {
        qDebug() << "Errore: impossibile avviare il processo. Riprovo...";
        for (int i = 0; i < 3; ++i) { // Tentativi di riavvio del processo
            qDebug() << "Tentativo " << (i + 1) << " di avvio del processo";
            this->savedNetworksProcess.start(program, arguments);
            if (this->savedNetworksProcess.waitForStarted()) {
                break;
            }
        }
        if (this->savedNetworksProcess.state() != QProcess::Running) {
            qDebug() << "Errore: impossibile avviare il processo dopo diversi tentativi";
            this->savedNetworksProcess.kill();
            return;
        }
    }


    // timer
    if (this->knownNetworksTimer.isActive()) {
        this->knownNetworksTimer.stop();
    }
    // Avvia il timer di connessione
    this->knownNetworksTimer.start();

    // Aggiorna lo stato a "in connessione" appena il processo viene avviato
    emit savedNetworksChanged("In corso");
}


void WiFiManager::handleSavedNetworksTimeout() {
    if (this->savedNetworksProcess.state() == QProcess::Running) {
        this->savedNetworksProcess.kill();
        qDebug() << "Errore: il processo ha superato il timeout";
    }
}



void WiFiManager::handleSavedNetworksFinished(int exitCode, QProcess::ExitStatus exitStatus)
{
    if (this->knownNetworksTimer.isActive()) {
        this->knownNetworksTimer.stop();
    }
    if (exitCode == 0 && exitStatus == QProcess::NormalExit) {
        qDebug() << "Ricerca finita";
        QString output = this->savedNetworksProcess.readAllStandardOutput();
        qDebug() << "output:" << output;

        savedNetworks.clear();

        QStringList lines = output.split("\n");
        for (const QString &line : lines) {
            QStringList fields = line.split(":"); // Dividi i campi (in formato "NAME:TYPE")
            if (fields.size() == 3) {
                QString name = fields[0].trimmed();
                QString uuid = fields[1].trimmed();
                QString type = fields[2].trimmed();

                if (type == "802-11-wireless" && !name.isEmpty()) {
                    // Usa nmcli per ottenere direttamente l'SSID della connessione
                    QProcess ssidProcess;
                    QStringList ssidArguments;
                    ssidArguments << "-t" << "-f" << "802-11-wireless.ssid" << "connection" << "show" << uuid;
                    ssidProcess.start("nmcli", ssidArguments);
                    ssidProcess.waitForFinished();
                    QString ssid = ssidProcess.readAllStandardOutput().trimmed();
                    ssid = ssid.section(":", 1).trimmed();

                    qDebug() << "SSID trovato:" << ssid;
                    savedNetworks.append(ssid);
                }
            }
        }
        qDebug() << "Reti trovate:" << savedNetworks;
        this->scanForSavedNetworks = false;
        emit savedNetworksChanged("Ricerca finita");


    } else {
        qDebug() << "Ricerca fallita";
        qDebug() << "Errore connessione: " << this->savedNetworksProcess.readAllStandardError();
        savedNetworks.clear();
        emit savedNetworksChanged("Ricerca fallita");

    }
    startScanNetworks();
}


WiFiManager::~WiFiManager() {
    qDebug() << "DIstruttore";
    if (scanProcess.state() == QProcess::Running) {
        scanProcess.kill();  // Termina il processo se � ancora in esecuzione
        scanProcess.waitForFinished();  // Attendi la terminazione
    }
}