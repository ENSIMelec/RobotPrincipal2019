#include "ICodeurManager.h"

int ICodeurManager::getRightTicks()
{
	return rightTicks-oldRightTicks;
}

int ICodeurManager::getLeftTicks()
{
	return leftTicks-oldLeftTicks;
}

int ICodeurManager::getTime()
{
	return tempsLast-oldTempsLast;
}
