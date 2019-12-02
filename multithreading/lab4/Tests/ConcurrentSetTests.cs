using System.Reflection;
using System.Linq;
using System;
using System.Collections.Generic;
using lab4.Collections;
using Xunit;

namespace lab4.Tests
{
    public class ConcurrentSetTests
    {
        private ConcurrentSet<int> set = new ConcurrentSet<int>(8);

        [Fact] 
        void NegativeSize()
        {
            Assert.Throws<ArgumentOutOfRangeException>(
                () => new ConcurrentSet<int>(-1)
            );
        }

        [Fact] 
        void AddElement()
        {
            Assert.True( set.Add(100) );
            Assert.True( set.Contains(100) );
            Assert.False( set.Add(100) );
        }

        [Fact] 
        void RemoveElement()
        {
            Assert.False( set.Remove(100) );

            set.Add(100);

            Assert.True( set.Remove(100) );
            Assert.False( set.Contains(100) );
        }

        [Fact] 
        void IsEmpty()
        {
            Assert.True(set.IsEmpty);

            set.Add(100);
            Assert.False(set.IsEmpty);
        }
       
        [Fact] 
        void SimpleForeach()
        {
            int[] numbers = new[] { 1, 2, 3, 4, 5 };

            foreach(int n in numbers) set.Add(n);

            foreach(int m in set) Assert.Contains(m, numbers);
        }
    }
}