# Wörterbuch mit C und gtk+
Beleg für das Modul Programmierung 1 (I120) bei Prof. Dr.-Ing. Arnold Beck

von Christoph Born

![Screenshot](docs/main%20window.png)

## Aufgabenstellung

**Variante Aufgabenstellung:** Wörterbuch deu-engl / engl-deu  
**Variante Bedienoberfläche:** gtk+

### Anforderungen

- Verwaltung von Datensätzen, die Verbindung aus deutschem und englischem Wort speichern

- Operationen mit Datensätzen:
  - Erfassen
  - Löschen
  - Suchen
  - Anzeigen: sortierte tabellarische Auflistung aller Daten  
    (wahlweise nach englischer/deutscher Sortierung)

- Datenspeicherung:
  - programmintern durch geeignete Datenstrukturen  
    (Bereitstellung von Speicherbereichen in der erforderlichen Größe per malloc/free für alle verwalteten Daten, insbesondere für Zeichenketten)

  - in Datei(-en)  
    (beim Verlassen des Programms speichern, sofern sich die Daten beim Programmlauf verändert haben)

- Programm sinnvoll in mindestens 3 C-Module eingeteilt
  - getrennt kompilierbar
  - je ein c- und Headerfile

- Quelltexte sorgsam kommentiert (einschließlich Urheberschaft im Kopf der Quelltexte)

- Kann auf den Rechnern in den Laboren der Fakultät vorgeführt und übersetzt werden

## Programmbuild

### Voraussetzungen

- gtk+ 3.0
- pkg-config
- gcc

### Vorgehen

```bash
./buildcmd.sh
./beleg-prog1 # Anwendung starten (optional)
```

## Übersicht Dateien

- `beleg-prog1.glade`: Layout-Definition des GTK-GUI, erstellt mit Glade  
  Das Binärimage hängt hiervon ab, **bitte stellen Sie diese Datei deshalb zur Ausführung immer im selben Ordner unter diesem Namen bereit.**

- `main.*`, `dict.*`, `list.*`: Quelltext-Module  
  (Beschreibung siehe jeweilige Header-Datei)

- `buildcmd.sh`: Skript zum Kompilieren des Programms

- `unit_tests/`: Tests für die Module anhand vorgefertigter Szenarien  
  (für die fertige Applikation nicht benötigt)

  - `dicttest*`, `listtest*`: Quellcode, zu erwartende Ausgabe und ggf. weitere benötigte Dateien  
    (`dicttest_assets/*` können Sie als Ausgangspunkt zum Ausprobieren des Programms benutzen.)

  - `runtest.sh`: Skript zur Ausführung des Tests für ein Modul  
  (Nutzungshinweis wird bei Ausführung ohne Argumente ausgegeben, siehe auch Quelltext-Datei des jeweiligen Tests)

  - `unit_test.h`: enthält Makros, die in den Tests genutzt werden

- `docs/`: Dokumentation in Ergänzung zu dieser README-Datei - aktuell nur die enthaltenen Bilder

## Aufbau Hauptfenster

![Screenshot beschriftet](docs/main%20window%20annotated.svg)

1. Menüleiste: Enthält Optionen rund um das Öffnen und Speichern von Dateien sowie Info-Dialog

2. Suche: Bei Eingabe eines Suchbegriffs startet die Suche in den Worten der daneben gewählten Sprache

3. Datentabelle:
   - Klick auf die Spaltenüberschrift: Sprache der Sortierung ändern
   - Ziehen mit der Maus an Spaltenüberschrift: Reihenfolge der Spalten ändern
   - Die Größe des Fensters kann mit der Maus durch Ziehen an der unteren rechten Ecke verändert werden
   - Bei Bedarf erscheint eine Scrollbar

4. Änderung Einträge:
   - ➖ Entfernen: markierten Eintrag löschen (nach Bestätigungs-Abfrage)
   - ➕ Hinzufügen: Dialog zur Eingabe einer neuen Übersetzung öffnen

## GUI-Elemente und ihre IDs in Glade

![GUI-Elemente in Glade](docs/GUI%20elements%20Glade.png)