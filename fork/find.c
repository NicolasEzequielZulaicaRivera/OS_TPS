#define _GNU_SOURCE
#include "util.h"
#include <string.h>
#include <fcntl.h>
#include <dirent.h>


typedef char string[80];
typedef char * _string;
typedef _string (*strSearchFunc)(const char * , const char * );

void findInDir( int dirFd, string path, string searchTerm, strSearchFunc searchFunc );
void findInDir( int dirFd, string path, string searchTerm, strSearchFunc searchFunc ){

	DIR *directory = fdopendir(dirFd);

	if (directory == NULL) {
		perror("error con opendir");
		close(dirFd);
		return;
	}
	
	struct dirent* entry;
	while ( ( entry = readdir(directory) ) ) {
		if (entry->d_type == DT_DIR) { // DIRECTORY
			if( !strcmp(entry->d_name,".") || !strcmp(entry->d_name,"..") ) continue;

			string newPath;
			strcpy( newPath, path );
			strcat( newPath, entry->d_name );
			strcat( newPath, "/" );

			int fd = openat( dirFd, entry->d_name, O_DIRECTORY );

			if( fd < 0 ){
				perror("Error en openat");
				continue;
			}

			findInDir( fd, newPath, searchTerm, searchFunc );
			
			continue;
		}
		if (entry->d_type == DT_REG) { // FILE
			if( searchFunc( entry->d_name, searchTerm ) )
				printf("%s%s\n", path, entry->d_name);
			continue;
		}
		/// UNKNOWN
	}
	closedir( directory );
}

int
main(int argc, char *argv[])
{
	if( argc != 2 && argc != 3 ){
		errno = EINVAL;
		perror("Interfaz: find [-i] <cadena> \n");
		_exit(-1);
	}

	string cadena ;
	strcpy( cadena, argv[ argc-1 ] );

	strSearchFunc searchFunc = strstr;

	if( argc == 3 ){
		if( strcmp( argv[1], "-i" ) ){
			errno = EINVAL;
			perror("Interfaz: find [-i] <cadena> \n");
			_exit(-1);
		}
		
		searchFunc = strcasestr;
	}

	int fd = open( ".", O_DIRECTORY );

	if( fd<0 ){
		perror("Error en open");
		return -1;
	}

	findInDir( fd, "", cadena, searchFunc );

	return 0;
}