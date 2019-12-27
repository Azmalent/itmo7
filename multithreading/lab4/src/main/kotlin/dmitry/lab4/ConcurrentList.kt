package dmitry.lab4

import java.util.concurrent.atomic.AtomicMarkableReference

typealias AMR<T> = AtomicMarkableReference<T>

class ConcurrentList<T> where T : Comparable<T> {
    private data class Node<T>(val key: T?) {
        val next = AMR<Node<T>>(null, false)
    }

    private data class Window<T>(val previous: Node<T>, val current: Node<T>)

    private val head = Node<T>(null)
    private val tail = Node<T>(null)

    init {
        head.next.set(tail, false)
    }

    val isEmpty: Boolean
        get() = head.next.reference === tail

    fun contains(key: T): Boolean {
        var current = head.next.reference
        val marked = booleanArrayOf(false)
        current.next.get(marked)

        while (current !== tail && key > current.key!!) {
            current = current.next.reference
            current.next.get(marked)
        }

        return current.key == key && !marked[0]
    }

    private fun find(key: T): Window<T> {
        if (isEmpty) {
            return Window(head, tail)
        }

        retry@ while (true) {
            var previous = head
            var current = previous.next.reference

            while (true) {
                val marked = booleanArrayOf(false)
                var next = current!!.next.get(marked)

                while (marked[0]) {
                    if (!previous.next.compareAndSet(current, next, false, false)) {
                        continue@retry
                    }
                    current = next
                    next = current!!.next[marked]
                }

                if (current === tail || key <= current.key!!) {
                    return Window(previous, current)
                }

                previous = current
                current = next
            }
        }
    }

    fun add(key: T): Boolean {
        val newNode = Node(key)

        while (true) {
            val (previous, current) = find(key)

            if (current.key == key) return false

            newNode.next.set(current, false)
            if (previous.next.compareAndSet(current, newNode, false, false)) {
               return true
            }
        }
    }

    fun remove(key: T): Boolean {
        while (true) {
            val (previous, current) = find(key)

            if (current.key != key) return false

            //Здесь нельзя использовать attemptMark(),
            //поскольку он возвращает true, если метка уже стоит
            val next = current.next.reference
            if (!current.next.compareAndSet(next, next, false, true)) {
                continue
            }

            previous.next.compareAndSet(current, next, false, false)
            return true
        }
    }
}