# 3D Fight

## Gamepad

Damit das Gamepad verwendet werden kann, muss die Qt Bibliothek Qt Gamepad installiert sein.
Nur bestimmte Qt Versionen enthalten diese Bibliothek.

- Download `qt-opensource-linux-x64-5.12.2.run`: [Qt 5.12.2](http://download.qt.io/official_releases/qt/5.12/5.12.2/)
- Ausführbar machen:
```bash
chmod +x qt-opensource-linux-x64-5.12.2.run
```
- Ausführen:
```bash
./qt-opensource-linux-x64-5.12.2.run
```
- Bei `Komponenten auswählen` muss der Haken bei `Qt 5.12.2 / Desktop gcc 64-bit` gesetzt werden
- Hinzufügen des Installationspfades zur Umgebungsvariable `PATH`:
```bash
gedit ~/.profile
```
Füge diese Zeile hinzu:
```
export PATH="$HOME/Qt5.12.2/5.12.2/gcc_64/lib/cmake:$PATH"
```
Danach neu einloggen, um die Änderungen wirksam zu machen
- Check, ob die Umgebungsvariable gesetzt ist:
```bash
env
```
müsste folgendes Ergebnis liefern:
```
PATH=/home/.../Qt5.12.2/5.12.2/gcc_64/lib/cmake:...
```
- Falls eine andere Qt Version installiert ist, die der neuen in die Quere kommt, kann sie deinstalliert werden:
```bash
sudo apt remove qt*dev
```

## Steuerung

### Tastatur:
- `W`, `A`, `S`, `D`, `Q` und `E`: Lenken
- `Shift`: Schnell fliegen
- `Space`: Schießen
- `R`: Verfolgungsrakete schießen

### Gamepad:
- `L`, `L1` und `R1`: Lenken
- `R2`: Schnell fliegen
- `X`: Schießen
- `O`: Verfolgungsrakete schießen