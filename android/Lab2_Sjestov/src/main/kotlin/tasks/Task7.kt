package tasks

import java.lang.Exception
import java.lang.IndexOutOfBoundsException

/**
 Задание 7:
 Объявите массив целых чисел с 6 элементами и присвойте его какой-нибудь переменной. Обратитесь к
 7 элементу массива и выведите его на экран. Перехватите возникшее исключение и выведите
 сообщение об исключении и его имя.
 */
object Task7 : ITask {
    private val array = arrayOf(1, 2, 3, 4, 5, 6)

    override fun run() {
       try {
           array[7]
       } catch (e: Exception) {
           println("Перехвачено исключение ${e.javaClass.name}")
           e.printStackTrace()
       }
    }
}