#ifndef ICODEURMANAGER_H_INCLUDED
#define ICODEURMANAGER_H_INCLUDED

/// @brief Classe abstraite s’occupant de deux codeurs
class ICodeurManager
{
public:
	int getRightTicks();
	int getLeftTicks();
	int getTime();

	virtual void readAndReset() = 0;
	virtual void reset() = 0;

// Protégé pour que les classes filles puisse modifier ces valeurs dans readAndReset()
protected:
	int leftTicks = 0, rightTicks = 0, tempsLast=0;
	int oldLeftTicks = 0, oldRightTicks = 0, oldTempsLast=0;
};

#endif //ICODEURMANAGER_H_INCLUDED