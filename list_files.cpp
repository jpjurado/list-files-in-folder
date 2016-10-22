#include <iostream>
#include <fstream>
#include <string>
#include <dirent.h>

void ProcessDirectory(std::string directory, std::ofstream* file);
void ProcessFile(std::string file_dir, std::ofstream* file);
void ProcessEntity(struct dirent* entity, std::ofstream* file);
int WriteFile(std::ofstream* file, std::string file_path);

std::string path = "/path/to/directory/";
std::string file_name = "./out/list.csv";

int main()
{
    std::string directory = "theDirectoryYouWant";
	
	std::ofstream file_csv;
  	file_csv.open (file_name.c_str());
    
    ProcessDirectory(directory, &file_csv);    

  	file_csv.close();
    return 0;
}

int WriteFile(std::ofstream* file, std::string file_path){
  	*file << file_path <<std::endl;
  	return 0;
}

void ProcessDirectory(std::string directory,std::ofstream* file)
{
    std::string dirToOpen = path + directory;
    DIR *dir = opendir(dirToOpen.c_str());

    //set the new path for the content of the directory
    path = dirToOpen + "/";

    std::cout << "Process directory: " << dirToOpen.c_str() << std::endl;

    if(NULL == dir)
    {
        std::cout << "could not open directory: " << dirToOpen.c_str() << std::endl;
        return;
    }

    dirent* entity = readdir(dir);

    while(entity != NULL)
    {
        ProcessEntity(entity, file);
        entity = readdir(dir);
    }

    //we finished with the directory so remove it from the path
    path.resize(path.length() - 1 - directory.length());
    closedir(dir);
}

void ProcessEntity(struct dirent* entity,std::ofstream* file)
{
    //find entity type
    if(entity->d_type == DT_DIR)
    {//it's an direcotry
        //don't process the  '..' and the '.' directories
        if(entity->d_name[0] == '.')
        {
            return;
        }

        //it's an directory so process it
        ProcessDirectory(std::string(entity->d_name), file);
        return;
    }

    if(entity->d_type == DT_REG)
    {//regular file
        ProcessFile(std::string(entity->d_name), file);
        return;
    }

    //there are some other types
    //read here http://linux.die.net/man/3/readdir
    std::cout << "Not a file or directory: " << entity->d_name << std::endl;
}

void ProcessFile(std::string file_dir,std::ofstream* file)
{
    std::cout << "Process file     : " << file_dir.c_str() << std::endl;
    WriteFile(file,file_dir.c_str());
    //if you want to do something with the file add your code here
}