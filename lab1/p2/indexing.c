#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#define PAGE_SIZE 45

struct wordnpages{
	char* word;
	int quant;
	int pages[100];
	int pages_count;
};

int main(int argc, char **argv){
	char *main_str;
	char **words;
	struct wordnpages *cmap;
	int c_words = 0;
	int c_word_symbs = 0;
	int c_symbs = -1; //counter for going throug all string straight
	int filewordsquant = 0;

	int length = -1;
	int words_quantity = 1;
	int curr_word_length = 0;
	int tmp_counter = 0;
	int wfree_count = 0;
	int temp_iterator = 0;
	int cmap_size = 0;

	char *str1;
	char ch;

	printf("-----------------------------------------------------\n"); //-----test

	FILE *file;

	file = fopen(argv[1], "r");
	if(!file) {printf("No such file.\n"); goto crash;};

get_filewordsquant:
	ch = fgetc(file);
	if(ch == ' ' || ch == '\n') filewordsquant++;
if(ch != EOF) goto get_filewordsquant;
	fseek(file, 0, SEEK_SET);
	printf("File words quantity = %d\n", filewordsquant);

	cmap = malloc( (filewordsquant) * sizeof(struct wordnpages) );


	int indexing_count = 0;
	int linesize = 0;

indexing:
getlinesize:
	ch = fgetc(file);
	if(ch != '\n' && ch != EOF){linesize++; goto getlinesize;}
	linesize++;

	fseek(file, (-1)*(linesize), SEEK_CUR);

	str1 = malloc( (linesize) * sizeof(char));

	fgets(str1, linesize+1, file);
	str1[linesize-1] = '\0';
	indexing_count++;


	main_str = str1;
	if(strcmp(main_str, "") == 0) {goto endcycles;};
	c_words = 0;
	c_word_symbs = 0;
	c_symbs = -1; //counter for going throug all string straight

	length = -1;
	words_quantity = 1;
	curr_word_length = 0;
	tmp_counter = 0;

lover_case:
	length++;
	if(main_str[length] >= 65 && main_str[length] <= 90) main_str[length] += 32;
	if(main_str[length] != '\0') goto lover_case;

length = 0;
calc_symbs:
	length++;
	if(main_str[length] == ' ') words_quantity++;
	if(main_str[length] != '\0') goto calc_symbs;

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

	if( main_str[c_symbs] == ',' ||
	    main_str[c_symbs] == '.' ||
	    main_str[c_symbs] == '-' ||
	    main_str[c_symbs] == '(' ||
	    main_str[c_symbs] == ')' ||
	    main_str[c_symbs] == '!'
	  ) goto write_word; //skip this symbols
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
		strcmp(words[c_words], "am") != 0 &&
		strcmp(words[c_words], "by") != 0 &&
		strcmp(words[c_words], "are") != 0 &&
		strcmp(words[c_words], "all") != 0 &&
		strcmp(words[c_words], ""  ) != 0 &&
		strcmp(words[c_words], " "  ) != 0 &&
		strcmp(words[c_words], "a"  ) != 0 &&
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
	temp_iterator+=2;
	int countwordsx = 0;
	char* curword;
	char is_present;
	int is_pres_count;
	int tmp_page;
	int check_pages_count = 0;
	char is_page_present = 0;
countwords:
	curword = words[countwordsx];
	is_present = 0;
	is_pres_count = 0;
is_word_present:
	if(is_pres_count >= cmap_size) goto cont3;
	if(strcmp(cmap[is_pres_count].word, curword) == 0) {
		cmap[is_pres_count].quant += 1;
		check_pages_count = 0;
		is_page_present = 0;
		check_pages:
			if(cmap[is_pres_count].pages[check_pages_count] == (indexing_count/PAGE_SIZE) ) is_page_present = 1;
			check_pages_count++;
		if(check_pages_count < cmap[is_pres_count].pages_count) goto check_pages;
		if(is_page_present == 0){
			cmap[is_pres_count].pages[ cmap[is_pres_count].pages_count ] = (indexing_count/PAGE_SIZE);
			cmap[is_pres_count].pages_count += 1;
		}
		is_present = 1;
	}
	is_pres_count++;
if(is_pres_count < cmap_size) goto is_word_present;
cont3:
	if(is_present == 0){
			cmap[cmap_size] = (struct wordnpages){.word=curword, .quant=1, .pages[0]=(indexing_count/PAGE_SIZE), .pages_count=1};
			cmap_size++;
	}
	countwordsx++;
	if(countwordsx < words_quantity) goto countwords;


endcycles:

linesize = 0;
if(ch == EOF) goto endpr;
goto indexing;

endpr:
	fclose(file);
	int maxindex;
	int general_print_count = 0;
	int printpages_count = 0;
	print:
		maxindex = 0;
		tmp_counter = 0;
	printmax:
		if( strcmp(cmap[tmp_counter].word, cmap[maxindex].word) < 0 ){
			maxindex = tmp_counter;
		}
		tmp_counter++;
	if(tmp_counter < cmap_size) goto printmax;
		if(strcmp(cmap[maxindex].word, "}") == 0) goto greater100;
		if(cmap[maxindex].quant > 100) goto greater100;
		printf("%d. %s - ", general_print_count+1, cmap[maxindex].word);
		printpages_count = 0;
		printpages:
			printf("%d, ", cmap[maxindex].pages[printpages_count]);
			printpages_count++;
		if(printpages_count < cmap[maxindex].pages_count) goto printpages;
		printf("\n");

	greater100:
		cmap[maxindex].word = "}"; // will not concure to output
		general_print_count++;
	if(general_print_count < cmap_size) goto print;


wfree_count = 0;
wfree:
	if(words[wfree_count]){
		free(words[wfree_count]);
	}
	wfree_count++;
if(wfree_count < words_quantity-1) goto wfree;
	if(words) free(words);
	if(cmap) free(cmap);

crash:
	return 0;
}