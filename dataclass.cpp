#include "dataclass.h"

dataClass::dataClass(QObject *parent): QObject(parent){
	locAll = nullptr;
}

dataClass::~dataClass(){
	delete locAll;
}
