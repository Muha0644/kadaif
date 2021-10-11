#pragma once
#include <QString>

enum fileType{
	nothing = 65,
	focus,
	gfx,
	image,
	loc,
	decision,
	event
};

fileType parser(QString path);
