namespace Trainning2025
{
    public class Solution
    {
        public static void Main()
        {
            int[] nums = { 2, 2, 1, 1, 1, 2, 2 };
            MajorityElement(nums);
        }

        public static int MajorityElement(int[] nums)
        {
            Dictionary<int, int> repetitions = new Dictionary<int, int>();

            if (nums.Length == 0) return 0;

            int m = nums[0];
            repetitions[m] = 1;

            for(int i = 1; i < nums.Length; i++)
            {
                if (repetitions.ContainsKey(nums[i]))
                {
                    repetitions[nums[i]]++;
                    if (repetitions[nums[i]] > repetitions[m])
                    {
                        m = nums[i];
                    }
                } 
                else
                {
                    repetitions[nums[i]] = 1;
                }
            }
            return m;
        }
    }
}