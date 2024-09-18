#include<stdio.h>
#include "types.h"
#include "mp3.h"
#include "mp3_macro.h"
#include<string.h>

Status read_and_validate_edit(char *argv[],Mp3DataInfo *mp3Info)
{
	 /*Check the taken file is '.mp3' or not*/
        if(strstr(argv[4],".mp3")!=NULL)
	{
		mp3Info->mp3_edit_name=argv[4];
	}
	else
	{
		return m_failure;
	}

	mp3Info->mp3_duplicate_name="sample.mp3";

         // No failure return m_success
	return m_success;
}

Status open_files_edit(Mp3DataInfo *mp3Info)
{
       
        //Open the Source fIle
	mp3Info->mp3_file_name=fopen(mp3Info->mp3_edit_name,"r+");
	if(mp3Info->mp3_file_name==NULL)
	{
		perror("fopen");
		fprintf(stderr, "ERROR: Unable to open source file %s\n", mp3Info->mp3_edit_name);
		return m_failure;
	}

        //Open the Duplicate File
	mp3Info->mp3_dup_file_name=fopen(mp3Info->mp3_duplicate_name,"r+");
	if(mp3Info->mp3_dup_file_name==NULL)
	{
		perror("fopen");
		fprintf(stderr, "ERROR: Unable to open duplicate file %s\n", mp3Info->mp3_duplicate_name);
		return m_failure;
	}
        
        // No failure return m_success
	return m_success;
}

Status edit_status(char *argv[],Mp3DataInfo *mp3Info)
{
        //Function Calls
	if(open_files_edit(mp3Info)==m_failure)
	{
		printf("open file is failure\n");
		return m_failure;
	}
	if(mp3_header_check_edit(mp3Info)==m_failure)
	{
		printf("Header checking is failure\n");
		return m_failure;
	}
	if(edit_option_check(argv,mp3Info)==m_failure)
	{
		printf("Selected option to edit is Failed\n");
		return m_failure;
	}
	if(mp3_edit(argv,mp3Info)==m_failure)
	{
		printf("Edit the mp3 file data is failure\n");
		return m_failure;
	}
	if(duplicate_file_data_to_main_file_after_change(mp3Info)==m_failure)
	{
		printf("Error:Unable to copy the duplicate file data to source file\n");
		return m_failure;
	}
	return m_success;

}

Status mp3_header_check_edit(Mp3DataInfo *mp3Info)
{
        
	char ch[ID3_SIZE];
	fread(ch,ID3_SIZE,1,mp3Info->mp3_file_name);

	mp3Info->mp3_ide=ch;

	//printf("The ID3 TAG is %s\n",mp3Info->mp3_ide);
        
        //Read the First 3 bytes of header and check that is a ID3
	if(strcmp(mp3Info->mp3_ide,"ID3")==0)
	{
		//printf("ID3 tag Check is Success\n");
	}
	else
	{
                //It is not a ID3 then return m_failure
		printf("ID3 tag Check is Failed\n");
		return m_failure;
	}

	short num;
	char temp=0;

	fread(&num,2,1,mp3Info->mp3_file_name);

        //Read the next 2 bytes of header and convert the bytes into little endian to big endian
	for(int i=0;i<1;i++)
	{
		temp=*((char*)&num+i);
		*((char*)&num+i)=*((char*)&num+(2-1-i));
		*((char*)&num+(2-1-i))=temp;
	}
	//printf("After conversion %X\n",num);

        //Check the ID3 is a version 3 or not
	if(num==0x0300)
	{
		//printf("The version checing is success\n");
	}
	else
	{
                //If it is not a version 3 then m_failure
		printf("The version checking is failure\n");
		return m_failure;
	}

	char buffer[HEADER_SIZE];
	fseek(mp3Info->mp3_file_name,0,SEEK_SET);
        
        //If the ID3 and version checking is success then read the header of first 10 bytes from source file and write into a duplicate file
	fread(buffer,HEADER_SIZE,1,mp3Info->mp3_file_name);
	fwrite(buffer,HEADER_SIZE,1,mp3Info->mp3_dup_file_name);

        // No failure return m_success
	return m_success;
}

//Intilalize the flags for checking purpose
int t_flag=0;
int a_flag=0;
int b_flag=0;
int y_flag=0;
int m_flag=0;
int c_flag=0;

Status edit_option_check(char *argv[],Mp3DataInfo *mp3Info)
{
	printf("\e[1;33m--------------- SELECTED EDIT OPTION ----------------\n\e[0m");

        //If the argument vector 2 is '-t' then the user selects for to change the song title
	if(strcmp(argv[2],"-t")==0)
	{
		printf("\e[1;34m--------------- CHANGE THE SONG TITLE ---------------\n\e[0m");

                //Then set t_flag as 1
		t_flag=1;
	}

        //If the argument vector 2 is '-a' then the user selects for to change the artist name
	else if(strcmp(argv[2],"-a")==0)
	{
		printf("\e[1;34m--------------- CHANGE THE ARTIST NAME ---------------\n\e[0m");
		
                //Then set a_flag as 1
                a_flag=1;
	}

        //If the argument vector 2 is '-A' then the user selects for to change the Album name
	else if(strcmp(argv[2],"-A")==0)
	{
		printf("\e[1;34m-------------- CHANGE THE ALBUM NAME --------------\n\e[0m");

                //Then set b_flag as 1
		b_flag=1;
	}

        //If the argument vector 2 is '-y' then the user selects for to change the year
	else if(strcmp(argv[2],"-y")==0)
	{
		printf("\e[1;34m--------------- CHANGE THE YEAR ---------------\n\e[0m");

                //Then set y_flag as 1
		y_flag=1;
	}

        //If the argument vector 2 is '-m' then the user selects for to change the content
	else if(strcmp(argv[2],"-m")==0)
	{
		printf("\e[1;34m--------------- CHANGE THE CONTENT ---------------\n\e[0m");

                //Then set m_flag as 1
		m_flag=1;
	}

        //If the argument vector 2 is '-c' then the user selects for change the comment
	else if(strcmp(argv[2],"-c")==0)
	{
		printf("\e[1;34m--------------- CHANGE THE COMMENT ---------------\n\e[0m");

                //Then set c_flag as 1
		c_flag=1;
	}

	else
	{
                //Then the user entered any other argument then return m_failure
		return m_failure;
	} 
       
        // No failure return m_success
	return m_success;
}

int num;
char temp;
short skip;
int size;

Status mp3_edit(char *argv[],Mp3DataInfo *mp3Info)
{
	
        //Read the TAGS from Source file and write into a duplicate file
	char buffer[TAG_SIZE];
	fread(buffer,TAG_SIZE,1,mp3Info->mp3_file_name);
	fwrite(buffer,TAG_SIZE,1,mp3Info->mp3_dup_file_name);
	mp3Info->mp3_tag=buffer;
	//printf("The Tag is %s\n",mp3Info->mp3_tag);
        
        //Read the 4 bytes and find the size of the data of every TAG
	fread(&num,4,1,mp3Info->mp3_file_name);

        //Function Call
	int store=main_file_endian_conversion(num); //Function Call for convert the size of arument vector 3 from little endian to big endian and write in a duplicate file
	//printf("store %d\n",store);

	char data[store];

        //Check the TAG is a TIT2 or not
	if(strcmp(mp3Info->mp3_tag,"TIT2")==0)
	{
                //If the TAG is TIT2 then t_flag is equal to 1 then change the title
		if(t_flag==1)
		{
			printf("\e[1;31mTITLE : \e[0m");

                        //Storing the arument vector 3 data into one structure pointer
			mp3Info->tag_data=argv[3];

			printf("%s\n",mp3Info->tag_data);

                        //Function Call for convert the size of arument vector 3 from little endian to big endian and write in a duplicate file
			duplicate_file_endian_conversion(argv,mp3Info);

                        //Size of the arument vector 3 data
			size=strlen(mp3Info->tag_data);
                       
                        //Read the data of TIT2 TAG of that size from source file
			fread(data,store,1,mp3Info->mp3_file_name);
                        
                        //Write the data of arument vector 3 of that size into a duplicate file
			fwrite(mp3Info->tag_data,size,1,mp3Info->mp3_dup_file_name);
			printf("\e[1;32m--------------- SONG TITLE CHANGED SUCCESSFULLY ---------------\n\e[0m");

                        //Function call for copy the remaining data from source file to duplicate file
			copy_remaining_data(mp3Info);
		}
		else
		{   
                        //IF the t_flag is not equal to 1 then copy the same data of TIT2 TAG from source file to duplicate file
			main_file_data_to_duplicate_file(mp3Info);

			fread(data,store,1,mp3Info->mp3_file_name);
			fwrite(data,store,1,mp3Info->mp3_dup_file_name);

                        //Function Call to check the other tags
			mp3_edit(argv,mp3Info);
		}
	}

        //Check the TAG is a TPE1  or not
	else if(strcmp(mp3Info->mp3_tag,"TPE1")==0)
	{
                //If the TAG is TPE1 then a_flag is equal to 1 then change the artist name
		if(a_flag==1)
		{
			printf("\e[1;31mARTIST : \e[0m");
                        
                        //Storing the arument vector 3 data into one structure pointer
			mp3Info->tag_data=argv[3];

			printf("%s\n",mp3Info->tag_data);

                         //Function Call for convert the size of arument vector 3 from little endian to big endian and write in a duplicate file
			duplicate_file_endian_conversion(argv,mp3Info);
                          
                        //Size of the arument vector 3 data 
			size=strlen(mp3Info->tag_data);

                        //Read the data of TPE1 TAG of that size from source file
			fread(data,store,1,mp3Info->mp3_file_name); 

                         //Write the data of arument vector 3 of that size into a duplicate file
			fwrite(mp3Info->tag_data,size,1,mp3Info->mp3_dup_file_name);
			printf("\e[1;32m--------------- ARTIST NAME CHANGED SUCCESSFULLY ---------------\n\e[0m");

                         //Function call for copy the remaining data from source file to duplicate file
			copy_remaining_data(mp3Info);
		}
		else
		{
                         //IF the a_flag is not equal to 1 then copy the same data of TPE1 TAG from source file to duplicate file
			main_file_data_to_duplicate_file(mp3Info);

			fread(data,store,1,mp3Info->mp3_file_name);
			fwrite(data,store,1,mp3Info->mp3_dup_file_name);
                       
                        //Recursive Function Call to check the other tags
			mp3_edit(argv,mp3Info);
		}

	}

        //Check the TAG is a TALB or not
	else if(strcmp(mp3Info->mp3_tag,"TALB")==0)
	{
                //If the TAG is TALB then b_flag is equal to 1 then change the album name
		if(b_flag==1)
		{
			printf("\e[1;31mALBUM : \e[0m");

                        //Storing the arument vector 3 data into one structure pointer
			mp3Info->tag_data=argv[3];
			printf("%s\n",mp3Info->tag_data);

                         //Function Call for convert the size of arument vector 3 from little endian to big endian and write in a duplicate file
			duplicate_file_endian_conversion(argv,mp3Info);
                       
                        //Size of the arument vector 3 data
			size=strlen(mp3Info->tag_data);

                        //Read the data of TALB TAG of that size from source file
			fread(data,store,1,mp3Info->mp3_file_name);

                         //Write the data of arument vector 3 of that size into a duplicate file
			fwrite(mp3Info->tag_data,size,1,mp3Info->mp3_dup_file_name);
			printf("\e[1;32m---------------	ALBUM NAME CHANGED SUCCESSFULLY ---------------\n\e[0m");
                         
                         //Function call for copy the remaining data from source file to duplicate file
			copy_remaining_data(mp3Info);
		}
		else
		{
                         //IF the b_flag is not equal to 1 then copy the same data of TALB TAG from source file to duplicate file
			main_file_data_to_duplicate_file(mp3Info);

			fread(data,store,1,mp3Info->mp3_file_name);
			fwrite(data,store,1,mp3Info->mp3_dup_file_name);

                        //Recursive Function Call to check the other tags
			mp3_edit(argv,mp3Info);
		}

	}

        //Check the TAG is a TYER or not
	else if(strcmp(mp3Info->mp3_tag,"TYER")==0)
	{
                //If the TAG is TYER then y_flag is equal to 1 then change the year
		if(y_flag==1)
		{
			printf("\e[1;31mYEAR : \e[0m");

                        //Storing the arument vector 3 data into one structure pointer
			mp3Info->tag_data=argv[3];
			printf("%s\n",mp3Info->tag_data);

                         //Function Call for convert the size of arument vector 3 from little endian to big endian and write in a duplicate file
			duplicate_file_endian_conversion(argv,mp3Info);

                        //Size of the arument vector 3 data
			size=strlen(mp3Info->tag_data);

                        //Read the data of TYER TAG of that size from source file
			fread(data,store,1,mp3Info->mp3_file_name);

                         //Write the data of arument vector 3 of that size into a duplicate file
			fwrite(mp3Info->tag_data,size,1,mp3Info->mp3_dup_file_name);

			printf("\e[1;32m--------------- YEAR CHANGED SUCCESSFULLY ---------------\n\e[0m");

                         //Function call for copy the remaining data from source file to duplicate file
			copy_remaining_data(mp3Info);
		}
		else
		{
                        //IF the y_flag is not equal to 1 then copy the same data of TYER TAG from source file to duplicate file
			main_file_data_to_duplicate_file(mp3Info);

			fread(data,store,1,mp3Info->mp3_file_name);
			fwrite(data,store,1,mp3Info->mp3_dup_file_name);

                        //Recursive Function Call to check the other tags
			mp3_edit(argv,mp3Info);
		}
	}
        
        //Check the TAG is a TCON or not
	else if(strcmp(mp3Info->mp3_tag,"TCON")==0)
	{
                //If the TAG is TCON then m_flag is equal to 1 then change the content
		if(m_flag==1)
		{
			printf("\e[1;31mMUSIC : \e[0m");

                        //Storing the arument vector 3 data into one structure pointer
			mp3Info->tag_data=argv[3];
			printf("%s\n",mp3Info->tag_data);

                         //Function Call for convert the size of arument vector 3 from little endian to big endian and write in a duplicate file
			duplicate_file_endian_conversion(argv,mp3Info);
                        
                        //Size of the arument vector 3 data
			size=strlen(mp3Info->tag_data);

                        //Read the data of TCON TAG of that size from source file 
			fread(data,store,1,mp3Info->mp3_file_name);

                         //Write the data of arument vector 3 of that size into a duplicate file
			fwrite(mp3Info->tag_data,size,1,mp3Info->mp3_dup_file_name);
  
			printf("\e[1;32m--------------- CONTENT CHANGED SUCCESSFULLY ---------------\n\e[0m");

                        //Function call for copy the remaining data from source file to duplicate file
			copy_remaining_data(mp3Info);
		}
		else
		{
                        //IF the m_flag is not equal to 1 then copy the same data of TCON TAG from source file to duplicate file
			main_file_data_to_duplicate_file(mp3Info);

			fread(data,store,1,mp3Info->mp3_file_name);
			fwrite(data,store,1,mp3Info->mp3_dup_file_name);

                        //Recursive Function Call to check the other tags
			mp3_edit(argv,mp3Info);
		}
	}

        //Check the TAG is a TCOM or not
	else if(strcmp(mp3Info->mp3_tag,"TCOM")==0)
	{
                //If the TAG is TCCOM then c_flag is equal to 1 then change the comment
		if(c_flag==1)
		{
			printf("\e[1;31mCOMMENT : \e[0m");

                        //Storing the arument vector 3 data into one structure pointer
			mp3Info->tag_data=argv[3];

			printf("%s\n",mp3Info->tag_data);
                        
                         //Function Call for convert the size of arument vector 3 from little endian to big endian and write in a duplicate file
			duplicate_file_endian_conversion(argv,mp3Info);

                        //Size of the arument vector 3 data
			size=strlen(mp3Info->tag_data);

                        //Read the data of TCOM TAG of that size from source file
			fread(data,store,1,mp3Info->mp3_file_name);

                         //Write the data of arument vector 3 of that size into a duplicate file
			fwrite(mp3Info->tag_data,size,1,mp3Info->mp3_dup_file_name);
			printf("\e[1;31m--------------- COMMENT CHANGED SUCCESSFULLY ---------------\n\e[0m");

                         //Function call for copy the remaining data from source file to duplicate file
			copy_remaining_data(mp3Info);
		}
		else
		{
                        //IF the c_flag is not equal to 1 then copy the same data of TCOM TAG from source file to duplicate file
                        main_file_data_to_duplicate_file(mp3Info);

			fread(data,store,1,mp3Info->mp3_file_name);
			fwrite(data,store,1,mp3Info->mp3_dup_file_name);

                        //Recursive Function Call to check the other tags
			mp3_edit(argv,mp3Info);
		}
	}

	else
	{
                //IF any other TAG is present copy the same data of that TAG from source file to duplicate file
		main_file_data_to_duplicate_file(mp3Info);
		fread(data,store,1,mp3Info->mp3_file_name);
		fwrite(data,store,1,mp3Info->mp3_dup_file_name);

               //Recursive Function Call to check the other tags
		mp3_edit(argv,mp3Info);
	}
}

int main_file_endian_conversion(int value)
{
        //Convert the data from little endian to big endian and find the size of every tag to read the data from source file
	for(int i=0;i<2;i++)
	{
		temp=*((char*)&value+i);
		*((char*)&value+i)=*((char*)&value+(4-1-i));
		*((char*)&value+(4-1-i))=temp;
	}
	return value;

}

Status main_file_data_to_duplicate_file(Mp3DataInfo *mp3Info)
{
        //Write the TAGS of source file into a duplicate file
	fwrite(&num,4,1,mp3Info->mp3_dup_file_name);

	//Skip 2 bytes for Flag 
	fread(&skip,2,1,mp3Info->mp3_file_name);
	fwrite(&skip,2,1,mp3Info->mp3_dup_file_name);
}

Status duplicate_file_endian_conversion(char *argv[],Mp3DataInfo *mp3Info)
{
	mp3Info->tag_data=argv[3];
	size=strlen(mp3Info->tag_data);

        //Convert the size of argument vector 3 from little endian to big endian and write in a duplicate file
	for(int i=0;i<2;i++)
	{
		temp=*((char*)&size+i);
		*((char*)&size+i)=*((char*)&size+(4-1-i));
		*((char*)&size+(4-1-i))=temp;
	}

	fwrite(&size,4,1,mp3Info->mp3_dup_file_name);
	//Skip 2 bytes for Flag
	fread(&skip,2,1,mp3Info->mp3_file_name);
	fwrite(&skip,2,1,mp3Info->mp3_dup_file_name);
}

Status copy_remaining_data(Mp3DataInfo *mp3Info)
{
        //After edit of the source file data copy the remaining source file data into duplicate file of character by character
	char ch;
	while(fread(&ch,1,1,mp3Info->mp3_file_name)!=0)
	{
		fwrite(&ch,1,1,mp3Info->mp3_dup_file_name);
	}

        //Close the both files
	fclose(mp3Info->mp3_file_name);
	fclose(mp3Info->mp3_dup_file_name);
}

Status duplicate_file_data_to_main_file_after_change(Mp3DataInfo *mp3Info)
{
        //Open the both files
	if(open_files_edit(mp3Info)==m_failure)
	{        
		printf("Error:Unable to open files\n");
		return m_failure;
	}

        //Copy all data of duplicate file after edit into a source file of character by character
	char ch;
	while(fread(&ch,1,1,mp3Info->mp3_dup_file_name)!=0)
	{
		fwrite(&ch,1,1,mp3Info->mp3_file_name);
	}
	return m_success;
}
