#pragma once

#include <unordered_map>
#include <optional>
#include <lc3.h>
#include <util/StringView.h>

namespace LC3::Language {

using Util::StringView;

class SymbolTable {
    using table_type = std::unordered_map<StringView, LC3::Word>;

public:
    using iterator = table_type::const_iterator;

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

    iterator begin() const {
        return m_table.begin();
    }

    iterator end() const {
        return m_table.end();
    }

    static std::optional<SymbolTable> make(const SyntaxTreeNode& root);

private:
    table_type m_table;
};

} // namespace LC3::Language
