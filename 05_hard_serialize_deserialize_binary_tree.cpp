/*
  LeetCode 297. Serialize and Deserialize Binary Tree
  Сложность: Hard

  Условие:
  Нужно спроектировать алгоритм сериализации бинарного дерева в
  строку и десериализации этой строки обратно в дерево, идентичное
  по структуре исходному (значения узлов и nullptr на своих местах).
  Формат строки может быть любым — важно, чтобы serialize/deserialize
  были согласованы между собой.

  Идея решения (preorder-обход с маркером null):
  Используем обход в прямом порядке (корень -> левое поддерево ->
  правое поддерево), потому что в preorder-последовательности первым
  идёт корень, что упрощает рекурсивное восстановление: как только мы
  прочитали значение корня, дальше в этой же последовательности идёт
  ПОЛНОСТЬЮ описанное левое поддерево, а за ним — правое.

  Serialize:
  - Если узел nullptr — записываем специальный маркер "#".
  - Иначе записываем val узла, затем рекурсивно сериализуем левое,
    затем правое поддерево.
  - Значения разделяем запятыми — так строка однозначно разбивается
    обратно на токены при десериализации.

  Deserialize:
  - Разбиваем строку на токены по запятой и кладём их в очередь.
  - Рекурсивно строим дерево: берём токен из очереди; если это "#" —
    возвращаем nullptr; иначе создаём узел с этим значением и
    рекурсивно строим его левое, затем правое поддерево из той же
    очереди (порядок точно соответствует порядку записи в serialize).

  Благодаря маркеру null дерево восстанавливается однозначно даже
  при несимметричной форме (например, только левые потомки) — не
  нужно ни отдельно передавать размер поддеревьев, ни угадывать
  структуру.

  Сложность по времени: O(n) и для serialize, и для deserialize,
  где n — число узлов дерева (каждый узел и каждый null-маркер
  обрабатывается один раз).
  Сложность по памяти: O(n) на саму строку и на очередь токенов,
  плюс O(h) на стек рекурсии, где h — высота дерева.
*/

#include <queue>
#include <sstream>
#include <string>

/**
 * Definition for a binary tree node.
 * struct TreeNode {
 *     int val;
 *     TreeNode *left;
 *     TreeNode *right;
 *     TreeNode() : val(0), left(nullptr), right(nullptr) {}
 *     TreeNode(int x) : val(x), left(nullptr), right(nullptr) {}
 *     TreeNode(int x, TreeNode *left, TreeNode *right) : val(x), left(left), right(right) {}
 * };
 */
class Codec {
public:
    std::string serialize(TreeNode* root) {
        std::ostringstream out;
        serializeHelper(root, out);
        return out.str();
    }

    TreeNode* deserialize(const std::string& data) {
        std::queue<std::string> tokens;
        std::stringstream ss(data);
        std::string token;
        while (std::getline(ss, token, ',')) {
            tokens.push(token);
        }
        return deserializeHelper(tokens);
    }

private:
    static constexpr const char* kNullMarker = "#";

    void serializeHelper(TreeNode* node, std::ostringstream& out) {
        if (node == nullptr) {
            out << kNullMarker << ",";
            return;
        }
        out << node->val << ",";
        serializeHelper(node->left, out);
        serializeHelper(node->right, out);
    }

    TreeNode* deserializeHelper(std::queue<std::string>& tokens) {
        const std::string token = tokens.front();
        tokens.pop();

        if (token == kNullMarker) {
            return nullptr;
        }

        TreeNode* node = new TreeNode(std::stoi(token));
        node->left = deserializeHelper(tokens);
        node->right = deserializeHelper(tokens);
        return node;
    }
};

// Your Codec object will be instantiated and called as such:
// Codec ser, deser;
// TreeNode* ans = deser.deserialize(ser.serialize(root));
