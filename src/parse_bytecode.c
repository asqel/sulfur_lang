#include "../include/bytecode.h"
#include <stdlib.h>
#include <stdint.h>

uint64_t get_i64_at(unsigned char *text, int pos) {
	uint64_t res = 0;
	res |= text[pos + 7] << (7  * 8);
	res |= text[pos + 6] << (6  * 8);
	res |= text[pos + 5] << (5  * 8);
	res |= text[pos + 4] << (4  * 8);
	res |= text[pos + 3] << (3  * 8);
	res |= text[pos + 2] << (2  * 8);
	res |= text[pos + 1] << (1  * 8);
	res |= text[pos];
}

void parse_bytecode_header(bytecode_info *info, uti_Bytes bytecode, char * path) {
	unsigned char *header = &(bytecode.vals[4 + 8]);
	info->file_type = header[0];
	info->date_time = &(header[1]);

	int time_str_end = 1;
	while (header[time_str_end] != '\0')
		time_str_end++;

	info->var_left   = &(bytecode.vals[get_i64_at(header, time_str_end + 1) + 8]);
	info->var_right  = &(bytecode.vals[get_i64_at(header, time_str_end + 1 + 8) + 8]);
	info->strings    = &(bytecode.vals[get_i64_at(header, time_str_end + 1 + 16) + 8]);
	parse_extra_data(info, &(bytecode.vals[get_i64_at(header, time_str_end + 1 + 24)]));
	
}


bytecode_info parse_bytecode(uti_Bytes bytecode, char *path) {
	bytecode_info res;
	res.byte_code = bytecode;
	res.code = &(bytecode.vals[get_i64_at(bytecode.vals, 4)]);

	parse_bytecode_header(&res, bytecode, path);

	res.var_left_len = get_i64_at(bytecode.vals, res.var_left - 8);
	res.var_right_len = get_i64_at(bytecode.vals, res.var_right - 8);
	res.strings_len = get_i64_at(bytecode.vals, res.strings - 8);
}


/*
nothing will be copied
only 2 pointer will be allocated .keys / .values
*/
bytecode_info *parse_extra_data(bytecode_info *info, unsigned char *extra_data) {
	info->extra_data.key_len = get_i64_at(extra_data, 0);
	info->extra_data.keys = malloc(sizeof(char *) * info->extra_data.key_len);
	info->extra_data.values = malloc(sizeof(extra_data_t) * info->extra_data.key_len);
	int current_key = 0;
	int p = 8;
	while (current_key < info->extra_data.key_len) {
		int key_end = p;
		while (extra_data[key_end] != '\0') key_end++;
		
		key_end++;
		info->extra_data.keys[current_key] = p;
		info->extra_data.values[current_key].data_len = get_i64_at(extra_data, key_end);
		key_end += 8;
		info->extra_data.values[current_key].data = &(extra_data[key_end]);
		p = key_end + info->extra_data.values[current_key].data_len;
		current_key++;
	}
	return info;
}