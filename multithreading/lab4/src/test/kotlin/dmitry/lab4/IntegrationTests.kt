package dmitry.lab4

import io.kotlintest.IsolationMode
import io.kotlintest.shouldBe
import io.kotlintest.specs.WordSpec

class IntegrationTests : WordSpec() {
    override fun isolationMode() = IsolationMode.InstancePerLeaf

    private val list = ConcurrentList<Int>()

    private fun runThreads(n: Int, initializer: (Int) -> (() -> Unit)) {
        val threads = Array(n) { i ->
            val function = initializer(i)
            Thread(function)
        }

        for (i in 0 until n) {
            threads[i].start()
        }

        for (i in 0 until n) {
            threads[i].join()
        }
    }

    init {
        "Addition" should {
            "work with 1000 parallel threads" {
                runThreads(1000) {
                    i -> { list.add(i) }
                }

                for (i in 0 until 1000) {
                    list.contains(i) shouldBe true
                }
            }
        }

        "Deletion" should {
            "work with 1000 parallel threads" {
                for (i in 0 until 1000) {
                    list.add(i)
                }

                runThreads(1000) {
                    i -> { list.remove(i) }
                }

                list.isEmpty shouldBe true
            }

            "work in parallel with addition" {
                for (i in 0 until 400) {
                    list.add(i * 5)
                }

                fun adder(n: Int): () -> Unit = { list.add(n) }
                fun remover(n: Int): () -> Unit = { list.remove(n) }

                runThreads(2000) { i ->
                    if (i%5 != 0) adder(i)
                    else remover(i)
                }

                for (i in 0 until 2000) {
                    list.contains(i) shouldBe (i%5 != 0)
                }
            }
        }
    }
}