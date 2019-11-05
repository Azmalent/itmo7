package tasks

/**
 Задание 6:
 Создайте массив из 15 чисел, введенных с клавиатуры. Отсортируйте полученный массив массив в
 порядке возрастания и выведите его на экран.
 */
object Task6 : ITask {
    override fun run() {
        val values = arrayOfNulls<Int>(15)

        for (i in 0 until 15) {
            println("Введите число $i: ")
            values[i] = readLine()?.toIntOrNull() ?: 0
        }

        values.sort()
        println(values.joinToString(" "))
    }
}