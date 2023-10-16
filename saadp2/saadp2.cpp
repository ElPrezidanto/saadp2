#include <iostream>
#include <vector>
#include <algorithm>
#include <unordered_map>
#include <string>
#include <sstream>
#include <chrono>
#include <locale>



int boyer_moore_horspool(const std::string& text, const std::string& pattern, int& comparisons) {
    int len_text = text.length();
    int len_pattern = pattern.length();
    comparisons++;
    if (len_pattern == 0) {
        return -1; // Пустой шаблон
    }
    // Подготовка таблицы сдвигов для плохого символа
    std::unordered_map<char, int> bad_char_table;
    for (int i = 0; i < len_pattern - 1; ++i) {
        bad_char_table[pattern[i]] = len_pattern - 1 - i; //данные. 
    }

    // Начало поиска
    int i = len_pattern - 1;
    while (i < len_text) {
        int k = 0;
        while (k < len_pattern && pattern[len_pattern - 1 - k] == text[i - k]) {
            ++k;
            ++comparisons; // Увеличиваем количество сравнений
        }

        comparisons++;
        if (k == len_pattern) {
            return i - len_pattern + 1; // Найдено соответствие
        }

        int bad_char_shift = bad_char_table.count(text[i]) ? bad_char_table[text[i]] : len_pattern;
        i += std::max(1, bad_char_shift);
    }

    return -1; // Совпадение не найдено
}

std::pair<std::vector<std::string>,int> findSubstringInText(const std::string& text, const std::string& substring, int& comparisons) {
    std::vector<std::string> result;

    size_t pos = 0;
    while ((pos = text.find(substring, pos)) != std::string::npos) {
        int start = pos;
        int end = pos;
        for (int i = start;i >= 0;i--) {
            if (text[i] == ' ') {
                start = i + 1;
                break;
            }
        }
        for (int i = end;i < text.length();i++) {
            if (text[i] == ' ') {
                end = i;
                break;
            }
        }
        result.push_back(text.substr(start, end - start + 1));
        ++pos;
        ++comparisons; // Увеличиваем количество сравнений
    }

    return std::make_pair(result,comparisons);
}

//std::pair<int,std::string> longestPalindrome(const std::string& s, int& comparisons) {
//    int len = s.length();
//    if (len == 0) {
//        return std::make_pair(0, "");
//    }
//
//    // Реверсирование строки
//    std::string reversed = s;
//    std::reverse(reversed.begin(), reversed.end());
//
//    // Сборка строки для поиска палиндрома
//    std::string searchStr = s + "#" + reversed;
//    int searchLen = searchStr.length();
//
//    // Подготовка таблицы сдвигов для плохого символа
//    std::vector<int> bad_char_table(256, -1);
//    for (int i = 0; i < len; ++i) {
//        bad_char_table[static_cast<unsigned char>(s[i])] = i;
//    }
//
//    // Процесс поиска с использованием Бойера-Мура-Хорспула
//    int maxLen = 0;
//    int maxLenIdx = 0;
//
//    for (int i = len; i < searchLen; ) {
//        int k = 0;
//        while (k < len && searchStr[i - k] == searchStr[len - 1 - k]) {
//            ++k;
//            ++comparisons;
//        }
//
//        if (k == len) {
//            // Найден палиндром
//            int currentLen = 2 * len - i;
//            if (currentLen > maxLen) {
//                maxLen = currentLen;
//                maxLenIdx = i - len;
//            }
//        }
//
//        int bad_char_shift = bad_char_table[static_cast<unsigned char>(searchStr[i])];
//        i += std::max(1, bad_char_shift);
//        ++comparisons; // Сравнение символа
//    }
//
//    return std::make_pair(comparisons, s.substr(maxLenIdx, maxLen));
//}

std::string longestPalindrome(const std::string& s, int comparisons) {
    int n = s.length();
    comparisons++;
    if (n == 0) {
        return "";
    }

    // Создаем таблицу для хранения информации о палиндромах
    std::vector<std::vector<bool>> isPalindrome(n, std::vector<bool>(n, false));

    // Инициализация палиндромов длиной 1
    for (int i = 0; i < n; ++i) {
        isPalindrome[i][i] = true;
    }

    int start = 0;  // Начальный индекс максимального палиндрома
    int maxLength = 1;  // Длина максимального палиндрома

    // Проверка палиндромов длиной больше 1
    for (int length = 2; length <= n; ++length) {
        for (int i = 0; i <= n - length; ++i) {
            int j = i + length - 1;
            std::string reversed = s.substr(i, length);
            reverse(reversed.begin(), reversed.end());
            comparisons++;
            if (boyer_moore_horspool(s.substr(i, length), reversed, comparisons) != -1) {
                isPalindrome[i][j] = true;
                comparisons++;
                if (length > maxLength) {
                    start = i;
                    maxLength = length;
                }
            }
        }
    }

    return s.substr(start, maxLength);
}

void nomber1() {
    std::string text = "This is a sample subtext. Sample text is used for testing.";
    std::string substring = "text";
    int comparisons = 0;
    std::transform(
        text.begin(), text.end(), text.begin(), [](unsigned char c)
        {
            return std::tolower(c);
        }
    );
    // Поиск подстроки в тексте
    std::vector<std::string> substrings;// = findSubstringInText(text, substring, comparisons);
    std::pair<std::vector<std::string>, int> result = findSubstringInText(text, substring, comparisons);
    comparisons = result.second;
    substrings = result.first;

    std::cout << "Found substrings:" << std::endl;
    for (const auto& str : substrings) {
        std::cout << str << std::endl;
    }

    std::cout << "Number of comparisons: " << comparisons << std::endl;
}

void nomber2(){
    std::string input = "babad";
    int comp = 0;
    std::string result = longestPalindrome(input, comp);

    std::cout << "Longest palindrome: " << result << std::endl;
    std::cout << "comp: " << comp << std::endl;
}

// Пример использования
int main() {
    nomber2();

    return 0;
}


std::pair<int, int> RabinKarpSearch(const std::string& pattern, const std::string& text) {
    const int prime = 31; // Простое число для хэширования
    const int mod = 1e9 + 7; // Модуль для вычисления хэша

    int patternLength = pattern.size();
    int textLength = text.size();

    int patternHash = 0;
    int currentHash = 0;
    int power = 1;

    int count = 0;
    int comparisons = 0; // Счетчик сравнений

    // Вычисляем хэши для образца и начального фрагмента текста
    for (int i = 0; i < patternLength; i++) {
        patternHash = (patternHash * prime + pattern[i]) % mod;
        currentHash = (currentHash * prime + text[i]) % mod;

        if (i < patternLength - 1) {
            power = (power * prime) % mod;
        }
    }

    // Проходим по тексту и ищем совпадения с образцом
    for (int i = 0; i <= textLength - patternLength; i++) {
        comparisons++; // Увеличиваем счетчик сравнений
        if (patternHash == currentHash) {
            bool match = true;
            for (int j = 0; j < patternLength; j++) {
                comparisons++; // Увеличиваем счетчик сравнений
                if (pattern[j] != text[i + j]) {
                    match = false;
                    break;
                }
            }
            if (match) {
                count++;
            }
        }

        if (i < textLength - patternLength) {
            // Обновляем хеш для следующего фрагмента текста
            currentHash = (currentHash - (text[i] * power) % mod + mod) % mod;
            currentHash = (currentHash * prime + text[i + patternLength]) % mod;
        }
    }

    return std::make_pair(count, comparisons);
}
