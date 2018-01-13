/*
 *  Tested with http://www.spoj.com/problems/LCS0/
 */
#include <stdio.h>

// Settings for lower case Latin Letters of 50016 length 
#define	WLEN	  32	// word length
#define	LOGWLEN	  50016	// log word length -- round(LOG2(WLEN)
#define SMAX	  50016	// maximum string length -- multiple of WLEN
#define BITMAX	  1563	// maximum bit string length -- round(SMAX/WLEN)
#define	ALPHASIZE 26	// alphabet size

typedef unsigned int WORD;
typedef short unsigned int INDEX;	// i: alpha[i] == char

int nwords;				// Number of words for bits
WORD bit1[BITMAX];
WORD bit2[BITMAX];
WORD a_strings[ALPHASIZE][BITMAX];
WORD *pb1, *pb2, *t1;

char alpha[ALPHASIZE] = {'A', 'B', 'C', 'D', 'E', 'F', 'G', 'H', 'I', 'J', 'K', 'L', 'M', 'N', 'O', 'P', 'Q', 'R', 'S', 'T', 'U', 'V', 'W', 'X', 'Y', 'Z'};	// Alphabet, just for debugging

WORD bitmask[LOGWLEN] =
		{0x55555555, 0x33333333, 0x0f0f0f0f, 0x00ff00ff, 0x0000ffff};

/*
 * Calculates ROW i 
 * ROW i = x AND ((x - (rowi-1 << 1)) != x). Where x = ROW i
 */
void bitops(WORD * last, WORD * cur, INDEX index) {
	WORD x, y;
	int j;
	WORD *a_s;
	WORD top_borrow, bottombit;

	a_s = &a_strings[index][0];
	bottombit = 1;
	for (j = 0; j < nwords; j++) {
		y = *(last++);
		x =  y | *(a_s++);
		top_borrow = (y >> (WLEN - 1)) & 0x1;
		y = ((y << 1) | bottombit);
		if (x < y)
			top_borrow = 1;
		*(cur++) = x & ((x - y) ^ x);
		bottombit = top_borrow;
	}
}


/*
 * Prints in console Alphabet String for String A
 * E.G: A-String: 01 | B-String: 10 for String A = "ab"
 */
void debugAlphabetString(int len) {
	for (int i = 0; i < ALPHASIZE; i++) {
		printf("%c-string :  ", alpha[i]);
		for (int j = len - 1; j >= 0; j--)
			if (j % WLEN == 0)
				printf("%1x ", (a_strings[i][j / WLEN] >> (j % WLEN)) & 1);
			else
				printf("%1x", (a_strings[i][j/WLEN] >> (j % WLEN)) & 1);
		printf("\n");
	}
}

/*
 * Builds the "Alphabet String" for String A
 * Later, this will be used to calculate ROW i using (ROW i-1 OR Alphabet String[i° character in String B])
 */
void alphastrings(INDEX * s, int len) {
	INDEX *p;
	int i, j;

	for (i = 0; i < ALPHASIZE; i++)
		for (j = 0; j < nwords; j++)
			a_strings[i][j] = 0;
		
	p =  s;
	j = len;
	for (i = 0; i < j; i++)
		a_strings[*(p++)][i / WLEN] |=  1 << (i % WLEN);

	// debugAlphabetString(len); // DEBUG
}

/*
 * Counts number of 1s in top ROW: LCS final result!
 */
int bitcount(WORD * wp) {
	WORD w, count;
	int j, rshift, i;

	count = 0;
	for (i = 0; i < nwords; i++) {
		w = *(wp++);
		rshift = 1;
		for (j = 0; j < LOGWLEN; j++) {
			w = (w & bitmask[j]) + ((w & ~bitmask[j]) >> rshift);
			rshift <<= 1;
		}
		count += w;
	}
	return (count);
}

int bitlcs(INDEX * a, int lena, INDEX * b, int lenb) {
	int i;
	INDEX *pbstring;
	int j, k;

	nwords = (lena + WLEN - 1) / WLEN;

	alphastrings(a, lena);

	pb1 = &bit1[0];
	for (i = 0; i < nwords; i++)
		*pb1++ = 0;		// Equivalent to: bit1[i] = 0;
	pb1 = &bit1[0]; pb2 = &bit2[0];
	pbstring = b;

	for (i = 1; i <= lenb; i++) {
		bitops(pb1, pb2, *(pbstring++));

		/* DEBUG: shows rows content
		printf("row[%2d] :   ", i);
		for (j = lena - 1; j >= 0; j--) {
			k = (pb2[j / WLEN] >> (j % WLEN)) & 1;
			if (j % WLEN == 0)
				printf("%1x ", k);
			else
				printf("%1x", k);
		}
		printf("\n");
		*/
	

		t1 = pb1; pb1 = pb2; pb2 = t1;
	}
	return (bitcount(pb1));
}

/*
 * Reads a String from stdin
 */
int readString(INDEX * s) {
	int c, index, i = 0;
	while((c = getchar()) != EOF && c != '\n') {
		s[i++] = (int) c - 97; 
	}
	return i;
}


int main(int argc, char const *argv[]) {	
	INDEX stringA[SMAX];
	INDEX stringB[SMAX];
	int lenA, lenB;

	printf("Insert String A:\n");
	lenA = readString(stringA);

	printf("Insert String B:\n");
	lenB = readString(stringB);

	printf("\n");

	// printf("LenA: %d | LenB: %d\n", lenA, lenB);

	printf("LCS: %d\n", bitlcs(stringA, lenA, stringB, lenB));
	return 0;
}