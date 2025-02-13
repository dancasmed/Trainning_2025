namespace Trainning2025
{
    public class Solution
    {
        public static void Main()
        {
            int[] nums = { 1, 1, 1, 2, 2, 3 };
            RemoveDuplicates(nums);
        }

        public static int RemoveDuplicates(int[] nums)
        {
            int MAX_REP = 2;
            int res = 0;
            Dictionary<int, int> repetitions = new Dictionary<int, int>();
            int[] tmp = nums;

            int i2 = 0;
            
            for(int i=0; i< tmp.Length; i++)
            {
                if (repetitions.ContainsKey(tmp[i]))
                {
                    if (repetitions[tmp[i]] < MAX_REP)
                    {
                        repetitions[tmp[i]]++;
                        nums[i2] = tmp[i];
                        i2++;
                        res++;
                    }
                } else
                {
                    repetitions[tmp[i]] = 1;
                    nums[i2] = tmp[i];
                    i2++;
                    res++;
                }
            }

            return res;
        }
    }
}