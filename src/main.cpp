#include "chip8_class.hpp"
#include "platform_class.hpp"
#include "chrono"
#include "iostream"

using std::chrono::time_point;

int main(int argc, char* argv[])
{
	if(argc != 4)
	{
		std::cerr << "Usage: " << argv[0] << " <Scale> <Delay> <ROM> \n";
		std::exit(EXIT_FAILURE);
	}

	int videoScale = std::stoi(argv[1]);	// stoi: string to integer
	int cycleDelay = std::stoi(argv[2]);
	char const* romFileName = argv[3];
	
	platform_class platObj("CHIP-8 Emulator",
			       VIDEO_WIDTH * videoScale,
			       VIDEO_HEIGHT * videoScale,
			       VIDEO_WIDTH,
			       VIDEO_HEIGHT);
	
	chip8_class chip8obj;
	chip8obj.LoadROM(romFileName);

	int videoPitch = sizeof(chip8obj.video_display[0]) * VIDEO_WIDTH;

	time_point lastCycleTime = std::chrono::high_resolution_clock::now();

	bool quit = false;
	while(!quit)
	{
		quit = platObj.ProcessInput(chip8obj.keypad);	// passing pointer to keypad[], which is a public  member of chip8_class
		time_point currentTime = std::chrono::high_resolution_clock::now();
		float dt;	// difference in time
		dt = (std::chrono::duration<float, std::chrono::milliseconds::period>(currentTime - lastCycleTime)).count();

		if(dt > cycleDelay)
		{
			lastCycleTime = currentTime;
			chip8obj.InstrCycle();
			platObj.Update(chip8obj.video_display, videoPitch);
		}
	}

	return 0;
}
