namespace Trainning2025
{
    public class Solution
    {
        public static void Main()
        {
            int[] nums = { 0, 0, 1, 1, 1, 2, 2, 3, 3, 4 };
            RemoveDuplicates(nums);
        }

        public static int RemoveDuplicates(int[] nums)
        {
            Dictionary<int, bool> tmp = new Dictionary<int, bool>();
            for (int i = 0; i < nums.Length; i++)
            {
                tmp[nums[i]] = true;
            }
            for (int i = 0; i < tmp.Keys.Count(); i++)
            {
                nums[i] = tmp.Keys.ElementAt(i);
            }

            return tmp.Keys.Count();
        }
    }
}