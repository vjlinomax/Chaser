/*
  ==============================================================================

    Achievements.cpp
    Created: 7 Mar 2018 7:55:39pm
    Author:  Joris

  ==============================================================================
*/

#include "Achievements.h"
#include <vector>

const Achievement Achievements::Darklord( "Greetings, Hellchild!",
	"You have set your sequence to exactly 666 steps. All hail the Dark Lord.", 0);
const Achievement Achievements::Skidoo( "23", 
	"You were not supposed to be here yet. Madness this way lies. Get out while you can.", 22 );
const Achievement Achievements::FirstBorn( "Congratulations! It's a sequence!",
	"You have named your first sequence! Careful, they grow up fast.", 0 );
const Achievement Achievements::MakeUpYourMind( "Pick a team!",
	"You have toggled the same slice 20 times now. Decide already!", 19 );

Achievement* Achievements::getAchievement( int hash )
{
    std::vector< Achievement> achievements;
    achievements.push_back(Darklord);
    achievements.push_back(Skidoo);
    achievements.push_back(FirstBorn);
  //  achievements.push_back(MakeUpYourMind); //not including an achievement here, will reset it on every launch

	for ( auto achievement : achievements )
		if ( achievement.id == hash )
			return new Achievement(achievement);

	return nullptr;
}
