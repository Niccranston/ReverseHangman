
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stdbool.h>
#include <ctype.h>
bool readfile(char *filename);
bool duplicates(const char *word);
void wordcategory(char ***words, int *wordcount, int wordlength, char inputletter);
void rungame(int wordlength);
char **words;
int wordcount = 0;
int wordlength;
int guesses;
char inputletter;
char guessedletters[26] = {0};
int main(int argc, char *argv[])
{
char *filename = argv[1];
wordlength = atoi(argv[2]);
guesses = atoi(argv[3]);
readfile(filename);
rungame(wordlength);
return 0;
}
bool readfile(char *filename)
{
FILE *fptr = fopen(filename, "r");
char buffer[32];
while (fscanf(fptr, "%31s", buffer) == 1)
{
if (strlen(buffer) == wordlength && !duplicates(buffer))
{
wordcount++;
}
}
words = (char **)malloc(wordcount * sizeof(char *));
rewind(fptr);
int index = 0;
while (fscanf(fptr, "%31s", buffer) == 1)
{
int len = strlen(buffer);
if (len == wordlength && !duplicates(buffer))
{
words[index] = (char *)malloc((len + 1) * sizeof(char));
strcpy(words[index], buffer);
index++;
}
}
fclose(fptr);
return true;
}
bool duplicates(const char *word)
{
int letters[26] = {0};
for (int i = 0; word[i] != '\0'; i++)
{
int index = word[i] - 'a';
if (letters[index] > 0)
{
return true;
}
letters[index]++;
}
return false;
}
void wordcategory(char ***words, int *wordcount, int wordlength, char inputletter)
{
int positioncounter[wordlength];
memset(positioncounter, 0, sizeof(positioncounter));
int nolettercounter = 0;
for (int i = 0; i < *wordcount; i++)
{
const char *word = (*words)[i];
int hasletter = false;
for (int j = 0; j < wordlength; j++)
{
if (word[j] == inputletter)
{
positioncounter[j]++;
hasletter = true;
}
}
if (!hasletter)
{
nolettercounter++;
}
}
int largestcount = positioncounter[0];
int largestposition = 0;
for (int i = 1; i < wordlength; i++)
{
if (positioncounter[i] > largestcount)
{
largestcount = positioncounter[i];
largestposition = i;
}
}
char **updatedwords;
int updatedwordcount;
if (nolettercounter > largestcount)
{
updatedwords = (char **)malloc(nolettercounter * sizeof(char *));
updatedwordcount = 0;
for (int i = 0; i < *wordcount; i++)
{
const char *word = (*words)[i];
int hasletter = 0;
for (int j = 0; j < wordlength; j++)
{
if (word[j] == inputletter)
{
hasletter = 1;
break;
}
}
if (!hasletter)
{
updatedwords[updatedwordcount] = (char *)malloc((wordlength + 1) *
sizeof(char));
strcpy(updatedwords[updatedwordcount], word);
updatedwordcount++;
}
}
}
else
{
updatedwords = (char **)malloc(*wordcount * sizeof(char *));
updatedwordcount = 0;
for (int i = 0; i < *wordcount; i++)
{
const char *word = (*words)[i];
if (word[largestposition] == inputletter)
{
updatedwords[updatedwordcount] = (char *)malloc((wordlength + 1) *
sizeof(char));
strcpy(updatedwords[updatedwordcount], word);
updatedwordcount++;
}
}
}
for (int i = 0; i < *wordcount; i++)
{
free((*words)[i]);
}
free(*words);
*wordcount = updatedwordcount;
*words = updatedwords;
}
void rungame(int wordlength)
{
char guessedletters[26] = {0};
char mysteryword[wordlength + 1];
int guesscounter = guesses;
memset(mysteryword, '-', wordlength);
mysteryword[wordlength] = '\0';
while (guesscounter > 0)
{
char inputletter;
printf("Guessed letters: ");
for (int i = 0; i < 26; i++)
{
if (guessedletters[i] == 1)
{
printf("%c ", 'a' + i);
}
}
printf("\nEnter Letter: ");
if (scanf(" %c", &inputletter) != 1)
{
printf("Enter a single letter only.\n");
while (getchar() != '\n');
continue;
}
if (!isalpha(inputletter))
{
printf("Please enter a letter.\n");
continue;
}
if (guessedletters[inputletter - 'a'] == 1)
{
printf("You've already guessed '%c'. Try again.\n", inputletter);
continue;
}
wordcategory(&words, &wordcount, wordlength, inputletter);
guessedletters[inputletter - 'a'] = 1;
for (int i = 0; i < wordcount; i++)
{
const char *word = words[i];
for (int j = 0; j < wordlength; j++)
{
if (word[j] == inputletter)
{
mysteryword[j] = inputletter;
}
}
}
printf("Current word: %s |", mysteryword);
guesscounter--;
printf(" Guesses remaining: %d | ", guesscounter);
if (strcmp(mysteryword, words[0]) == 0)
{
printf("\nCongratulations! The word was: %s\n", mysteryword);
break;
}
}
if (guesscounter == 0)
{
int randomidx = rand() % wordcount;
printf("\nThe random word was: %s\n", words[randomidx]);
}
}
