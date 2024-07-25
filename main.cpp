// Project UID db1f506d06d84ab787baf250c265e24e

#include <iostream>
#include <iomanip>
#include <cstdlib>
#include <map>
#include <set>
#include <vector>
#include <sstream>
#include <cmath>
#include <string>
#include "csvstream.h"
#include <fstream>

using namespace std;

class Classifier {

    private: 

        int numPosts;
        map <string, int> uniqueWords;
        map <string, int> uniqueLabels;
        map <string, int> matchingPosts;
        map <pair<string, string>, int> labelWordPair;
        map <string, string> row;

    public: 

        // default constructor
        Classifier () : numPosts(0) {}

        // word splitter
        set <string> unique_words(const string &str) {
            istringstream source{str};
            set <string> words;
            string word;

            while (source >> word) {
                words.insert(word);
            }
            return words;
        }
        

        // log functions 
        double logPrior (string &label) {
            return log((double)uniqueLabels.find(label)->second / 
                (double)numPosts);    
        }

        double logScore(string &label, set<string> &s){
            double logScore = 0;
            logScore += logPrior(label);
            for (auto word: s) {
                logScore += logLikelihood(label, word);
            }
            return logScore;
        }

        double logLikelihood (string label, string word) {
            pair <string, string> currentPair;
            currentPair.first = label;
            currentPair.second = word;

            double logScore = 0;

            if (matchingPosts[word] == 0) {
                logScore = log(1.0 / numPosts);
            } else if (labelWordPair[currentPair] == 0) {
                logScore = log(((double)matchingPosts[word]) / numPosts);
            } else {
                logScore = log(((double)labelWordPair[currentPair]) / 
                    uniqueLabels[label]); 
            }

            return logScore;
        }

        pair <string, double> labelPredicting (pair<string, string> 
                    pairTest, set<string> &setLabel) {

            pair <string, double> currentPair;
            pair <string, double> maxPair;

            set <string> setText = unique_words(pairTest.second);
            maxPair.first = *(setLabel.begin());
            maxPair.second = logScore(maxPair.first, setText);

            for (auto currentLabel : setLabel) {
                currentPair.first = currentLabel;
                currentPair.second = logScore(currentLabel, setText);
                if (currentPair.second == maxPair.second) {
                    if (currentPair.first < maxPair.first) {
                        maxPair = currentPair;
                    }
                } else if (currentPair.second > maxPair.second) {
                    maxPair = currentPair;
                }
            }

            return maxPair;
        }

        vector <pair<string, string> > returnVector(csvstream &trainFile) {
            string currentWord;
            set <string> strSet;
            vector<pair<string, string> > returnVector;
            
            while (trainFile >> row) {
                string uniqueWordsPosts = row["content"];
                string label = row["tag"];
                pair <string, string> textPair = {label, uniqueWordsPosts};
                returnVector.push_back(textPair);
            }

            return returnVector;
        }
};

int main(int argc, char *argv[]) {

    stringstream out;

    cout << setprecision(3);
    out << setprecision(3);

    ifstream trainFile(argv[1]);
    ifstream testFile(argv[2]);

    // error checking
    if (!trainFile.is_open()) {
        cout << "Error opening file: " << argv[1] << endl;
        return 1;
    } 
    
    if (!testFile.is_open()) {
        cout << "Error opening file: " << argv[2] << endl;
        return 1;
    }
    
    // trainFile.open(argv[1]);
    // testFile.open(argv[2]);

    csvstream trainFile_csv(trainFile);
    csvstream testFile_csv(testFile);

    map <string, string> row;
    int numPosts = 0;
    map <string, int> uniqueWords;

    Classifier classify;

    while (trainFile_csv >> row) {

        numPosts++;
        set<string> uniqueWordsPosts = 
        classify.unique_words(row["content"]);
        
        for (auto &i : uniqueWordsPosts) {
            if (uniqueWords.count(i)) {
                uniqueWords[i]++;
            } else {
                pair<string, int>newWordPair{i, 1};
                uniqueWords.insert(newWordPair);
            }
        }
    }

        bool isDebug = false;

    if (argc != 3) {
        string debug = argv[3];
        if (argc != 4 || (argc == 4 && debug != "--debug")) {

            cout << "Usage: main.exe TRAIN_FILE TEST_FILE [--debug]" 
            << endl;
            return 1;

        }

        isDebug = true;

    }
    
    map <string, int> uniqueLabels;
    map <pair<string, string>, int> labelWordPair;
    set <string> setLabel;
    vector <pair<string, string> > trainVector; // nothing here only declaring
    int vocabSize = 0;
    

    if (isDebug) {
        out << "training data:" << endl;
    }

    for (int i = 0; i < numPosts; ++i) {

        if (isDebug) {
            out << " label = " << trainVector.at(i).first << 
            ", content = " << trainVector.at(i).second << endl;
        }

        setLabel.insert(trainVector.at(i).first);

    } 

    out << "trained on " << numPosts << " examples" << endl;

    if (isDebug) {

        out << "vocabulary size = " << vocabSize << endl << endl;
        out << "classes:" << endl;

        for (auto it : setLabel) {

            out << " " << it << ", " << uniqueLabels[it] 
                << " examples, log-prior = " << classify.logPrior(it) << endl;
        }

        out << "classifier parameters:" << endl;

        for (auto it : labelWordPair) {
            out << " ";
            out << it.first.first << ":" << it.first.second 
                << ", count = " << it.second << ", log-likelihood = " 
                << classify.logLikelihood(it.first.first, it.first.second) 
                << endl;
        }
    }

    out << endl << "test data:" << endl;

    string testName;
    csvstream testFile_1(testName);
    
    vector <pair<string, string> > testVector = 
            classify.returnVector(testFile_1);
    int numPostsPredicted = 0;
    int numCorrectPosts = 0;

    for (int i = 0; i < (int) testVector.size(); ++i) {

        pair <string, double> predict = 
              classify.labelPredicting(testVector.at(i), setLabel);

        if (testVector.at(i).first == predict.first) {

            ++numCorrectPosts;
            ++numPostsPredicted;

            out << " correct = " << testVector.at(i).first << ", predicted = "
                << predict.first << ", log-probability score = " 
                << predict.second << endl;
            out << " content = " << testVector.at(i).second << endl << endl;
        }
    }

    out << "performance: " << numCorrectPosts << " / " << numPostsPredicted 
        << " posts predicted correctly" << endl;

    csvstream trainFiles(argv[1]);
    csvstream testFiles(argv[2]);
    string nameTestFile = argv[2];

    cout << out.str();

    return 0;
    

}