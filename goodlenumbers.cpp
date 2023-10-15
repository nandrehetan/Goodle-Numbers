#include <bits/stdc++.h>
#include <chrono> 
#include <random> 

int generateRandomNumber(std::mt19937_64& rng, int lowerBound, int upperBound) 
{ 
        return rng() % (upperBound - lowerBound + 1) + lowerBound; 
} 

using namespace std;

vector<long long> primeNumbers;
vector<char> bufferCharacters = {'!', '@', '#', '$', '%', '^', '&', '*', '(', ')', '-', '_', '+', '=', '}', '{', '|', '`'};

vector<char> randomCharacters = {'1', '2', '3', '4', '5', '6', '7', '8', '9', '0', '-', '_', '+', '=', ')', '(', '*', '&', '^', '%', '$', '#', '@', '!', 'q', 'w', 'e', 'r', 't', 'y', 'u', 'i', 'o', 'p', '[', '{', '}', ']', '|', 'a', 's', 'd', 'f', 'g', 'h', 'j', 'k', 'l', ';', ':', 'z', 'x', 'c', 'v', 'b', 'n', 'm', '<', '>', '?', '/'};

vector<long long> sieveOfEratosthenes(int n) {
        int* array = new int[n + 1](); 
        vector<long long> resultVector; 
        for (int i = 2; i <= n; i++) {
                if (array[i] == 0) {
                        resultVector.push_back(i); 
                        for (int j = 2 * i; j <= n; j += i) {
                                array[j] = 1;
                        }
                } 
        }
        return resultVector;
}

long long modularExponentiation(long long base, long long exponent, long long modulus) {
        if (exponent == 0) {
                return 1;
        }
        long long result = modularExponentiation(base, exponent / 2, modulus) % modulus;

        if (exponent & 1) {
                return (result * result * base) % modulus;
        }
        else {
                return (result * result) % modulus;
        }
}

long long diffieHellmanKeyExchange(long long base, long long privateA, long long privateB){
        long long primeModulus, publicKeyA, publicKeyB, secretKeyA, secretKeyB;
        primeModulus = 7919;
        publicKeyA = modularExponentiation(base, privateA, primeModulus);
        publicKeyB = modularExponentiation(base, privateB, primeModulus); 
        secretKeyA = modularExponentiation(publicKeyB, privateA, primeModulus); 
        secretKeyB = modularExponentiation(publicKeyA, privateB, primeModulus); 
        return secretKeyA;
}

string addBufferCharacters(long long key){
        long long invertedNumber = 0;
        long long bufferCharactersCount = bufferCharacters.size();
        string bufferString = "";
        for (int i = 62; i >= 0; i--) {
                if ((key >> i) & 1){
                }
                else {
                        invertedNumber += (1LL << i);
                }
        }
        long long difference = key - invertedNumber;
        if (difference < 0) {
                difference *= -1;
        }
        long long startIndex = invertedNumber % bufferCharactersCount;
        long long endIndex = difference % bufferCharactersCount;
        bufferString += bufferCharacters[startIndex];
        int randomNumber = rand();
        randomNumber %= 25;
        while (randomNumber > 0) {
                int left = 0;
                int right = randomCharacters.size();
                int index = rand();
                index %= randomCharacters.size();
                if (randomCharacters[index] != bufferCharacters[startIndex] && randomCharacters[index] != bufferCharacters[endIndex]) {
                        bufferString += randomCharacters[index];
                        randomNumber--;
                }
        }
        bufferString += bufferCharacters[endIndex];
        return bufferString;
}

vector<char> calculateStartAndEndCharacters(long long key){
        long long invertedNumber = 0;
        long long bufferCharactersCount = bufferCharacters.size();
        string bufferString = "";
        for (int i = 62; i >= 0; i--) {
                if ((key >> i) & 1){
                }
                else {
                        invertedNumber += (1LL << i);
                }
        }
        long long difference = key - invertedNumber;
        if (difference < 0) {
                difference *= -1;
        }
        long long startIndex = invertedNumber % bufferCharactersCount;
        long long endIndex = difference % bufferCharactersCount;
        return {bufferCharacters[startIndex], bufferCharacters[endIndex]};
}

string encryptMessage(string message, long long key){
        int messageLength = message.length();
        int primesCount = primeNumbers.size();
        string encryptedText = "";
        int j = 0;
        long long currentIndex = key + ((key >> j & 1) << (j));
        long long skips = key + ((key >> j & 1) << (j));
        currentIndex = currentIndex % primesCount;
        int i = 0;
        while (i < messageLength) {
                long long multiplication = primeNumbers[currentIndex] * int(message[i]);
                string multiplicationString = to_string(multiplication);
                encryptedText += multiplicationString;
                encryptedText += addBufferCharacters(skips);
                j++;
                i++;
                if (j == 63) {
                        j = 0;
                }
                skips = key + ((key >> j & 1) << (j));
                currentIndex += skips;
                currentIndex = currentIndex % primesCount;
        }
        return encryptedText;
}

string decryptMessage(string cypher, long long key){
        int cypherLength = cypher.length();
        int primesCount = primeNumbers.size();
        string decryptedText = "";
        int j = 0;
        long long currentIndex = key + ((key >> j & 1) << (j));
        long long skips = key + ((key >> j & 1) << (j));
        currentIndex = currentIndex % primesCount;
        int i = 0;
        string number = "";
        while (i < cypherLength && cypher[i] >= '0' && cypher[i] <= '9') {
                number += cypher[i];
                i++;
        }
        long long d = stoll(number);
        long long currentPrimeNumber = primeNumbers[currentIndex];
        d = d / currentPrimeNumber;
        decryptedText += char(d);
        vector<char> startAndEnd = calculateStartAndEndCharacters(skips);
        while (cypher[i] != startAndEnd[1]) {
                i++;
        }
        i++;
        while (i < cypherLength) {
                string number = "";
                while (i < cypherLength && cypher[i] >= '0' && cypher[i] <= '9') {
                        number += cypher[i];
                        i++;
                }
                long long numberValue = stoll(number);
                j++;
                if (j == 63) {
                        j = 0;
                }
                skips = key + ((key >> j & 1) << (j));
                currentIndex += skips;
                currentIndex = currentIndex % primesCount;
                long long currentPrimeNumber = primeNumbers[currentIndex];
                numberValue = numberValue / currentPrimeNumber;
                decryptedText += char(numberValue);
                startAndEnd = calculateStartAndEndCharacters(skips);
                while (cypher[i] != startAndEnd[1]) {
                        i++;
                }
                i++;
        }
        return decryptedText;
}

int main()
{
        int maxNumber = 1e6;
        primeNumbers = sieveOfEratosthenes(maxNumber);
        unsigned long long secretKey = diffieHellmanKeyExchange(7911, 1567689892, 292787734);
        string message;
        getline(cin, message);
        string encryptedText = encryptMessage(message, secretKey);
        cout << encryptedText << endl;
        string decryptedText = decryptMessage(encryptedText, secretKey);
        cout << decryptedText << endl;
        return 0;
}
