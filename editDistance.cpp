/*
 *  Tested with http://www.spoj.com/problems/EDIST/
 */
#include<iostream>
#include <string.h>
#define oo 1 << 30
using namespace std;

int EDUkkonen (string a, string b, int threshold) {
	if (a == b)
		return 0;

	// Swap a and b so b longer or same length as a
	// a will be shorter
	if (a.length() > b.length()) {
		string tmp = a;
		a = b;
		b = tmp;
	}

	int aLen = a.length(), bLen = b.length();

	// Performing suffix trimming:
  	// We can linearly drop suffix common to both strings since they don't increase distance at all
	while (aLen > 0 && a[aLen - 1] == b[bLen - 1]) {
		aLen--;
		bLen--;
	}

	if (aLen == 0) {
		return bLen < threshold ? bLen : threshold;
	}

	// Performing prefix trimming
	// We can linearly drop prefix common to both strings since they don't increase distance at all
	int tStart = 0;
	while (tStart < aLen && a[tStart] == b[tStart]) {
		tStart++;
	}

	aLen -= tStart;
	bLen -= tStart;

	if (aLen == 0) {
		return bLen < threshold ? bLen : threshold;
	}

	threshold = bLen < threshold ? bLen : threshold;

	int dLen = bLen - aLen;

	if (threshold < dLen) {
		return threshold;
	}

	// Equivalent: floor(min(threshold, aLen) / 2)) + 2
	int ZERO_K = ((aLen < threshold ? aLen : threshold) >> 1) + 2;

	int arrayLength = dLen + ZERO_K * 2 + 2;

	int currentRow[arrayLength];
	int nextRow[arrayLength];
	for (int i = 0; i < arrayLength; i++) {
		currentRow[i] = -1;
		nextRow[i] = -1;
	}

	int aCharCodes[aLen];
	int bCharCodes[bLen];
	int i, t;
	for (i = 0, t = tStart; i < aLen; i++, t++) {
		aCharCodes[i] = a[t];
		bCharCodes[i] = b[t];
	}

	// If B has more characters, add them to bCharCodes
	while (i < bLen) {
		bCharCodes[i++] = b[t++];
	}

	i = 0;
	int conditionRow = dLen + ZERO_K;
	int endMax = conditionRow << 1; // Equivalent: conditionRow * 2
	do {
		i++;

		int tmp[arrayLength];

		// Array Swap
		memcpy(tmp, currentRow, sizeof (currentRow));
		memcpy(currentRow, nextRow, sizeof (nextRow));
		memcpy(nextRow, tmp, sizeof (tmp));

		int start;
		int previousCell;
		int currentCell = -1;
		int nextCell;

		if (i <= ZERO_K) {
			start = -i + 1;
			nextCell = i - 2;
		} else {
			start = i - (ZERO_K << 1) + 1;
			nextCell = currentRow[ZERO_K + start];
		}

		int end;
		if (i <= conditionRow) {
			end = i;
			nextRow[ZERO_K + i] = -1;
		} else {
			end = endMax - i;
		}

		for (int k = start, rowIndex = start + ZERO_K; k < end; k++, rowIndex++) {
			previousCell = currentCell;
			currentCell = nextCell;
			nextCell = currentRow[rowIndex + 1];

      		// Equivalent: max(t, previousCell, nextCell + 1)
			int t = currentCell + 1;
			t = t < previousCell ? previousCell : t;
			t = t < nextCell + 1 ? nextCell + 1 : t;

			while (t < aLen && t + k < bLen && aCharCodes[t] == bCharCodes[t + k]) {
				t++;
			}

			nextRow[rowIndex] = t;
		}
	} while (nextRow[conditionRow] < aLen && i <= threshold);

	return i - 1;
}

int EDUkkonenWithouThreshold (string a, string b) {
	if (a == b)
		return 0;

	// Swap a and b so b longer or same length as a
	// a will be shorter
	if (a.length() > b.length()) {
		string tmp = a;
		a = b;
		b = tmp;
	}

	int aLen = a.length(), bLen = b.length();

	// Performing suffix trimming:
  	// We can linearly drop suffix common to both strings since they don't increase distance at all
	while (aLen > 0 && a[aLen - 1] == b[bLen - 1]) {
		aLen--;
		bLen--;
	}

	if (aLen == 0) {
		return bLen;
	}

	// Performing prefix trimming
	// We can linearly drop prefix common to both strings since they don't increase distance at all
	int tStart = 0;
	while (tStart < aLen && a[tStart] == b[tStart]) {
		tStart++;
	}

	aLen -= tStart;
	bLen -= tStart;

	if (aLen == 0) {
		return bLen;
	}

	int dLen = bLen - aLen;

	// Equivalent: floor(aLen / 2)) + 2
	int ZERO_K = (aLen >> 1) + 2;

	int arrayLength = dLen + ZERO_K * 2 + 2;

	int currentRow[arrayLength];
	int nextRow[arrayLength];
	for (int i = 0; i < arrayLength; i++) {
		currentRow[i] = -1;
		nextRow[i] = -1;
	}

	int aCharCodes[aLen];
	int bCharCodes[bLen];
	int i, t;
	for (i = 0, t = tStart; i < aLen; i++, t++) {
		aCharCodes[i] = a[t];
		bCharCodes[i] = b[t];
	}

	// If B has more characters, add them to bCharCodes
	while (i < bLen) {
		bCharCodes[i++] = b[t++];
	}

	i = 0;
	int conditionRow = dLen + ZERO_K;
	int endMax = conditionRow << 1; // Equivalent: conditionRow * 2
	do {
		i++;

		int tmp[arrayLength];

		// Array Swap
		memcpy(tmp, currentRow, sizeof (currentRow));
		memcpy(currentRow, nextRow, sizeof (nextRow));
		memcpy(nextRow, tmp, sizeof (tmp));

		int start;
		int previousCell;
		int currentCell = -1;
		int nextCell;

		if (i <= ZERO_K) {
			start = -i + 1;
			nextCell = i - 2;
		} else {
			start = i - (ZERO_K << 1) + 1;
			nextCell = currentRow[ZERO_K + start];
		}

		int end;
		if (i <= conditionRow) {
			end = i;
			nextRow[ZERO_K + i] = -1;
		} else {
			end = endMax - i;
		}

		for (int k = start, rowIndex = start + ZERO_K; k < end; k++, rowIndex++) {
			previousCell = currentCell;
			currentCell = nextCell;
			nextCell = currentRow[rowIndex + 1];

      		// Equivalent: max(t, previousCell, nextCell + 1)
			int t = currentCell + 1;
			t = t < previousCell ? previousCell : t;
			t = t < nextCell + 1 ? nextCell + 1 : t;

			while (t < aLen && t + k < bLen && aCharCodes[t] == bCharCodes[t + k]) {
				t++;
			}

			nextRow[rowIndex] = t;
		}
	} while (nextRow[conditionRow] < aLen);

	return i - 1;
}


int main() {
	string stringA, stringB;
	printf("Insert String A\n");
	cin >> stringA;

	printf("Insert String B\n");
	cin >> stringB;

	printf("\n");
	printf("EDUkkonen: %d\n", EDUkkonen(stringA, stringB, oo)); // Ukkonen
	printf("EDUkkonenWithouThreshold: %d\n", EDUkkonenWithouThreshold(stringA, stringB)); // Ukkonen without threshold
}