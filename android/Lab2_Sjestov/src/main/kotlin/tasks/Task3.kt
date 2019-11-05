package tasks

/**
 Задание 3:
 1) Создайте 5 переменных и присвоите им значения, считанные с клавиатуры.
 2) Проанализируйте их, если среди них есть одинаковые, то выведите их на экран. Если они все разные
 выведите на экран фразу «Все разные», а если все 5 одинаковые, то выведите все пять строк на экран
 через запятую.
 Если все сделали правильно, то:
 при вводе “5 9 8 3 1”, будет выведено «Все разные»;
 при вводе “один два два три два”, будет выведено «два два два»;
 при вводе b b b b b, будет выведено «b, b, b, b, b».
 */
object Task3 : ITask {
    override fun run() {
        val values = arrayOfNulls<String>(5)

        for (i in 0 until 5) {
            println("Введите переменную $i: ")
            values[i] = readLine() ?: ""
        }

        val groups = values.groupingBy { it }.eachCount()

        println(when (groups.size) {
            5 -> "Все разные"
            1 -> values.joinToString(", ")
            else -> {
                groups.filter { it.value > 1 }
                      .map { generateSequence { it.key }.take(it.value).joinToString(" ") }
                      .joinToString("\n")
            }
        })
    }
}