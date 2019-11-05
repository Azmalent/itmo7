package tasks

/**
 Задание 10:
 Напишите класс MoneyConverter для перевода денег по валюте Рубля в Евро, Доллары и Юани. У
 класса должен быть метод convertToRoubles, для перевода денег.
 */
object Task10 : ITask {
    class MoneyConverter(val dollars: Double, val euros: Double, val yuans: Double) {
        companion object {
            const val USD_COURSE = 63.48
            const val EUR_COURSE = 70.30
            const val CNY_COURSE = 9.06
        }

        fun convertToRoubles(): Double = dollars * USD_COURSE + euros * EUR_COURSE + yuans * CNY_COURSE
    }

    override fun run() {
        val converter = MoneyConverter(10.0, 5.0, 200.0)
        println("10 долларов + 5 евро + 200 юаней = ${converter.convertToRoubles()} руб.")
    }
}