#ifndef MAKE_DIR_H
#define MAKE_DIR_H

#include <iostream>
// The following is written for Linux only
void make_dir( const char* dir ) {
	const char* prefix = "mkdir -p ";
	char buffer[256];
	strcpy(buffer, prefix);
	strcat(buffer, dir);
	int flag = system( buffer );
	if( flag==0 ) {
		std::cout << "Directory '" << dir << "' may not be created properly.";
		std::cout << "It could either be the directory has already exist.";
		std::cout << "Or you do not have permission to do so. " << std::endl << std::endl;
	}
}

#endif
