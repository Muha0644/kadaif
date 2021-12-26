#include "parser.h"
#include <QString>

fileType parseType(QString path){
	if(path.length() < 4) return nothing;
	QString ext = path.last(4);
	if(ext == ".gfx"){
		return gfx;
	} else if(ext == ".dds" || ext == ".tga"){
		return image;
	} else if(ext == ".yml"){
		return loc;
	} else if(ext == ".txt"){

	}
	return nothing;
};
