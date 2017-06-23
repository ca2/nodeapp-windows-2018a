#pragma once


int_bool file_exists_raw(const char * path1);

void file_put_contents_raw(const char * path, const char * psz);

void file_add_contents_raw(const char * path, const char * psz);

void file_beg_contents_raw(const char * path, const char * psz);

uint64_t file_length_raw(const char * path);