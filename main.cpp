#include <iostream>
#include <fstream>
#include <vector>
#include <map>
#include <pthread.h>

using namespace std;

// Structure to pass data to threads
struct ThreadData {
    vector<string> words;
    map<string, int> wordCount;
};

// Thread function
void* countWords(void* arg) {

    ThreadData* data = (ThreadData*) arg;

    for (int i = 0; i < data->words.size(); i++) {
        data->wordCount[data->words[i]]++;
    }

    pthread_exit(NULL);
}

int main() {

    ifstream file("input.txt");

    if (!file) {
        cout << "Cannot open file\n";
        return 1;
    }

    vector<string> allWords;
    string word;

    // Read file words
    while (file >> word) {
        allWords.push_back(word);
    }

    file.close();

    int N = 2; // Number of threads (you can change this)
    int segmentSize = allWords.size() / N;

    pthread_t threads[N];
    ThreadData threadData[N];

    // Divide words among threads
    for (int i = 0; i < N; i++) {

        int start = i * segmentSize;
        int end;

        if (i == N - 1)
            end = allWords.size();
        else
            end = start + segmentSize;

        for (int j = start; j < end; j++) {
            threadData[i].words.push_back(allWords[j]);
        }

        pthread_create(&threads[i], NULL, countWords, &threadData[i]);
    }

    // Wait for threads to finish
    for (int i = 0; i < N; i++) {
        pthread_join(threads[i], NULL);
    }

    // Merge results
    map<string, int> finalCount;

    for (int i = 0; i < N; i++) {
        for (auto pair : threadData[i].wordCount) {
            finalCount[pair.first] += pair.second;
        }
    }

    // Print final results
    cout << "\nFinal Word Frequency:\n";

    for (auto pair : finalCount) {
        cout << pair.first << " : " << pair.second << endl;
    }

    return 0;
}