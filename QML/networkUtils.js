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


function updateNetworkList(networks, wifiListModel) {

    if (!networks || networks.length === 0) {
            console.warn("Nessuna rete disponibile.");
            return;
        }

    wifiListModel.clear();
    // Mappa per evitare duplicati e per mantenere traccia degli elementi attuali
    var existingNetworks = {};
    for (var j = 0; j < wifiListModel.count; j++) {
        existingNetworks[wifiListModel.get(j).ssid] = true;
    }

    // Aggiungi solo le reti che non sono gi� presenti nel modello
    for (var i = 0; i < networks.length; i++) {
        var network = networks[i];
        if (!existingNetworks[network.ssid]) {
            wifiListModel.append({
                ssid: network.ssid,
                requiresPassword: network.requiresPassword,
                networkKnown: network.networkKnown
            });
        }
    }

    // Rimuovi le reti che non sono pi� presenti
    for (var k = wifiListModel.count - 1; k >= 0; k--) {
        var ssid = wifiListModel.get(k).ssid;
        var found = false;
        for (var m = 0; m < networks.length; m++) {
            if (networks[m].ssid === ssid) {
                found = true;
                break;
            }
        }
        if (!found) {
            wifiListModel.remove(k);
        }
    }
}
