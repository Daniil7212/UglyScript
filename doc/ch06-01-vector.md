# Vector

Vector является динамическим массивом, позволяет хранить элементы одного типа вместе в определённом порядке. Он хранится также в одной из переменных.

## Содержание

- [vector](#vector)
- [push](#push)
- [pop](#pop)
- [size](#size)
- [clear](#clear)

## vector

Функция `vector` создаёт вектор определённого типа.

**Стандартный вид:**

`vector type name`

- `type` - тип данных элементов в векторе.
- `name` - имя переменной, в которой будет хранится вектор.

**Пример:**

```bash
func main
  vector int v
endf
```

**Ошибки:**

- Использование неверных типов данных.
- Попытка использовать уже использующееся название переменной.

**Сложность работы:** O(1)

## push

Функция `push` добавляет элемент в конец вектора.

**Стандартный вид:**

`push name e`

- `name` - имя вектора, в который будет добавляться элемент.
- `e` - элемент, который нужно добавить.

**Пример:**

```bash
func f
  println i
endf
func main
  vector int v
  push v 1
  push v 5
  push v 3
  foreach i v f
endf
```

Вывод:

```
1
5
3
```

**Ошибки:**

- Использование несуществующих векторов.
- Попытка добавить элемент другого типа данных

**Сложность работы:** O(1)

## pop

Функция `pop` удаляет последний элемент в векторе или `string`.

**Стандартный вид:**

`pop name`

- `name` - имя вектора (или `string`), из которого будет удаляться элемент.

**Пример:**

```bash
func f
  println i
endf
func main
  vector int v
  push v 1
  push v 5
  push v 3
  pop v
  foreach i v f
  
  create string s
  set s "print"
  pop s
  println s
endf
```

Вывод:

```
1
5
prin
```

**Ошибки:**

- Использование несуществующих переменных.

**Сложность работы:** O(1)

## size

Функция `pop` считает длину вектора или некого значения.

**Стандартный вид:**

`pop var vec`

- `var` - имя переменной (типа `int`), в которую сохранится результат.
- `vec` - значение или переменная (типа `vector`, `string` или `int`) длину которого нужно посчитать.

**Пример:**

```bash
func f
  println i
endf
func main
  vector int v
  push v 1
  push v 5
  push v 3
  create int a
  size a v
  println a
  
  create string s
  set s "print"
  size a s
  println a
  
  create int n
  set n 123456789
  size a n
  println a
endf
```

Вывод:

```
3
5
9
```

**Ошибки:**

- Использование несуществующих переменных.
- Использование не поддерживаемых типов данных.

**Сложность работы:** O(1)

## clear

Функция `clear` очищает `vector` или `string`.

**Стандартный вид:**

`сlear name`

- `name` - имя переменной (типа `vector` или `string`), которую нужно очистить.

**Пример:**

```bash
func f
  println i
endf
func main
  vector int v
  push v 1
  push v 5
  push v 3
  clear v
  
  create int a
  size a v
  println a
endf
```

Вывод:

```
0
```

**Ошибки:**

- Использование несуществующих переменных.

**Сложность работы:** O(n), где n - размер вектора.
