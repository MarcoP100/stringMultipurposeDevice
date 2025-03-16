#!/bin/bash

echo "Avvio fix-eglfs.sh"

# Trova quale cardX ha "DSI" nel percorso
for CARD in /dev/dri/card*; do
    if udevadm info -q all -n $CARD | grep -q "dsi"; then
        DISPLAY_CARD=$(basename $CARD)
        break
    fi
done

echo "Scheda trovata: $DISPLAY_CARD"

if [ -n "$DISPLAY_CARD" ]; then
    echo "{ \"device\": \"/dev/dri/$DISPLAY_CARD\" }" > /home/marco/progettiQt/StringMultipurposeDevice/eglfs.json
    echo "Impostata $DISPLAY_CARD in eglfs.json"
else
    echo "Errore: Nessuna scheda trovata per il display!"
fi
