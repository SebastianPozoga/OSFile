OSFile
======
PL: Lekka i przyjazna biblioteka. Pozwala stworzyć wirtualny system plików. Dzięki prostocie kodu i małym gabarytom  całość można łatwo włączyć do innych projektów.  Zapewnia łatwe i minimalistyczne API. Pozwala przechowywać dużo rozmaitych danych w jednym fizycznym pliku. 
Jej podstawowe zastosowania to systemy wbudowane. Wystarczy napisać własną implementację OSF_VHDD i nasz system może znaleźć się w pamięci naszego mikrokontrolera. Czy jest jakiś prostszy sposób na organizację danych? 
W podobny sposób możemy stworzyć mały system plików przechowywany w pamięci operacyjnej komputera. Myślałeś o własnej bazie danych z optymalizacją w C++?? Chciałbyś, żeby miała elastyczną strukturę? A może mini system cache... Biblioteka sprawdzi się wszędzie tam gdzie potrzebna jest (nie zawsze jawna) drzewiasta organizacja danych. 

Zalety:
+ mała i lekka
+ otwarty kod
+ swobodna edycja
+ możliwość zamknięcia projektu
+ Implementacja klasy OSF_DiskList - Po nadpisaniu pozwala łatwo robić kolekcje w drzewiastej strukturze.
+ prosta w obsłudze

Licencja
MIT License

FAQ
Chcesz jej użyć? Masz pytania? 
Pisz: sebastian@pozoga.eu
