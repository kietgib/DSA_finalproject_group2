#include <iostream>
#include <vector>
#include <string>
#include <fstream>
#include <cstdlib>
#include <ctime>

using namespace std;

class Hash
{
private:
    int a, b;
    int p;
    int m;

public:
    Hash(int tableSize)
    {
        p = 10000019; // large prime
        m = tableSize;
        srand(time(nullptr));
        a = rand() % p + 1;
        b = rand() % p;
    }

    int hash(int key) const
    {
        return ((a * key + b) % p) % m;
    }
};

template <typename K, typename V>
class SecureHashTable
{
private:
    struct Entry
    {
        K key;
        V value;
    };

    vector<vector<Entry>> table;
    Hash hasher;

public:
    SecureHashTable(int size = 26)
        : table(size), hasher(size) {}

    void insert(K key, V value)
    {
        int idx = hasher.hash((int)key);
        for (auto &e : table[idx])
        {
            if (e.key == key)
            {
                e.value = value;
                return;
            }
        }
        table[idx].push_back({key, value});
    }

    V get(K key) const
    {
        int idx = hasher.hash((int)key);
        for (auto &e : table[idx])
        {
            if (e.key == key)
                return e.value;
        }
        return nullptr;
    }
};

struct TrieNode
{
    bool isEnd;
    SecureHashTable<char, TrieNode *> children;

    TrieNode() : isEnd(false), children(26) {}
};

class Trie
{
private:
    TrieNode *root;

    void dfs(TrieNode *node, const string &prefix) const
    {
        if (node->isEnd)
            cout << prefix << endl;

        for (char c = 'a'; c <= 'z'; c++)
        {
            TrieNode *child = node->children.get(c);
            if (child != nullptr)
            {
                dfs(child, prefix + c);
            }
        }
    }

public:
    Trie()
    {
        root = new TrieNode();
    }

    void insert(const string &word)
    {
        TrieNode *curr = root;
        for (char c : word)
        {
            if (c < 'a' || c > 'z')
                continue;

            TrieNode *next = curr->children.get(c);
            if (next == nullptr)
            {
                next = new TrieNode();
                curr->children.insert(c, next);
            }
            curr = next;
        }
        curr->isEnd = true;
    }

    void loadFile(const string &filename)
    {
        ifstream file(filename);
        string word;
        while (file >> word)
        {
            for (char &c : word)
                c = tolower(c);
            insert(word);
        }
        file.close();
    }

    void autocomplete(const string &prefix) const
    {
        TrieNode *curr = root;
        for (char c : prefix)
        {
            curr = curr->children.get(c);
            if (curr == nullptr)
            {
                cout << "Khong co tu khoa goi y" << endl;
                return;
            }
        }
        dfs(curr, prefix);
    }
};

int main()
{
    Trie trie;
    trie.loadFile("input.txt");

    int n;
    string prefix;
    cout << ">=================================<" << endl;
    cout << "Nhan 1 de bat dau hoac 0 de thoat" << endl;
    cout << "1. Bat dau" << endl
         << "2. Ket thuc" << endl;
    cout << "Chon: ";
    cin >> n;
    cout << ">=================================<" << endl;

    if (n == 0)
    {
        cout << ">=======================<" << endl;
        cout << "Ket thuc chuong trinh";
        cout << ">=======================<" << endl;
        return 0;
    }

    while (true)
    {
        cout << ">======================<" << endl;
        cout << "Nhap ki tu can tim: ";
        cin >> prefix;

        for (char &c : prefix)
            c = tolower(c);

        cout << ">========================<" << endl;
        cout << "Goi y de xuat:" << endl;
        trie.autocomplete(prefix);
        cout << ">========================<" << endl;

        cout << "Nhan 1 de tiep tuc hoac 0 de thoat" << endl;
        cin >> n;

        if (n == 0)
        {
            cout << ">========================<" << endl;
            cout << "Ket thuc chuong trinh" << endl;
            cout << ">========================<" << endl;
            return 0;
        }
    }

    return 0;
}