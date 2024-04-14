#ifndef TRIE
#define TRIE

#include "../object/object.h"
#include <stack>
#include <unordered_map>
#include <assert.h>

namespace native {

class trie {
    private:
    struct node {
        // std::unordered_map<char, int> next;
        std::stack<obj::object*> objects;
        std::unordered_map<char, int> next;
    };
    std::vector<node> nodes;
    public:
    trie() {
        nodes.push_back(node());
    }
    void insert(obj::object* object) {
        std::string name = object->name;
        int beg = 0;
        int len = name.size();
        for (int i = 0; i < len; i ++) {
            std::unordered_map<char, int>::iterator it = nodes[beg].next.find(name[i]);
            if (it == nodes[beg].next.end()) nodes[beg].next[name[i]] = nodes.size(), nodes.push_back(node());
            beg = nodes[beg].next[name[i]];
        }
        if (nodes[beg].objects.empty() || nodes[beg].objects.top()->valid_space < object->valid_space) {
            nodes[beg].objects.push(object);
            return;
        }
        assert(false);
    }
    obj::object* find(std::string name) {
        int len = name.size();
        int beg = 0;
        for (int i = 0; i < len; i ++) {
            std::unordered_map<char, int>::iterator it = nodes[beg].next.find(name[i]);
            if (it == nodes[beg].next.end()) {
                debug(name);
                assert(false);
            }
            beg = (*it).second;
        }   
        assert(!nodes[beg].objects.empty());
        return nodes[beg].objects.top();
    }
};

}  // namespace native

#endif