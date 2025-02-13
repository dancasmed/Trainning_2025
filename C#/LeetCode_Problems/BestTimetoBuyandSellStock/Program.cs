namespace Trainning2025
{
    public class Solution
    {
        public static void Main()
        {
            int[] prices = { 7, 1, 5, 3, 6, 4 };
            MaxProfit(prices);
        }
        public static int MaxProfit(int[] prices)
        {
            int minPriceIdx = 0;
            int maxProfit = 0;


            for(int i = 1; i<prices.Length; i++)
            {
                if (prices[i] < prices[minPriceIdx])
                {
                    minPriceIdx = i;
                } else if (maxProfit < prices[i] - prices[minPriceIdx])
                {
                    maxProfit = prices[i] - prices[minPriceIdx];
                }
            }

            return maxProfit;
        }
    }
}