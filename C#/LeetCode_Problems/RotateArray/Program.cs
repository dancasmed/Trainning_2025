namespace Trainning2025
{
    public class Solution
    {
        public static void Main()
        {
            int[] nums = { 1, 2, 3, 4, 5, 6, 7 };
            int k = 3;
            Rotate(nums, k);
        }

        public static void Rotate(int[] nums, int k)
        {
            k %= nums.Length;
            Revert(nums, 0, nums.Length - 1);
            Revert(nums, 0, k - 1);
            Revert(nums, k, nums.Length - 1);
        }

        private static void Revert(int[] nums, int left, int right)
        {
            while (left < right)
            {
                int tmp = nums[left];
                nums[left] = nums[right];
                nums[right] = tmp;
                left++;
                right--;
            }
        }

    }
}