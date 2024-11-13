function startNetworkScan(wifiListModel) {
    if (WiFiManager) {
        WiFiManager.scanNetworks();
        if (wifiListModel) {
            wifiListModel.clear();  // Pulisci il modello per inserire nuovi risultati
        }
    } else {
        console.log("WiFiManager non disponibile");
    }
}
