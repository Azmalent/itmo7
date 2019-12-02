using System.Data;
using System;
using System.Collections;
using System.Collections.Generic;
using static System.Threading.Interlocked;

namespace lab4.Collections
{
    public sealed class ConcurrentDistinctList<T> : IEnumerable<T>
    {
        private class Node
        {
            public readonly T Value;
            public volatile Node Next;
            public volatile bool IsRemoved = false;

            public Node(T value, Node next)
            {
                Value = value;
                Next = next;
            }
        }

        private volatile Node head = null;
        
        public bool IsEmpty => head == null;

        public bool Add(T item)
        {
            if (item == null) throw new NullReferenceException("Item is null");

            var newNode = new Node(item, null);

            //Повторяем, пока не получится
            //Если другой поток удалит последний элемент, придётся начать с начала
            while (true)
            {
                //Если список пуст, атомарно устанавливаем голову
                if (CompareExchange(ref head, newNode, null) == null) return true;

                Node node = head;
                do
                {
                    if (!node.IsRemoved)
                    {
                        //Если такое значение уже есть, ничего не добавляем
                        if (node.Value.Equals(item)) return false;
                        //Атомарно ставим новый элемент в конец цепочки
                        if (CompareExchange(ref node.Next, newNode, null) == null) return true;
                    }
                    
                    node = node.Next;
                } while(node != null);
            } 
        }

        public bool Remove(T item)
        {
            if (item == null) throw new NullReferenceException("Item is null");
            if (IsEmpty) return false;

            if (head.Value.Equals(item))
            {
                Node oldHead = Exchange(ref head, head.Next);
                oldHead.IsRemoved = true;
                return true;
            }

            Node previous = null; 
            Node current = head;

            do
            {
                if (current.Value.Equals(item)) 
                {
                    current.IsRemoved = true;
                    previous.Next = current.Next;
                    return true;
                }

                previous = current;
                current = current.Next;
            } while (current != null);

            return false;
        }

        public bool Contains(T item)
        {
            if (item == null) throw new NullReferenceException("Item is null");

            Node current = head;
            while (current != null)
            {
                if (current.Value.Equals(item) && !current.IsRemoved) return true;
                current = current.Next;
            }

            return false;
        }

        /* -------------------------- */
        /* --Реализация-IEnumerable-- */
        /* -------------------------- */
        public IEnumerator<T> GetEnumerator()
        {
            Node node = head;
            while (node != null)
            {
                if (!node.IsRemoved) yield return node.Value;
                node = node.Next;
            }
        }

        IEnumerator IEnumerable.GetEnumerator()
        {
            return this.GetEnumerator();
        }
    }
}