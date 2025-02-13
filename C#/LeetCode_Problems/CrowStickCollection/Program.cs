namespace Trainning2025
{
    public class Solution
    {
        public static void Main()
        {
            int[] forest = { 0, 0, 0, 0, 0, 0, 0, 0, 0, 0 };
            int crow = 3;
            CrowStickCollection(forest, crow);
        }

        public static int[] CrowStickCollection(int[] forest, int crow)
        {
            const int maxMoves = 2;
            const int goal = 100;

            List<int> res = new List<int>();
            
            int total = 0;
            
            int left = crow;
            int right = crow;
            int visited = 0;

            while(total < goal && visited < forest.Length - 1)
            {
                for(int i = left -1; i >=0 && i >= left - maxMoves; i-- )
                {
                    total += forest[i];
                    visited++;
                    if (forest[i] > 0)
                    {
                        res.Add(i);
                    }
                }
                left -= maxMoves;
                for(int i = right + 1; i <forest.Length && i <= right+maxMoves; i++)
                {
                    total += forest[i];
                    visited++;
                    if (forest[i] > 0)
                    {
                        res.Add(i);
                    }
                }
                right += maxMoves;
            }

            if (total < goal && forest[crow] > 0)
            {
                total += crow;
                res.Add(crow);
            }

            return res.ToArray();
        }
    }
}