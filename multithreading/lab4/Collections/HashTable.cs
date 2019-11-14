using System;
using System.Linq;
using System.Collections.Generic;

namespace lab4.Collections
{
    public sealed class HashTable<T>
    {
        private readonly List<T>[] table;
        public HashTable(int tableSize)
        {
            if (tableSize < 1) throw new ArgumentOutOfRangeException("Размер хеш-таблицы не может быть меньше 1");

            table = new List<T>[tableSize];
            for (int i = 0; i < table.Length; i++) table[i] = new List<T>();
        }
        
        private int GetKey(T item)
        {
            return item.GetHashCode() % table.Length;
        }

        public bool Add(T item)
        {
            int key = GetKey(item);
            var list = table[key];

            if (list.Contains(item)) return false;

            list.Insert(0, item);
            return true;
        }

        public bool Remove(T item)
        {
            int key = GetKey(item);
            var list = table[key];

            if (!list.Contains(item)) return false;

            list.Remove(item);
            return true;
        }

        public bool Contains(T item)
        {
            int key = GetKey(item);
            return table[key].Contains(item);
        }

        public bool IsEmpty => table.All(l => !l.Any());

        /* -------------------------- */
        /* --Реализация-IEnumerable-- */
        /* -------------------------- */
        public IEnumerator<T> GetEnumerator()
        {
            for (int i = 0; i < table.Length; i++)
            {
                foreach(T item in table[i])
                {
                    yield return item;
                }
            }
        }
    }
}