#include<stdio.h>
#include "types.h"
#include "mp3.h"
#include "mp3_macro.h"
#include<string.h>


Status read_and_validate_view(char *argv[],Mp3DataInfo *mp3Info)
{
        /*Check the taken file is '.mp3' or not*/
	if(strstr(argv[2],".mp3")!=NULL)
	{
		mp3Info->mp3_view_name=argv[2];
		//printf("the file is %s\n",mp3Info->mp3_view_name);
		printf("\e[1;34m-----------------------------------------------------------\n\e[0m");
		printf("\e[1;33m           MP3 TAG READER AND EDITOR FOR ID3v2              \n\e[0m");
		printf("\e[1;34m------------------------------------------------------------\n\e[0m");

	}
	else
	{
                /*If the taken file is not '.mp3' file return m_failure*/
		return m_failure;
	}
        
        // No failure return m_success
	return m_success;
}

Status open_files_view(Mp3DataInfo *mp3Info)
{
        //Open the source file
	mp3Info->mp3_file_name=fopen(mp3Info->mp3_view_name,"r");
	if(mp3Info->mp3_file_name==NULL)
	{
		perror("fopen");
		fprintf(stderr, "ERROR: Unable to open file %s\n", mp3Info->mp3_view_name);
		return m_failure;
	}

         // No failure return m_success
	return m_success;
}

Status view_status(Mp3DataInfo *mp3Info)
{
        
        //Function Calls
	if(open_files_view(mp3Info)==m_failure)
	{
		printf("open file is failure\n");
		return m_failure;
	}
	if(mp3_header_check_view(mp3Info)==m_failure)
	{
		printf("Header checking is failure\n");
		return m_failure;
	}
	if(mp3_view(mp3Info)==m_failure)
	{
		printf("View the mp3 file data is failure\n");
                return m_failure;
	}

         // No failure return m_success
	return m_success;

}

Status mp3_header_check_view(Mp3DataInfo *mp3Info)
{
        
        
	char ch[ID3_SIZE];
	fread(ch,ID3_SIZE,1,mp3Info->mp3_file_name);
	mp3Info->mp3_ide=ch;
	//printf("The String is %s\n",mp3Info->mp3_ide);

        /*check the first 3 bytes of the header of our mp3 file is a ID3 or not*/
	if(strcmp(mp3Info->mp3_ide,"ID3")==0)
	{
		//printf("ID3 tag Check is Success\n");
	}
	else
	{
                /*If the 3 bytes are not a ID3 then return m_failure*/
		printf("ID3 tag Check is Failed\n");
		return m_failure;
	}
       
       
	short num;
	char temp=0;
	fread(&num,2,1,mp3Info->mp3_file_name);

        /*Convert the data into little endian to big endian*/
	for(int i=0;i<1;i++)
	{
		temp=*((char*)&num+i);
		*((char*)&num+i)=*((char*)&num+(2-1-i));
		*((char*)&num+(2-1-i))=temp;
	}
	//printf("After conversion %X\n",num);

        /*Check the version of our mp3 file 0300 version or not*/
	if(num==0x0300)
	{
		//printf("The version checing is success\n");
	}
	else
	{
                /*It is not version of 0300 then return m_failure*/
		printf("The version checking is failure\n");
		return m_failure;
	}

	//Skip 10 bytes of header
	fseek(mp3Info->mp3_file_name,10,SEEK_SET);

         // No failure return m_success
	return m_success;
}

Status mp3_view(Mp3DataInfo *mp3Info)
{
	/*Read the 4 bytes for check the TAG*/
	char buffer[TAG_SIZE];
	fread(buffer,TAG_SIZE,1,mp3Info->mp3_file_name);
	mp3Info->mp3_tag=buffer;
	//printf("The Tag is %s\n",mp3Info->mp3_tag);

        /*Read the size of every TAG*/
	int num;
	char temp=0;
	fread(&num,4,1,mp3Info->mp3_file_name);

        /*Convert the data into little endian to big endian to find tag size*/
	for(int i=0;i<2;i++)
	{
		temp=*((char*)&num+i);
		*((char*)&num+i)=*((char*)&num+(4-1-i));
		*((char*)&num+(4-1-i))=temp;
	}
	//printf("After conversion %X\n",num);

	//Skip 2 bytes for Flag
	short skip;
	fread(&skip,2,1,mp3Info->mp3_file_name);

	char ch;
	int i=0;
	char data[num];

        /*Check the TITLE TAG is present or not*/
	if(strcmp(mp3Info->mp3_tag,"TIT2")==0)
	{
		printf("\e[1;31mTITLE : \e[0m");
                 
                /*if the Title TAG is Present Read the data of the Title TAG and print the data*/
		for(i=0;i<num;i++)
		{
			fread(&ch,1,1,mp3Info->mp3_file_name);
			printf("%c",ch);
		}

		printf("\n");

                //Function Call
		mp3_view(mp3Info);
	}

         /*Check the ARTIST TAG is present or not*/
	else if(strcmp(mp3Info->mp3_tag,"TPE1")==0)
	{
		printf("\e[1;32mARTIST : \e[0m");

                /*if the ARTIST TAG is Present Read the data of the ARTIST TAG and print the data*/
		for(i=0;i<num;i++)
		{
			fread(&ch,1,1,mp3Info->mp3_file_name);
			printf("%c",ch);
		}
		printf("\n");

                 //Function Call
		mp3_view(mp3Info);
	}

         /*Check the ALBUM TAG is present or not*/
	else if(strcmp(mp3Info->mp3_tag,"TALB")==0)
	{
		printf("\e[1;33mALBUM : \e[0m");
                
                /*if the ALBUM TAG is Present Read the data of the ALBUM TAG and print the data*/
	       	for(i=0;i<num;i++)
		{
			fread(&ch,1,1,mp3Info->mp3_file_name);
			printf("%c",ch);
		}

		printf("\n");	

                 //Function Call
		mp3_view(mp3Info);
	}

         /*Check the YEAR TAG is present or not*/
	else if(strcmp(mp3Info->mp3_tag,"TYER")==0)
	{
		printf("\e[1;34mYEAR : \e[0m");
                 
                /*if the YEAR TAG is Present Read the data of the YEAR TAG and print the data*/
		for(i=0;i<num;i++)
		{
			fread(&ch,1,1,mp3Info->mp3_file_name);
			printf("%c",ch);
		}

		printf("\n");

                 //Function Call
		mp3_view(mp3Info);
	}

         /*Check the MUSIC TAG is present or not*/
	else if(strcmp(mp3Info->mp3_tag,"TCON")==0)
	{
		printf("\e[1;35mMUSIC : \e[0m");

                /*if the MUSIC TAG is Present Read the data of the MUSIC TAG and print the data*/
		for(i=0;i<num;i++)
		{
			fread(&ch,1,1,mp3Info->mp3_file_name);
			printf("%c",ch);
		}

		printf("\n");

                //Function Call
		mp3_view(mp3Info);
	}

         /*Check the COMMENT TAG is present or not*/
	else if(strcmp(mp3Info->mp3_tag,"TCOM")==0)
	{
		printf("\e[1;36mCOMMENT : \e[0m");
                
                /*if the COMMENT TAG is Present Read the data of the COMMENT TAG and print the data*/
		for(i=0;i<num;i++)
		{
			fread(&ch,1,1,mp3Info->mp3_file_name);
			printf("%c",ch);
		}

		printf("\n");

                /*Whenever the COMMENT TAG is present return m_success*/
		return m_success;
	}

	else
	{
                /*If any TAG is not present read the data of that TAG using the size of that data*/
		fread(data,num,1,mp3Info->mp3_file_name);

                //Function Call
		mp3_view(mp3Info);
	}
}

