/*
  LeetCode 4. Median of Two Sorted Arrays
  Сложность: Hard

  Условие:
  Даны два отсортированных массива nums1 и nums2 размеров m и n.
  Нужно найти медиану объединённого отсортированного массива.
  Требуемая сложность по времени: O(log(m+n)).

  Идея решения (бинарный поиск по разбиению):
  Наивно можно слить массивы за O(m+n), но это не удовлетворяет
  требованию по логарифмической сложности.

  Работаем всегда с массивом, который короче (nums1), и ищем в нём
  такую точку разреза i (0..m), что вместе с точкой разреза j в
  nums2 = (m+n+1)/2 - i они делят объединённый массив ровно пополам:
  слева окажется (m+n+1)/2 элементов, справа — остальные.

  Разрез считается корректным, если:
    nums1[i-1] <= nums2[j]  и  nums2[j-1] <= nums1[i]
  (с учётом границ массива, где вместо несуществующего элемента
  подставляется -infinity слева или +infinity справа).

  Если nums1[i-1] > nums2[j] — разрез i слишком большой, двигаем
  правую границу бинарного поиска влево.
  Если nums2[j-1] > nums1[i] — разрез i слишком маленький, двигаем
  левую границу вправо.

  Когда корректный разрез найден, медиана вычисляется из четырёх
  пограничных элементов:
    maxLeft  = max(nums1[i-1], nums2[j-1])
    minRight = min(nums1[i],   nums2[j])
  Если (m+n) нечётно — медиана это maxLeft.
  Если чётно — медиана это (maxLeft + minRight) / 2.0.

  Бинарный поиск ведётся не по значениям, а по
  позиции разреза i в более коротком массиве (диапазон 0..m), поэтому
  сложность O(log(min(m, n))), что укладывается в требуемую O(log(m+n)).

  Сложность по времени: O(log(min(m, n))).
  Сложность по памяти: O(1).
*/

#include <algorithm>
#include <limits>
#include <stdexcept>
#include <vector>

class Solution {
public:
    double findMedianSortedArrays(std::vector<int>& nums1, std::vector<int>& nums2) {
        if (nums1.size() > nums2.size()) {
            return findMedianSortedArrays(nums2, nums1);
        }

        const int m = static_cast<int>(nums1.size());
        const int n = static_cast<int>(nums2.size());
        const int totalLeft = (m + n + 1) / 2;

        int low = 0;
        int high = m;

        while (low <= high) {
            const int i = (low + high) / 2;
            const int j = totalLeft - i;

            const int nums1Left  = (i == 0) ? std::numeric_limits<int>::min() : nums1[i - 1];
            const int nums1Right = (i == m) ? std::numeric_limits<int>::max() : nums1[i];
            const int nums2Left  = (j == 0) ? std::numeric_limits<int>::min() : nums2[j - 1];
            const int nums2Right = (j == n) ? std::numeric_limits<int>::max() : nums2[j];

            if (nums1Left <= nums2Right && nums2Left <= nums1Right) {
                if ((m + n) % 2 == 1) {
                    return static_cast<double>(std::max(nums1Left, nums2Left));
                }
                const int maxLeft  = std::max(nums1Left, nums2Left);
                const int minRight = std::min(nums1Right, nums2Right);
                return (maxLeft + minRight) / 2.0;
            } else if (nums1Left > nums2Right) {
                high = i - 1;
            } else {
                low = i + 1;
            }
        }

        throw std::invalid_argument("Входные массивы должны быть отсортированы");
    }
};
