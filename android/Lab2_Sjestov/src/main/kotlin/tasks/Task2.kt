package tasks

/**
 Задание 2:
 1) Объявите четыре переменные типа string – s1,s2,s3,s4. Присвойте им произвольные значения.
 2) Выведите в консоль значения этих переменных в 1 строку, используя 1 метод.
 3) Выведите в консоль значения этих переменных в 4 строки, используя 1 метод.
 */
object Task2 : ITask {
    val s1 = "The quick "
    val s2 = "brown fox "
    val s3 = "jumps over "
    val s4 = "the lazy dog"

    fun printOneLine() {
        arrayOf(s1, s2, s3, s4).forEach(::print)
    }

    fun printIndividualLines() {
        arrayOf(s1, s2, s3, s4).forEach(::println)
    }

    override fun run() {
        printOneLine()
        printIndividualLines()
    }
}