#include "Compiler.h"

Trie::Trie() {
    nodes.push_back(node());
}

void Trie::insert(Object* object) {
    std::string name = object->name;
    int beg = 0;
    size_t len = name.size();
    for (int i = 0; i < len; i ++) {
        std::map<char, int>::iterator it = nodes[beg].next.find(name[i]);
        if (it == nodes[beg].next.end()) nodes[beg].next[name[i]] = nodes.size(), nodes.push_back(node());
        beg = nodes[beg].next[name[i]];
    }
    if (nodes[beg].objects.empty() || nodes[beg].objects.top()->validSpace < object->validSpace) {
        nodes[beg].objects.push(object);
        return;
    }
    assert(false);
}

Object* Trie::find(std::string name) {
    size_t len = name.size();
    int beg = 0;
    for (int i = 0; i < len; i ++) {
        std::map<char, int>::iterator it = nodes[beg].next.find(name[i]);
        if (it == nodes[beg].next.end()) assert(false);
        beg = (*it).second;
    }   
    assert(!nodes[beg].objects.empty());
    return nodes[beg].objects.top();
}