#ifndef STATE_H
#define STATE_H

struct KeyBoardState
{
	bool keyStates[256];
	KeyBoardState()
	{
		for (int i = 0; i < 256; i++)
			keyStates[i] = false;
	}
};

#endif // !STATE_H
