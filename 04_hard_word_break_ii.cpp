/*
  LeetCode 140. Word Break II
  Сложность: Hard

  Условие:
  Дана строка s и словарь wordDict (список слов без повторов).
  Нужно вернуть ВСЕ способы разбить s на последовательность слов из
  словаря, разделённых пробелом, так, чтобы конкатенация слов давала
  ровно s. Каждое слово словаря можно использовать многократно.

  Идея решения (DP с мемоизацией + backtracking):
  Наивный backtracking (пробуем каждый префикс, если он есть в
  словаре — рекурсивно разбиваем остаток) может пересчитывать одни
  и те же суффиксы строки много раз — экспоненциальная сложность.

  Устраняем повторные вычисления мемоизацией по позиции в строке:
  memo[i] хранит СПИСОК всех вариантов разбиения суффикса s[i..].
  Если для позиции i ответ уже вычислен — просто возвращаем его,
  а не считаем заново.

  Рекурсия wordBreak(i):
  1. Если i == s.size() — достигли конца строки, единственный
     "пустой" вариант разбиения — базовый случай для сборки фраз.
  2. Если memo[i] уже вычислен — возвращаем сохранённый результат.
  3. Иначе перебираем все слова словаря (или все префиксы s[i..]),
     которые совпадают с началом суффикса s[i..]. Для каждого такого
     слова word рекурсивно получаем все разбиения оставшейся части
     wordBreak(i + word.size()) и для каждого варианта приписываем
     word спереди (с пробелом, если остаток не пуст).
  4. Сохраняем и возвращаем результат в memo[i].

  Для ускорения поиска подходящих слов словарь хранится в
  unordered_set, а перебираются не все слова словаря, а все префиксы
  суффикса s[i..] длиной от 1 до maxWordLen — это ограничивает
  внутренний перебор длиной самого длинного слова в словаре.

  Сложность по времени: в худшем случае (когда почти любой префикс —
  слово словаря, например s = "aaaa...a") число самих разбиений может
  быть экспоненциальным по построению ответа — это неотъемлемая часть
  задачи (сама по себе выдача экспоненциального числа строк). Но с
  мемоизацией мы не пересчитываем один и тот же суффикс дважды: работа
  по извлечению множества вариантов на каждой позиции выполняется один
  раз. С учётом длины слов сложность оценивается как
  O(n * maxWordLen + суммарная длина всех итоговых предложений).
  Сложность по памяти: O(n * количество вариантов) на хранение memo.
*/

#include <algorithm>
#include <string>
#include <unordered_map>
#include <unordered_set>
#include <vector>

class Solution {
public:
    std::vector<std::string> wordBreak(std::string s, std::vector<std::string>& wordDict) {
        std::unordered_set<std::string> dict(wordDict.begin(), wordDict.end());

        size_t maxWordLen = 0;
        for (const auto& word : wordDict) {
            maxWordLen = std::max(maxWordLen, word.size());
        }

        std::unordered_map<size_t, std::vector<std::string>> memo;
        return breakFrom(s, 0, dict, maxWordLen, memo);
    }

private:
    std::vector<std::string> breakFrom(
        const std::string& s,
        size_t start,
        const std::unordered_set<std::string>& dict,
        size_t maxWordLen,
        std::unordered_map<size_t, std::vector<std::string>>& memo) {

        if (start == s.size()) {
            return {""};
        }

        auto cached = memo.find(start);
        if (cached != memo.end()) {
            return cached->second;
        }

        std::vector<std::string> sentences;
        const size_t remaining = s.size() - start;
        const size_t upperBound = std::min(maxWordLen, remaining);

        for (size_t len = 1; len <= upperBound; ++len) {
            const std::string word = s.substr(start, len);
            if (dict.find(word) == dict.end()) {
                continue;
            }

            std::vector<std::string> suffixSentences =
                breakFrom(s, start + len, dict, maxWordLen, memo);

            for (const std::string& suffix : suffixSentences) {
                if (suffix.empty()) {
                    sentences.push_back(word);
                } else {
                    sentences.push_back(word + " " + suffix);
                }
            }
        }

        memo[start] = sentences;
        return sentences;
    }
};
