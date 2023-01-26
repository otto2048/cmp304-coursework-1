#include "FlowGA.h"
#include "Input.h"
#include "Output.h"

int main()
{
	srand(time(0)); //initialise random number generator with time

	Output output;
	Input input;
	bool running = true;
	FlowGA flow;

	while (input.getUserInput() != "X" && input.getUserInput() != "x")
	{
		string filePath = input.mainMenuInteraction();

		if (filePath == "X" || filePath == "x")
		{
			break;
		}

		if (!flow.setUp(filePath))
		{
			system("pause");
			continue;
		}
		input.startMenuInteraction(flow);

		//allow user to go back to main menu from start menu
		while (input.getUserInput() == "B" || input.getUserInput() == "b")
		{
			filePath = input.mainMenuInteraction();

			if (input.getUserInput() == "X" || input.getUserInput() == "x")
			{
				break;
			}

			if (!flow.setUp(filePath))
			{
				system("pause");
				break;
			}

			input.startMenuInteraction(flow);
		}
	}

	output.outputExitingProgram();
	return 0;
}