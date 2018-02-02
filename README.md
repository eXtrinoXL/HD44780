# MOVED TO GITLAB

<https://gitlab.com/extrinoxl>

<https://gitlab.com/extrinoxl/lib/hd44780>


# eXtrinoXL HD44780
Biblioteka obsługująca wyświetlacz alfanumeryczny ze sterownikiem **HD44780**. Sterowanie odbywa się w trybie 4-bitowym, bez odczytu flagi zajętości z dowolnym przypisaniem sygnałów sterujących.

Przygotowana jest do obsługi mikrokontrolera **Atmel AVR XMEGA128A3U** znajdującego się na płytce **eXtrino XL FULL** wyprodukowanej przez Leon Instruments.

## Platformio IO

Bibliotekę można użyć bezpośrednio w projektach utworzonych za pomocą **Platformio IO**. Wystarczy sklonować repozytorium do katalogu **lib/** a następnie dołączyć ją do kodu:

```
#include <hd44780.h>
```

## Źródła
* [Kurs na stronie Leon Instruments](https://extronic.pl/content/48-kurs-xmega-wyswietlacz-lcd)
* [Oryginalna biblioteka HD44780 autorstwa Radosława Kwietnia](http://radzio.dxp.pl)

## Autorzy
* Radosław Kwiecień - <http://radzio.dxp.pl>
* Dominik Bieczyński (Leon Instruments) - <https://extronic.pl>
* Paweł 'felixd' Wojciechowski - <http://www.konopnickiej.com>

## Narzędzia
 * Edytor: [Visual Studio Code](https://code.visualstudio.com/) + [PlatformioIO](http://platformio.org/)
 * Formatowanie kodu: **clang-format**
 * Kodowanie: **UTF-8**
 * Znak końca linii: **Unix LF**

## Uwagi do Platformio IO / VSCode

### IntelliSense i <avr/io.h> a xmega128a3u
W celu poprawnego działania **IntelliSense** w **VSCode** upewnij się, że plik `.vscode/c_cpp_properties.json` w sekcji `defines` ma dokładnie tak samo określony rodzaj mikrokontrolera:

```
            "defines": [
                "__AVR_ATxmega128A3U__",
```

Więcej informacji pod linkiem: https://github.com/Microsoft/vscode-cpptools/issues/690
