namespace Trainning2025
{
    public class Solution
    {
        public static void Main()
        {
            Console.WriteLine("Helllo world!");

            int[] nums1 = { 1, 2, 3, 0, 0, 0 };
            int m = 3;
            int[] nums2 = { 2, 5, 6 };
            int n = 3;

            Merge(nums1, m, nums2, n);
        }

        public static void Merge(int[] nums1, int m, int[] nums2, int n)
        {
            int[] numst = new int[m];
            for (int i=0; i<m; i++)
            {
                numst[i] = nums1[i];
            }

            int i1 = 0;
            int i2 = 0;

            for (int i = 0; i < m + n; i++)
            {
                if (i1 < m && i2 < n)
                {
                    if (numst[i1] > nums2[i2])
                    {
                        nums1[i] = nums2[i2];
                        i2++;
                    }
                    else
                    {
                        nums1[i] = numst[i1];
                        i1++;
                    }
                }
                else if (i1 < m)
                {
                    nums1[i] = numst[i1];
                    i1++;
                }
                else if (i2 < n)
                {
                    nums1[i] = nums2[i2];
                    i2++;
                }
                else
                {
                    //this should not happen
                }
            }
            return;
        }
    }
}
