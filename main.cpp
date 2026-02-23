#include <iostream>
#include <fstream>
#include <vector>
#include <map>
#include <pthread.h>

using namespace std;

struct ThreadData {
    vector<string> words;
    map<string, int> wordCount;
};

// Thread function
void* countWords(void* arg) {
    ThreadData* data = (ThreadData*)arg;

    for (string word : data->words) {
        data->wordCount[word]++;
    }

    pthread_exit(NULL);
}

int main() {
    int N;
    cout << "Enter number of threads: ";
    cin >> N;

    ifstream file("input.txt");
    if (!file) {
        cout << "Error opening file!\n";
        return 1;
    }

    // Read all words from file
    vector<string> allWords;
    string word;
    while (file >> word) {
        allWords.push_back(word);
    }
    file.close();

    int totalWords = allWords.size();
    int segmentSize = totalWords / N;

    vector<pthread_t> threads(N);
    vector<ThreadData> threadData(N);

    // Divide words among threads
    for (int i = 0; i < N; i++) {
        int start = i * segmentSize;
        int end = (i == N - 1) ? totalWords : start + segmentSize;

        for (int j = start; j < end; j++) {
            threadData[i].words.push_back(allWords[j]);
        }

        pthread_create(&threads[i], NULL, countWords, &threadData[i]);
    }

    // Wait for all threads
    for (int i = 0; i < N; i++) {
        pthread_join(threads[i], NULL);
    }

    //Print Intermediate Results
    cout << "\n Intermediate Results\n";
    for (int i = 0; i < N; i++) {
        cout << "\nThread " << i + 1 << ":\n";
        for (auto pair : threadData[i].wordCount) {
            cout << pair.first << " : " << pair.second << endl;
        }
    }

    // Merge results
    map<string, int> finalCount;
    for (int i = 0; i < N; i++) {
        for (auto pair : threadData[i].wordCount) {
            finalCount[pair.first] += pair.second;
        }
    }

    // Print final result
    cout << "\nFinal Word Frequency\n";
    for (auto pair : finalCount) {
        cout << pair.first << " : " << pair.second << endl;
    }

    return 0;
}