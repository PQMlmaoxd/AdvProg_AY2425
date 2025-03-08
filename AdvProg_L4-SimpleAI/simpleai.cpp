#include "simpleai.h"
#include <iostream>
#include <vector>
#include <string>
#include <set>
#include <map>
using namespace std;

int readMaxGuess()
{
    int maxGuess;
    cout << endl << "Enter the number of incorrect guesses: ";
    cin >> maxGuess;
    return maxGuess;
}

int readWordLen()
{
    int wordLen;
    cout << endl << "Enter the number characters of your secret word: ";
    cin >> wordLen;
    return wordLen;
}

/***
    Args:
        wordLen (int): The desired length of input word
        vocabulary (vector<string>): The vocabulary
    Returns:
        answer (vector<string>) : A set of words from the vocabulary where the number of characters is equal to wordLen
***/
vector<string> filterWordsByLen(int wordLen, const vector<string>& vocabulary)
{
    vector<string> answer;
    for(auto x : vocabulary) {
        if ((int)x.size() == wordLen) answer.push_back(x);
    }
    return answer;
}

/***
    Args:
        selectedChars (set<char>): The predicted characters
    Returns:
        answer (char) : The next character given the provided word is not in the vocabulary
***/
char nextCharWhenWordIsNotInDictionary(const set<char>& selectedChars)
{
    // Duyệt từ 'a' đến 'z' và chọn ký tự đầu tiên chưa được chọn
    for(char c = 'a'; c <= 'z'; c++) {
        if(selectedChars.find(c) == selectedChars.end()){
            return c;
        }
    }
    return 'a'; // Trường hợp tất cả đã chọn, trả về 'a' như mặc định
}

/***
    Args:
        candidateWords (vector<string>): The candidate words for the current given string 
    Returns:
        answer (map<char, int>) : A map đếm số lần xuất hiện của mỗi ký tự trong candidateWords
***/
map<char, int> countOccurrences(const vector<string>& candidateWords)
{
    map<char, int> answer;
    for(const string &word : candidateWords) {
        for(char c : word) {
            answer[c]++;
        }
    }
    return answer;
}

/***
    Args:
        occurrences (map<char, int>): The map which count the occurrences of characters in the candidate words
        selectedChars (set<char>): The predicted characters
    Returns:
        answer (char) : The most frequent character (không thuộc selectedChars)
***/
char findMostFrequentChar(const map<char, int>& occurrences, const set<char>& selectedChars)
{
    char answer = '\0';
    int maxCount = -1;
    for(auto p : occurrences) {
        char c = p.first;
        int count = p.second;
        if(selectedChars.find(c) == selectedChars.end()){
            if(count > maxCount) {
                maxCount = count;
                answer = c;
            }
        }
    }
    // Nếu không tìm thấy ký tự nào (tất cả đã đoán), ta gọi fallback
    if(answer == '\0'){
        return nextCharWhenWordIsNotInDictionary(selectedChars);
    }
    return answer;
}

/***
    Args:
        candidateWords (vector<string>): The candidate words for the current given string 
        selectedChars (set<char>): The predicted characters
    Returns:
        answer (char) : The most suitable character for prediction
***/
char findBestChar(const vector<string>& candidateWords, const set<char>& selectedChars)
{
    if(candidateWords.empty()){
        return nextCharWhenWordIsNotInDictionary(selectedChars);
    }
    map<char, int> occurrences = countOccurrences(candidateWords);
    return findMostFrequentChar(occurrences, selectedChars);
}

string getWordMask(char nextChar)
{
    string mask;
    cout << "The next char is: " << nextChar << endl;
    cout << "Please give me your answer: ";
    cin >> mask;
    return mask;
}

/***
    Args:
        ch (char): The predicted character by the AI
        mask (string): The response mask by the player
    Returns:
        answer (bool) : Trả về false nếu ký tự dự đoán sai, true nếu đúng (tức là mask chứa ký tự ch)
***/
bool isCorrectChar(char ch, const string& mask)
{
    return mask.find(ch) != string::npos;
}

/***
    Args:
        mask (string): The response mask by the player
    Returns:
        answer (bool) : Trả về true nếu mask là từ hoàn chỉnh (không chứa ký tự '-' nào), false nếu không.
        (Ví dụ: - false: g__d, true: good)
***/
bool isWholeWord(const string& mask)
{
    return mask.find('-') == string::npos;
}

/***
    Hàm này hỗ trợ hàm filterWordsByMask
    Args:
        mask (string): The response mask by the player
        word (string): Input word
        ch (char): The predicted character by the AI
    Returns:
        answer (bool) : Trả về false nếu word không phù hợp với mask (về mặt vị trí và ký tự dự đoán)
        Ví dụ: - mask: "-ood", ch: 'd' vs word: "boot" -> false
               - mask: "-ood", ch: 'd' vs word: "good" -> true
***/
bool wordConformToMask(const string& word, const string& mask, char ch)
{
    if(word.size() != mask.size()) return false;
    for(size_t i = 0; i < word.size(); i++){
        if(mask[i] != '-') {
            // Nếu mask đã tiết lộ ký tự thì word phải khớp
            if(word[i] != mask[i]) return false;
        } else {
            // Nếu vị trí chưa được tiết lộ nhưng word có chứa ký tự dự đoán thì không hợp lệ
            if(word[i] == ch) return false;
        }
    }
    return true;
}

/***
    Args:
        mask (string): The response mask by the player
        words (vector<string>): The candidate words
        ch (char): The predicted character by the AI
    Returns:
        answer (vector<string>) : Một danh sách các từ thỏa mãn mask và ký tự dự đoán
        Ví dụ: với words: {"good", "boot", "hood"}, mask: "-ood" và ch: 'd'
               Kết quả trả về: {"good", "hood"}
***/
vector<string> filterWordsByMask(const vector<string>& words, const string& mask, char ch)
{
    vector<string> answer;
    for(const string &word : words) {
        if(wordConformToMask(word, mask, ch)) {
            answer.push_back(word);
        }
    }
    return answer;
}
