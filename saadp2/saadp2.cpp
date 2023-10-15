#include <iostream>
#include <unordered_map>

int boyer_moore_horspool(const std::string& text, const std::string& pattern) {
    int len_text = text.length();
    int len_pattern = pattern.length();

    if (len_pattern == 0) {
        return -1; // Пустой шаблон
    }

    // Подготовка таблицы сдвигов для плохого символа
    std::unordered_map<char, int> bad_char_table;
    for (int i = 0; i < len_pattern - 1; ++i) {
        bad_char_table[pattern[i]] = len_pattern - 1 - i;
    }

    // Начало поиска
    int i = len_pattern - 1;
    while (i < len_text) {
        int k = 0;
        while (k < len_pattern && pattern[len_pattern - 1 - k] == text[i - k]) {
            ++k;
        }

        if (k == len_pattern) {
            return i - len_pattern + 1; // Найдено соответствие
        }

        int bad_char_shift = bad_char_table.count(text[i]) ? bad_char_table[text[i]] : len_pattern;
        i += std::max(1, bad_char_shift);
    }

    return -1; // Совпадение не найдено
}

int main() {
    setlocale(LC_ALL, "RU");
    std::string text = "ABAAABCD";
    std::string pattern = "ABC";

    int result = boyer_moore_horspool(text, pattern);

    if (result != -1) {
        std::cout << "Совпадение найдено в позиции " << result << std::endl;
    }
    else {
        std::cout << "Совпадение не найдено" << std::endl;
    }

    return 0;
}
