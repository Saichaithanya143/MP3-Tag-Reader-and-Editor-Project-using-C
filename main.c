#include<stdio.h>
#include "types.h"
#include "mp3.h"
#include<string.h>

int main(int argc,char *argv[])
{
	//Variable is assiging for the structure Mp3DataInfo
	Mp3DataInfo mp3Info;

	//We have to pass atleast 2 command line arguments to view or edit the mp3 file data
	if(argc>=2)
	{
		//Function Call for check operation is selected
		int ret=check_operation_type(argv);

		//If the ret is equal to m_view then we selected for view the mp3 file data
		if(ret==m_view)
		{
			printf("\e[1;35m--------------- SELECTED VIEW DETAILS ---------------\n\e[0m");

			//Function call for read and validatae for view the mp3 file data
			if(read_and_validate_view(argv,&mp3Info)== m_success)
			{
				//printf("Read and Validate for viewing the mp3 file data is Success\n");

				//Function Call for the view status
				if(view_status(&mp3Info)==m_success)
				{
					//If the condition is success then display all details
					printf("\e[1;34m------------------------------------------------------------\n\e[0m");
					printf("\e[1;32m--------------- DETAILS DISPLAYED SUCCESSFULLY ---------------\n\e[0m");
				}
				else
				{
					//If the condition is failure then view of mp3 file data is failed
					printf("View Status is failure\n");
				}
			}
			else
			{
				//Read and Validate of view of mp3 file data is failed
				printf("Read and Validate for viewing the mp3 file data is Failure\n");
			}
		}

		//If the ret is equal to m_edit then we selected for edit the mp3 file data
		else if(ret==m_edit)
		{
			printf("\e[1;35m--------------- SELECTED EDIT DETAILS ---------------\n\e[0m");

			//Function call for read and validatae for view the mp3 file data
			if(read_and_validate_edit(argv,&mp3Info)==m_success)
			{
				//printf("Read and Validate for edit the mp3 file data is Success\n");

				//Function Call for the view status
				if(edit_status(argv,&mp3Info)==m_success)
				{
					//printf("Edit_staus is success\n");
				}
				else
				{
					//If the condition is failure then edit of mp3 file data is failed
					printf("Edit status is Failure\n");
				} 
			}
			else
			{
				//Read and Validate of view of mp3 file data is failed
				printf("Read and Validate for edit the mp3 file data is Failure\n");
			}
		}

		//If the ret is equal to m_help, then dispaly the data to help how to view or edit the mp3 file data
		else if(ret==m_help)
		{
			printf("\e[1;35m-------------------------------- HELP MENU -----------------------------\n\e[0m");
			printf("\e[1;32m1. -v -> to view mp3 file contents\n\e[0m");
			printf("\e[1;32m2. -e -> to edit mp3 file contents\n\e[0m");
			printf("\e[1;34m        2.1. -t -> to edit song title\n");
			printf("	2.2. -a -> to edit artist name\n");
			printf("	2.3. -A -> to edit album name\n");
			printf("	2.4. -y -> to edit year\n");
			printf("	2.5. -m -> to edit content\n");
			printf("	2.6. -c -> to edit comment\n\e[0m");
		}
		else
		{
			printf("Unsupported format\n");
		}
	}

	//If the argument count is less than 2 then the execution of the project is failed then an error message is occured for how to pass the details
	else
	{
		printf("\e[1;31mERROR: ./a.out : INVALID ARGUMENTS\n\e[0m");
		printf("\e[1;32mUSAGE:\n\e[0m");
		printf("\e[1;34mTo view please pass like: ./a.out -v mp3filename\n");
		printf("To edit please pass like: ./a.out -e -t/-a/-A/-m/-y/-c changing_text mp3filename\n");
		printf("To get help pass like: ./a.out --help\n\e[0m");
	}
}

//Function Declaration for the check operation type
OperationType check_operation_type(char *argv[])
{
	//If the argument vector of 2 is equal to -v then return m_view to view the details of mp3 file data
	if(strcmp(argv[1],"-v")==0)
	{
		return m_view;
	}

	//If the argument vector of 2 is equal to -e then return m_edit to edit the details of mp3 file data
	else if(strcmp(argv[1],"-e")==0)
	{
		return m_edit;
	}

	//If the argument vector of 2 is equal to --help then return m_help to help the user for how to pass the command line arguments foe view or edit the mp3 file data
	else if(strcmp(argv[1],"--help")==0)
	{
		return m_help;
	}
}


