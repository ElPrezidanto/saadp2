#include <iostream>
#include <vector>
#include <algorithm>
#include <unordered_map>
#include <string>
#include <sstream>
#include <chrono>
#include <locale>
#include <fstream>
using namespace std;



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
            comparisons++;
            if (text[i] == ' ' or text[i] == ',') {
                start = i + 1;
                break;
            }
            start = 0;
        }
        for (int i = end;i < text.length();i++) {
            comparisons++;
            if (text[i] == ' ' or text[i] == ',') {
                end = i - 1;
                break;
            }
            end = text.length() - 1;
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

pair<string,int> longestPalindrome(const std::string& s, int comparisons) {
    int n = s.length();
    comparisons++;
    if (n == 0) {
        return make_pair("",comparisons);
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

    return make_pair(s.substr(start, maxLength), comparisons);
}

void nomber1() {
    ofstream output("output.txt"); // Создаем файл для записи
    if (!output.is_open()) {
        cout << "Error creating the file." << endl;
        return;
    }
    string pattern;
    string text;
    int comparisons = 0;
    // Считываем подстроку
    cout << "Enter the substring: " << endl;
    getline(cin, pattern);
    // Считываем текст
    cout << "Enter the text: " << endl;
    getline(cin, text);
    // Записываем подстроку в файл
    output << pattern << endl;
    // Записываем текст в файл
    output << text << endl;
    output.close();
    std::transform(
        text.begin(), text.end(), text.begin(), [](unsigned char c)
        {
            return std::tolower(c);
        }
    );
    std::vector<std::string> substrings;
    std::pair<std::vector<std::string>, int> result = findSubstringInText(text, pattern, comparisons);
    comparisons = result.second;
    substrings = result.first;   

    std::cout << "Found substrings:" << std::endl;
    for (const auto& str : substrings) {
        std::cout << str << std::endl;
    }

    std::cout << "Number of comparisons: " << comparisons << std::endl;
}

void nomber2(){
    ofstream output("output.txt"); // Создаем файл для записи
    if (!output.is_open()) {
        cout << "Error creating the file." << endl;
        return;
    }
    string text;
    int comparisons = 0;
    // Считываем текст
    cout << "Enter the text: " << endl;
    getline(cin, text);
    // Записываем текст в файл
    output << text << endl;
    output.close();
    std::transform(
        text.begin(), text.end(), text.begin(), [](unsigned char c)
        {
            return std::tolower(c);
        }
    );
    comparisons = longestPalindrome(text, comparisons).second;
    std::string result = longestPalindrome(text, comparisons).first;
    std::cout << "Longest palindrome: " << result << std::endl;
    std::cout << "Number if comparisons: " << comparisons << std::endl;
}

// Пример использования
int main() {
    nomber2();

    return 0;
}
