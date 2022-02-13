#include <stdio.h>
#include <stdlib.h>
#include <string.h>

struct wordquant{
	char* word;
	int quant;
};

int main(int argc, char **argv){
	char *main_str = argv[1];
	char **words;
	struct wordquant *map;
	int c_words = 0;
	int c_word_symbs = 0;
	int c_symbs = -1; //counter for going throug all string straight

	int length = -1;
	int words_quantity = 1;
	int curr_word_length = 0;
	int tmp_counter = 0;

	printf("\n|--------------------|\n");

lover_case:
	length++;
	if(main_str[length] >= 65 && main_str[length] <= 90) main_str[length] += 32;
	if(main_str[length] != '\0') goto lover_case;

length = 0;
calc_symbs:
	length++;
	if(main_str[length] == ' ') words_quantity++;
	if(main_str[length] != '\0') goto calc_symbs;

	printf("Length = %d;", length);
	printf("\nWords quant = %d;\n\n", words_quantity);

	words = (char**) malloc((words_quantity + 1) * sizeof(char*));


fill_words:

    tmp_counter = c_symbs+1;
word_len:
	if(main_str[tmp_counter] != ' ' && main_str[tmp_counter] != '\0') {
		curr_word_length++; tmp_counter++;
		goto word_len;
	}

	words[c_words] = (char*) malloc( (curr_word_length+1) * sizeof(char) );
	curr_word_length = 0;

write_word:
	c_symbs++;
	
	if(main_str[c_symbs] == '\0') goto continue_1;
	if(main_str[c_symbs] == ' ' || main_str[c_symbs] == '\n') {c_words++; c_word_symbs = 0; goto fill_words;} //end of word
	if(main_str[c_symbs] == ',' || main_str[c_symbs] == '.') goto write_word; //skip this symbols
	words[c_words][c_word_symbs] = main_str[c_symbs];
	c_word_symbs++;
	goto write_word;
continue_1:

	c_words = 0;
remove_stopwords:
	if(c_words >= words_quantity) goto continue_2; // no words left
	if( strcmp(words[c_words], "the") != 0 &&
		strcmp(words[c_words], "in" ) != 0 &&
		strcmp(words[c_words], "to" ) != 0 &&
		strcmp(words[c_words], "but") != 0 &&
		strcmp(words[c_words], "any") != 0 &&
		strcmp(words[c_words], "is" ) != 0 &&
		strcmp(words[c_words], "and") != 0 &&
		strcmp(words[c_words], ""  ) != 0 &&
		strcmp(words[c_words], " "  ) != 0 &&
		strcmp(words[c_words], ","  ) != 0 &&
		strcmp(words[c_words], "."  ) != 0 &&
		strcmp(words[c_words], "for") != 0) { // not a stop word
		c_words++; goto remove_stopwords;
	}
	tmp_counter = c_words;
	free(words[c_words]);
move_left:
	words[tmp_counter] = words[tmp_counter+1];
	tmp_counter++;
if(tmp_counter < words_quantity) goto move_left;
	words_quantity--;
goto remove_stopwords;

continue_2:
	map = malloc( (words_quantity+1) * sizeof(struct wordquant));
	int map_size = 0;


int countwordsx = 0;
	char* curword;
	char is_present;
	int is_pres_count;
countwords:
	curword = words[countwordsx];
	is_present = 0;
	is_pres_count = 0;
is_word_present:
	if(is_pres_count >= map_size) goto cont3;
	if(strcmp(map[is_pres_count].word, curword) == 0) {
		map[is_pres_count].quant += 1;
		is_present = 1;
	}
	is_pres_count++;
if(is_pres_count < map_size) goto is_word_present;
cont3:
	if(is_present == 0){
			map[map_size] = (struct wordquant){.word=curword, .quant=1};
			map_size++;
	}
	countwordsx++;
	if(countwordsx < words_quantity) goto countwords;


int maxnum;
int maxindex;
int general_print_count = 0;
print:
	maxnum = 0;
	maxindex = 0;
	tmp_counter = 0;
printmax:
	if(map[tmp_counter].quant >= maxnum){
		maxnum = map[tmp_counter].quant;
		maxindex = tmp_counter;
	}
	tmp_counter++;
if(tmp_counter < map_size) goto printmax;
	printf("%d - {'%s', %d}\n", general_print_count+1, map[maxindex].word, map[maxindex].quant);
	map[maxindex].quant = 0;
	general_print_count++;
if(general_print_count < map_size && general_print_count < 25) goto print;


int wfree_count = 0;
endcycles:

wfree:
	if(words[wfree_count]) free(words[wfree_count]);
	wfree_count++;
if(wfree_count < words_quantity) goto wfree;
	free(words);
	free(map);

	return 0;
}