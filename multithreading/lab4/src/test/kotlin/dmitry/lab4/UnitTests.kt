package dmitry.lab4

import io.kotlintest.IsolationMode
import io.kotlintest.shouldBe
import io.kotlintest.shouldThrow
import io.kotlintest.specs.WordSpec
import java.lang.IllegalArgumentException

class UnitTests : WordSpec() {
    override fun isolationMode() = IsolationMode.InstancePerLeaf

    private val set = ConcurrentSet<Int>(1)

    init {
        "Constructor" should {
            "throw IllegalArgumentException on non-positive hashtableSize" {
                shouldThrow<IllegalArgumentException> {
                    ConcurrentSet<Int>(-1)
                }
            }
        }

        "Addition" should {
            "return true on success" {
                set.add(1) shouldBe true
                set.contains(1) shouldBe true
            }

            "return false if element exists" {
                set.add(1)
                set.add(1) shouldBe false
            }

            "work with multiple elements" {
                set.add(1) shouldBe true
                set.add(2) shouldBe true
                set.add(3) shouldBe true

                set.contains(1) shouldBe true
                set.contains(2) shouldBe true
                set.contains(3) shouldBe true
            }
        }

        "Deletion" should {
            "return true on success" {
                set.add(1)
                set.remove(1) shouldBe true

                set.isEmpty shouldBe true
            }

            "return false if element doesn't exist" {
                set.remove(1) shouldBe false
            }
        }
    }
}