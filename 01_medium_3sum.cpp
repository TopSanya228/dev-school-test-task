/*
  LeetCode 15. 3Sum
  Сложность: Medium
 
  Условие:
  Дан массив целых чисел nums. Нужно найти все уникальные тройки индексов
  (i, j, k), такие что nums[i] + nums[j] + nums[k] == 0, i != j != k.
  Тройки в ответе не должны повторяться (по значениям).
 
  Идея решения:
  1. Сортируем массив — это позволяет применить метод двух указателей
     и легко пропускать дубликаты.
  2. Идём по массиву и фиксируем первый элемент тройки nums[i].
     - Если nums[i] > 0, дальше сумма трёх отсортированных положительных
       чисел не может быть равна 0 — можно сразу прервать цикл.
     - Пропускаем повторяющиеся значения nums[i], чтобы не получать
       дублирующиеся тройки.
  3. Для оставшейся части массива (от i+1 до конца) ищем пару чисел,
     сумма которых равна -nums[i], с помощью двух указателей left/right,
     двигающихся навстречу друг другу:
     - если сумма меньше нужной — двигаем left вправо (увеличиваем сумму);
     - если больше — двигаем right влево (уменьшаем сумму);
     - если равна — записываем тройку и дополнительно пропускаем
       соседние дубликаты у left и right.
 
  После сортировки задача "найти пару с заданной
  суммой" в отсортированном массиве решается двумя указателями за
  линейное время, а не за квадратичное (как перебор всех пар).
 
  Сложность по времени: O(n^2) — внешний цикл O(n), внутренний поиск пары
  двумя указателями O(n).
  Сложность по памяти: O(1) дополнительной памяти (не считая результата
  и памяти, которую использует сортировка).
*/

#include <algorithm>
#include <vector>

class Solution {
public:
    std::vector<std::vector<int>> threeSum(std::vector<int>& nums) {
        std::vector<std::vector<int>> result;
        std::sort(nums.begin(), nums.end());
        const int n = static_cast<int>(nums.size());

        for (int i = 0; i < n - 2; ++i) {
            if (nums[i] > 0) {
                break;
            }
            if (i > 0 && nums[i] == nums[i - 1]) {
                continue;
            }

            int left = i + 1;
            int right = n - 1;
            const int target = -nums[i];

            while (left < right) {
                const int sum = nums[left] + nums[right];
                if (sum == target) {
                    result.push_back({nums[i], nums[left], nums[right]});
                    ++left;
                    --right;
                    while (left < right && nums[left] == nums[left - 1]) {
                        ++left;
                    }
                    while (left < right && nums[right] == nums[right + 1]) {
                        --right;
                    }
                } else if (sum < target) {
                    ++left;
                } else {
                    --right;
                }
            }
        }

        return result;
    }
};
