# Checkers-AI

Данная библиотека является реализацией игры в классические шашки. Игра происходит против искусственного интеллекта, выбор ходов которого основан на алгоритме MiniMax.

## Описание алгоритма
В начале игры задаётся сложность от 1 до 3.
Это глубина дерева перебора всевозможных ходов компьютером для поиска наилучшего хода.
На четных итерациях компьютер перебирает свои ходы, а на нечетных - ходы игрока.
Далее по всем перебранным ходам строится дерево, где глубина узла - это номер итерации, на которой он был получен.
Затем для каждого узла мы высчитываем "очки" по формуле:
```очки = количество шашек компьютера - количество шашек человека + 5 * (количество дамок у компьютера - количество дамок у человека)```.
Таким образом, в каждом узле мы получаем некоторое значение, которое должно минимизироваться человеком и максимизироваться компьютером, т. е. на четной глубине компьютер максимизирует очки, а на нечетной - минимизирует.
Обойдя дерево ДФСом, компьютер находит оптимальный ход и выполняет его. С увеличением глубины дерева сложность игры увеличивается, потому что компьютер просматривает больше ходов наперед, делая более оптимальный ход.

## Возможные эвристики для улучшения результатов искусственного интеллекта
1) Увеличение веса каждой шашки при приближении к позиции становления дамкой (т.е. для компьютера нижняя грань поля, а для человека - верхняя). Это поможет быстрее получать дамки, тем самым с большей вероятностью побеждать.
2) Увеличения веса шашки от количества шашек, защищающих её (т.е стоящие на соседних клетках от неё). Таким образом, мы не будем оставлять шашки беззащитными.
3) На поздней стадии игры, когда у обоих сторон остались только дамки, побеждающей сторонее выгоднее сокращать расстояние до шашек противника, для того, чтобы атаковать. Иначе - увеличивать расстояние, чтобы защищаться.

## Эвристика для улучшения скорости работы и уменьшения требуемой памяти
Мы можем воспользоваться алгоритмом Альфа-Бета-отсечения для улучшения алгоритма MiniMax. Данное улучшение основывается на том, что мы отсекаем поддеревья, в которых результат уже не может стать оптимальным. Тем самым уменьшается количество перебираемых ходов, что значительно ускоряет наш алгоритм.

## Детали реализации
В классе Bot реализована основная логика поиска оптимальных ходов компьютером:
1) Метод MakeTurn делает оптимальный ход.
2) KillLookup находит все возможные шашки, которые можно побить выбранной.
3) FindPossibleTurns находит все возможные ходы.
4) DFS - реализация алгоритма MiniMax.


В классе Board реализована логика доски для игры в шашки: метод MakeMove делает переданный ход на текущей доске.


В классе Player реализована логика игрока-человека с поддержкой ввода ходов и отслеживания их корректности.   


В классе Game реализована логика переключения ходов, запуска и завершения игры.    

Игра завершается, когда у кого-то из игроков не останется шашек.

**Соблюдение правила "если можно бить, то нужно бить" является обязательством пользователя.**


## Пользовательский интерфейс
В начале игры пользователь вводит сложность и выбирает сторону, за которую хочет играть (w - белые, b - черные).  
Далее, когда подходит очередь пользователя ходить, он должен сначала выбрать шашку, введя два числа через пробел. Затем нужно сделать ход. Существует два вида ходов:
1) Ход на соседнюю клетку: пользователь вводит позицию клетки, которая находится на расстоянии один по диагонали от выбранной шашки. На этом ход заканчивается.
2) Ход на расстояние больше, чем 1: если пользователь ходит дамкой, то он указывает конечную позицию и отвечает "No" на вопрос, иначе пользователь должен ввести все прыжки через побитые шашки поочередно, отвечая каждый раз на вопрос "Yes" и указывая побитую шашку. Когда все нужные шашки будут побиты, нужно ввести -1 -1 для завершения серии ходов.
