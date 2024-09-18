#ifndef MP3_H
#define MP3_H

typedef struct _Mp3DataInfo
{

	char *mp3_view_name;
	char *mp3_edit_name;
	FILE *mp3_file_name;
	char *mp3_ide;
	char *mp3_tag;
	char *tag_data;
	char *mp3_duplicate_name;
	FILE *mp3_dup_file_name;

} Mp3DataInfo;

/* Check operation type */
OperationType check_operation_type(char *argv[]);

/*VIEW FUNCTION PROTOTYPES*/

/* Get File pointers for i/p and o/p files */
Status open_files_view(Mp3DataInfo *mp3Info);

/* Read and validate view args from argv */
Status read_and_validate_view(char *argv[],Mp3DataInfo *mp3Info);

/*Perform for view the mp3 file data*/
Status view_status(Mp3DataInfo *mp3Info);

/*Check the header of mp3 file*/
Status mp3_header_check_view(Mp3DataInfo *mp3Info);

/*Get the data of mp3 file*/
Status mp3_view(Mp3DataInfo *mp3Info);

/*EDIT FUNCTION PROTOTYPES*/

/* Read and validate view args from argv */
Status read_and_validate_edit(char *arg[],Mp3DataInfo * mp3Info);

/* Get File pointers for i/p and o/p files */
Status open_files_edit(Mp3DataInfo *mp3Info);

/*Get the data of mp3 file after edit*/
Status mp3_edit(char *argv[],Mp3DataInfo *mp3Info);

/*Little Endian to Big Endian Conversion for source file*/
int main_file_endian_conversion(int value);

/*Read and write the data of TAG and FLAG of source file into duplicate file*/
Status main_file_data_to_duplicate_file(Mp3DataInfo *mp3Info);

/*Little Endian to Big Endian Conversion for duplicate file*/
Status duplicate_file_endian_conversion(char * argv[],Mp3DataInfo *mp3Info);

/*Perform for view the mp3 file data*/
Status edit_status(char *argv[],Mp3DataInfo *mp3Info);

/*Check the option from argv[2] of which we have going to edit " -t -> TITLE NAME/-a -> ARTIST NAME/-A -> ALBUM NAME/-y -> YEAR/-m -> CONTENT TYPE/-c -> COMMENT*/
Status edit_option_check(char *argv[],Mp3DataInfo *mp3Info);

/*Check the header of mp3 file*/
Status mp3_header_check_edit(Mp3DataInfo *mp3Info);

/*Copy remaining data of source file into duplictae file after edit the data*/
Status copy_remaining_data(Mp3DataInfo *mp3Info);

/*Copy the duplicate file data into original source after edit*/
Status duplicate_file_data_to_main_file_after_change(Mp3DataInfo *mp3Info);
#endif
