package tasks

/**
 Задание 5:
 Решите предыдущую задачу (No4), но с использованием конструкции «switch-case-default».
 */
object Task5 : ITask {
    private fun getMark(points: Int) : Int? {
        return when (points) {
            in 0..60   -> 2
            in 60..74  -> 3
            in 75..90  -> 4
            in 91..100 -> 5
            else -> null
        }
    }

    override fun run() {
        println("Введите баллы: ")
        val points = readLine()?.toIntOrNull() ?: 0
        val mark = getMark(points)

        if (mark != null) println("Ваша оценка - $mark")
        else println("Вы ввели некорректное количество баллов!")
    }
}