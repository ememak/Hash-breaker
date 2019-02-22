Aplikacja znajdująca ciąg znaków o zadanym początku i końcu mająca hash md5 zadany przez użytkownika.
kompilacja: make, być może wcześniej należy dostosować Makefile (np flaga -arch)
Użycie:
plik md5.x przyjmuje ciąg znaków jako argument linii poleceń i zwraca jego md5
np.
$./md5.x krzeslo
4610b44d50ba9f5e639b5f62f385182c

sama aplikacja służy do znalezienia ciągu znaków o hashu md5 równym podanemu na wejściu, robi to metodą brute force zaczynając od najkrótszych możliwych ciągów
pliki HC_cpu i HC_gpu mają ten sam sposób działania, ale pierwszy używa biblioteki openmp do obliczeń na cpu, drugi - CUDY do obliczeń na GPU
używają też innego kodu do liczenia samego md5, więc może to istotnie wpływać na szybkość ich działania (myślę, że md5.cu > md5.c, ale nie testowałem tego)
użycie:
plik wykonywalny musi otrzymać ciąg 32 znaków szesnastkowych będący hashem md5 szukanego ciągu znaków
opcja --charsize określa wiekość liter a-f szesnastkowych (domyślnie small = a-f, można zmienić na big = A-F), opcja działa na hash, prefix i suffix
--prefix określa prefix szukanego ciągu znaków, szesnastkowo, --suffix analogicznie
argument opcji --charset to alfabet, spośród którego będą konstruowane testowane słowa, znaki nie powinny się powtarzać (wpływa na wydajność); domyślnie opcja ustawiona na all - znaki od \x00 do \xff
np.
$ ./HC_gpu.x 4610b44d50ba9f5e639b5f62f385182c --charset abcdefghijklmnopqrstuvwxyz --prefix 6b --suffix 6f
Init end!, prefix = k, suffix = o, charset = abcdefghijklmnopqrstuvwxyz
Init end!, prefixlen = 1, suffixlen = 1, charsetlen = 26
CALCULATED: 14 
CALCULATED: 9 14 
CALCULATED: 14 9 14 
CALCULATED: 18 14 9 14 
krzeslo
CALCULATED: 10 18 14 9 14 
Calculation time = 0.565187
Example string with your hash, prefix and suffix is:krzeslo (107 114 122 101 115 108 111 )

Testy wydajności:
laptop z procesorem i5-5200U, 2.20GHz, 2 rdzenie z multithreadingiem
karta graficzna GeForce 920M

(zadany hash dotyczy słowa krzeslo)
$./HC_cpu.x 4610b44d50ba9f5e639b5f62f385182c --charset abcdefghijklmnopqrstuvwxyz --prefix 6b --suffix 6f
Init end!, prefix = k, suffix = o, charset = abcdefghijklmnopqrstuvwxyz
Init end!, prefixlen = 1, suffixlen = 1, charsetlen = 26
...
Calculation time = 6.674288
Example string with your hash, prefix and suffix is:krzeslo (107 114 122 101 115 108 111 )

$ OMP_NUM_THREADS=2 ./HC_cpu.x 4610b44d50ba9f5e639b5f62f385182c --charset abcdefghijklmnopqrstuvwxyz --prefix 6b --suffix 6f
Init end!, prefix = k, suffix = o, charset = abcdefghijklmnopqrstuvwxyz
Init end!, prefixlen = 1, suffixlen = 1, charsetlen = 26
...
Calculation time = 7.120021
Example string with your hash, prefix and suffix is:krzeslo (107 114 122 101 115 108 111 )

$ OMP_NUM_THREADS=1 ./HC_cpu.x 4610b44d50ba9f5e639b5f62f385182c --charset abcdefghijklmnopqrstuvwxyz --prefix 6b --suffix 6f
Init end!, prefix = k, suffix = o, charset = abcdefghijklmnopqrstuvwxyz
Init end!, prefixlen = 1, suffixlen = 1, charsetlen = 26
...
Calculation time = 12.858109
Example string with your hash, prefix and suffix is:krzeslo (107 114 122 101 115 108 111 )

Po powyższym widać, że wersja na CPU zużywa dosyć efektywnie całość dostępnej mocy obliczeniowej, jednak użycie hyperthreadów może ją trochę przyspieszyć
nie przeprowadzałem dłuższych testów ze względu na ilość zajmowanego czasu oraz spodziewając się liniowego skalowania czasu działania


$ ./HC_gpu.x 4610b44d50ba9f5e639b5f62f385182c --charset abcdefghijklmnopqrstuvwxyz --prefix 6b --suffix 6f
Init end!, prefix = k, suffix = o, charset = abcdefghijklmnopqrstuvwxyz
Init end!, prefixlen = 1, suffixlen = 1, charsetlen = 26
..
Calculation time = 0.576072
Example string with your hash, prefix and suffix is:krzeslo (107 114 122 101 115 108 111 )

$ ./HC_gpu.x 4610b44d50ba9f5e639b5f62f385182c --charset abcdefghijklmnopqrstuvwxyz --prefix 6b
Init end!, prefix = k, suffix = , charset = abcdefghijklmnopqrstuvwxyz
Init end!, prefixlen = 1, suffixlen = 0, charsetlen = 26
...
Calculation time = 1.954640
Example string with your hash, prefix and suffix is:krzeslo (107 114 122 101 115 108 111 )

$ ./HC_gpu.x 4610b44d50ba9f5e639b5f62f385182c --charset abcdefghijklmnopqrstuvwxyz
Init end!, prefix = , suffix = , charset = abcdefghijklmnopqrstuvwxyz
Init end!, prefixlen = 0, suffixlen = 0, charsetlen = 26
...
Calculation time = 23.271387
Example string with your hash, prefix and suffix is:krzeslo (107 114 122 101 115 108 111 )

wiersja na GPU działa dużo szybciej, jest w stanie łamać hashe o 2 znaki dłuższe w podobnym czasie
testowałem także wcześniej kilka innych słów, wydajność jest podobna
