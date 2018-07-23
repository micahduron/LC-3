#pragma once

#include <optional>
#include <lc3.h>
#include <util/StringView.h>

namespace LC3::Language {

using Util::StringView;

class SymbolTable {
public:
    SymbolTable() {}
    SymbolTable(const SymbolTable& other) = delete;
    SymbolTable(SymbolTable&& other) = default;

    SymbolTable& operator = (const SymbolTable& other) = delete;
    SymbolTable& operator = (SymbolTable&& other) = default;

    void add(const StringView& symbolName, LC3::Word addr) {
        m_table[symbolName] = addr;
    }

    std::optional<LC3::Word> get(const StringView& symbolName) const {
        auto symIter = m_table.find(symbolName);
        
        if (symIter != m_table.end()) {
            return { symIter->second };
        }
        return {};
    }

    static std::optional<SymbolTable> make(const SyntaxTreeNode& root);

private:
    std::unordered_map<StringView, LC3::Word> m_table;
};

} // namespace LC3::Language
