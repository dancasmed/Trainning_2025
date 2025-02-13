namespace Trainning2025
{
    public class Solution
    {
        public static void Main()
        {
            int[] nums = { 3, 2, 2, 3 };
            int val = 3;
            RemoveElement(nums, val);
        }

        public static int RemoveElement(int[] nums, int val)
        {
            int res = 0;

            List<int> tmp = new List<int>(nums);
            tmp.RemoveAll(e => e == val);
            for(int i=0; i< tmp.Count(); i++)
            {
                nums[i] = tmp.ElementAt(i);
            }
            res = tmp.Count();
            return res;
        }
    }
}